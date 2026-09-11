# Stage 2 — 1.2.0 split activation and secondary resource lifecycle

This stage covers the native 1.2.0 split-screen/local-coop activation and secondary split resource lifecycle.

## 0x0076C1F0

Verified behavior:

- recomputes the split-active boolean at object `+0x3064`;
- general aspect decision uses threshold `0.5625`;
- status `7` uses the verified special threshold `0.4250000119`;
- when split is active and `+0x3008 == nullptr`, allocates `0x230` bytes aligned to `0x10`;
- constructs the secondary split resource through `0xB8D6A0`;
- stores the resulting pointer at split object `+0x3008`;
- sets resource flag `0x2000` at resource `+0x04`;
- registers the resource in the native manager with id `0x1E`.

The cleanup counterpart clears resource bit `0x2000`, invokes the native virtual release at vtable offset `+0x30`, then clears `+0x3008`.

The reconstruction is implemented in `src/split_resource_120.cpp` and is classified `Verified` in the current manifest.

## Downstream render chain

The previously listed follow-up targets are also reconstructed in the current tree:

- `0x00A23A00` — conditional split draw operation;
- `0x00A23B40` — secondary split draw operation;
- `0x00A24850..0x00A24C70` — split-aware render/UI family and associated geometry consumers.

This document is retained as the stage-history record, but there is no remaining `Partial` status for `0x76C1F0` in the authoritative final manifest.
