# `functions.csv` status

`analysis/functions.csv` is the historical incremental manifest produced during the decompilation passes. It intentionally preserves earlier `Discovered` / `Partial` classifications as an audit trail and is **not** the authoritative current completion status after the 2026-09-11 final sweep.

Current status is defined by:

- `FINAL_LOCAL_J2_SPLIT_DECOMP_AUDIT_2026-09-11.md`
- `final_local_j2_split_manifest_2026-09-11.csv`

In particular, the old parent entries `A1B0E0`, `B793B0`, `9EDE30`, `A2D710`, `9F2280` and `7B5C50` must be interpreted using the final manifest: `A1B0E0` is out of split scope, while the other five have their complete split-relevant portions reconstructed/isolated. Generic code in those parent functions is deliberately outside this repository's scope.