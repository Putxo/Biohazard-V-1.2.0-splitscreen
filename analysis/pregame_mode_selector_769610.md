# RE5DX9 1.2.0 — 0x769610 Benchmark/Demo-FPS scenario selector

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Correction to earlier interpretation

The five values handled at `0x76965C..0x7696AD` are hexadecimal `0x320..0x324`, i.e. decimal stages **800..804**. Project stage maps identify these as the Benchmark / Demo-FPS scenario family, not Lost in Nightmares, Desperate Escape, Mercenaries or Mercenaries Reunion.

This matters because the two-slot-shaped setup reached by four of these variants must **not** be used as evidence for local-coop reachability in those gameplay modes.

## 0x769610 selector

The function reads the selected player-state value at `playerRoot + index*0x5940 + 0x273D8` and maps:

- `0x320 (800) -> self+0x1849 = 0`
- `0x321 (801) -> self+0x1849 = 1`
- `0x322 (802) -> self+0x1849 = 2`
- `0x323 (803) -> self+0x1849 = 3`
- `0x324 (804) -> self+0x1849 = 4`

For other values the previous `+0x1849` value is retained.

Native computes `BL = (variant >= 4)` and separately reduces the compact variant modulo four for an earlier reset path.

## Branch controlling the two-slot-shaped setup

At `0x769971`:

- `BL == 0` (variants 0..3 / stages 800..803): enters `0x7699A1` and reaches `0x769A63..0x769B7F`.
- `BL == 1` (variant 4 / stage 804): enters `0x769987`, writes `session+0x478 = 0`, emits event `0x26`, and skips that setup.

`0x769A63..0x769B7F` writes:

- `session+0x478 = 3`
- `session+0x47C = 0`
- slot modes 0 and 1 = 1
- slot 0 device = preferred device `input+0x5D8`
- slot 1 device = -1
- paired fields handled by `0xC42A70`, `0xC42A90`, `0xC42AB0`, `0xC42D70` are zeroed for both slots
- event `0x25`
- `session+0x570 = 2`

## Independent binary corroboration

Direct 1.2 disassembly also shows:

- `0x715D10` tests the stage range `0x320 <= stage < 0x384`.
- caller `0x718180` dispatches scenario identifiers `0x323`, `0x324` or `0x320` according to `session+0x79D` after calling `0x769610`.
- `0x76C44B` has a specific comparison against stage `0x324`.
- the surrounding `0x769C05` family compares against `0x322` and `0x324`.

These references reinforce that `0x769610` belongs to the special Benchmark/Demo scenario family.

## Consequence for split-screen reconstruction

The search for native local co-op in LIN / DE / Mercenaries / Reunion must continue elsewhere. The high-value native route is the mode predicate at `0xC43BB0` and its input precheck caller `0x799B50`, because the exact 1.2 PE explicitly recognizes modes 2 and 5 when `session+0x5CC == 2`.
