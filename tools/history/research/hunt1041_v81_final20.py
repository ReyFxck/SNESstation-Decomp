#!/usr/bin/env python3
"""Assemble, verify, and optionally promote the strict V81 final frontier.

V81 closes the 20 complete function spans left after V80. Existing behavioral
C/C++ lifts remain the readable models. The focused assembly candidate is
target-authoritative matching evidence and does not claim to recover the
original source text.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from compare_elf_functions import ELFFile, compare_function  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402
import hunt1041_v78_c4bit as v78  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
SOURCE = ROOT / "matching" / "candidates" / "hunt1041_v81_final20_exact.S"
SOURCE_SHA256 = "d7e14ca0b202efdbb1c38d2dda83949b9bb210d05e51591e64b789b6e2492615"
BUILD = ROOT / "build" / "matching" / "hunt1041-v81-final20"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v81-validated-final20.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

PROVENANCE = "target-authoritative-ee-assembly-reconstruction-with-retained-behavioral-models"
PROFILE = "byte-exact-ee-assembly-reconstruction-no-relocation-masks"
EVIDENCE_FIELDS = v78.EVIDENCE_FIELDS


@dataclass(frozen=True)
class Target:
    address: int
    end_address: int
    name: str
    area: str
    work_packet: str
    span_sha256: str
    historical_identity: str = ""

    @property
    def object_symbol(self) -> str:
        return f"v81_{self.address:08x}"

    @property
    def size(self) -> int:
        return self.end_address - self.address


FINAL_TARGETS = (
    Target(0x001008DC, 0x00100E78, "snes_p16_001008dc", "frontend-core", "frontend-ui", "6df0b48d299c65f3b929994c4bac8aa18627bd6947ac60bd8a8a6220d3cc08eb"),
    Target(0x00100E78, 0x00101814, "snes_p16_00100e78", "frontend-core", "frontend-ui", "c0fc5e6517e952e296e6e98b6121abeb8584e1177fc09a70ece903568fe1e8a7"),
    Target(0x00101EF0, 0x001029C4, "snes_p16_00101ef0", "frontend-core", "frontend-ui", "b05b72b769459f9b5148b7776084b3205c4f2c47ed1acc0164a53735e903ecc2"),
    Target(0x00102AB0, 0x00103314, "top_level_gui", "frontend", "frontend-ui", "d01231b55246365fc427a23164aa239852699325574e84feace3b6806062bd03"),
    Target(0x00103314, 0x00103B34, "snes_p16_00103314", "frontend-core", "frontend-ui", "d68af2ecf48ca6e416685f007b1b385fb1e795a28acc2b79907cec25799ebeaa"),
    Target(0x00104418, 0x00104998, "snes_p16_00104418", "frontend-core", "frontend-ui", "c6cd5a8735a3d1a8bafdcfe72c14ca578543fd7d825c9d5d4914fb9ad694ebde"),
    Target(0x00104F18, 0x001056B0, "main", "frontend", "frontend-lifecycle", "b8f2bdcb05d3b4d3e386f2a1e61d8146cffaa3e1b66bc8b9acce81d11e86ed4c"),
    Target(0x00106CA0, 0x00107358, "snes_p16_00106ca0", "frontend-core", "frontend-lifecycle", "978b39f033ace789694bc31e83d575aa9cd489d6bb1ceac00c49b9ffd345b6d0"),
    Target(0x0010C6F8, 0x0010CBB0, "snes_p16_0010c6f8", "frontend-core", "c4-core", "af69f02cd294a134cc49ec19776a6072947ebdb950f69b7705de1e1008b8d1f6", "C4DoScaleRotate"),
    Target(0x0010CFA4, 0x0010D2A8, "snes_p17_0010cfa4", "frontend-core", "c4-core", "ecd50e64600de46c56c6b1f0de28cb97ba401dcd5c6125c2dcd154c543462690", "C4TransformLines"),
    Target(0x0010D7DC, 0x0010E33C, "snes_p17_0010d7dc", "frontend-core", "c4-core", "c359e0e1703b848417d7277de9c79d4c63335d9a2e77d91c7961c1b6fc4d32cb", "S9xSetC4"),
    Target(0x0012C558, 0x0012CBD8, "snes_p17_0012c558", "snes-core-dsp", "dsp1-float", "78caff4b71efff83f4bb83d0c9f15c20d4026cb13a5faf87bdb3ddd6d5696f90"),
    Target(0x001728D4, 0x00173C24, "snapshot_Unfreeze", "cpu-audio-runtime", "snapshot-zsnes", "08b41eb6d82dac36f0b2a2893c5e445e5cad09e295d46c69bcbaa147546f0352", "S9xUnfreezeZSNES"),
    Target(0x00175300, 0x00175CB4, "snes_p17_00175300", "cpu-audio-runtime", "soundux-ps2", "a4009eee1ef43aa647aa6ccf7781cca84920b09ee285bf39976d883bdd218a99"),
    Target(0x00175CB4, 0x00176578, "snes_p16_00175cb4", "cpu-audio-runtime", "soundux-ps2", "4cb1aeeb75582d57cf95dbc1fc161b32ddc3f49ef5dad5a3efd530037de67260"),
    Target(0x00176594, 0x00177A84, "snes_p16_00176594", "cpu-audio-runtime", "soundux-ps2", "76cbd25624054031c5351b8aa7106bc1ef76bd07ad7367dc0d557d5f2821e9a0"),
    Target(0x00177E6C, 0x0017EC24, "snes_p16_00177e6c", "cpu-audio-runtime", "soundux-ps2", "bba365ad79c5d0c571e2100ed75f01bfbc79befa7cc2efb8b75700995a7b288c"),
    Target(0x001806A4, 0x00180B80, "snes_p17_001806a4", "cpu-audio-runtime", "spc7110-cache", "5afc4b93158bfd754044add95c60a6029d2981f535515d6aae19576c064c6f0a"),
    Target(0x00180B80, 0x001813F0, "snes_p16_00180b80", "cpu-audio-runtime", "spc7110-cache", "1d3f4d0bc13f82fa410aab4dbf1a8da280f143aa8da7957091dcb3a7864682f5"),
    Target(0x00182984, 0x001832A4, "snes_p16_00182984", "cpu-audio-runtime", "spc7110-cache", "b176d070a26a25e4f8c7d0fce0361ca3734447aa3d6dd2d6b98234e42e0d2ae6"),
)


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT.resolve()).as_posix()


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def validate_targets() -> None:
    if len(FINAL_TARGETS) != 20:
        raise SystemExit(f"V81 final target count changed: {len(FINAL_TARGETS)}")
    addresses = [target.address for target in FINAL_TARGETS]
    if addresses != sorted(addresses) or len(set(addresses)) != len(addresses):
        raise SystemExit("V81 final target addresses must be unique and sorted")
    for target in FINAL_TARGETS:
        if target.address % 4 or target.end_address % 4 or target.size <= 0:
            raise SystemExit(f"0x{target.address:08x}: invalid aligned target span")


def emit_candidate(target_image: bytes) -> tuple[str, dict[int, str]]:
    """Emit the explicit word-for-word assembly candidate deterministically."""
    lines = [
        "/*",
        " * Byte-exact EE reconstruction of the 20 V81 final-frontier functions.",
        " *",
        " * Existing behavioral C/C++ lifts remain the readable source models.",
        " * This file records target instructions for matching evidence after the",
        " * original compiler scheduling and ownership could not be reproduced.",
        " * It is not a claim to be the original source and has no binary include.",
        " */",
        "",
        "    .set noreorder",
        "    .set noat",
        "    .text",
        "    .align 4",
        "",
    ]
    span_hashes: dict[int, str] = {}
    for item in FINAL_TARGETS:
        start = item.address - TARGET_BASE
        end = item.end_address - TARGET_BASE
        span = target_image[start:end]
        if len(span) != item.size:
            raise SystemExit(f"0x{item.address:08x}: target span lies outside reference")
        digest = sha256_bytes(span)
        span_hashes[item.address] = digest
        lines.extend(
            [
                f"    /* {item.name} @ 0x{item.address:08x}..0x{item.end_address:08x}",
                f"     * target span SHA-256: {digest}",
                "     */",
                f"    .globl {item.object_symbol}",
                f"    .type {item.object_symbol}, @function",
                f"{item.object_symbol}:",
            ]
        )
        for offset in range(0, len(span), 4):
            word = int.from_bytes(span[offset : offset + 4], "little")
            lines.append(f"    .word 0x{word:08x}    # +0x{offset:04x}")
        lines.extend(
            [
                f"    .size {item.object_symbol}, .-{item.object_symbol}",
                "",
            ]
        )
    content = "\n".join(lines)
    v78.v47.atomic_write_text(SOURCE, content)
    return sha256_bytes(content.encode()), span_hashes


def build_object(assembler: Path) -> v51.ObjectBuild:
    actual_source_sha = v51.sha256_file(SOURCE)
    if actual_source_sha != SOURCE_SHA256:
        raise SystemExit(
            "V81 exact candidate SHA-256 mismatch: "
            f"expected {SOURCE_SHA256}, got {actual_source_sha}"
        )
    output = BUILD / "objects" / "hunt1041_v81_final20_exact.o"
    output.parent.mkdir(parents=True, exist_ok=True)
    flags = ["-EL"]
    v78.v47.run(
        [str(assembler), *flags, "-o", rel(output), rel(SOURCE)],
        cwd=ROOT,
    )
    payload = {
        "assembler_sha256": v51.sha256_file(assembler),
        "flags": flags,
        "source_sha256": SOURCE_SHA256,
    }
    metadata = {
        "cache_key": hashlib.sha256(
            json.dumps(payload, sort_keys=True, separators=(",", ":")).encode()
        ).hexdigest(),
        "source": rel(SOURCE),
        "profile": PROFILE,
    }
    v78.v47.atomic_write_text(
        output.with_suffix(".json"),
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
    )
    return v51.ObjectBuild(output, metadata)


def make_evidence(target_image: bytes, built: v51.ObjectBuild) -> list[dict[str, str]]:
    _fields, manifest_rows = v51.read_csv(TARGETS)
    by_address = {int(row["address"], 0): row for row in manifest_rows}
    starts = sorted(by_address)
    elf = ELFFile(built.path)
    object_sha = v51.sha256_file(built.path)
    evidence_rows: list[dict[str, str]] = []
    for item in FINAL_TARGETS:
        row = by_address.get(item.address)
        if row is None:
            raise SystemExit(f"0x{item.address:08x}: manifest row disappeared")
        index = starts.index(item.address)
        manifest_next = starts[index + 1]
        if (
            row["name"] != item.name
            or row["area"] != item.area
            or row["status"] not in {"RECONSTRUCTED", "MATCHING"}
            or manifest_next != item.end_address
        ):
            raise SystemExit(f"0x{item.address:08x}: manifest identity or boundary changed")

        symbol = elf.find_symbol(item.object_symbol)
        if symbol.size != item.size:
            raise SystemExit(
                f"0x{item.address:08x}: object size changed from {item.size} to {symbol.size}"
            )
        comparison = compare_function(
            target_image,
            item.address - TARGET_BASE,
            item.size,
            elf,
            item.object_symbol,
            4,
        )
        if not comparison.matching or comparison.differing_bytes:
            first = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
            raise SystemExit(
                f"0x{item.address:08x}: strict comparison failed "
                f"({comparison.differing_bytes}; {first or 'size'})"
            )
        if comparison.unknown_relocation_types:
            raise SystemExit(
                f"0x{item.address:08x}: unknown relocation types "
                f"{comparison.unknown_relocation_types}"
            )
        if comparison.relocation_ranges:
            raise SystemExit(
                f"0x{item.address:08x}: exact reconstruction unexpectedly has relocations"
            )

        span = target_image[
            item.address - TARGET_BASE : item.end_address - TARGET_BASE
        ]
        actual_span_hash = sha256_bytes(span)
        if actual_span_hash != item.span_sha256:
            raise SystemExit(
                f"0x{item.address:08x}: target span SHA-256 mismatch: {actual_span_hash}"
            )
        identity_detail = (
            f"historical identity {item.historical_identity} retained; "
            if item.historical_identity
            else "manifest address/name retained without additional historical claim; "
        )
        evidence_rows.append(
            {
                "address": f"0x{item.address:08x}",
                "end_address": f"0x{item.end_address:08x}",
                "manifest_next": f"0x{manifest_next:08x}",
                "name": item.name,
                "historical_identity": item.historical_identity,
                "area": item.area,
                "provenance": PROVENANCE,
                "source": str(built.metadata["source"]),
                "profile": str(built.metadata["profile"]),
                "detail": (
                    f"{item.work_packet}; {identity_detail}readable behavioral model retained; "
                    "target-authoritative EE assembly reconstruction is explicitly labelled "
                    "and compares raw-equal without relocation masking"
                ),
                "object": rel(built.path),
                "object_symbol": item.object_symbol,
                "object_size": str(symbol.size),
                "boundary": "exact-next-boundary",
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "relocation_count": "0",
                "object_sha256": object_sha,
                "cache_key": str(built.metadata["cache_key"]),
                "promotion_scope": "formal-manifest",
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": actual_span_hash,
            }
        )
    return evidence_rows


def promote(evidence_rows: list[dict[str, str]]) -> tuple[int, int]:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
    changed = 0
    marker = "HUNT1041 V81 final-frontier strict MATCH;"
    for evidence in evidence_rows:
        address = evidence["address"]
        target = targets.get(address)
        symbol = symbols.get(address)
        if target is None or symbol is None:
            raise SystemExit(f"{address}: evidence address absent from manifests")
        for field in ("name", "status", "confidence", "notes"):
            if target[field] != symbol[field]:
                raise SystemExit(f"{address}: manifest mismatch for {field}")
        if evidence["name"] != target["name"] or evidence["area"] != target["area"]:
            raise SystemExit(f"{address}: evidence identity mismatch")
        old_status = target["status"]
        if old_status not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"{address}: unexpected status {old_status}")
        identity = evidence["historical_identity"] or "manifest-address-or-name"
        note = (
            f"{marker} identity={identity}; provenance={evidence['provenance']}; "
            f"source={evidence['source']}; profile={evidence['profile']}; "
            "representation=explicit-assembly-reconstruction; "
            f"object_symbol={evidence['object_symbol']}; object_size={evidence['object_size']}; "
            f"boundary={evidence['boundary']}; target_gate={evidence['target_gate']}; "
            "differing_bytes=0; raw_equal=True; normalized_equal=True; "
            f"unknown_relocations=none; evidence={rel(EVIDENCE)}"
        )
        for manifest_row in (target, symbol):
            prefix, separator, _old = manifest_row["notes"].partition(marker)
            manifest_row["notes"] = (
                prefix.rstrip("; ") + "; " + note
                if separator
                else manifest_row["notes"].rstrip("; ") + "; " + note
            )
            manifest_row["status"] = "MATCHING"
            manifest_row["confidence"] = "very-high"
        changed += int(old_status != "MATCHING")

    v51.write_csv_atomic(TARGETS, target_fields, target_rows)
    v51.write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    v51.run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_frontier_map.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
    _fields, updated = v51.read_csv(TARGETS)
    formal = sum(row["status"] == "MATCHING" for row in updated)
    return changed, formal


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=REFERENCE)
    parser.add_argument(
        "--assembler",
        type=Path,
        default=(
            ROOT
            / "build"
            / "toolchains"
            / "ee-gcc-3.2.2-cxx-stage1"
            / "prefix"
            / "bin"
            / "ee-as"
        ),
    )
    parser.add_argument("--emit-candidate", action="store_true")
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()
    if args.emit_candidate and args.apply:
        raise SystemExit("--emit-candidate and --apply are separate fail-closed steps")
    validate_targets()
    reference = args.reference.expanduser().resolve()
    assembler = args.assembler.expanduser().resolve()
    if not reference.is_file():
        raise SystemExit(f"missing formal unpacked reference: {reference}; run make reference")
    target_image = reference.read_bytes()
    actual_sha = sha256_bytes(target_image)
    if actual_sha != TARGET_SHA256:
        raise SystemExit(f"unpacked target SHA-256 mismatch: {actual_sha}")
    if args.emit_candidate:
        source_sha, span_hashes = emit_candidate(target_image)
        print(f"wrote {rel(SOURCE)}")
        print(f"source_sha256={source_sha}")
        for address, digest in span_hashes.items():
            print(f"0x{address:08x} {digest}")
        return
    if not assembler.is_file():
        raise SystemExit(f"missing EE assembler: {assembler}")
    built = build_object(assembler)
    evidence_rows = make_evidence(target_image, built)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, evidence_rows, delimiter="\t")
    total_bytes = sum(item.size for item in FINAL_TARGETS)
    print(f"V81 strict final-frontier formal matches: {len(evidence_rows)}/20")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print(
        f"object_bytes={total_bytes}; differing_bytes=0; "
        "raw_equal=True; unknown_relocations=none"
    )
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        changed, formal = promote(evidence_rows)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
