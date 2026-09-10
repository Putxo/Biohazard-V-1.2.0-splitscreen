#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SplitGeometryNode120 {
    std::uint8_t _00[0x18];
    std::int32_t x;              // +0x1C
    std::int32_t y;              // +0x20
    std::int32_t width;          // +0x24
    std::int32_t height;         // +0x28
    std::int32_t xOrigin;        // +0x2C
    std::int32_t scratch;        // +0x30
    std::int32_t playerIndex;    // +0x34
    std::uint8_t _38[0x188-0x38];
    std::int32_t mappedDevice;   // +0x188
    void* resource;              // +0x18C
};

extern int GetRuntimeStatus_C42D90();
extern int ResolveSplitX_76A1E0(const SplitRenderState120* split, int playerIndex);
extern int ResolveSplitY_76A250(const SplitRenderState120* split, int xIndex);
extern int GetUiWidth_9E3C80(int splitIndex);
extern int GetUiHeight_9E3CD0(int splitIndex);
extern int GetSessionDeviceForPlayer(int playerIndex); // session +0x490[player]

// 0x00A22C00 -- VERIFIED against direct disassembly of the unpacked 1.2.0 EXE
// through the RET at 0x00A22CF6.
//
// Split-specific tail:
//  - only manager status 1
//  - requires split +0x3064 active
//  - applies only when FULL is NOT effectively active
//    (i.e. !fullScreenSplitMode || transientSplitFlag)
//  - correction is based on horizontal letterbox/pillarbox excess at 16:9:
//      excess = width - height * (16/9)
//      correction = -(excess / 2) * uiWidth / width
//  - correction is added to object +0x2C.
void UpdateSplitGeometry_A22C00(SplitGeometryNode120* self,
                                const SplitRenderState120* split,
                                int renderWidth,
                                int renderHeight)
{
    if (!self->resource)
        return;

    self->mappedDevice = GetSessionDeviceForPlayer(self->playerIndex);

    self->x = ResolveSplitX_76A1E0(split, self->playerIndex);
    if (self->x == -1)
        self->x = 0;

    self->y = ResolveSplitY_76A250(split, self->x);
    self->width  = GetUiWidth_9E3C80(self->x);
    self->height = GetUiHeight_9E3CD0(self->x);

    // Exact signed divide-by-two idiom at A22C71..A22C7E.
    self->xOrigin = (self->width - 0x500) / 2;

    if (GetRuntimeStatus_C42D90() == 1 && split->splitActive) {
        const bool fullEffective =
            split->fullScreenSplitMode != 0 && split->transientSplitFlag == 0;

        if (!fullEffective) {
            constexpr float kAspect16By9 = 1.7777777777777777f; // raw 0x3FE38E39 @ 0xF597FC
            const int excess = static_cast<int>(
                static_cast<float>(renderWidth) -
                static_cast<float>(renderHeight) * kAspect16By9);

            const int halfExcess = excess / 2;
            const int correction = (-halfExcess * self->width) / renderWidth;
            self->xOrigin += correction;
        }
    }

    self->scratch = 0;
}

} // namespace re5::split120
