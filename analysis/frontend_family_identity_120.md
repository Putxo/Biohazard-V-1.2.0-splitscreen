# RE5DX9 1.2.0 — frontend family identity audit

Authority: direct disassembly and raw rdata from unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Normal Mercenaries

The vtable containing update/state entry `0xA47CE0` also contains method `0xA41410`.

`0xA41410` directly pushes `0xFA9C04`, whose raw rdata string is:

`id\\jpn\\merce\\merce_00`

It then loads current session fields `+0x5C8` and `+0x5CC` into the menu object. This gives an instruction-backed identity for the `0xA47CE0` family as the normal Mercenaries frontend.

Its selection commit branch is `0xA48051..0xA480A0`:

- selected 0,1,2: `session+0x5CC = selected`, `session+0x79E = 0`
- selected 3: `session+0x5CC = 0`, `session+0x79E = 1`

This proves that physical row index 2 survives as exact `+0x5CC == 2`, while index 3 is carried through the separate `+0x79E` special-mode flag.

## Mercenaries Reunion

The vtable containing update/state entry `0xA49720` also contains init method `0xA45D80`.

`0xA45D80` directly requests both:

- `0xFA9F58` -> `id\\jpn\\merce_dc\\merce_00`
- `0xFA9F3C` -> `id\\jpn\\merce_dc\\merce_dc01`

Therefore the `0xA49720` selector family is the `merce_dc` / Mercenaries Reunion frontend.

Its verified three-way confirmation block `0xA499DB..0xA49B27` stores `self+0x18C` directly to `session+0x5CC` at `0xA49AE4` and clears `session+0x79E`. Navigation uses count 3.

Combined with verified `0xC43BB0`, when session mode is 5, row index 2 is exactly the native local/split route predicate `+0x5CC == 2`.

## Extra / DLC family

The vtable at `0xFAA64C` contains update/state entry `0xA52970`.

The associated setup path at `0xA4F543` directly requests `0xFAA688`, whose rdata string is:

`id\\jpn\\extra\\extra_03`

The same rdata neighborhood contains `extra_01`, `extra_02`, `top50..top57`, confirming this as the Extra/DLC frontend family used by the DLC menus rather than a Mercenaries menu.

Its independent three-way selector `0xA52CB0..0xA52DFC` writes `self+0x190` to `session+0x5CC` at `0xA52DD4` after bitmask validation.

The later LIN/DE selector audit closes the row semantics required by this repository: row 2 is unconditionally enabled by `0x76A410`, is navigable, and is committed unchanged as `session+0x5CC == 2`. The visible localized row label is not embedded at this instruction site, but the binary-side local/split route semantics are complete.

## Resource archive mode cross-check

The central archive loader at `0x7D4E91..0x7D4ECF` selects by `session+0x1C`:

- 8 -> `Image\\Archive\\Extra1Resource`
- 9 -> `Image\\Archive\\Extra2Resource`
- 10 -> `Image\\Archive\\Merce2Resource`
- other branch -> `Image\\Archive\\MerceResource`

This independently matches the Extra1/Extra2 DLC and Merce2/Reunion resource families seen above.

## Build check

The frontend identity and row-semantic reconstruction is included in the full Win32 static build and final split/J2 audit.
