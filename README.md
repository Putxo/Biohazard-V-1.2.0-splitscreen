# Resident Evil 5 1.2.0 — Split-Screen / Local Co-op Decompilation

Target: **RE5DX9 1.2.0 (Master 23 Jan 2023)**.

This repository intentionally covers only the native **split-screen and local cooperative** implementation present in RE5 1.2.0. It is not a general RE5 decompilation and it does not include the original executable or game assets.

## Scope

The reconstruction is limited to:

- two-camera / two-viewport split-screen rendering;
- Default vs Full split-screen layout;
- HUD, text and UI transforms/clipping for each local viewport;
- local J2 input assignment (keyboard/controller/device slot routing);
- native Add Player / local-player activation flow;
- Story local-coop join flow;
- Lost in Nightmares local-coop join flow;
- Desperate Escape local-coop join flow;
- Mercenaries split-screen join flow;
- Mercenaries Reunion split-screen join flow;
- supporting helpers required by those systems.

## Reference binary

The reverse-engineering work uses the unpacked runnable 1.2.0 analysis candidate previously identified for this project:

`SHA-256 1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`

The packed retail executable is not committed here.

## Current progress

### Stage 1

Recovered/identified:

- `0x00763170` — effective Full split predicate.
- `0x0076C0C0` — central split geometry calculation.
- `0x00723460` — recurring native local-join detector.
- `0x00720CE0` — transition into native Add Player state.
- `0x00716720` — activate/deactivate local player slot.
- `0x00725C23` — state-13 callsite into local-player activation.

### Stage 2

`0x0076C1F0` has been promoted from Discovered to Partial and reconstructed as the split activation/resource-lifecycle routine:

- updates `splitActive` at `+0x3064`;
- general aspect threshold `0.5625`;
- special status-7 path involving threshold `0.425`;
- allocates a native `0x230` secondary split resource when needed;
- stores it at `+0x3008`;
- sets resource flag `0x2000`;
- registers it using native id `0x1E`;
- cleanup clears `0x2000`, calls virtual release at vtable `+0x30`, and nulls the pointer.

Next render anchors added to the catalog:

- `0x00A23A00` — split draw operation.
- `0x00A23B40` — secondary split draw operation.
- `0x00A24850..0x00A24C70` — larger split-only render/UI family.

The next pass follows those functions to recover viewport-specific rendering and HUD/text containment.

## Status rules

- `Discovered`: address/function role identified but not reconstructed.
- `Partial`: substantial behavior reconstructed, unresolved helpers/semantics remain.
- `Verified`: instruction-level behavior is sufficiently established from the binary evidence available in this project.

No function is marked Verified only because it has a plausible pseudocode translation.
