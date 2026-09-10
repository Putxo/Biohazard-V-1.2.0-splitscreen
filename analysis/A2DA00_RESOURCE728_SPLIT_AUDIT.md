# RE5DX9 1.2.0 — 0xA2DA00 resource 0x728 split audit

Direct instruction audit performed against the unpacked 1.2.0 PE.

## Verified split-aware block: 0xA2DA32..0xA2DABA

The parent function starts at 0xA2DA00 and returns with `RET 4`. The split-specific geometry begins after the parent gates and the `0x9E3D40` measurement for resource `0x728`.

Native behavior:

- default X: `0x480 - measuredAdvance`
- default Y: `0x48`
- when runtime status is `1`:
  - X is transformed by `0x76A460`
  - `splitScale` is read from `0x123457C + 0x3070`
  - Y becomes `0x76A4A0(trunc(splitScale * 117.0f))`
  - source Y becomes `trunc(splitScale * 36.0f)`

Exact constants decoded from the PE:

- `0xF5FD30 = 0x42EA0000 = 117.0f`
- `0xF52E14 = 0x42100000 = 36.0f`
- `0x10BCA08 = 0xBF800000 = -1.0f` (used later by the renderer pack)

The reconstructed split block is implemented in `src/split_resource728_geometry_120.cpp` and standalone-compiles as Intel i386 COFF with the project freestanding Win32 Clang configuration.

The remainder of 0xA2DA00 after 0xA2DABA is the renderer argument pack and draw call to 0x7DE3F0. It contains no additional split-state branch before the return at 0xA2DB22.
