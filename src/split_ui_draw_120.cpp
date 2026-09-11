#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

struct UiPanel120 {
    unsigned char _00[0x24];
    int gridX;
    int gridY;
    int originY;
    int originX;
};

extern SplitRenderState120* gSplitRenderState_123457C;
extern int PanelTransform_76C2A0(SplitRenderState120* self, int value);
extern void DrawUiPrimitive_9E5DE0(UiPanel120* self,
                                   int width,
                                   int x,
                                   int y,
                                   void* local,
                                   int id,
                                   int height,
                                   int unkMinusOne);

// 0x00A23B40..0x00A23C3F -- VERIFIED against direct disassembly of the
// unpacked RE5DX9 1.2.0 executable.
//
// Important corrections versus the earlier provisional translation:
//   * split state comes from native global 0x123457C, not a hardcoded fake;
//   * Full X transform is exactly 0x76A460;
//   * Y transform is exactly 0x76C2A0 with ECX = split object;
//   * low-level draw call is 0x9E5DE0, not 0x9E3E70.
void DrawSecondarySplitMenu_A23B40(void* opaque)
{
    auto* self = static_cast<UiPanel120*>(opaque);

    int width;
    if (QueryGameStatus_C42D90() == 1)
        width = static_cast<int>(gSplitRenderState_123457C->splitScale * 30.0f);
    else
        width = 0x1E;

    // Exact signed division idioms emitted by the original function.
    int x = self->gridX / 10;
    int y = (self->gridY * 7) / 8;

    int local = -1;

    if (QueryGameStatus_C42D90() == 1) {
        SplitRenderState120* split = gSplitRenderState_123457C;

        if (IsFullSplitEffective(split))
            x = FullCoordTransform_76A460(split, x);
        else
            x = static_cast<int>(static_cast<float>(x) + split->splitOffset);

        y = PanelTransform_76C2A0(split, y + width / 2);
    } else {
        y -= width / 2;
    }

    y -= self->originX; // native +0x30
    x -= self->originY; // native +0x2C

    DrawUiPrimitive_9E5DE0(self, width, x, y, &local, 0x951, 0x60, -1);
}

} // namespace re5::split120
