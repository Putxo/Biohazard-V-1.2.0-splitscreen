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

Confirmed split-state consumers are represented by a reconstructed function or by an explicitly isolated split-specific block in `src/`. Coincidences in unrelated D4/DB/DF/E0/E1/E3 families and stack/object layouts were rejected when the base register was not the split object.

No confirmed direct consumer of the split object found by this audit remains unclassified.

## Final local-J2 ownership audit

The ownership sweep covered the **145 high-confidence executable references** to `InputManager + 0x614` (`mKeyboardPlayerNo`) around the native `0x01249C40` input object.

This includes readers and writers during local join, Story gameplay, Mercenaries/Reunion, LIN/DE, keyboard/controller fallback, prompt routing, two-player UI loops, frontend owner capture and command translation. References using displacement `+0x614` on unrelated objects were excluded by base provenance.

A source-snapshot recheck after the final caller work found **68 explicit source/header occurrences** of `0x614`; these are source-level representations and are not expected to be one textual occurrence per executable instruction. The executable-side provenance classification remains authoritative.

No confirmed `mKeyboardPlayerNo` consumer found by the final sweep remains unclassified.

## Exact 0x799B50 direct-caller closure

The reference executable contains exactly **40 direct CALL instructions to `0x00799B50`**. They were re-enumerated directly from the reference binary and are mapped one-for-one in:

`analysis/799b50_direct_callers_manifest_2026-09-11.csv`

The final closure added or corrected:

- the complete `0x7B4210..0x7B637B` owner-aware UI route family with native ABIs;
- `0x7D755F` inside `ResolveRoutedPromptCode_7D7500`;
- `0x9E1DDF` inside the reconstructed `0x9E1D4D..0x9E1DF7` local/J2 input-mask block;
- native stack-only `RET 8` ABIs for `0xA02F20` and `0xA02F40`;
- exact feature blocks for the five late callers `0xA04BDA`, `0xA0880A`, `0xA0B2DB`, `0xA0B7F1`, `0xA0BFBB`.

Those last five entries deliberately reconstruct only the verified owner-gated feature block inside their much larger parent menus. They are **not** claims that the entire unrelated parent functions are fully decompiled.

Result: **40/40 direct `0x799B50` callers represented and classified.**

## Canonical native-address and semantic cleanup

The audit removed false source duplication where a single native VA had accumulated multiple C++ implementations under descriptive aliases. Native `C42B60`, `C43BB0` and `799B50` now each have one canonical external implementation; historical names are inline adapters in `include/re5/native_abi_audit_120.hpp`.

`0x7B43C0` was corrected from an analysis helper signature that passed synthetic gate/overlay/coordinate arguments to its real native ABI: ECX=self, two stack arguments, `RET 8`, with the owner gate and global state read internally.

The final semantic sweep also caught and fixed the last split-relevant omission in `0xA4AE73`: the Mercenaries frontend completion path now performs the native `0x7B43C0` routed UI/input reset after setting `self+0x3C = 1`, matching the verified executable tail instead of leaving that side effect as a comment-only note.

Stale stage-analysis documents that still described already-closed items as `Partial`, `not yet proved`, or future targets were refreshed to match the authoritative final manifest.

## Parent-family closure

Several old `Partial`/`Discovered` entries described entire native parent functions even though only a small subsection is related to split-screen. They are resolved as follows:

- `0x00A1B0E0`: **Out of split scope**. Direct disassembly `0xA1B0E0..0xA1B16B` contains no split fields or split helper calls. `0xA1B240` is a separate neighboring split-change detector and remains independently reconstructed.
- `0x00B793B0`: **Split-relevant scope complete**. `0xB794AD..0xB79659` contains the split cursor/clamp behavior; the remaining parent body is generic cursor/update work.
- `0x009EDE30`: **Split-relevant scope complete**. All direct split-field/helper blocks through `0x9EF569` are isolated. Remaining instructions are generic resource work.
- `0x00A2D710`: **Split-relevant scope complete**. Split input/aspect correction and both draw-wrapper families are reconstructed; renderer-specific non-split tails are intentionally excluded.
- `0x009F2280`: **Split-relevant scope complete**. Projection `0x9F24E1..0x9F2661` and transient visibility `0x9F2A80..0x9F2A9C` are reconstructed; the rest is generic widget update logic.
- `0x007B5C50`: **Split-relevant scope complete**. Affine precompute `0x7B5D60..0x7B5F44` and split hit-test `0x7B6034..0x7B60A8` are reconstructed; surrounding state code is not split-specific.

## Additional closure represented in source

The current source covers the native local-J2/split families around:

- `0x79AB10..0x79AD94` input availability/owner synchronization;
- `0x79A570..0x79A614` owner-aware local UI command mapping;
- `0x79B8F0..0x79BBC6` full owner-aware input command mapping;
- `0x7D7500..0x7D7615` routed prompt/device selection;
- `0x7DEA10..0x7DF0FF` per-local-player overlay/UI geometry;
- `0x7DFBA0..0x7DFF5A` two-local-player update loop;
- `0x7DC143..0x7DC1B4` measured split X-delta;
- coordinate helpers `0x76A420`, `0x76A460`, `0x76A4A0`;
- widget projection families `0x9F3200` and `0x9F3852`;
- J1/J2 anchor families `0xA2ABC7` and `0xA2B0AD`;
- menu/HUD consumers including the complete `0xA25B00` direct-access family, `0xA2DC50`, `0xA2DED0`, `0xA2EED0`, `0xA302D0`, remaining `0xA33F60`, and `0x852B00`.

## Current build validation

Final GitHub Actions validation after ABI cleanup, semantic closure, documentation reconciliation and the new closure guard:

- workflow: `Static Win32 decomp build`
- run: **#212**
- run id: `34598435659`
- head: `1ff76b890bae53ccf9973d3bb58779a8539e3768`
- result: **success**
- source/CMake coverage check: **132/132 `src/*.cpp` files listed in CMake**
- canonical native ABI audit: **passed**
- final split/J2 semantic closure audit: **passed**
- compiler target: i686 Win32 COFF static library
- C++17 freestanding build with `-Wall -Wextra -Wpedantic`
- archive inspection passed;
- source snapshot artifact uploaded successfully.

CI contains both a build-graph guard and a final semantic closure guard. The latter rejects unfinished markers in active source, stale open-status statements in the resolved stage documents, non-closed entries in the authoritative final manifest, and loss of the verified `0xA4AE73 -> 0x7B43C0` completion side effect.

## Completion statement

Within the repository's stated scope — **the executable-side RE5 1.2.0 native implementation of local J2 and split-screen** — the final static audit has no known confirmed split-object, `mKeyboardPlayerNo`, direct `0x799B50` reference, ABI conflict, or verified split-relevant semantic side effect left unclassified or unrepresented.

This is a decompilation/static-analysis completion statement, **not** a claim that every path has been runtime-tested in the real Windows/DirectX game. Runtime testing remains the separate functional-certification phase for Story, LIN/DE, Mercs/Reunion, keyboard/controller combinations, Default/Full layouts, aspect changes, leave/rejoin and persistence.
