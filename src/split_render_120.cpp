#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

struct DisplaySize120 {
    int width;
    int height;
};

extern DisplaySize120 GetCurrentDisplaySize_12345D4();
extern float K_ASPECT_CAP_F554F0();
extern float K_ASPECT_MIN_10BCCDC();
extern float K_SCALE_F597FC();
extern float K_BASE_10BB498();
extern float K_SCALE_F597F8();
extern double K_HALF_F597F0();
extern float K_ONE_F554EC();
extern float K_HALF_F52EA8();
extern float K_OFFSET_SCALE_10BCA54();
extern float K_VERTICAL_BASE_10BCAE8();

static int trunc_to_int(float v)
{
    return static_cast<int>(v);
}

// -----------------------------------------------------------------------------
// 0x0076C0C0 — PARTIAL decompilation of the central split geometry routine.
//
// Confirmed writes made by the original routine:
//   +3068 splitI0
//   +306C splitI1
//   +3070 splitScale
//   +3074 splitParam
//   +3078 splitScale2
//   +307C splitOffset
//   +3080 splitVertical
//
// Confirmed selector:
//   (+3084 != 0 && +3085 == 0) selects the Full split offset branch.
//   Otherwise the bordered/default branch is used.
//
// Constants are still named by evidence address rather than invented semantics.
// -----------------------------------------------------------------------------
void RecalculateSplitGeometry_76C0C0(SplitRenderState120* self)
{
    DisplaySize120 display = GetCurrentDisplaySize_12345D4();
    int width = display.width;
    int height = display.height;

    const float fw = static_cast<float>(width);
    const float fh = static_cast<float>(height);
    const float ratio = fh / fw;

    const float cap = K_ASPECT_CAP_F554F0();
    if (ratio > cap)
        height = trunc_to_int(fw * cap);

    const float fHeight = static_cast<float>(height);
    const float adjustedRatio = fHeight / static_cast<float>(width);

    if (K_ASPECT_MIN_10BCCDC() > adjustedRatio)
        width = trunc_to_int(fHeight * K_SCALE_F597FC());

    float scale = fHeight * K_BASE_10BB498() / static_cast<float>(width);
    scale *= K_SCALE_F597F8();

    const int splitI0 = trunc_to_int(scale * static_cast<float>(width));
    const int splitI1 = height / 2;

    const float splitParam = static_cast<float>(splitI0) /
        static_cast<float>(static_cast<double>(width) * K_HALF_F597F0());

    self->splitScale = scale;
    self->splitScale2 = scale;
    self->splitI0 = splitI0;
    self->splitI1 = splitI1;
    self->splitParam = splitParam;

    if (IsFullSplitEffective(self)) {
        float off = K_ONE_F554EC() - scale;
        off *= K_HALF_F52EA8();
        self->splitOffset = off;
    } else {
        const DisplaySize120 now = GetCurrentDisplaySize_12345D4();
        const int horizontal = (width - now.width) / 2;
        self->splitOffset = static_cast<float>((horizontal * 0x320) / now.width);
    }

    self->splitVertical =
        K_VERTICAL_BASE_10BCAE8() - scale * K_OFFSET_SCALE_10BCA54();
}

} // namespace re5::split120
