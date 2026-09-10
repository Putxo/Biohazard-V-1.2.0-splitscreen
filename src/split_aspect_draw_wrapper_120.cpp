#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct AspectDrawLayout120 {
    std::uint8_t _00[0x1C];
    std::int32_t uiA;      // +0x1C
    std::int32_t uiB;      // +0x20
    std::int32_t width;    // +0x24
};

extern int MeasureHeader_9E3D40(void* self, int resourceId, int arg6, int sentinel);
extern int QueryGameStatus_C42D90();
extern SplitRenderState120* gSplitRenderState_123457C;
struct RenderDimensions120 { std::uint8_t _00[0x50]; std::int32_t width; std::int32_t height; };
extern RenderDimensions120* gRenderDimensions_12345D4;

// 0x00A2D8B0..0x00A2D94B -- geometry half of the first split-aware draw wrapper.
// Native ABI: thiscall, six stack args, RET 0x18.
// arg1 is the layout object whose +0x24 width participates in both centering
// and the split correction; arg3 is the incoming X accumulator.
int ComputeFirstAspectDrawX_A2D8B0(void* self,
                                   AspectDrawLayout120* arg1,
                                   int arg2,
                                   int arg3,
                                   int arg4,
                                   const std::uint32_t* arg5,
                                   int arg6)
{
    (void)arg4;
    (void)arg5;

    const int measured = MeasureHeader_9E3D40(self, arg2, arg6, -1);

    // Exact CDQ/SUB/SAR signed-halving idiom used by the native code.
    const int halfMeasured = measured / 2;
    const int halfWidth = arg1->width / 2;
    int x = arg3 + (halfWidth - halfMeasured);

    if (QueryGameStatus_C42D90() == 1) {
        const SplitRenderState120* split = gSplitRenderState_123457C;
        if (split->splitActive && !IsFullSplitEffective(split)) {
            const int renderWidth = gRenderDimensions_12345D4->width;
            const int renderHeight = gRenderDimensions_12345D4->height;
            constexpr float k16By9 = 1.7777777910232544f; // 0x3FE38E39

            const int excess = static_cast<int>(
                static_cast<float>(renderWidth) -
                static_cast<float>(renderHeight) * k16By9);
            const int halfExcess = excess / 2;
            x += (-halfExcess * arg1->width) / renderWidth;
        }
    }

    return x;
}

extern int MeasureHeader_9E3DE0(void* self, int resourceId, int arg6);

// 0x00A2DB30..0x00A2DBC9 -- geometry half of the second/twin wrapper.
// It differs only in the measuring helper (0x9E3DE0) and has the same native
// split correction from 0xA2DB71..0xA2DBC7.
int ComputeSecondAspectDrawX_A2DB30(void* self,
                                    AspectDrawLayout120* arg1,
                                    int arg2,
                                    int arg3,
                                    int arg4,
                                    const std::uint32_t* arg5,
                                    int arg6)
{
    (void)arg4;
    (void)arg5;

    const int measured = MeasureHeader_9E3DE0(self, arg2, arg6);
    const int halfMeasured = measured / 2;
    const int halfWidth = arg1->width / 2;
    int x = arg3 + (halfWidth - halfMeasured);

    if (QueryGameStatus_C42D90() == 1) {
        const SplitRenderState120* split = gSplitRenderState_123457C;
        if (split->splitActive && !IsFullSplitEffective(split)) {
            const int renderWidth = gRenderDimensions_12345D4->width;
            const int renderHeight = gRenderDimensions_12345D4->height;
            constexpr float k16By9 = 1.7777777910232544f;

            const int excess = static_cast<int>(
                static_cast<float>(renderWidth) -
                static_cast<float>(renderHeight) * k16By9);
            const int halfExcess = excess / 2;
            x += (-halfExcess * arg1->width) / renderWidth;
        }
    }

    return x;
}

// 0x00A2D9C0..0x00A2D9FB -- exact forwarding wrapper around A2D8B0.
// 0x9E3D80 returns an extent; native halves it with CDQ/SUB/SAR and subtracts
// that half from arg4 before forwarding all six arguments.
extern int MeasureExtent_9E3D80(void* self, int arg2, int arg6);
extern void DrawWrapper_A2D8B0(void* self,
                               AspectDrawLayout120* arg1,
                               int arg2,
                               int arg3,
                               int arg4,
                               const std::uint32_t* arg5,
                               int arg6);

void DrawWrapperShifted_A2D9C0(void* self,
                               AspectDrawLayout120* arg1,
                               int arg2,
                               int arg3,
                               int arg4,
                               const std::uint32_t* arg5,
                               int arg6)
{
    const int extent = MeasureExtent_9E3D80(self, arg2, arg6);
    DrawWrapper_A2D8B0(self, arg1, arg2, arg3, arg4 - extent / 2, arg5, arg6);
}

} // namespace re5::split120
