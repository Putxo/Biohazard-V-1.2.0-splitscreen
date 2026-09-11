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

## Current status — static executable-side scope closed

As of **2026-09-11**, the executable-side native implementation in the scope above has reached static audit closure.

The final sweep covers:

- local-J2 detection, Add Player, activation and transition state machines;
- `mKeyboardPlayerNo` / device ownership and routing for Story, LIN/DE and Mercs/Reunion;
- direct consumers of the split-state object at `0x0123457C`, including `+0x3064..+0x3085`;
- Default/Full viewport and coordinate helpers;
- split HUD, prompts, menu geometry, resources, cursor, projection, aspect correction and hit testing;
- remaining direct split consumers found by the expanded binary sweep;
- canonical native ABI cleanup and final split/J2 semantic side-effect reconciliation.

The expanded split-state audit classified **91 candidate instruction groups by base provenance**; no confirmed direct split-object consumer found by that audit remains unclassified. The corresponding high-confidence `InputManager+0x614` ownership sweep is likewise classified, with unrelated displacement collisions excluded.

The last semantic closure pass also restored the verified native `0xA4AE73 -> 0x7B43C0` Mercenaries frontend completion side effect and reconciled stale stage-analysis documents with the authoritative final manifest.

Final audit:

- `analysis/FINAL_LOCAL_J2_SPLIT_DECOMP_AUDIT_2026-09-11.md`
- `analysis/final_local_j2_split_manifest_2026-09-11.csv`

### Build validation

Commit `1ff76b890bae53ccf9973d3bb58779a8539e3768` passed GitHub Actions **Static Win32 decomp build #212** (`run id 34598435659`).

The target is built for `i686-pc-windows-msvc` in C++17 freestanding mode with `-Wall -Wextra -Wpedantic`. All **132/132** decomp source files are present in the CMake build graph, the canonical native ABI audit passes, the final split/J2 semantic closure audit passes, and the static archive is produced successfully.

CI now guards both build coverage and semantic closure. It rejects stale ABI aliases/redeclarations, unfinished active-source markers, non-closed authoritative manifest entries, stale open-status statements in resolved stage documents, and loss of the verified Mercenaries completion side effect.

This status means the **static/decompilation scope is closed**. It does **not** claim runtime certification in the real Windows/DirectX game. Runtime testing of Story, LIN/DE, Mercs/Reunion, controller/keyboard combinations, Default/Full layouts, aspect changes, leave/rejoin and persistence remains a separate validation phase.

## Status rules

- `OutOfScope`: audited code is not part of the local-J2/split implementation.
- `Verified`: instruction-level behavior is sufficiently established from the binary evidence available in this project.
- `Split-relevant scope complete`: every split/local-J2 block inside a larger generic parent function is reconstructed or explicitly isolated; unrelated parent code is intentionally excluded.

No function is marked Verified only because it has a plausible pseudocode translation.
