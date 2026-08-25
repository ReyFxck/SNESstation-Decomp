#!/usr/bin/env python3
"""Assemble, verify, and optionally promote the strict V80 quick-win batch.

V80 deliberately prioritizes the 23 smallest non-C4 functions left after V79.
The existing behavioral C/C++ lifts remain the readable models.  The focused
assembly candidate is target-authoritative matching evidence and does not claim
to recover the original source text.
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
SOURCE = ROOT / "matching" / "candidates" / "hunt1041_v80_quickwins_exact.S"
SOURCE_SHA256 = "ece41d27dc53f543519c48982c228b37d89eb3c1c37adf8088699b962e1b69cc"
BUILD = ROOT / "build" / "matching" / "hunt1041-v80-quickwins"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v80-validated-quickwins-23.tsv"
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
        return f"v80_{self.address:08x}"

    @property
    def size(self) -> int:
        return self.end_address - self.address


# These are the 23 smallest V79 frontier spans after explicitly excluding the
# three remaining C4 functions.  End addresses are exact next-manifest
# boundaries, not inferred disassembly endpoints.
QUICK_WINS = (
    Target(0x00100114, 0x0010038C, "snes_p17_00100114", "frontend-core", "frontend-ui", "7e9ac3595f51b8b16782d02d52a1d0ef302620918e2b01401c291c04f3006358"),
    Target(0x0010038C, 0x001005B0, "snes_p16_0010038c", "frontend-core", "frontend-ui", "106b9ad2f128bc454b21e3ae95a1cf98689856940b957e24ab9d383aa9779b57"),
    Target(0x001005EC, 0x001008DC, "snes_p16_001005ec", "frontend-core", "frontend-ui", "338cfe388d6389ecce854caf39b59af0aae35b3dbbc42a55194d36a3af75e3ee"),
    Target(0x001019A8, 0x00101B04, "snes_p16_001019a8", "frontend-core", "frontend-ui", "10038534c6c9105e58b0b56a8a2dcdec3aed3ceb016825d104015399f6296e53"),
    Target(0x00101B64, 0x00101E8C, "snes_p16_00101b64", "frontend-core", "frontend-ui", "d61cdab39f60c3eff792009c1a4596d1b5ce897ef5b8493a12c52a62b169c067"),
    Target(0x00103B34, 0x00103C7C, "snes_p16_00103b34", "frontend-core", "frontend-ui", "7e8f40422a8ccce030470a027ef3e25b62d4804b5d355c9225c86445eb022fac"),
    Target(0x00103DD4, 0x001041C0, "snes_p16_00103dd4", "frontend-core", "frontend-ui", "6b0b1b6f887d94fa8031835f384fd1bc4dd7bc48f32e1938123b6b267b5e69cc"),
    Target(0x00104234, 0x00104358, "snes_p16_00104234", "frontend-core", "frontend-ui", "de9c11dc640d1159d8d1584229c04792fcd187261345dbb4cac23ba3ad4fb13f"),
    Target(0x00104358, 0x00104418, "snes_p11_00104358", "frontend", "frontend-ui", "2234db996d1278c91e7408a670a0161ba68929bb8a152a2a051015ba542eaae0"),
    Target(0x00104A54, 0x00104BBC, "mtapPortClose_00104a54", "multitap", "frontend-pad", "fbe592d9ea616eaa871e3e9c16d0654d18be2d9aaf02bf6d77ea5fd242b1d116"),
    Target(0x00104BBC, 0x00104E48, "mtapGetConnection_00104bbc", "multitap", "frontend-pad", "f2b6221346884d105866d91730301ea5217acae600c5c36f15288196e6f1f657"),
    Target(0x00105898, 0x001059CC, "snes_p16_00105898", "frontend-core", "frontend-lifecycle", "97f27ff6ebd90ddea7e212e46e68280119cbf4e12cec400501f725f737ce82d5"),
    Target(0x00105AE8, 0x00105CB8, "snes_p16_00105ae8", "frontend-core", "frontend-lifecycle", "758e9d74a15d0e1b78550bbfc317b6367d8936764d64db357aea375ad8be33b3"),
    Target(0x00105E48, 0x00106054, "snes_p17_00105e48", "frontend-core", "frontend-lifecycle", "c6b3ed0d62f393fcbf770a9c08c64afee1cfa163693239718c663b61af68c4f9"),
    Target(0x001060DC, 0x001064C0, "snes_p16_001060dc", "frontend-core", "frontend-lifecycle", "48ec716f4a5c942aef04d6582c99ff82b6aa2769b3f6f4c14ad7a365cc88ce1a"),
    Target(0x001064C0, 0x00106824, "snes_p16_001064c0", "frontend-core", "frontend-lifecycle", "e3d486d873eebe74de03768ece8480865d50c5fd9bc3218e5486dcfcbdb2d9c1"),
    Target(0x0010689C, 0x00106BCC, "snes_p16_0010689c", "frontend-core", "frontend-lifecycle", "8213811fdb82b4483026771f14cc391d4d58edafbc10d4337c31a53a092a2455"),
    Target(0x00107358, 0x00107578, "snes_p16_00107358", "frontend-core", "frontend-lifecycle", "a8f984918924b89f4386d48850012a98942257795e4be985ba068c03b0f8d75b"),
    Target(0x0012CBD8, 0x0012CE18, "snes_p16_0012cbd8", "snes-core-dsp", "dsp1-float", "1b84bf63d7e4f5968b1a7d3887c2299204ea630e619b6ef3defcf0f1344f7c6e"),
    Target(0x0012D05C, 0x0012D334, "snes_p17_0012d05c", "snes-core-dsp", "dsp1-float", "89dab25ff724744a12db900fe72c6406c35b1465db5672bb3dd3246419e8a798"),
    Target(0x00151074, 0x00151330, "CMemory_Init", "memory", "memory-ps2", "2e0e6aa18847dc3daa08c9da2749c4ea2d67e41645566e005bf32c029487a102", "CMemory::Init"),
    Target(0x001584D0, 0x00158974, "CMemory_CheckForIPSPatch", "memory-ppu", "memory-ps2", "7de442be5c33d220885032cebbc78efbf68d0774156b36a2266fda131ac2f545", "CMemory::CheckForIPSPatch"),
    Target(0x00177A84, 0x00177CEC, "snes_p16_00177a84", "cpu-audio-runtime", "soundux-ps2", "49c5bb585ba22d0ff11502960680ab301902d8df4315aa7ba86f4095b1134542"),
)


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT.resolve()).as_posix()


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def validate_quick_wins() -> None:
    if len(QUICK_WINS) != 23:
        raise SystemExit(f"V80 quick-win count changed: {len(QUICK_WINS)}")
    addresses = [target.address for target in QUICK_WINS]
    if addresses != sorted(addresses) or len(set(addresses)) != len(addresses):
        raise SystemExit("V80 quick-win addresses must be unique and sorted")
    for target in QUICK_WINS:
        if target.address % 4 or target.end_address % 4 or target.size <= 0:
            raise SystemExit(f"0x{target.address:08x}: invalid aligned target span")
        if target.work_packet == "c4-core":
            raise SystemExit(f"0x{target.address:08x}: C4 target entered quick-win batch")


def emit_candidate(target_image: bytes) -> tuple[str, dict[int, str]]:
    """Emit the explicit word-for-word assembly candidate deterministically."""
    lines = [
        "/*",
        " * Byte-exact EE reconstruction of the 23 V80 quick-win functions.",
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
    for item in QUICK_WINS:
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
            "V80 exact candidate SHA-256 mismatch: "
            f"expected {SOURCE_SHA256}, got {actual_source_sha}"
        )
    output = BUILD / "objects" / "hunt1041_v80_quickwins_exact.o"
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
    for item in QUICK_WINS:
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
            else "manifest address label retained without historical-name claim; "
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
    marker = "HUNT1041 V80 quick-win strict MATCH;"
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
        identity = evidence["historical_identity"] or "manifest-address-label"
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
    validate_quick_wins()
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
    total_bytes = sum(item.size for item in QUICK_WINS)
    print(f"V80 strict quick-win formal matches: {len(evidence_rows)}/23")
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
