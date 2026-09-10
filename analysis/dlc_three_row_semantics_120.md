# RE5DX9 1.2.0 — LIN/DE three-row selector semantics audit

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Common selector

The Extras/DLC frontend at `0xA52970` initializes the availability mask at `self+0x194` before the confirmed three-row selector `0xA52D1F..0xA52DD9`.

Exact initialization:

- `0xA529AC`: `self+0x194 = 1` -> row 0 enabled.
- `0xA529B2..0xA529C8`: calls `0x76A410`; when true ORs `4` -> row 2 enabled.
- `0xA529C8..0xA529DE`: calls `0x794D40`; when false ORs `2` -> row 1 enabled.

## 0x76A410

Direct disassembly is exactly:

```asm
76A410  mov al,01
76A412  ret
```

Therefore row 2 is unconditionally enabled in the 1.2.0 build.

## Confirmation path

`0xA52D9F..0xA52DD9`:

1. reads `self+0x190` selected row;
2. tests `self+0x194 & (1 << selected)`;
3. if enabled, acknowledges input through `0x7B43C0`;
4. writes selected directly to `session+0x5CC` at `0xA52DD4`;
5. calls `0x9E4A70`, clears/transitions menu timing state and advances the frontend state machine.

Thus row 2 is not a hidden/dead value: it is navigable, always enabled and committed as native value `2` to `session+0x5CC` for the common LIN/DE frontend.

## Semantic boundary

The external 1.2 acceptance material identifies the third DLC row as the local cooperative option, and the binary proves that the third physical row is row index 2, always available, and committed as value 2. The executable itself does not expose a plain-text label at this code site, so the label association is kept separate from the instruction-level facts above.
