import csv
import hashlib
import shutil
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

import libgcc_contracts as libgcc
import runtime_refactors as runtime


class RuntimeRefactorTests(unittest.TestCase):
    def live(self):
        return (
            libgcc.read_table(libgcc.DEFAULT_EXTERNAL, libgcc.EXTERNAL_FIELDS),
            libgcc.read_table(libgcc.DEFAULT_CONTRACTS, libgcc.CONTRACT_FIELDS),
            libgcc.read_table(libgcc.DEFAULT_FRONTIER, libgcc.FRONTIER_FIELDS),
        )

    def test_four_calls_close_one_source_contract(self):
        rows = runtime.validate_manifest(runtime.parse_args(["validate"]))
        self.assertEqual(4, len(rows))
        self.assertEqual({"snprintf"}, {row["former_contract"] for row in rows})
        self.assertEqual({"sprintf"}, {row["replacement_contract"] for row in rows})
        self.assertEqual(336, sum(int(row["extent_hex"], 0) for row in rows))
        self.assertIn("contracts_closed=1 call_sites=4", runtime.summary())

    def test_ledger_tampering_is_rejected(self):
        rows = runtime.expected_rows()
        rows[0]["callee_address"] = "0x0019e364"
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "manifest.tsv"
            with path.open("w", encoding="utf-8", newline="") as stream:
                writer = csv.DictWriter(stream, runtime.FIELDS, delimiter="\t", lineterminator="\n")
                writer.writeheader()
                writer.writerows(rows)
            args = runtime.parse_args(["validate", "--manifest", str(path)])
            with self.assertRaisesRegex(runtime.RuntimeRefactorError, "ledger drift"):
                runtime.validate_manifest(args)

    def test_snprintf_cannot_reenter_any_live_namespace(self):
        for index in range(3):
            maps = self.live()
            maps[index][0]["symbol"] = "snprintf"
            with self.assertRaisesRegex(runtime.RuntimeRefactorError, "snprintf returned"):
                runtime.validate_live_contracts(*maps)

    def test_wrong_sprintf_target_is_rejected(self):
        maps = self.live()
        next(row for row in maps[1] if row["symbol"] == "sprintf")["target_address"] = "0x0019e364"
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "canonical target drift"):
            runtime.validate_live_contracts(*maps)

    def test_missing_requester_is_rejected(self):
        maps = self.live()
        next(row for row in maps[0] if row["symbol"] == "sprintf")["requesters"] = ""
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "requester ownership"):
            runtime.validate_live_contracts(*maps)

    def test_runtime_shim_is_rejected(self):
        maps = self.live()
        maps[2][0]["resolution_kind"] = "compatibility-runtime-shim"
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "runtime shim returned"):
            runtime.validate_live_contracts(*maps)

    def test_source_regression_is_rejected(self):
        row = runtime.expected_rows()[0]
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / row["source"]
            path.parent.mkdir(parents=True)
            path.write_text("char *snes_dispatch_00105718(char *b, unsigned n) { snprintf(b,n,\"x\"); return b; }\n")
            with self.assertRaisesRegex(runtime.RuntimeRefactorError, "formatter call drift"):
                runtime.validate_sources([row], Path(tmp))

    def fixture(self, target=runtime.CALLEE, extra_call=False):
        raw = struct.pack("<II", 0x0C000000 | (target >> 2),
                          (0x0C000000 | (target >> 2)) if extra_call else 0)
        row = dict(runtime.expected_rows()[0], target_address="0x00100000", extent_hex="0x8",
                   call_address="0x00100000", target_sha256=hashlib.sha256(raw).hexdigest())
        return raw, [row]

    def test_private_decoder_requires_exact_direct_callee(self):
        raw, rows = self.fixture()
        self.assertEqual(1, len(runtime.verify_calls(raw, rows)))
        raw, rows = self.fixture(target=runtime.CALLEE - 4)
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "direct formatter call drift"):
            runtime.verify_calls(raw, rows)

    def test_private_decoder_rejects_duplicate_calls(self):
        raw, rows = self.fixture(extra_call=True)
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "direct formatter call drift"):
            runtime.verify_calls(raw, rows)

    def test_private_decoder_checks_span_hash_and_bounds(self):
        raw, rows = self.fixture()
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "hash drift"):
            runtime.verify_calls(raw[:-1] + b"\x01", rows)
        with self.assertRaisesRegex(runtime.RuntimeRefactorError, "outside reference"):
            runtime.verify_calls(raw[:4], rows)

    def test_private_gate_rejects_noncanonical_image(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "reference.bin"
            path.write_bytes(self.fixture()[0])
            with self.assertRaisesRegex(runtime.RuntimeRefactorError, "frozen unpacked target"):
                runtime.verify_reference(path, runtime.expected_rows())

    @unittest.skipUnless(shutil.which("cc"), "host C compiler unavailable")
    def test_host_numeric_outputs_and_snapshot_capacity(self):
        harness = r'''
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
extern char *snes_dispatch_00105718(char *, size_t);
extern const char *CMemory_StaticRAMSize_00156934(uint8_t, uint32_t, char [32]);
extern const char *CMemory_Size_00156994(uint8_t, char [32]);
extern const char *CMemory_MapMode_00156c0c(uint8_t, char [16]);
int main(void) {
    char buffer[66], expected[64];
    size_t n, used;
    unsigned value;
    const char *path = "cdrom0:\\ROMS\\SNAP";
    assert(snes_dispatch_00105718(NULL, 0) == NULL);
    for (n = 0; n <= 64; ++n) {
        memset(buffer, 0x5a, sizeof(buffer));
        snprintf(expected, sizeof(expected), "%s", path);
        if (n && n <= strlen(path)) expected[n - 1] = '\0';
        assert(snes_dispatch_00105718(buffer + 1, n) == buffer + 1);
        assert(buffer[0] == 0x5a);
        if (n) assert(strcmp(buffer + 1, expected) == 0);
        used = n ? strlen(expected) + 1 : 0;
        while (used < 65) assert(buffer[1 + used++] == 0x5a);
    }
    for (value = 0; value <= 255; ++value) {
        memset(buffer, 0x5a, sizeof(buffer));
        if (value >= 17) strcpy(expected, "Corrupt");
        else if (!value) strcpy(expected, "0Kb");
        else snprintf(expected, sizeof(expected), "%uKb", 1u << value);
        assert(CMemory_StaticRAMSize_00156934(value, 0, buffer + 1) == buffer + 1);
        assert(strcmp(expected, buffer + 1) == 0 && buffer[0] == 0x5a && buffer[33] == 0x5a);
        memset(buffer, 0x5a, sizeof(buffer));
        if (value < 7 || value > 30) strcpy(expected, "Corrupt");
        else snprintf(expected, sizeof(expected), "%uMbits", 1u << (value - 7));
        assert(CMemory_Size_00156994(value, buffer + 1) == buffer + 1);
        assert(strcmp(expected, buffer + 1) == 0 && buffer[0] == 0x5a && buffer[33] == 0x5a);
        memset(buffer, 0x5a, sizeof(buffer));
        snprintf(expected, sizeof(expected), "%02X", value & 0xefu);
        assert(CMemory_MapMode_00156c0c(value, buffer + 1) == buffer + 1);
        assert(strcmp(expected, buffer + 1) == 0 && buffer[0] == 0x5a && buffer[17] == 0x5a);
    }
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            exe = Path(tmp) / "formatter-model-test"
            result = subprocess.run([
                "cc", "-std=c99", "-O2", "-fno-builtin", "-ffunction-sections", "-fdata-sections",
                "-Wl,--gc-sections", "-o", str(exe),
                str(runtime.ROOT / "src/ps2/small_dispatch_recovered.c"),
                str(runtime.ROOT / "src/snes9x/memmap_metadata_recovered.c"), "-x", "c", "-",
            ], input=harness, text=True, capture_output=True)
            self.assertEqual(0, result.returncode, result.stderr)
            result = subprocess.run([str(exe)], text=True, capture_output=True)
            self.assertEqual(0, result.returncode, result.stderr)


if __name__ == "__main__":
    unittest.main()
