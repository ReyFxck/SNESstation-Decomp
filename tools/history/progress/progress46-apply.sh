#!/usr/bin/env bash
set -euo pipefail
cd "${1:-$PWD}"

[[ -f tools/score_cdvd_candidate.py ]] || {
  echo "Run from the SNESstation-Decomp repository root after Progress 45." >&2
  exit 2
}

python3 - <<'PY'
from pathlib import Path

path = Path("tools/score_cdvd_candidate.py")
text = path.read_text(encoding="utf-8")

old = 'def load_compare(root: Path):\n    path = root / "tools" / "compare_elf_functions.py"\n    spec = importlib.util.spec_from_file_location("compare_elf_functions_local", path)\n    if spec is None or spec.loader is None:\n        raise SystemExit("cannot load compare_elf_functions.py")\n    module = importlib.util.module_from_spec(spec)\n    spec.loader.exec_module(module)\n    return module\n'
new = 'def load_compare(root: Path):\n    import sys\n\n    path = root / "tools" / "compare_elf_functions.py"\n    module_name = "compare_elf_functions_local"\n    spec = importlib.util.spec_from_file_location(module_name, path)\n    if spec is None or spec.loader is None:\n        raise SystemExit("cannot load compare_elf_functions.py")\n\n    module = importlib.util.module_from_spec(spec)\n\n    # Python 3.13 dataclasses resolve class annotations through sys.modules\n    # while decorators execute. Register this transient module exactly as\n    # normal import machinery would before exec_module().\n    sys.modules[module_name] = module\n    try:\n        spec.loader.exec_module(module)\n    except Exception:\n        sys.modules.pop(module_name, None)\n        raise\n\n    return module\n'

if old in text:
    path.write_text(text.replace(old, new, 1), encoding="utf-8")
    print("Progress 46 applied.")
elif new in text:
    print("Progress 46 already applied.")
else:
    raise SystemExit("Unexpected tools/score_cdvd_candidate.py; loader block not found.")
PY

python3 -m py_compile tools/score_cdvd_candidate.py

echo
echo "Run only:"
echo "  ./tools/run-cdvd-frontier.sh"
