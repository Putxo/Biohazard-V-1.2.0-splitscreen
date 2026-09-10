# RE5DX9 1.2.0 — 0x00A2D7F0 verification

Exact binary audited: unpacked runnable 1.2.0 candidate, SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Function

- VA: `0x00A2D7F0..0x00A2D864`
- stack cleanup: `RET 8`
- two stack arguments
- calls `0x7B4370` unconditionally using global UI/input manager `0x11B20C4`
- split-only second update runs only when `0xC42D90(...) == 1`

## Exact status-1 calculation

Native instructions compute:

1. `ratio720 = (renderHeight * 720) / renderWidth` using signed `IDIV`.
2. Divide that result by four with `CDQ; AND EDX,3; ADD EAX,EDX; SAR EAX,2`, i.e. signed truncation toward zero.
3. Convert to float, multiply by the constant at `0xF650E4`.
4. Raw PE value at `0xF650E4` is `0xBE800000`, exactly `-0.25f`.
5. Convert with `CVTTSS2SI`, therefore truncate toward zero again.
6. Call `0x7B4480` with native stack arguments `{0, computedValue, arg2}`.

The reconstruction is in `src/split_aspect_input_120.cpp` and standalone compilation succeeded as Intel i386 COFF using Clang Win32 freestanding target.
