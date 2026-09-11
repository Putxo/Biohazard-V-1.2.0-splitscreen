#!/usr/bin/env python3
from __future__ import annotations

import csv
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
ANALYSIS = ROOT / "analysis"

errors: list[str] = []

# Final authoritative manifest must not contain unfinished in-scope work.
manifest = ANALYSIS / "final_local_j2_split_manifest_2026-09-11.csv"
with manifest.open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
for row in rows:
    status = row["status"].strip()
    if status not in {"Verified", "OutOfScope"}:
        errors.append(f"final manifest unfinished status: {row['VA']} {row['name']} -> {status}")

# Active source must not carry ordinary unfinished-work markers.
unfinished = re.compile(r"\b(?:TODO|FIXME|PLACEHOLDER|UNIMPLEMENTED|NOT IMPLEMENTED)\b", re.I)
for path in sorted(SRC.glob("*.cpp")):
    for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if unfinished.search(line):
            errors.append(f"{path.relative_to(ROOT)}:{lineno}: unfinished marker: {line.strip()}")

# High-risk native side effects that must remain represented explicitly.
mercs_completion = (SRC / "split_mercs_frontend_completion_120.cpp").read_text(encoding="utf-8")
required_completion = "UiRoute43C0_7B43C0(gUiInput_11B20C4, 1, 0);"
if required_completion not in mercs_completion:
    errors.append("A4AE73 completion is missing native 7B43C0 routed UI/input reset")

# Historical analysis pages were refreshed when their previously open points
# were closed. Reintroducing these phrases means the repo documentation has
# drifted back to an unfinished state.
closed_docs = {
    ANALYSIS / "frontend_family_identity_120.md": (
        "still needs to be tied",
        "No row label is assigned here without that final trace",
    ),
    ANALYSIS / "frontend_5cc_writers_120.md": (
        "What is proved / not yet proved",
        "Not yet assigned without further class/resource tracing",
    ),
    ANALYSIS / "STAGE2_RESOURCE_LIFECYCLE.md": (
        "function remains `Partial`",
        "These are the next targets",
    ),
}
for path, forbidden in closed_docs.items():
    text = path.read_text(encoding="utf-8")
    for phrase in forbidden:
        if phrase in text:
            errors.append(f"{path.relative_to(ROOT)} still contains stale unfinished statement: {phrase}")

# Every decomp source must remain in the build graph.
cmake = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
for path in sorted(SRC.glob("*.cpp")):
    rel = path.relative_to(ROOT).as_posix()
    if rel not in cmake:
        errors.append(f"source missing from CMakeLists.txt: {rel}")

if errors:
    print("Split/J2 closure audit FAILED:")
    for error in errors:
        print(f"ERROR: {error}")
    sys.exit(1)

print(
    f"Split/J2 closure audit passed: {len(rows)} final manifest rows closed, "
    f"{len(list(SRC.glob('*.cpp')))} sources in build graph, semantic completion guards present."
)
