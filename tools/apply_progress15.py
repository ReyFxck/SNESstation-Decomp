#!/usr/bin/env python3
"""Apply Progress 15 on top of the successfully applied Progress 14 tree."""
from __future__ import annotations

import csv
import shutil
import subprocess
import tempfile
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

PROMOTIONS = {
    "0x00143780": {
        "name": "renderer_4bpp_setup",
        "status": "RECONSTRUCTED",
        "confidence": "very-high",
        "notes": "Progress 15: complete 0x00143780..0x001437a4 leaf recovered; implicit $t4-indexed cache word set to -1 and renderer-ready byte cleared",
    },
    "0x001a3380": {
        "name": "_fpadd_parts_d",
        "status": "RECONSTRUCTED",
        "confidence": "very-high",
        "notes": "Progress 15: complete soft-double parts add core recovered including special classes, sticky alignment, signed add/subtract and normalization",
    },
    "0x001a8484": {
        "name": "padInit",
        "status": "RECONSTRUCTED",
        "confidence": "very-high",
        "notes": "Progress 15: complete NEW/XPADMAN init flow recovered; both bind/retry delay corridors, mod-version query, 8 state records and command 0x10 RPC represented",
    },
}


def read_csv(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open(newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        if not reader.fieldnames:
            raise SystemExit(f"missing CSV header: {path}")
        return list(reader.fieldnames), list(reader)


def counts_for(rows: list[dict[str, str]]) -> Counter:
    return Counter(r["status"] for r in rows)


def check_input_baseline(rows: list[dict[str, str]]) -> None:
    counts = counts_for(rows)
    reconstructed = counts["RECONSTRUCTED"] + counts["MATCHING"]
    mapped = sum(counts.values()) - counts["UNKNOWN"]
    allowed = {
        (799, 827, 2, 26),
        (802, 827, 1, 24),
    }
    state = (reconstructed, mapped, counts["PARTIAL"], counts["IDENTIFIED"])
    if state not in allowed:
        raise SystemExit(
            "Progress 15 expects a clean P14/P15 manifest state; got "
            f"reconstructed={reconstructed} mapped={mapped} "
            f"partial={counts['PARTIAL']} identified={counts['IDENTIFIED']}"
        )


def patch_csv(path: Path) -> list[dict[str, str]]:
    fields, rows = read_csv(path)
    seen = set()

    for row in rows:
        address = row["address"].lower()
        if address in PROMOTIONS:
            for key, value in PROMOTIONS[address].items():
                if key in row:
                    row[key] = value
            seen.add(address)

    missing = set(PROMOTIONS) - seen
    if missing:
        raise SystemExit(f"{path}: missing Progress 15 targets: {sorted(missing)}")

    tmp = path.with_suffix(path.suffix + ".p15tmp")
    with tmp.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    tmp.replace(path)
    return rows


def validate_manifest_pair(a: list[dict[str, str]],
                           b: list[dict[str, str]]) -> str:
    aa = {r["address"].lower(): r["status"] for r in a}
    bb = {r["address"].lower(): r["status"] for r in b}
    if len(aa) != len(a) or len(bb) != len(b):
        raise SystemExit("duplicate address found in progress manifests")
    if aa != bb:
        raise SystemExit("progress_targets.csv and symbols.csv address/status maps diverged")

    counts = Counter(aa.values())
    reconstructed = counts["RECONSTRUCTED"] + counts["MATCHING"]
    mapped = sum(counts.values()) - counts["UNKNOWN"]
    if reconstructed != 802 or mapped != 827:
        raise SystemExit(
            f"unexpected P15 totals: reconstructed={reconstructed} mapped={mapped}"
        )
    if counts["PARTIAL"] != 1 or counts["IDENTIFIED"] != 24:
        raise SystemExit(
            f"unexpected residual statuses: PARTIAL={counts['PARTIAL']} "
            f"IDENTIFIED={counts['IDENTIFIED']}"
        )
    return (
        f"matching={counts['MATCHING']} reconstructed={reconstructed} mapped={mapped} "
        f"partial={counts['PARTIAL']} identified={counts['IDENTIFIED']}"
    )


def syntax_check() -> int:
    cc = shutil.which("cc")
    if cc is None:
        return -1
    files = sorted((ROOT / "src").rglob("*.c"))
    for src in files:
        subprocess.run(
            [cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-fsyntax-only",
             "-I", str(ROOT / "include"), str(src)],
            cwd=ROOT,
            check=True,
        )
    return len(files)


SMOKE_C = r"""
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t fp_class;
    uint32_t sign;
    int32_t exponent;
    uint32_t pad_0c;
    uint64_t fraction;
} DfParts;

extern void snes___unpack_d(const uint64_t *, DfParts *);
extern uint64_t snes___pack_d(const DfParts *);
extern const DfParts *snes_p15_fpadd_parts_d_001a3380(
    const DfParts *, const DfParts *, DfParts *);

typedef int (*BindFn)(unsigned, uint32_t, void *);
typedef int (*BoundFn)(unsigned, void *);
typedef void (*DelayFn)(uint32_t, void *);
typedef int (*ModFn)(void *);
typedef int (*CallFn)(unsigned, unsigned, unsigned, void *, unsigned,
                      void *, unsigned, void *, void *, void *);

typedef struct {
    uint32_t word00, word04, word08, word0c, word10;
    uint32_t word14, word18, word1c, word20, word24;
} PadState;

typedef struct {
    int initialised;
    uint8_t rpc_buffer[128];
    PadState state[8];
    BindFn bind;
    BoundFn is_bound;
    DelayFn delay;
    ModFn get_mod_version;
    CallFn call;
    void *opaque;
} PadRuntime;

extern int snes_p15_padInit_001a8484(PadRuntime *, int);
extern void snes_p15_renderer_4bpp_setup_00143780(
    int32_t *, unsigned, uint8_t *);

typedef struct {
    unsigned binds[2];
    unsigned delays[2];
    unsigned mod_calls;
    unsigned rpc_calls;
    int failed;
} PadProbe;

static int bind_cb(unsigned client, uint32_t id, void *opaque)
{
    PadProbe *p = (PadProbe *)opaque;
    uint32_t want = client == 0 ? UINT32_C(0x80000100) : UINT32_C(0x80000101);
    if (client > 1 || id != want) {
        p->failed = 1;
        return -1;
    }
    ++p->binds[client];
    return 0;
}

static int bound_cb(unsigned client, void *opaque)
{
    PadProbe *p = (PadProbe *)opaque;
    return p->binds[client] >= 2;
}

static void delay_cb(uint32_t iterations, void *opaque)
{
    PadProbe *p = (PadProbe *)opaque;
    unsigned client = p->binds[1] ? 1u : 0u;
    if (iterations != UINT32_C(0x00100001))
        p->failed = 1;
    ++p->delays[client];
}

static int mod_cb(void *opaque)
{
    PadProbe *p = (PadProbe *)opaque;
    ++p->mod_calls;
    return 0x1234;
}

static int call_cb(unsigned client, unsigned function, unsigned mode,
                   void *send, unsigned send_size, void *recv,
                   unsigned recv_size, void *end_function,
                   void *end_parameter, void *opaque)
{
    PadProbe *p = (PadProbe *)opaque;
    uint32_t command = 0;
    memcpy(&command, send, sizeof(command));
    if (client != 0 || function != 1 || mode != 0 ||
        send != recv || send_size != 0x80 || recv_size != 0x80 ||
        end_function != NULL || end_parameter != NULL ||
        command != UINT32_C(0x10))
        p->failed = 1;
    ++p->rpc_calls;
    return 0;
}

static uint64_t rng_state = UINT64_C(0x8d26f3a95b1c47e1);

static uint64_t rng64(void)
{
    uint64_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng_state = x;
    return x;
}

static double bits_to_double(uint64_t u)
{
    double d;
    memcpy(&d, &u, sizeof(d));
    return d;
}

static uint64_t double_to_bits(double d)
{
    uint64_t u;
    memcpy(&u, &d, sizeof(u));
    return u;
}

static int test_softadd(void)
{
    unsigned i;
    for (i = 0; i < 200000; ++i) {
        uint64_t ua = rng64();
        uint64_t ub = rng64();
        DfParts a, b, tmp;
        const DfParts *r;
        uint64_t got, expected;
        volatile double da, db, sum;

        if (((ua >> 52) & 0x7ffu) == 0x7ffu)
            ua &= ~(UINT64_C(0x7ff) << 52);
        if (((ub >> 52) & 0x7ffu) == 0x7ffu)
            ub &= ~(UINT64_C(0x7ff) << 52);

        snes___unpack_d(&ua, &a);
        snes___unpack_d(&ub, &b);
        r = snes_p15_fpadd_parts_d_001a3380(&a, &b, &tmp);
        got = snes___pack_d(r);

        da = bits_to_double(ua);
        db = bits_to_double(ub);
        sum = da + db;
        expected = double_to_bits(sum);

        if (got != expected) {
            fprintf(stderr,
                    "softadd mismatch i=%u a=%016llx b=%016llx got=%016llx expected=%016llx\n",
                    i, (unsigned long long)ua, (unsigned long long)ub,
                    (unsigned long long)got, (unsigned long long)expected);
            return 1;
        }
    }

    {
        uint64_t pi = UINT64_C(0x7ff0000000000000);
        uint64_t ni = UINT64_C(0xfff0000000000000);
        DfParts a, b, tmp;
        const DfParts *r;
        snes___unpack_d(&pi, &a);
        snes___unpack_d(&ni, &b);
        r = snes_p15_fpadd_parts_d_001a3380(&a, &b, &tmp);
        if (r->fp_class >= 2u)
            return 2;
    }
    return 0;
}

static int test_pad(void)
{
    PadRuntime rt;
    PadProbe probe;
    unsigned i;

    memset(&rt, 0xa5, sizeof(rt));
    memset(&probe, 0, sizeof(probe));
    rt.initialised = 0;
    rt.bind = bind_cb;
    rt.is_bound = bound_cb;
    rt.delay = delay_cb;
    rt.get_mod_version = mod_cb;
    rt.call = call_cb;
    rt.opaque = &probe;

    if (snes_p15_padInit_001a8484(&rt, 123) != 0)
        return 10;
    if (!rt.initialised || probe.failed ||
        probe.binds[0] != 2 || probe.binds[1] != 2 ||
        probe.delays[0] != 2 || probe.delays[1] != 2 ||
        probe.mod_calls != 1 || probe.rpc_calls != 1)
        return 11;

    for (i = 0; i < 8; ++i) {
        if (rt.state[i].word00 || rt.state[i].word04 || rt.state[i].word08 ||
            rt.state[i].word14 || rt.state[i].word18 || rt.state[i].word1c)
            return 12;
        if (rt.state[i].word0c != UINT32_C(0xa5a5a5a5) ||
            rt.state[i].word10 != UINT32_C(0xa5a5a5a5) ||
            rt.state[i].word20 != UINT32_C(0xa5a5a5a5) ||
            rt.state[i].word24 != UINT32_C(0xa5a5a5a5))
            return 13;
    }
    return 0;
}

static int test_renderer(void)
{
    int32_t words[64];
    uint8_t ready = 1;
    unsigned i;
    for (i = 0; i < 64; ++i)
        words[i] = 123;
    snes_p15_renderer_4bpp_setup_00143780(words, 7, &ready);
    if (ready != 0 || words[7 + 28] != -1)
        return 20;
    if (words[6 + 28] != 123 || words[8 + 28] != 123)
        return 21;
    return 0;
}

int main(void)
{
    int rc;
    rc = test_pad();
    if (rc) return rc;
    rc = test_renderer();
    if (rc) return rc;
    rc = test_softadd();
    if (rc) return rc;
    puts("Progress 15 smoke: PASS");
    return 0;
}
"""


def smoke_check() -> str:
    cc = shutil.which("cc")
    if cc is None:
        return "Progress 15 smoke: SKIPPED (cc not found)"

    with tempfile.TemporaryDirectory(prefix="snes-p15-") as td:
        td_path = Path(td)
        test_c = td_path / "smoke.c"
        exe = td_path / "smoke"
        test_c.write_text(SMOKE_C, encoding="utf-8")
        cmd = [
            cc, "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror",
            str(test_c),
            str(ROOT / "src/ps2/progress15_pad_init_recovered.c"),
            str(ROOT / "src/ps2/progress15_softdouble_add_recovered.c"),
            str(ROOT / "src/snes9x/progress15_renderer_leaf_recovered.c"),
            str(ROOT / "src/ps2/progress13_libgcc_softfloat_recovered.c"),
            "-o", str(exe),
        ]
        subprocess.run(cmd, cwd=ROOT, check=True)
        proc = subprocess.run(
            [str(exe)], cwd=ROOT, text=True, capture_output=True, check=True
        )
        return proc.stdout.strip() or "Progress 15 smoke: PASS"


def main() -> None:
    _, before = read_csv(TARGETS)
    check_input_baseline(before)

    targets = patch_csv(TARGETS)
    symbols = patch_csv(SYMBOLS)
    totals = validate_manifest_pair(targets, symbols)

    subprocess.run(
        ["python3", str(ROOT / "tools" / "update_progress.py")],
        cwd=ROOT, check=True
    )

    checked = syntax_check()
    syntax_line = (
        f"host syntax check: PASS ({checked} recovered C translation units)"
        if checked >= 0 else
        "host syntax check: SKIPPED (cc not found)"
    )
    smoke_line = smoke_check()

    validation = ROOT / "analysis" / "progress15_validation.txt"
    validation.write_text(
        "Progress 15 validation\n"
        "======================\n"
        f"{totals}\n"
        "expected proxy: reconstructed=802/1137=70.54%, mapped=827/1137=72.74%\n"
        "promoted: 0x00143780, 0x001a3380, 0x001a8484\n"
        "remaining PARTIAL: main@0x00104f18\n"
        f"{syntax_line}\n"
        f"{smoke_line}\n",
        encoding="utf-8",
    )

    print(totals)
    print(syntax_line)
    print(smoke_line)
    print("Progress 15 applied successfully")


if __name__ == "__main__":
    main()
