#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

struct RenderDimensions120Exact {
    std::uint8_t _00[0x50];
    std::int32_t width;
    std::int32_t height;
};

extern int QueryGameStatus_C42D90();
extern RenderDimensions120Exact* gRenderDimensions_12345D4;

// 0x0076C2A0..0x0076C34C -- VERIFIED from direct disassembly of the unpacked
// RE5DX9 1.2.0 executable.
//
// Native ABI: thiscall-like helper, ECX = SplitRenderState120*, one stack
// argument, RET 4. Outside manager status 1 it returns the input unchanged.
//
// The 0x38E38E39 signed-multiply sequence implements trunc((height * 10) / 9)
// for the positive render dimensions used here. If that exceeds width, the
// working height is clamped to trunc(width * 0.9f). When splitActive is set,
// the denominator becomes trunc(workingHeight * 16/9); otherwise it remains
// the physical width. The final scalar is then applied to the input.
int PanelTransform_76C2A0(SplitRenderState120* self, int value)
{
    if (QueryGameStatus_C42D90() != 1)
        return value;

    const int physicalWidth = gRenderDimensions_12345D4->width;
    const int physicalHeight = gRenderDimensions_12345D4->height;

    int workingHeight = physicalHeight;
    const int tenNinthsHeight = (physicalHeight * 10) / 9;
    if (tenNinthsHeight > physicalWidth)
        workingHeight = static_cast<int>(static_cast<float>(physicalWidth) * 0.9f);

    int effectiveWidth = physicalWidth;
    if (self->splitActive != 0)
        effectiveWidth = static_cast<int>(static_cast<float>(workingHeight) *
                                          1.7777777910232544f);

    // Exact integer ordering at 0x76C30E..0x76C319.
    const int scaled = (workingHeight * 0x320) / effectiveWidth;

    // Exact signed /2 idiom and SSE scalar-single chain at 0x76C321..0x76C348.
    const int centered = ((scaled - 0x1C2) / 2) + 0x168;
    float factor = static_cast<float>(centered) * 0.0013888889225199819f;
    factor += 0.5f;
    factor *= static_cast<float>(value);
    return static_cast<int>(factor);
}

} // namespace re5::split120
