#!/usr/bin/env python3
"""Generate the V80 address-level map for the 20 unmatched functions."""
from __future__ import annotations

import argparse
import csv
import io
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
READINESS = ROOT / "analysis" / "source_readiness.csv"
OUTPUT = ROOT / "analysis" / "matching" / "hunt1041-v80-frontier-map-20.tsv"
EXPECTED_FRONTIER = 20
MATCHING_GATE = (
    "strict EE GCC 3.2.2 object vs hash-pinned unpacked ELF; "
    "precise MIPS relocation masks only"
)
FIELDS = (
    "address",
    "name",
    "area",
    "track",
    "work_packet",
    "historical_identity",
    "working_translation_unit",
    "priority",
    "source_model",
    "matching_gate",
    "next_action",
)


@dataclass(frozen=True)
class Packet:
    addresses: tuple[int, ...]
    track: str
    name: str
    translation_unit: str
    priority: str
    next_action: str
    identities: tuple[tuple[int, str], ...] = ()


PACKETS = (
    Packet(
        (
            0x00100114, 0x0010038C, 0x001005EC, 0x001008DC, 0x00100E78,
            0x001019A8, 0x00101B64, 0x00101EF0, 0x00102AB0, 0x00103314,
            0x00103B34, 0x00103DD4, 0x00104234, 0x00104358, 0x00104418,
        ),
        "frontend-ownership",
        "frontend-ui",
        "src/ps2/frontend_ui_recovered.cpp",
        "P2",
        "freeze UI/GS class and global ownership; compile the corridor as EE C++",
    ),
    Packet(
        (0x00104A54, 0x00104BBC),
        "frontend-ownership",
        "frontend-pad",
        "src/ps2/frontend_pad_recovered.cpp",
        "P1",
        "replace the rejected libmtap hypothesis with proven pad state/layout and calls",
    ),
    Packet(
        (
            0x00104F18, 0x00105898, 0x00105AE8, 0x00105E48, 0x001060DC,
            0x001064C0, 0x0010689C, 0x00106CA0, 0x00107358,
        ),
        "frontend-ownership",
        "frontend-lifecycle",
        "src/app/main_frontend_recovered.cpp",
        "P1",
        "freeze path, memory-card and application globals plus object/link order",
    ),
    Packet(
        (
            0x0010C6F8, 0x0010CFA4, 0x0010D7DC,
        ),
        "historical-source",
        "c4-core",
        "src/snes9x/c4_ps2_recovered.cpp",
        "P1",
        "recover c4emu unaligned-word access, state layout and remaining PS2 deltas",
        (
            (0x0010C6F8, "C4DoScaleRotate"),
            (0x0010CFA4, "C4TransformLines"),
            (0x0010D7DC, "S9xSetC4"),
        ),
    ),
    Packet(
        (0x0012C558, 0x0012CBD8, 0x0012D05C),
        "historical-source",
        "dsp1-float",
        "src/snes9x/dsp1_ps2_recovered.cpp",
        "P2",
        "recover the SNES Station floating implementation and exact DSP globals",
    ),
    Packet(
        (0x00151074, 0x001584D0),
        "historical-source",
        "memory-ps2",
        "src/snes9x/memory_ps2_recovered.cpp",
        "P1",
        "recover allocator ownership and the PS2 stream/IPS patch behavior",
        (
            (0x00151074, "CMemory::Init"),
            (0x001584D0, "CMemory::CheckForIPSPatch"),
        ),
    ),
    Packet(
        (0x001728D4,),
        "historical-source",
        "snapshot-zsnes",
        "src/snes9x/snapshot_ps2_recovered.cpp",
        "P2",
        "recover the fio path, unaligned reads and target state layout",
        ((0x001728D4, "S9xUnfreezeZSNES"),),
    ),
    Packet(
        (0x00175300, 0x00175CB4, 0x00176594, 0x00177A84, 0x00177E6C),
        "historical-source",
        "soundux-ps2",
        "src/snes9x/soundux_ps2_recovered.cpp",
        "P1",
        "freeze PS2 channel/state layout and reproduce mixer arithmetic together",
    ),
    Packet(
        (0x001806A4, 0x00180B80, 0x00182984),
        "historical-source",
        "spc7110-cache",
        "src/snes9x/spc7110_ps2_recovered.cpp",
        "P1",
        "recover custom cache/index records and PS2 fio ownership as one object",
    ),
)

