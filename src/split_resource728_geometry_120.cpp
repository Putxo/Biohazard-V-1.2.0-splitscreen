#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct Resource728Geometry120 { int x; int y; int sourceY; };
extern SplitRenderState120* gSplitRenderState_123457C;

// 0x00A2DA32..0x00A2DABA -- VERIFIED split-aware geometry feeding the
// resource 0x728 draw inside 0x00A2DA00.
Resource728Geometry120 ComputeSplitResource728Geometry_A2DA32(
    int sourceY,
    int measuredAdvance)
{
    Resource728Geometry120 out{};
    out.x = 0x480 - measuredAdvance;
    out.y = 0x48;
    out.sourceY = sourceY;

    if (QueryGameStatus_C42D90() == 1) {
        SplitRenderState120* split = gSplitRenderState_123457C;
        out.x = FullCoordTransform_76A460(split,out.x);
        out.y = SplitVerticalTransform_76A4A0(
            split,static_cast<int>(split->splitScale * 117.0f));
        out.sourceY = static_cast<int>(split->splitScale * 36.0f);
    }

    return out;
}

} // namespace re5::split120
