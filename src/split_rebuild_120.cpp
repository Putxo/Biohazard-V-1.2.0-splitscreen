#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

// -----------------------------------------------------------------------------
// RE5DX9 1.2.0 -- dynamic split-screen rebuild detector
// -----------------------------------------------------------------------------

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

// 0x00A1B240..0x00A1B2D8 -- VERIFIED direct thiscall helper.
// ECX is the observer/owner object. There are no stack arguments.
// Returns AL=1 when dimensions or effective FULL state changed.
bool DetectResolutionOrFullSplitChange_A1B240(SplitRebuildObserver120* self)
{
    const std::int32_t width = gRenderDimensions_12345D4->width;
    const std::int32_t height = gRenderDimensions_12345D4->height;

    // A1B249/A1B252 -> A1B2CF: either dimension changed.
    if (width != self->cachedWidth || height != self->cachedHeight) {
        self->cachedWidth = width;
        self->cachedHeight = height;
        return true;
    }

    const std::uint8_t full =
        IsFullSplitEffective(gSplitRenderState_123457C) ? 1u : 0u;

    // A1B278..A1B29D: state change updates +0x58 and returns true.
    if (full != self->cachedFullSplitState) {
        self->cachedFullSplitState = full;
        return true;
    }

    // A1B29E..A1B2CE writes the already-equal cached values again before
    // returning false. Keep those stores explicit for instruction-level
    // fidelity rather than optimizing them away in the reconstruction.
    self->cachedWidth = width;
    self->cachedHeight = height;
    self->cachedFullSplitState = full;
    return false;
}

extern void RebuildSplitUiGeometry_A1BE00(void* self);

// 0x00A1E650 -- verified callsite wrapper.
// Important correction: A1B240 and A1BE00 operate on the SAME ECX object.
// There is no separate observer argument and this is not part of A1B0E0.
void RefreshSplitUiIfDisplayChanged_A1E650(SplitRebuildObserver120* self)
{
    if (DetectResolutionOrFullSplitChange_A1B240(self))
        RebuildSplitUiGeometry_A1BE00(self);
}

} // namespace re5::split120
