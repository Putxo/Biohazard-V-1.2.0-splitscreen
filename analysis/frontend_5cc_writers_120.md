# RE5DX9 1.2.0 — frontend writers of session+0x5CC

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

This pass follows direct stores to `session+0x5CC`, the same field consumed by the verified local/split predicates.

## 0xA49AE4 — Mercenaries Reunion three-way selector

The path beginning at `0xA499DB` navigates `self+0x18C` with an explicit count of 3:

- `0xA49A40`: push 3 to `0x9E4BD0`
- `0xA49A54`: push 3 to `0x7B5870`
- selection result returns to `self+0x18C`
- confirmation at `0xA49AAB` validates `self+0x19C & (1 << selected)`
- `0xA49AE4` stores that exact selected value to `session+0x5CC`

Class/resource tracing through `0xA45D80` identifies this family as `merce_dc` / Mercenaries Reunion. Row index 2 therefore reaches the native Reunion local/split route unchanged.

## 0xA4A0FA — four-way selector

`0xA49F93` first loads `session+0x5CC` into `self+0x1A0`. The navigation path uses an explicit count of 4:

- `0xA4A043`: push 4 to `0x9E4BD0`
- `0xA4A057`: push 4 to `0x7B5870`
- result stored in `self+0x1A0`
- confirmation at `0xA4A0C0` validates `self+0x1A8 & (1 << selected)`
- `0xA4A0FA` writes selected back to `session+0x5CC`

This is a separate four-entry producer for the same session field. Its exact visible row labels are not required by the local-J2/split closure because the split route itself is already identified by the dedicated three-row families and native predicates.

## 0xA52DD4 — Extra / LIN-DE three-way selector

The Extra/DLC menu family at `0xA52CB0..0xA52DFC` uses `self+0x190` and an explicit count of 3:

- `0xA52D25`: push 3 to `0x9E4BD0`
- `0xA52D39`: push 3 to `0x7B5870`
- result stored in `self+0x190`
- confirmation at `0xA52D9F` validates `self+0x194 & (1 << selected)`
- `0xA52DD4` writes selected to `session+0x5CC`

Class/resource tracing identifies this as the Extra/DLC family. The dedicated LIN/DE audit proves row 2 is always enabled in 1.2.0 and is committed directly as value 2, closing the binary-side local cooperative selector semantics.

## Final identity status

Resolved directly from the executable:

- `0xA49AE4` belongs to Mercenaries Reunion;
- `0xA52DD4` belongs to the Extra/DLC frontend;
- both three-way producers expose and commit row index 2 natively;
- normal Mercenaries independently commits physical row 2 as `session+0x5CC == 2` in its `0xA48051..0xA480A0` selection path;
- the native predicates consume value 2 as the local/split route for the corresponding mode families.

Localized visible labels are presentation resources and are not needed to establish the executable-side route semantics reconstructed by this repository.
