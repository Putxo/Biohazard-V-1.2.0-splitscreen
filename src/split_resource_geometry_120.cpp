#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

// -----------------------------------------------------------------------------
// RE5DX9 1.2.0 -- split resource geometry tail
// Original family: 0x009EDE30
// Verified split-specific tail: 0x009EF4CC..0x009EF55F
// -----------------------------------------------------------------------------

struct SplitUiResourceGeometry120 {
    std::uint8_t _00[0x14];
    std::int32_t x;          // +0x14
    std::int32_t y;          // +0x18
    std::int32_t width;      // +0x1C
    std::int32_t width2;     // +0x20
};

extern int GetRuntimeStatus_C42D90();
extern SplitRenderState120* gSplitRenderState_123457C;

// 0x009EF4CC..0x009EF55F -- VERIFIED by direct disassembly of the unpacked
// RE5DX9 1.2.0 executable.
//
// Important correction to the earlier reconstruction: the default geometry
// writes and the splitActive X/width shift occur regardless of manager status.
// Only the scale2/FULL-dependent width adjustments are gated by status == 1.
// In the original enclosing function EBP is first replaced with [EBP+0x193C];
// this helper receives that already-resolved resource pointer.
void ApplySplitResourceGeometryTail_9EF4CC(SplitUiResourceGeometry120* resource)
{
    if (resource == nullptr)
        return;

    constexpr std::int32_t kBaseWidth = 0x500; // 1280

    resource->y = -180;
    resource->width = kBaseWidth;
    resource->width2 = kBaseWidth;

    SplitRenderState120* split = gSplitRenderState_123457C;

    if (GetRuntimeStatus_C42D90() == 1) {
        // FULL effective means +3084 != 0 && +3085 == 0. In that case the
        // first width remains 1280; width2 is still scaled in all status-1
        // cases.
        if (!IsFullSplitEffective(split)) {
            resource->width = static_cast<std::int32_t>(
                static_cast<float>(resource->width) * split->splitScale2);
        }

        resource->width2 = static_cast<std::int32_t>(
            static_cast<float>(resource->width2) * split->splitScale2);
    }

    if (split->splitActive != 0) {
        resource->x -= kBaseWidth;
        resource->width += kBaseWidth;
    }
}

} // namespace re5::split120
