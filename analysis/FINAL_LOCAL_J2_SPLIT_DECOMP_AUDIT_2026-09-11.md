# Final local-J2 / split-screen executable audit — 2026-09-11

## Target and scope

Target binary: **RE5DX9 1.2.0 (Master 23 Jan 2023)**, unpacked runnable analysis candidate.

SHA-256: `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`

This audit closes the executable-side native implementation needed for:

- local J2 discovery, input ownership and device routing;
- Add Player / local-player activation and transition state machines;
- Story local cooperative join;
- Lost in Nightmares / Desperate Escape local join;
- Mercenaries / Mercenaries Reunion local join;
- split state, viewport and two-player UI routing;
- Default / Full split geometry;
- HUD, prompt, cursor, resource, menu, projection, aspect and hit-test transforms that depend on the split state.

Generic code surrounding an isolated split branch is intentionally not counted as unfinished split work.

## Final direct split-state audit

The expanded audit searched direct consumers of the native split object at `0x0123457C`, including fields `+0x3064..+0x3085`, rather than limiting coverage to the historical `+0x3084/+0x3085` Full-mode audit.

The sweep produced **91 candidate instruction groups**. Every group was classified by pointer/base provenance rather than by displacement alone.

Confirmed split-state consumers are now represented by a reconstructed function or by an explicitly isolated split-specific block in `src/`. Coincidences in unrelated D4/DB/DF/E0/E1/E3 families and stack/object layouts were rejected when the base register was not the split object.

No confirmed direct consumer of the split object found by this audit remains unclassified.

## Final local-J2 ownership audit

A second sweep covered high-confidence references to `InputManager + 0x614` (`mKeyboardPlayerNo`) around the native `0x01249C40` input object.

This includes:

- readers and owner gates;
- writers during local join and frontend transitions;
- keyboard/controller fallback paths;
- Story gameplay owner checks;
- Mercenaries/Reunion owner gates and cleanup;
- LIN/DE join assignment;
- UI owner capture and owner-only routing;
- per-player prompt/device selection;
- two-player frontend loops;
- owner-aware command translation at `0x79A570..0x79A614` and `0x79B8F0..0x79BBC6`.

References using displacement `+0x614` on unrelated objects were excluded by base provenance. No confirmed `mKeyboardPlayerNo` consumer found by the final sweep remains unclassified.

## Parent-family closure

Several old `Partial`/`Discovered` entries described entire native parent functions even though only a small subsection is related to split-screen. They are resolved as follows:

- `0x00A1B0E0`: **Out of split scope**. Direct disassembly `0xA1B0E0..0xA1B16B` contains no split fields or split helper calls. `0xA1B240` is a separate neighboring split-change detector and remains independently reconstructed.
- `0x00B793B0`: **Split-relevant scope complete**. `0xB794AD..0xB79659` contains the split cursor/clamp behavior; the remaining parent body is generic cursor/update work.
- `0x009EDE30`: **Split-relevant scope complete**. All direct split-field/helper blocks through `0x9EF569` are isolated, including `0x9EEE00`, `0x9EEF19`, the `0x9EF000` block, `0x9EF16A`, `0x9EF37B`, `0x9EF413`, the `0x9EF450` loop and `0x9EF4CC` tail. Remaining instructions are generic resource work.
- `0x00A2D710`: **Split-relevant scope complete**. Split input/aspect correction and both draw-wrapper families are reconstructed; renderer-specific non-split tails are intentionally excluded.
- `0x009F2280`: **Split-relevant scope complete**. Projection `0x9F24E1..0x9F2661` and transient visibility `0x9F2A80..0x9F2A9C` are reconstructed; the rest is generic widget update logic.
- `0x007B5C50`: **Split-relevant scope complete**. Affine precompute `0x7B5D60..0x7B5F44` and split hit-test `0x7B6034..0x7B60A8` are reconstructed; surrounding state code is not split-specific.

## Additional closure performed in the final sweep

The final sweep added the remaining native local-J2/split consumers, including the families around:

- `0x79AB10..0x79AD94` input availability/owner synchronization;
- `0x79A570..0x79A614` owner-aware local UI command mapping;
- `0x79B8F0..0x79BBC6` full owner-aware input command mapping;
- `0x7D7500..0x7D7615` routed prompt/device selection;
- `0x7DEA10..0x7DF0FF` per-local-player overlay/UI geometry;
- `0x7DFBA0..0x7DFF5A` two-local-player update loop;
- `0x7DC143..0x7DC1B4` measured split X-delta;
- `0x79BE50` local-owner indicator routing;
- coordinate helpers `0x76A420`, `0x76A460`, `0x76A4A0`;
- widget projection families `0x9F3200` and `0x9F3852`;
- J1/J2 anchor families `0xA2ABC7` and `0xA2B0AD`;
- menu/HUD consumers including the complete `0xA25B00` direct-access family, `0xA2DC50`, `0xA2DED0`, `0xA2EED0`, `0xA302D0`, remaining `0xA33F60`, and `0x852B00`.

These sources are part of the current CMake target.

## Build validation

A final integration audit found that `src/split_input_command_mapping_120.cpp` was present in `main` but had not been listed in `CMakeLists.txt`. That was corrected in commit:

`bc146528ab92e0e413749dac0b7b1ee4927bb241`

The corrected GitHub Actions build:

- workflow: `Static Win32 decomp build`
- run: **#88**
- run id: `34572339136`
- result: **success**
- compiler: Clang 18 targeting `i686-pc-windows-msvc`
- C++17 freestanding build with `-Wall -Wextra -Wpedantic`
- all **124/124** build steps completed;
- `split_input_command_mapping_120.cpp` compiled as object step 20/124;
- the static archive `libBiohazardV_1.2.0_splitscreen_decomp_win32.a` was produced.

The earlier six `-Wundefined-internal` warnings in `split_ui_primary_120.cpp` were corrected in commit `1e89b85b0ac1dd614ff0a7921456b205cd73f367`. Run #88 contains no project-source warning; the only workflow warning is GitHub Actions' Node.js runtime deprecation for `actions/checkout@v4`.

To prevent a decompiled source from silently falling outside the build again, commit `6564be5f0c6d2fc974b38a80c97b2dbfd7181fa1` adds a CI pre-build check that enumerates every `src/*.cpp` and fails if any source is absent from `CMakeLists.txt`.

## Completion statement

Within the repository's stated scope — **the executable-side RE5 1.2.0 native implementation of local J2 and split-screen** — the final static audit has no known confirmed split-object or `mKeyboardPlayerNo` reference left unclassified.

This is a decompilation/static-analysis completion statement, **not** a claim that every path has been runtime-tested in the real Windows/DirectX game. Runtime testing remains the separate functional-certification phase for Story, LIN/DE, Mercs/Reunion, keyboard/controller combinations, Default/Full layouts, aspect changes, leave/rejoin and persistence.