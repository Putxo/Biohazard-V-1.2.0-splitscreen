#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

// -----------------------------------------------------------------------------
// RE5DX9 1.2.0 -- dynamic split-screen rebuild detector
// -----------------------------------------------------------------------------
//
// The original 1.2.0 helper is at 0x00A1B240. It is called from the larger
// 0x00A1B0E0 family and requests a UI/geometry rebuild when the render size or
// effective Full split-screen state changes.
//
// The original object owns three cached values at +0x50/+0x54/+0x58:
//   +0x50 cached render width
//   +0x54 cached render height
//   +0x58 cached effective fullscreen-split state
//
// This reconstruction expresses the native 1.2.0 semantics directly instead of
// the later Dev-port sidecar implementation.

struct SplitRebuildObserver120 {
    std::uint8_t _00[0x50];
    std::int32_t cachedWidth;          // +0x50
    std::int32_t cachedHeight;         // +0x54
    std::uint8_t cachedFullSplitState; // +0x58
};

struct RenderDimensions120 {
    std::uint8_t _00[0x50];
    std::int32_t width;                // +0x50
    std::int32_t height;               // +0x54
};

extern RenderDimensions120* gRenderDimensions_12345D4;
extern SplitRenderState120* gSplitRenderState_123457C;

// 0x00A1B240
// VERIFIED SEMANTICS
//
// Returns true exactly when one of the three cached inputs changes. On a width
// or height change both dimensions are refreshed and the function returns
// immediately. If dimensions are unchanged, the effective Full predicate is
// compared against +0x58 and that byte is refreshed only when it differs.
bool DetectResolutionOrFullSplitChange_A1B240(SplitRebuildObserver120* self)
{
    const std::int32_t width = gRenderDimensions_12345D4->width;
    if (width != self->cachedWidth) {
        self->cachedWidth = width;
        self->cachedHeight = gRenderDimensions_12345D4->height;
        return true;
    }

    const std::int32_t height = gRenderDimensions_12345D4->height;
    if (height != self->cachedHeight) {
        self->cachedWidth = gRenderDimensions_12345D4->width;
        self->cachedHeight = height;
        return true;
    }

    const std::uint8_t full =
        IsFullSplitEffective(gSplitRenderState_123457C) ? 1u : 0u;

    if (full != self->cachedFullSplitState) {
        self->cachedFullSplitState = full;
        return true;
    }

    return false;
}

// The only confirmed 1.2.0 caller is in the 0x00A1E650 family. When the helper
// returns true, that caller invokes 0x00A1BE00, the split-aware UI geometry
// rebuild routine. A1BE00 is intentionally not guessed here; it gets its own
// instruction-to-C pass.
extern void RebuildSplitUiGeometry_A1BE00(void* owner);

void RefreshSplitUiIfDisplayChanged_A1E650(
    void* owner,
    SplitRebuildObserver120* observer)
{
    if (DetectResolutionOrFullSplitChange_A1B240(observer))
        RebuildSplitUiGeometry_A1BE00(owner);
}

} // namespace re5::split120
