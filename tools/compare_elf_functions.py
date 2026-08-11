#!/usr/bin/env python3
"""Compare target functions with compiled ELF symbols, masking relocations only."""
from __future__ import annotations

import argparse
import csv
import hashlib
import struct
from dataclasses import dataclass
from pathlib import Path


SHT_SYMTAB = 2
SHT_RELA = 4
SHT_REL = 9
SHT_DYNSYM = 11
SHN_UNDEF = 0
ET_REL = 1


@dataclass(frozen=True)
class Section:
    index: int
    name: str
    type: int
    address: int
    offset: int
    size: int
    link: int
    info: int
    entry_size: int


@dataclass(frozen=True)
class Symbol:
    name: str
    value: int
    size: int
    section_index: int
    info: int


@dataclass(frozen=True)
class Comparison:
    symbol: str
    expected_size: int
    candidate_size: int
    relocation_ranges: tuple[tuple[int, int], ...]
    raw_equal: bool
    normalized_equal: bool
    differing_bytes: int
    first_differences: tuple[int, ...]

    @property
    def matching(self) -> bool:
        return self.expected_size == self.candidate_size and self.normalized_equal


class ELFFile:
    def __init__(self, path: Path):
        self.path = path
        self.data = path.read_bytes()
        self.sections: list[Section] = []
        self.symbols: list[Symbol] = []
        self.elf_class = 0
        self.endian = ""
        self.file_type = 0
        self.machine = 0
        self._parse()

    @staticmethod
    def _cstring(data: bytes, offset: int) -> str:
        if offset < 0 or offset >= len(data):
            return ""
        end = data.find(b"\0", offset)
        if end == -1:
            end = len(data)
        return data[offset:end].decode("utf-8", errors="replace")

    def _parse(self) -> None:
        if len(self.data) < 52 or self.data[:4] != b"\x7fELF":
            raise ValueError(f"{self.path}: not an ELF file")
        self.elf_class = self.data[4]
        byte_order = self.data[5]
        if self.elf_class not in (1, 2) or byte_order not in (1, 2):
            raise ValueError(f"{self.path}: unsupported ELF class or byte order")
        self.endian = "<" if byte_order == 1 else ">"

        if self.elf_class == 1:
            values = struct.unpack_from(self.endian + "16sHHIIIIIHHHHHH", self.data, 0)
            self.file_type = values[1]
            self.machine = values[2]
            section_offset = values[6]
            section_entry_size = values[11]
            section_count = values[12]
            section_names_index = values[13]
            section_format = self.endian + "IIIIIIIIII"
        else:
            values = struct.unpack_from(self.endian + "16sHHIQQQIHHHHHH", self.data, 0)
            self.file_type = values[1]
            self.machine = values[2]
            section_offset = values[6]
            section_entry_size = values[11]
            section_count = values[12]
            section_names_index = values[13]
            section_format = self.endian + "IIQQQQIIQQ"

        raw_sections: list[tuple[int, ...]] = []
        required_size = struct.calcsize(section_format)
        if section_entry_size < required_size:
            raise ValueError(f"{self.path}: invalid section-header entry size")
        for index in range(section_count):
            offset = section_offset + index * section_entry_size
            if offset + required_size > len(self.data):
                raise ValueError(f"{self.path}: truncated section header {index}")
            raw_sections.append(struct.unpack_from(section_format, self.data, offset))
        if not raw_sections or section_names_index >= len(raw_sections):
            raise ValueError(f"{self.path}: missing section-name table")

        names_header = raw_sections[section_names_index]
        names_offset, names_size = names_header[4], names_header[5]
        section_names = self.data[names_offset:names_offset + names_size]
        for index, raw in enumerate(raw_sections):
            self.sections.append(
                Section(
                    index=index,
                    name=self._cstring(section_names, raw[0]),
                    type=raw[1],
                    address=raw[3],
                    offset=raw[4],
                    size=raw[5],
                    link=raw[6],
                    info=raw[7],
                    entry_size=raw[9],
                )
            )
        self._parse_symbols()

    def _parse_symbols(self) -> None:
        for section in self.sections:
            if section.type not in (SHT_SYMTAB, SHT_DYNSYM) or section.entry_size == 0:
                continue
            if section.link >= len(self.sections):
                raise ValueError(f"{self.path}: invalid string-table link in {section.name}")
            string_section = self.sections[section.link]
            strings = self.data[string_section.offset:string_section.offset + string_section.size]
            count = section.size // section.entry_size
            for index in range(count):
                offset = section.offset + index * section.entry_size
                if self.elf_class == 1:
                    name, value, size, info, _other, section_index = struct.unpack_from(
                        self.endian + "IIIBBH", self.data, offset
                    )
                else:
                    name, info, _other, section_index, value, size = struct.unpack_from(
                        self.endian + "IBBHQQ", self.data, offset
                    )
                self.symbols.append(
                    Symbol(self._cstring(strings, name), value, size, section_index, info)
                )

    def find_symbol(self, name: str) -> Symbol:
        matches = [symbol for symbol in self.symbols if symbol.name == name and symbol.section_index != SHN_UNDEF]
        if not matches:
            raise ValueError(f"{self.path}: defined symbol {name!r} not found")
        functions = [symbol for symbol in matches if symbol.info & 0xF == 2]
        return max(functions or matches, key=lambda symbol: symbol.size)

    def symbol_bytes(self, symbol: Symbol, fallback_size: int) -> bytes:
        if symbol.section_index >= len(self.sections):
            raise ValueError(f"{self.path}: symbol {symbol.name!r} has invalid section index")
        section = self.sections[symbol.section_index]
        relative = symbol.value - section.address
        size = symbol.size or fallback_size
        if relative < 0 or relative + size > section.size:
            raise ValueError(f"{self.path}: symbol {symbol.name!r} extends outside {section.name}")
        start = section.offset + relative
        return self.data[start:start + size]

    def relocation_ranges(self, symbol: Symbol, width: int) -> tuple[tuple[int, int], ...]:
        ranges: set[tuple[int, int]] = set()
        for section in self.sections:
            if section.type not in (SHT_REL, SHT_RELA) or section.info != symbol.section_index:
                continue
            if section.entry_size == 0:
                continue
            count = section.size // section.entry_size
            for index in range(count):
                offset = section.offset + index * section.entry_size
                if self.elf_class == 1:
                    relocation_offset = struct.unpack_from(self.endian + "I", self.data, offset)[0]
                else:
                    relocation_offset = struct.unpack_from(self.endian + "Q", self.data, offset)[0]
                relative = relocation_offset - symbol.value
                symbol_size = symbol.size
                if 0 <= relative < symbol_size:
                    ranges.add((relative, min(relative + width, symbol_size)))
        return tuple(sorted(ranges))


