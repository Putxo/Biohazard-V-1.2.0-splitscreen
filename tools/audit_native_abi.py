#!/usr/bin/env python3
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
SRC = ROOT / "src"

# Native executable VAs whose only declaration must live in
# include/re5/native_abi_audit_120.hpp. Translation units receive that header
# through CMake's forced include and must never redeclare these symbols locally;
# a local prototype with a slightly different pointer type/calling convention
# silently creates a second C++ overload and can leave an unresolved fake symbol
# in the static archive while compilation still succeeds.
CANONICAL = (
    "QueryGameStatus_C42D90",
    "SessionSetSlotMode_C42A30",
    "SessionSetDevice_C42A50",
    "SetSessionLocalPair_C42A70",
    "SetSessionPartnerPair_C42A90",
    "SetSessionAuxPair_C42AB0",
    "CountActiveSessionSlots_C42B60",
    "IsMercsRow0_C42E80",
    "IsMercsRow1_C42EA0",
    "IsMercsLocalSplitRow_C42EC0",
    "IsDlcRow0_C432E0",
    "IsDlcRow1_C43300",
    "IsDlcLocalSplitRow_C43320",
    "IsNativeLocalCoopActive_C43BB0",
    "PreferredOwnerAllowsCurrentKeyboard_799B50",
    "SplitCoordTransform_76A420",
    "FullCoordTransform_76A460",
    "SplitVerticalTransform_76A4A0",
    "UiCommand4370_7B4370",
    "UiCommand4480_7B4480",
    "UiRoute5620_7B5620",
    "UiRoute5750_7B5750",
    "UiRoute43C0_7B43C0",
    "UiGetCursor_7B52A0",
    "UiGetSelection_7B5360",
    "UiOwnedInput38_7B4660",
    "UiSelectionStep_7B5900",
    "UiSelectionStepBy_7B59B0",
    "RootSetFlags_726120",
    "SetLocalPlayerActive_716720",
)

# Historical names that represented the same executable VA with a different
# C++ symbol. They are forbidden in src/ so the archive cannot regress to
# duplicate/fake native symbols.
STALE_ALIASES = (
    "IsMercsLocalSplit_C42EC0",
)

extern_re = re.compile(
    r"^\s*extern\b[^;\n]*\b(" + "|".join(map(re.escape, CANONICAL)) + r")\s*\(",
    re.MULTILINE,
)

errors: list[str] = []
for path in sorted(SRC.glob("*.cpp")):
    text = path.read_text(encoding="utf-8")
    rel = path.relative_to(ROOT)

    for match in extern_re.finditer(text):
        line = text.count("\n", 0, match.start()) + 1
        errors.append(
            f"{rel}:{line}: local redeclaration of canonical native ABI symbol "
            f"{match.group(1)}"
        )

    for alias in STALE_ALIASES:
        for match in re.finditer(r"\b" + re.escape(alias) + r"\b", text):
            line = text.count("\n", 0, match.start()) + 1
            errors.append(f"{rel}:{line}: stale native alias {alias}")

if errors:
    print("Native ABI audit FAILED:")
    for error in errors:
        print("ERROR:", error)
    sys.exit(1)

print(
    f"Native ABI audit passed: {len(CANONICAL)} canonical symbols have no local "
    f"extern redeclarations and {len(STALE_ALIASES)} stale alias set is absent."
)
