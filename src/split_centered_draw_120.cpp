#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// RE5DX9 1.2.0: 0x009E5E90..0x009E5F0E
// Native calling convention: thiscall, six stack arguments, RET 0x18.
//
// This helper measures the resource referenced by arg5, subtracts half of the
// measured scalar extent from both input coordinates, then forwards the draw
// to 0x9E5DE0 with a final sentinel argument of -1.

extern void MeasureUiResource_7DC6B0(
    int zero0,
    int resourceId,
    float* measuredExtent,
    int zero1,
    int arg1a,
    int arg1b,
    int minusOne);

extern bool DrawSplitDispatch_9E5DE0(
    void* self,
    int arg1,
    int x,
    int y,
    int arg4,
    int resourceId,
    int arg6,
    int sentinel);

void DrawCenteredSplitResource_9E5E90(
    void* self,
    int arg1,
    int x,
    int y,
    int arg4,
    int resourceId,
    int arg6)
{
    float measuredExtent = 0.0f;

    MeasureUiResource_7DC6B0(
        0,
        resourceId,
        &measuredExtent,
        0,
        arg1,
        arg1,
        -1);

    constexpr float kHalf = 0.5f; // 0x10BB498
    const int centeredX = static_cast<int>(
        static_cast<float>(x) - measuredExtent * kHalf);
    const int centeredY = static_cast<int>(
        static_cast<float>(y) - measuredExtent * kHalf);

    DrawSplitDispatch_9E5DE0(
        self,
        arg1,
        centeredX,
        centeredY,
        arg4,
        resourceId,
        arg6,
        -1);
}

} // namespace re5::split120