# V80 promotes the 23 smallest non-C4 spans from the frozen V79 queue.  Keep
# their packet metadata above so the transition from 43 to 20 is audited rather
# than silently deleting historical ownership information.
PROMOTED_V80 = frozenset(
    {
        0x00100114,
        0x0010038C,
        0x001005EC,
        0x001019A8,
        0x00101B64,
        0x00103B34,
        0x00103DD4,
        0x00104234,
        0x00104358,
        0x00104A54,
        0x00104BBC,
        0x00105898,
        0x00105AE8,
        0x00105E48,
        0x001060DC,
        0x001064C0,
        0x0010689C,
        0x00107358,
        0x0012CBD8,
        0x0012D05C,
        0x00151074,
        0x001584D0,
        0x00177A84,
    }
)


def read_rows(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream))


def packet_index() -> tuple[dict[int, Packet], dict[int, str]]:
    packets: dict[int, Packet] = {}
    identities: dict[int, str] = {}
    for packet in PACKETS:
        packet_identities = dict(packet.identities)
        if set(packet_identities) - set(packet.addresses):
            raise ValueError(f"{packet.name}: identity outside packet")
        for address in packet.addresses:
            if address in packets:
                raise ValueError(f"duplicate packet address 0x{address:08x}")
            packets[address] = packet
            if address in packet_identities:
                identities[address] = packet_identities[address]
    return packets, identities


def render() -> str:
    targets = read_rows(TARGETS)
    readiness = {int(row["address"], 0): row for row in read_rows(READINESS)}
    packets, identities = packet_index()
    unmatched = {
        int(row["address"], 0): row
        for row in targets
        if row["status"] != "MATCHING"
    }
    if len(unmatched) != EXPECTED_FRONTIER:
        raise ValueError(
            f"V80 frontier expected {EXPECTED_FRONTIER} entries, found {len(unmatched)}"
        )
    expected_unmatched = set(packets) - PROMOTED_V80
    if set(unmatched) != expected_unmatched:
        missing = sorted(set(unmatched) - expected_unmatched)
        stale = sorted(expected_unmatched - set(unmatched))
        raise ValueError(
            "V80 packet coverage changed: "
            f"missing={[f'0x{x:08x}' for x in missing]}; "
            f"stale={[f'0x{x:08x}' for x in stale]}"
        )

    output = io.StringIO(newline="")
    writer = csv.DictWriter(output, fieldnames=FIELDS, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    for address, target in sorted(unmatched.items()):
        ready = readiness.get(address)
        if ready is None:
            raise ValueError(f"0x{address:08x}: source readiness missing")
        if (
            ready["name"] != target["name"]
            or ready["area"] != target["area"]
            or ready["manifest_status"] != target["status"]
        ):
            raise ValueError(f"0x{address:08x}: manifest/readiness identity mismatch")
        packet = packets[address]
        writer.writerow(
            {
                "address": f"0x{address:08x}",
                "name": target["name"],
                "area": target["area"],
                "track": packet.track,
                "work_packet": packet.name,
                "historical_identity": identities.get(address, ""),
                "working_translation_unit": packet.translation_unit,
                "priority": packet.priority,
                "source_model": ready["source_files"],
                "matching_gate": MATCHING_GATE,
                "next_action": packet.next_action,
            }
        )
    return output.getvalue()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    content = render()
    if args.check:
        if not OUTPUT.is_file() or OUTPUT.read_text(encoding="utf-8") != content:
            raise SystemExit(
                f"stale {OUTPUT.relative_to(ROOT)}; run tools/update_frontier_map.py"
            )
        action = "verified"
    else:
        OUTPUT.parent.mkdir(parents=True, exist_ok=True)
        OUTPUT.write_text(content, encoding="utf-8")
        action = "wrote"
    print(f"{action} {OUTPUT.relative_to(ROOT)} ({EXPECTED_FRONTIER} entries)")


if __name__ == "__main__":
    main()
