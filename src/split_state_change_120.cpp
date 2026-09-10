#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SplitStateObserver120 {
    std::uint8_t _00[0x20];
    std::int32_t cachedWidth;          // +0x20
    std::int32_t cachedHeight;         // +0x24
    std::uint8_t _28[0xE0 - 0x28];
    std::int32_t cachedStatus;         // +0xE0
    std::uint8_t cachedFullEffective;  // +0xE4
    std::uint8_t _E5[0x198 - 0xE5];
    std::int32_t cachedResolvedIndex;  // +0x198
    std::uint8_t _19C[0x1A8 - 0x19C];
    std::uint8_t playerSelector;       // +0x1A8
};

struct RenderDimensionsState120 {
    std::uint8_t _00[0x50];
    std::int32_t width;                // +0x50
    std::int32_t height;               // +0x54
};

extern int GetRuntimeStatus_C42D90();
extern int ResolveSplitX_76A1E0(const SplitRenderState120* split, int playerIndex);
extern SplitRenderState120* gSplitRenderState_123457C;
extern RenderDimensionsState120* gRenderDimensions_12345D4;

// 0x009EB9D0..0x009EBA6A -- VERIFIED against the unpacked 1.2.0 executable.
// Returns true if any state that controls split UI geometry differs from the
// observer's cached copy. Width/height are refreshed by this routine on a
// dimension change; the other cached members are compared here but maintained
// by their owning update paths elsewhere.
bool SplitStateChangeDetector_9EB9D0(SplitStateObserver120* self)
{
    const int status = GetRuntimeStatus_C42D90();
    if (status != self->cachedStatus)
        return true;

    const int resolved = ResolveSplitX_76A1E0(
        gSplitRenderState_123457C,
        static_cast<int>(self->playerSelector));
    if (resolved != self->cachedResolvedIndex)
        return true;

    if (status == 1) {
        const std::uint8_t full =
            IsFullSplitEffective(gSplitRenderState_123457C) ? 1u : 0u;
        if (full != self->cachedFullEffective)
            return true;
    }

    const int width = gRenderDimensions_12345D4->width;
    const int height = gRenderDimensions_12345D4->height;
    const bool changed =
        width != self->cachedWidth || height != self->cachedHeight;

    self->cachedWidth = width;
    self->cachedHeight = height;
    return changed;
}

} // namespace re5::split120
