#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct RenderDimensionsAspect120 {
    std::uint8_t _00[0x50];
    std::int32_t width;                // +0x50
    std::int32_t height;               // +0x54
};

extern int GetRuntimeStatus_C42D90();
extern SplitRenderState120* gSplitRenderState_123457C;
extern RenderDimensionsAspect120* gRenderDimensions_12345D4;

static inline int half_toward_zero(int v)
{
    return v / 2;
}

// Shared split-only X correction present verbatim in both draw paths:
//   0x00A2D8F3..0x00A2D949 (predicate refs at A2D906/A2D90F)
//   0x00A2DB71..0x00A2DBC7 (predicate refs at A2DB84/A2DB8D)
// VERIFIED instruction-for-instruction against the unpacked 1.2.0 executable.
int ApplySplitAspectXCorrection_A2D906(int x, int elementWidth)
{
    if (GetRuntimeStatus_C42D90() != 1)
        return x;

    const SplitRenderState120* split = gSplitRenderState_123457C;
    if (split->splitActive == 0 || IsFullSplitEffective(split))
        return x;

    const int renderWidth = gRenderDimensions_12345D4->width;
    const int renderHeight = gRenderDimensions_12345D4->height;

    constexpr float kAspect16By9 = 1.77777779102325439453125f;
    const int excess = static_cast<int>(
        static_cast<float>(renderWidth) -
        static_cast<float>(renderHeight) * kAspect16By9);

    const int halfExcess = half_toward_zero(excess);
    const int correction = (-halfExcess * elementWidth) / renderWidth;
    return x + correction;
}

} // namespace re5::split120