def compare_function(
    target: bytes,
    target_offset: int,
    expected_size: int,
    elf: ELFFile,
    symbol_name: str,
    relocation_width: int = 4,
) -> Comparison:
    if target_offset < 0 or target_offset + expected_size > len(target):
        raise ValueError("target function range lies outside the target image")
    symbol = elf.find_symbol(symbol_name)
    candidate = elf.symbol_bytes(symbol, expected_size)
    expected = target[target_offset:target_offset + expected_size]
    ranges = elf.relocation_ranges(symbol, relocation_width)
    masked: set[int] = set()
    for start, end in ranges:
        masked.update(range(start, end))

    overlap = min(len(expected), len(candidate))
    differences = [
        index
        for index in range(overlap)
        if index not in masked and expected[index] != candidate[index]
    ]
    normalized_equal = not differences and len(expected) == len(candidate)
    return Comparison(
        symbol=symbol_name,
        expected_size=len(expected),
        candidate_size=len(candidate),
        relocation_ranges=ranges,
        raw_equal=expected == candidate,
        normalized_equal=normalized_equal,
        differing_bytes=len(differences) + abs(len(expected) - len(candidate)),
        first_differences=tuple(differences[:8]),
    )


def render_report(
    target_path: Path,
    object_path: Path,
    base_address: int,
    rows: list[dict[str, str]],
    results: list[tuple[dict[str, str], Comparison | str]],
) -> str:
    matches = sum(isinstance(result, Comparison) and result.matching for _, result in results)
    lines = [
        "# ELF function comparison report",
        "",
        "> Generated by `tools/compare_elf_functions.py`. Relocation bytes are masked on both sides; all other bytes must agree.",
        "",
        f"- Target image: `{target_path}`",
        f"- Target SHA-256: `{hashlib.sha256(target_path.read_bytes()).hexdigest()}`",
        f"- Target base: `0x{base_address:08x}`",
        f"- Candidate object: `{object_path}`",
        f"- Candidate SHA-256: `{hashlib.sha256(object_path.read_bytes()).hexdigest()}`",
        f"- Result: **{matches}/{len(rows)} relocation-normalized matches**",
        "",
        "| Address | Target | Object symbol | Target / object bytes | Relocations | Result | First non-relocation difference |",
        "|---:|---|---|---:|---:|---|---|",
    ]
    for row, result in results:
        if isinstance(result, str):
            lines.append(
                f"| `{row['address']}` | `{row['name']}` | `{row['object_symbol']}` | "
                f"— | — | **ERROR** | {result.replace('|', '/')} |"
            )
            continue
        status = "MATCHING" if result.matching else "DIFF"
        first = ", ".join(f"+0x{offset:x}" for offset in result.first_differences) or "—"
        lines.append(
            f"| `{row['address']}` | `{row['name']}` | `{row['object_symbol']}` | "
            f"{result.expected_size} / {result.candidate_size} | {len(result.relocation_ranges)} | "
            f"**{status}** | {first} |"
        )
    lines.extend(
        [
            "",
            "A row may be promoted in the project manifest only when its result is",
            "`MATCHING` and the compiler, flags, source revision and object provenance",
            "are recorded. This report never edits progress status automatically.",
            "",
        ]
    )
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", required=True, type=Path, help="unpacked raw target image")
    parser.add_argument("--base-address", type=lambda value: int(value, 0), default=0x00100000)
    parser.add_argument("--object", required=True, type=Path, help="compiled relocatable ELF object")
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--report", required=True, type=Path)
    parser.add_argument("--relocation-width", type=int, default=4)
    parser.add_argument("--require-all-matching", action="store_true")
    args = parser.parse_args()

    if args.relocation_width < 1:
        raise SystemExit("--relocation-width must be positive")
    try:
        target = args.target.read_bytes()
        elf = ELFFile(args.object)
        with args.manifest.open(encoding="utf-8", newline="") as stream:
            rows = list(csv.DictReader(stream))
    except (OSError, ValueError) as exc:
        raise SystemExit(f"comparison setup failed: {exc}") from exc

    required = {"address", "end", "name", "object_symbol"}
    if not rows or not required <= set(rows[0]):
        raise SystemExit(f"comparison manifest must contain {sorted(required)}")

    results: list[tuple[dict[str, str], Comparison | str]] = []
    for row in rows:
        try:
            address = int(row["address"], 0)
            end = int(row["end"], 0)
            if end <= address:
                raise ValueError("end address is not above start address")
            result = compare_function(
                target,
                address - args.base_address,
                end - address,
                elf,
                row["object_symbol"],
                args.relocation_width,
            )
        except ValueError as exc:
            result = str(exc)
        results.append((row, result))

    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(
        render_report(args.target, args.object, args.base_address, rows, results),
        encoding="utf-8",
    )
    matches = sum(isinstance(result, Comparison) and result.matching for _, result in results)
    print(f"wrote {args.report}: {matches}/{len(rows)} relocation-normalized matches")
    if args.require_all_matching and matches != len(rows):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
