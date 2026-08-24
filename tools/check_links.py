#!/usr/bin/env python3
"""Check local Markdown links in maintained project documentation."""
from __future__ import annotations

import re
import urllib.parse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
LINK_RE = re.compile(r"!?\[[^\]]*\]\((?P<target><[^>]+>|[^)\s]+)(?:\s+['\"][^)]*['\"])?\)")
IGNORED_PREFIXES = ("http://", "https://", "mailto:", "data:", "#")


def maintained_markdown() -> list[Path]:
    result = []
    for path in ROOT.rglob("*.md"):
        relative = path.relative_to(ROOT)
        if any(part in {".git", "build", "__pycache__"} for part in relative.parts):
            continue
        if relative.parts[:2] == ("docs", "archive"):
            continue
        if relative.parts and relative.parts[0] == "third_party" and path.name != "README.md":
            continue
        result.append(path)
    return sorted(result)


def link_target(raw: str) -> str | None:
    target = raw[1:-1] if raw.startswith("<") and raw.endswith(">") else raw
    target = urllib.parse.unquote(target).split("#", 1)[0]
    if not target or target.startswith(IGNORED_PREFIXES):
        return None
    if "://" in target or "{" in target or "}" in target:
        return None
    return target


def main() -> None:
    failures: list[str] = []
    checked = 0
    root_resolved = ROOT.resolve()
    for document in maintained_markdown():
        text = document.read_text(encoding="utf-8", errors="replace")
        for match in LINK_RE.finditer(text):
            target = link_target(match.group("target"))
            if target is None:
                continue
            checked += 1
            candidate = (document.parent / target).resolve()
            try:
                candidate.relative_to(root_resolved)
            except ValueError:
                failures.append(f"{document.relative_to(ROOT)}: link escapes repository: {target}")
                continue
            if not candidate.exists():
                line = text.count("\n", 0, match.start()) + 1
                failures.append(f"{document.relative_to(ROOT)}:{line}: missing {target}")

    if failures:
        raise SystemExit("broken documentation links:\n" + "\n".join(failures))
    print(f"documentation links: OK ({checked} local links)")


if __name__ == "__main__":
    main()
