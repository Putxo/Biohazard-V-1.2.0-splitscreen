# RE5DX9 1.2.0 — 0xB793B0 cursor audit

Direct source: unpacked runnable RE5DX9 1.2.0, SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Verified slices

### 0xB793B0..0xB794A9 — cursor acquisition / pre-clamp

- Native ABI is `thiscall`, `ECX=self`.
- Calls `0x84CF30` first.
- Clears `self+0x1F54` and byte `self+0x2920`.
- If `self+0x2DA8 != 0`, skips cursor acquisition and continues at the post-clamp parent path.
- Cursor input source is `self+0x2D64 + 0x708`.
- If byte `self+0x2918 != 0` and previous byte `self+0x2919 == 1`, cursor floats accumulate source deltas `+0x20C/+0x210`.
- Otherwise byte `self+0x291A` must be nonzero; cursor floats are replaced by integer absolute coordinates `+0x1F8/+0x1FC`, then `+0x291A` is cleared.
- Byte `self+0x2D9C` is passed to `0x76A1E0` with split state `0x123457C`; the returned value is not consumed by the following block.
- Render dimensions are read from `0x12345D4 +0x50/+0x54`.
- `CVTTSS2SI` snapshots cursor X/Y to integer locals **before** the split-specific clamp begins at `0xB794AD`.

### 0xB794AD..0xB79659 — split clamp

Already reconstructed in `src/split_cursor_120.cpp`.

### 0xB79659..0xB796A1 — final hard screen bounds

The final 0..width/height hard clamp uses the integer snapshots captured at `0xB79482` and `0xB7948F`, not a fresh conversion of the possibly modified float cursor after the split clamp.

Exact behavior:

```cpp
if (rawX < 0) cursorX = 0.0f;
if (rawX > renderWidth) cursorX = float(renderWidth);
if (rawY < 0) cursorY = 0.0f;
if (rawY > renderHeight) cursorY = float(renderHeight);
```

The parent then copies `+0x2918 -> +0x2919`, clears `+0x2918`, and continues into unrelated cursor/UI update work.

## Reconstruction

`src/split_cursor_input_120.cpp` contains:

- `AcquireCursorForSplitClamp_B793B0`
- `ApplyCursorScreenBounds_B79659`

Standalone compile verification:

- Clang target: `i686-pc-windows-msvc`
- C++17 freestanding
- Result: **Intel i386 COFF object — PASS**

The parent `0xB793B0` remains `Partial` because code after `0xB796A1` contains substantial unrelated cursor/UI state updates that are outside the split-specific scope.
