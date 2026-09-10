# RE5DX9 1.2.0 — native mode-local split predicate

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## 0xC43BB0 — VERIFIED

Exact body: `0xC43BB0..0xC43BF0`.

The function obtains the session from `[0x12340A4]+0x1042C`, reads `session+0x58` and implements:

```text
if ((mode == 2 || mode == 5) && session+0x5CC == 2)
    return true;
if (mode == 1)
    return false;
if (byte [argument-this + 0x56C] != 0)
    return false;
return C42B60(argument-this) >= 2;
```

The first branch is therefore direct 1.2 binary evidence that **modes 2 and 5 have a distinguished local route selected by `session+0x5CC == 2`**. This is not inferred from the older Dev port.

The function has many native callers throughout input/UI/game-state code; it is a central session predicate rather than a one-off menu test.

## 0x799B50 — VERIFIED

Exact body: `0x799B50..0x799B89`, `ECX=self`, no stack arguments.

It calls `0xC43BB0(session)`. If that predicate is false it immediately returns true. If true:

- reads `self+0x618`;
- negative value returns true;
- otherwise compares it with `[0x1249C40]+0x614`;
- returns equality.

Thus the modes-2/5 local route is coupled to the same input/player-selection state at offsets `+0x614/+0x618` that appears throughout the native local split/input family.

## Confirmed downstream use

`0x7B43C0` begins by loading global input object `0x1249C40` and calling `0x799B50`. A false result exits its update path; a true result continues into its normal input/UI state work. Many neighbouring `0x7B42xx..0x7B63xx` routines also call `0x799B50`.

## `session+0x5CC` writers found

Direct PE xref audit finds several writers. Of particular interest, menu/state-machine code around:

- `0xA49962` writes selected top-level row to `session+0x5C8`;
- `0xA49AE4` writes `self+0x18C` to `session+0x5CC` after a three-entry navigation path;
- `0xA4A0FA` writes `self+0x1A0` to `session+0x5CC`;
- `0xA52DD4` writes `self+0x190` to `session+0x5CC`.

These are now the primary frontend candidates to classify. No semantic mode/menu name is assigned to an individual writer until its owning class/state is traced directly.

## Scope consequence

The correct next path for Mercenaries/Reunion is no longer the `0x769610` Benchmark family. It is to identify which native frontend writer can produce `+0x5CC == 2` while `session+0x58` remains mode 2 or 5, then trace that state into the local-join/trial-split screen.
