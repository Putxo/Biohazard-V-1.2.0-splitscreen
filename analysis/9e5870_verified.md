# 0x009E5870 — Split UI dispatcher verification

Target: RE5DX9 1.2.0 unpacked runnable candidate, SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

Function boundary: `0x009E5870..0x009E5DD0`, followed by INT3 at `0x009E5DD3`.
Native ABI: thiscall; seven 32-bit stack arguments; `RET 0x1C`.

Verified control flow:

1. `0x799AF0(-1)` gate. Nonzero returns false.
2. `self+0x34` must equal input manager `+0x614` or be `-1`; otherwise false.
3. Explicit native event branches cover `0x112`, `0x115`, `0x118`, `0x14D`, `0x2B8`, `0x2B9`, `0x2BE`, `0x2BF`, `0x2C5`, `0x2C6`, `0x2C8`, and `0x694`.
4. Unhandled events scan the table at `0x01121C98`, stride `0x18`, until the following type-4 sentinel. Exactly 55 records precede the sentinel.
5. Matching records execute the verified `0x009E5C6E..0x009E5DD0` override/transform/draw path. No match returns false.

The `0x14D` shared-tail path was rechecked specifically: after drawing resource `0x14E`, native `0x9E5AC3` jumps into `0x9E593E`, which performs both `0x97E` and `0x97D` draws with the usual `cursor += -2 - extent` update between them.

The repository now contains:
- `src/split_explicit_fallback_cases_120.cpp`
- `src/split_table_fallback_120.cpp` with all 55 exact records
- `src/split_table_draw_120.cpp`
- `src/split_ui_dispatch_120.cpp` as the complete reconstructed dispatcher

`split_ui_dispatch_120.cpp` standalone cross-compile check: PASS as Intel i386 COFF using Clang with freestanding stubs.
