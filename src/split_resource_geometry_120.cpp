#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

// -----------------------------------------------------------------------------
// RE5DX9 1.2.0 -- split resource geometry tail
// Original family: 0x009EDE30
// Recovered split-specific tail: 0x009EF4CC..0x009EF55F
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

// PARTIAL / branch-exact reconstruction of the split-specific 1.2.0 tail.
//
// Confirmed behavior for manager status == 1:
//   resource+0x18 = -180
//   resource+0x1C = 1280
//   resource+0x20 = 1280
//   scaled = trunc(split.scale2 * 1280.0f)
//   if (!IsFullSplitEffective(split)) resource+0x1C = scaled
//   resource+0x20 = scaled
//   if (split.active) {
//       resource+0x14 -= 1280
//       resource+0x1C += 1280
//   }
//
// The enclosing 0x9EDE30 function performs additional non-split work before and
// after this tail, so this function is deliberately scoped to the recovered
// split-specific block only.
void ApplySplitResourceGeometryTail_9EF4CC(SplitUiResourceGeometry120* resource)
{
    if (GetRuntimeStatus_C42D90() != 1)
        return;

    auto* split = gSplitRenderState_123457C;

    resource->y = -180;
    resource->width = 1280;
    resource->width2 = 1280;

    const std::int32_t scaled =
        static_cast<std::int32_t>(split->splitScale2 * 1280.0f);

    if (!IsFullSplitEffective(split))
        resource->width = scaled;

    resource->width2 = scaled;

    if (split->splitActive != 0) {
        resource->x -= 1280;
        resource->width += 1280;
    }
}

} // namespace re5::split120
