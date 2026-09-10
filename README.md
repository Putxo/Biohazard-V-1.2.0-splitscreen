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

## Current restart status

This repository was restarted from zero for the split-screen-only decompilation. The first recovered/identified functions are tracked in `analysis/functions.csv` and reconstructed under `src/`.

Current high-confidence anchors:

- `0x00763170` — effective Full split predicate.
- `0x0076C0C0` — central split geometry calculation.
- `0x00723460` — recurring native local-join detector.
- `0x00720CE0` — transition into native Add Player state.
- `0x00716720` — activate/deactivate local player slot.
- `0x00725C23` — state-13 callsite into local-player activation.

## Status rules

- `Discovered`: address/function role identified but not reconstructed.
- `Partial`: substantial behavior reconstructed, unresolved helpers/semantics remain.
- `Verified`: instruction-level behavior is sufficiently established from the binary evidence available in this project.

No function is marked Verified only because it has a plausible pseudocode translation.
