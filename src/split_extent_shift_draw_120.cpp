#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// RE5DX9 1.2.0: 0x009E3E70..0x009E3ED0
// Native calling convention: thiscall, seven stack arguments, RET 0x1C.
//
// The helper measures the UI resource selected by arg4, converts the measured
// scalar extent to int with CVTTSS2SI, shifts arg2 left by that amount, forwards
// all seven arguments to 0x9E5DE0, and returns the integer extent.

extern void MeasureUiResource_7DC6B0(
    int zero0,
    int resourceId,
    float* measuredExtent,
    int zero1,
    int arg1a,
    int arg1b,
    int arg7);

extern bool DrawSplitDispatch_9E5DE0(
    void* self,
    int arg1,
    int x,
    int y,
    int arg4,
    int arg5,
    int arg6,
    int arg7);

int DrawShiftedByMeasuredExtent_9E3E70(
    void* self,
    int arg1,
    int x,
    int y,
    int resourceId,
    int arg5,
    int arg6,
    int arg7)
{
    float measuredExtent = 0.0f;

    MeasureUiResource_7DC6B0(
        0,
        resourceId,
        &measuredExtent,
        0,
        arg1,
        arg1,
        arg7);

    const int extent = static_cast<int>(measuredExtent);

    DrawSplitDispatch_9E5DE0(
        self,
        arg1,
        x - extent,
        y,
        resourceId,
        arg5,
        arg6,
        arg7);

    return extent;
}

} // namespace re5::split120
