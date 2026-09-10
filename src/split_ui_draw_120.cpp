#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

struct UiPanel120 {
    unsigned char _00[0x24];
    int gridX;
    int gridY;
    int originY;
    int originX;
};

extern int  GetRuntimeStatus_C42D90();
extern int  CoordScale2Offset_A239xx(int value);
extern int  PanelTransform_A239xx(int value);
extern void DrawUiPrimitive_1p2(UiPanel120* self,
                                int width,
                                int x,
                                int y,
                                void* local,
                                int id,
                                int height,
                                int unkMinusOne);

// 0x00A23B40 — instruction-derived reconstruction.
void DrawSecondarySplitMenu_A23B40(void* opaque)
{
    auto* self = static_cast<UiPanel120*>(opaque);
    int baseWidth;
    if (GetRuntimeStatus_C42D90() == 1) {
        auto* split = reinterpret_cast<SplitRenderState120*>(
            *reinterpret_cast<void**>(0x0123457C));
        baseWidth = static_cast<int>(split->splitScale * 30.0f);
    } else {
        baseWidth = 0x1E;
    }

    int x = self->gridX / 10;
    int y = (self->gridY * 7) / 8;

    if (GetRuntimeStatus_C42D90() == 1) {
        auto* split = reinterpret_cast<SplitRenderState120*>(
            *reinterpret_cast<void**>(0x0123457C));
        if (IsFullSplitEffective(split))
            x = CoordScale2Offset_A239xx(x);
        else
            x = static_cast<int>(static_cast<float>(x) + split->splitOffset);

        y = PanelTransform_A239xx(y + baseWidth / 2);
    } else {
        y -= baseWidth / 2;
    }

    y -= self->originX;
    x -= self->originY;

    int local = -1;
    DrawUiPrimitive_1p2(self, baseWidth, x, y, &local, 0x951, 0x60, -1);
}

} // namespace re5::split120
