# Direct split-core audit — 2026-09-10

Reference binary: RE5DX9 1.2.0 unpacked runnable candidate, image base 0x00400000.

## 0x009E5870..0x009E5DD0

Whole native dispatcher is now reconstructed end-to-end from direct disassembly:

- thiscall, ECX=self, seven stack arguments, `RET 0x1C`;
- preguard calls `0x799AF0(input,-1)` and rejects on nonzero AL;
- `self+0x34` must equal `input+0x614` or be `-1`;
- all explicit event branches are represented in `split_explicit_fallback_cases_120.cpp`;
- generic lookup scans the 55 records at `0x1121C98`, 24-byte stride;
- matched negative resource id consumes without drawing;
- nonnegative records enter the verified `0x9E5C6E..0x9E5DD0` coordinate/draw path.

The consolidated C++ entry point is `SplitUiTableFallback_9E5870` in `src/split_ui_dispatch_120.cpp`. The split-specific control flow is therefore considered VERIFIED.

## 0x00A22C00..0x00A22CF6

Corrected native ABI fidelity:

- thiscall with ECX=self and no stack arguments (`RET`);
- split state is read from global `0x123457C`;
- render dimensions are read from global `0x12345D4`;
- session is reached through `[0x12340A4]+0x1042C`;
- mapped device is `session+0x490[playerIndex]`;
- status-1 + split-active + !effective-FULL applies the exact 16:9 X-origin correction.

The earlier C++ form incorrectly modeled split state and render dimensions as caller parameters. `src/split_a22c00_120.cpp` now matches the native object/global flow.

## 0x00A1B240..0x00A1B2D8 / caller at 0x00A1E650

Direct audit correction:

- `0xA1B240` is thiscall on the same owner object used by the caller;
- width/height cache lives at owner `+0x50/+0x54`, effective FULL cache at `+0x58`;
- only confirmed split rebuild caller is the function beginning at `0xA1E650`, callsite `0xA1E653`;
- when AL is nonzero, `0xA1E65C` calls `0xA1BE00` with the same ECX object;
- `0xA1B0E0` is a separate neighboring function and is not the parent of this split rebuild path.

`src/split_rebuild_120.cpp` has been corrected accordingly.
