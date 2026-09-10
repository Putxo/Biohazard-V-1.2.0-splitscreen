# RE5DX9 1.2.0 — frontend writers of session+0x5CC

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

This pass follows direct stores to `session+0x5CC`, the same field used by the verified local-route predicate `0xC43BB0` for modes 2/5 (`+0x5CC == 2`).

## 0xA49AE4 — three-way selector

The path beginning at `0xA499DB` navigates `self+0x18C` with an explicit count of 3:

- `0xA49A40`: push 3 to `0x9E4BD0`
- `0xA49A54`: push 3 to `0x7B5870`
- selection result returns to `self+0x18C`
- confirmation at `0xA49AAB` validates `self+0x19C & (1 << selected)`
- `0xA49AE4` stores that exact selected value to `session+0x5CC`

Therefore this is a native three-entry frontend producer capable of producing the required value 2.

## 0xA4A0FA — four-way selector

`0xA49F93` first loads `session+0x5CC` into `self+0x1A0`. The navigation path uses an explicit count of 4:

- `0xA4A043`: push 4 to `0x9E4BD0`
- `0xA4A057`: push 4 to `0x7B5870`
- result stored in `self+0x1A0`
- confirmation at `0xA4A0C0` validates `self+0x1A8 & (1 << selected)`
- `0xA4A0FA` writes selected back to `session+0x5CC`

This is a separate four-entry frontend producer for the same session field.

## 0xA52DD4 — independent three-way selector

Another menu family at `0xA52CB0..0xA52DFC` uses `self+0x190` and an explicit count of 3:

- `0xA52D25`: push 3 to `0x9E4BD0`
- `0xA52D39`: push 3 to `0x7B5870`
- result stored in `self+0x190`
- confirmation at `0xA52D9F` validates `self+0x194 & (1 << selected)`
- `0xA52DD4` writes selected to `session+0x5CC`

This is a second native three-entry producer capable of producing local-route value 2.

## What is proved / not yet proved

Proved directly:

- all three paths write the exact menu-selected index to `session+0x5CC`;
- A49AE4 and A52DD4 are three-way selectors, hence expose index 2 natively;
- A4A0FA is a four-way selector and also exposes index 2;
- `0xC43BB0` later interprets `+0x5CC == 2` as the local route when real mode is 2 or 5.

Not yet assigned without further class/resource tracing:

- which of A49AE4 vs A52DD4 is Mercenaries and which is Mercenaries Reunion;
- exact visible labels for each selector row.

The external 1.2 acceptance documentation independently states that Mercenaries and Mercenaries Reunion expose a `PANTALLA PARTIDA` entry, but this file deliberately keeps the binary-to-menu identity unassigned until direct class/resource evidence closes it.
