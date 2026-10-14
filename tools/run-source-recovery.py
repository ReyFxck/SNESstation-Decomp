#!/usr/bin/env python3
"""Run every maintained source-recovery proof and retain individual logs."""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/source-recovery"


def proof_scripts():
    return sorted([*(ROOT / "tools").glob("run-*-source-recovery.py"),
                   *(ROOT / "tools").glob("run-*-source-recovery.sh")])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--list", action="store_true", help="list all discovered proof scripts")
    args = parser.parse_args()
    scripts = proof_scripts()
    if not scripts:
        raise SystemExit("no source-recovery proofs found")
    if args.list:
        for script in scripts:
            print(script.relative_to(ROOT))
        return
    BUILD.mkdir(parents=True, exist_ok=True)
    results = []
    for index, script in enumerate(scripts, 1):
        command = [sys.executable if script.suffix == ".py" else "bash", str(script)]
        log = BUILD / (script.stem + ".log")
        with log.open("w", encoding="utf-8") as output:
            result = subprocess.run(command, cwd=ROOT, stdout=output, stderr=subprocess.STDOUT)
        record = {"script": script.relative_to(ROOT).as_posix(),
                  "exit_code": result.returncode, "log": log.relative_to(ROOT).as_posix()}
        results.append(record)
        print(f"[{index}/{len(scripts)}] {'PASS' if result.returncode == 0 else 'FAIL'} "
              f"{script.name}; log={record['log']}", flush=True)
        (BUILD / "report.json").write_text(json.dumps(results, indent=2) + "\n")
    failed = [r for r in results if r["exit_code"]]
    print(f"source recovery proofs: {len(results) - len(failed)}/{len(results)} passed")
    if failed:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
