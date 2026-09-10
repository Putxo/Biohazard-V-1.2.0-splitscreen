# Stage 2 — 1.2.0 split activation and secondary resource lifecycle

This stage continues only the native 1.2.0 split-screen/local-coop subsystem.

## 0x0076C1F0

Recovered behavior:

- recomputes the split-active boolean written at object `+0x3064`;
- general aspect decision uses threshold `0.5625`;
- status `7` uses a special path involving threshold `0.425`;
- when split is active and `+0x3008 == nullptr`, creates the native secondary split resource;
- resource allocation size is `0x230` bytes;
- resource flag `0x2000` is set at resource `+0x04`;
- the resulting pointer is stored at split object `+0x3008`;
- resource is registered in the native manager using id `0x1E`.

The cleanup counterpart clears resource bit `0x2000`, invokes the native virtual release at vtable offset `+0x30`, then clears `+0x3008`.

## Confidence boundary

The resource size, object fields, flag, registration id and high-level activation flow are supported by the previous direct 1.2.0 analysis. The exact helper names and full status-7 branch are not yet proven, so the reconstruction keeps those helpers abstract and the function remains `Partial`.

## Next functions

The next render chain is:

- `0x00A23A00` — conditional split draw operation;
- `0x00A23B40` — secondary split draw operation;
- `0x00A24850..0x00A24C70` — larger split-only render/UI family.

These are the next targets for recovering viewport-specific rendering, UI geometry and text/HUD containment.
