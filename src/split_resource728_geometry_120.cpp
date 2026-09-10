#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct Resource728Geometry120 {
    int x;
    int y;
    int sourceY;
};

extern int QueryGameStatus_C42D90();
extern SplitRenderState120* gSplitRenderState_123457C;
extern int FullCoordTransform_76A460(int value);
extern int ScaleUiWord_76A4A0(int value);

// 0x00A2DA32..0x00A2DABA -- VERIFIED split-aware geometry feeding the
// resource 0x728 draw inside 0x00A2DA00.
//
// Direct 1.2.0 PE reconstruction:
//   default X = 0x480 - measuredAdvance
//   default Y = 0x48
//   status 1 X = 0x76A460(default X)
//   status 1 Y = 0x76A4A0(trunc(splitScale * 117.0f))
//   status 1 sourceY = trunc(splitScale * 36.0f)
//
// Exact PE constants:
//   0xF5FD30 = 0x42EA0000 = 117.0f
//   0xF52E14 = 0x42100000 = 36.0f
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
        out.x = FullCoordTransform_76A460(out.x);
        out.y = ScaleUiWord_76A4A0(
            static_cast<int>(split->splitScale * 117.0f));
        out.sourceY = static_cast<int>(split->splitScale * 36.0f);
    }

    return out;
}

} // namespace re5::split120
