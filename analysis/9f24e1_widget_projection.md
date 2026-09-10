# RE5DX9 1.2.0 — 0x009F24E1 split widget projection

Status: **Verified isolated block**

Directly audited from the unpacked runnable 1.2.0 executable (`SHA-256 1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`).

## Native range

`0x009F24E1..0x009F2661`, inside the larger `0x009F2280` widget/update routine.

The block consumes render width/height from global `0x012345D4`, split state from `0x0123457C`, owner fields `+0xE0` and `+0x1AA`, and the two precomputed floating extents held at `[esp+0x20]` / `[esp+0x24]`.

## Exact constants

- `0xF554EC = 1.0f`
- `0x10BB498 = 0.5f`
- `0xF597FC = 1.7777777910232544f`
- `0x10BCCDC = 0.5625f`
- `0xF52EAC = 1280.0f`
- `0xF52EB0 = 720.0f`
- `0xF9E534 = 1.009374976158142f`
- `0xF9E530 = 0.9906250238418579f`

## Branch structure

When split `+0x3064` is active and owner `+0xE0 != 0`, the native code calls `0x763170` (effective FULL predicate). If FULL is not effective, owner `+0x1AA` selects the two asymmetric edge factors `1.009375` / `0.990625`. Otherwise the active branch uses `(renderWidth/renderHeight)*0.5625`.

When split is inactive, owner `+0xE0 == 0` uses `(16/9)/(renderWidth/renderHeight)` as the vertical normalization factor; nonzero `+0xE0` uses `1.0f`.

Both outputs are truncated with `CVTTSS2SI` and immediately converted back to float with `CVTSI2SS`, matching the C++ reconstruction in `src/split_widget_projection_120.cpp`.

## Build verification

The reconstructed source compiled successfully with Clang 17 targeting `i686-pc-windows-msvc`; resulting object identified as **Intel i386 COFF**.
