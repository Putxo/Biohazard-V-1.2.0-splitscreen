# Local-join mode integration audit (RE5DX9 1.2.0)

Exact binary authority: unpacked runnable candidate, SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Native recurring join path

`0x724B28` is the only direct call to `0x723460` found in the full `.text` disassembly.

`0x723460` hard-gates the recurring J2 join path on `session+0x58 == 0` before any controller/keyboard/fallback detection. Searches for calls to `0x720CE0` show only the three callsites inside `0x723460`, so the native `BeginAddPlayer -> state 13 -> 0x716720(active=1)` topology is reached exclusively through this mode-zero live-join path in the audited binary.

This means LIN/DE/Mercenaries/Reunion must not be assumed to reuse the Story live-join detector. Their local-player setup, if present, has to be traced through separate pre-game/menu/session configuration paths.

## 0x716810 lifecycle monitor

Direct disassembly `0x716810..0x71688D` independently confirms the same mode restriction. It returns immediately when `(root+0x10434)&3 != 0` or `session+0x58 != 0`.

When allowed, it derives two gate bits from the low byte of `root+0x10438` (bit7 -> 1, bit6 -> 2), watches `self+0x358`, checks `session+0x2C == 2`, and can move `self+0x04` to state `0x0A` after `0x715BB0` succeeds and both gate bits are clear.

## Correction: A2E770/A2EC50 are removal paths

The calls to `0x716720` at `0xA2E89D`, `0xA2E914`, and `0xA2EDBF` are not alternative J2 activation paths.

Their arguments are pushed before `0x768C10`. `0x768C10` consumes no stack arguments and returns a component pointer in EAX, so those preserved arguments flow into `0x716720`; the first argument is zero. Therefore these callsites perform `active=0` local-player removal/deactivation.

The only directly verified `0x716720(active=1, slot, device)` call remains `0x725C23` inside Add-Player state 13.

## Remaining mode-integration target

Trace nonzero `session+0x58` setup and locate where the local slot mask (`session+0x478`), slot modes (`+0x480`) and device assignments are prepared before gameplay for LIN, DE, Mercenaries and Reunion.
