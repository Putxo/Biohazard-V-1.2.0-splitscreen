#!/usr/bin/env python3
from __future__ import annotations

import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]
SRC = ROOT / "src"

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

ALIASES = {
    "IsMercsLocalSplit_C42EC0": "IsMercsLocalSplitRow_C42EC0",
    "SplitXTransform_76A420": "SplitCoordTransform_76A420",
}

changed: list[pathlib.Path] = []
for path in sorted(SRC.glob("*.cpp")):
    text = path.read_text(encoding="utf-8")
    original = text

    for old, new in ALIASES.items():
        text = text.replace(old, new)

    # Remove only the individual extern prototype statement. Do not remove a
    # whole source line because several older units place multiple declarations
    # on the same line.
    for name in CANONICAL:
        pattern = re.compile(
            r"(?m)(?<!\w)extern\s+[^;\n]*\b"
            + re.escape(name)
            + r"\s*\([^;]*?\)\s*;\s*"
        )
        text = pattern.sub("", text)

    # These three callers used an obsolete one-argument C++ alias. The native
    # 76A460 ABI is thiscall: ECX=split, stack x.
    if path.name == "split_table_draw_120.cpp":
        text = text.replace(
            "FullCoordTransform_76A460(scaledBaseX)",
            "FullCoordTransform_76A460(split, scaledBaseX)",
        )
    elif path.name in {"split_ui_draw_120.cpp", "split_ui_primary_120.cpp"}:
        text = text.replace(
            "FullCoordTransform_76A460(x)",
            "FullCoordTransform_76A460(split, x)",
        )

    # Final warning cleanup: the const overload was never selected by any
    # native reconstruction and only produced -Wunused-function in the strict
    # Win32 build.
    if path.name == "split_ui_owner_native_routes_120.cpp":
        text = text.replace(
            "static inline const std::uint8_t* Record(const void* self,int index){return B(self)+index*0x70;}\n",
            "",
        )

    if text != original:
        path.write_text(text, encoding="utf-8")
        changed.append(path.relative_to(ROOT))

for path in changed:
    print(path)
print(f"Normalized {len(changed)} source files.")
