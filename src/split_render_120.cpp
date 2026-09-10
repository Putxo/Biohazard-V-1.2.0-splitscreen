#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

struct DisplaySize120 {
    int width;
    int height;
};

extern DisplaySize120 GetCurrentDisplaySize_12345D4();

static int trunc_to_int(float v)
{
    return static_cast<int>(v);
}

// 0x0076C0C0..0x0076C1E8 -- VERIFIED against direct disassembly of the
// unpacked RE5DX9 1.2.0 executable.
//
// Exact constants recovered from the PE:
//   0xF554F0  = 0.9f
//   0x10BCCDC = 0.5625f
//   0xF597FC  = 1.777777791f
//   0x10BB498 = 0.5f
//   0xF597F8  = 2.222222328f
//   0xF597F0  = 0.625 (double)
//   0xF554EC  = 1.0f
//   0xF52EA8  = 640.0f
//   0x10BCA54 = 360.0f
//   0x10BCAE8 = 180.0f
void RecalculateSplitGeometry_76C0C0(SplitRenderState120* self)
{
    const DisplaySize120 physical = GetCurrentDisplaySize_12345D4();

    int effectiveWidth = physical.width;
    int effectiveHeight = physical.height;

    const float physicalWidthF = static_cast<float>(physical.width);
    float effectiveHeightF = static_cast<float>(effectiveHeight);

    // Native 0x76C0DC..0x76C0ED: clamp H/W to <= 0.9.
    if ((effectiveHeightF / physicalWidthF) > 0.9f) {
        effectiveHeight = trunc_to_int(physicalWidthF * 0.9f);
        effectiveHeightF = static_cast<float>(effectiveHeight);
    }

    // Native 0x76C0F1..0x76C114: if H/W < 0.5625, synthesize a
    // 16:9 effective width from the clamped height.
    if ((effectiveHeightF / static_cast<float>(effectiveWidth)) < 0.5625f)
        effectiveWidth = trunc_to_int(effectiveHeightF * 1.777777791f);

    float scale = effectiveHeightF * 0.5f /
                  static_cast<float>(effectiveWidth);
    scale *= 2.222222328f;

    const int splitI0 = trunc_to_int(scale * static_cast<float>(effectiveWidth));
    const int splitI1 = effectiveHeight / 2;

    const float splitParam = static_cast<float>(splitI0) /
        static_cast<float>(static_cast<double>(effectiveWidth) * 0.625);

    self->splitScale  = scale;
    self->splitScale2 = scale;
    self->splitI0     = splitI0;
    self->splitI1     = splitI1;
    self->splitParam  = splitParam;

    // Exact predicate used by 0x76C14C/0x76C185.
    if (self->fullScreenSplitMode != 0 && self->transientSplitFlag == 0) {
        self->splitOffset = (1.0f - scale) * 640.0f;
    } else {
        // Native signed /2 idiom truncates toward zero; C++ signed division
        // has the same semantics here.
        const int halfDifference = (effectiveWidth - physical.width) / 2;
        self->splitOffset = static_cast<float>(
            (halfDifference * 0x320) / physical.width);
    }

    self->splitVertical = 180.0f - scale * 360.0f;
}

} // namespace re5::split120
