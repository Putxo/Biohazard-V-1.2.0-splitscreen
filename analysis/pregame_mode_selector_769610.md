# RE5DX9 1.2.0 — pregame scenario selector / two-local-slot branch

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## 0x769610 selector

At `0x76965C..0x7696AD`, the function reads the selected player-state value at `playerRoot + index*0x5940 + 0x273D8` and maps five consecutive values:

- `0x320 -> self+0x1849 = 0`
- `0x321 -> self+0x1849 = 1`
- `0x322 -> self+0x1849 = 2`
- `0x323 -> self+0x1849 = 3`
- `0x324 -> self+0x1849 = 4`

For other values the previous `+0x1849` value is retained.

Native then computes `BL = (variant >= 4)` and separately reduces the compact variant modulo four for an earlier reset path.

## Branch controlling the two-slot setup

At `0x769971`, native loads that `BL` flag:

- `BL == 0` (variants 0..3): enters `0x7699A1` and reaches the full two-slot preparation block `0x769A63..0x769B7F`.
- `BL == 1` (variant 4): enters `0x769987`, writes `session+0x478 = 0`, emits event `0x26`, and skips the two-slot setup.

Therefore the two-local-slot pregame branch is not a generic unconditional initializer. It is selected by this five-way scenario family, with only the first four members enabling the two-slot session layout.

## Exact two-slot layout

`0x769A63..0x769B7F`:

- `session+0x478 = 3` (slots 0 and 1 available)
- `session+0x47C = 0`
- slot modes 0 and 1 set to `1` via `0xC42A30`
- slot 0 device = preferred input device at `input+0x5D8`
- slot 1 device = `-1`
- the paired per-slot fields handled by `0xC42A70`, `0xC42A90`, `0xC42AB0`, `0xC42D70` are zeroed for both slots
- event `0x25` is emitted afterwards
- `session+0x570 = 2` before return

## Callers

Direct callers of `0x769610` found in the 1.2.0 PE:

- `0x71819B`
- `0x769CC0`
- `0xBF461C`

The `0x718180` and `0xBF45E0` callers both inspect `session+0x79D` after/before this setup and dispatch scenario identifiers `0x323` / `0x324` through other native helpers, confirming that this area is a pregame scenario-family initializer rather than the Story live-join detector.

## Current boundary

The numeric selector mapping and the two-slot/no-slot branch are instruction-verified. The semantic names of scenario IDs `0x320..0x324` are not assigned here yet; that requires tracing their producers/consumers further instead of guessing which one corresponds to LIN/DE/Mercenaries/Reunion.
