# RE5DX9 1.2.0 — additional 0x009EDE30 split blocks

Direct source: unpacked 1.2.0 executable, instruction-level `objdump -d -Mintel` audit.

## 0x009EF000..0x009EF092 — status-1 row transform

Within the 18-entry resource loop, after the caller has selected a three-float variant, status 1 performs:

- `CVTTSS2SI` on the row X coordinate.
- `0x76A420(split, x)` and stores the integer result back as float X.
- Loads `split+0x3070` into the row scale.
- Forces the temporary integer Y offset to zero.
- For a live node with id `< 0x291`, writes `+0x50=X`, `+0x54=Y +/- offset`, `+0x58=0`, `+0x80=0`, `+0x38=0x0D`, and copies the same scale into `+0x70/+0x74`.

The alternate branch at `0x009EF37B..0x009EF3C9` is the corresponding subtract-offset form. It applies the same status-1 X transform and `splitScale`, except loop index `0x0A` skips that transform before the shared node-write tail.

## 0x009EF450..0x009EF4C4 — four-node application loop

The native loop executes exactly four iterations. Each live node receives:

- `+0x70/+0x74 = uniformScale`.
- X from the selected local position pair.
- Under status 1, X is converted with `CVTTSS2SI -> 0x76A420 -> CVTSI2SS` before being stored back to `+0x50`.
- Y from the paired local value into `+0x54`.
- Final refresh call `0xCD8160(node)`.

These blocks are reconstructed in `src/split_resource_geometry_more_120.cpp`.

Standalone compile verification: Clang C++17 freestanding target `i686-pc-windows-msvc`; output recognized as Intel i386 COFF object.
