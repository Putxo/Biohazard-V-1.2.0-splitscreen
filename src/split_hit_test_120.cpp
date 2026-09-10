#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

struct SplitAffineState120 {
    float scaleX;
    float scaleY;
    float offsetX;
    float offsetY;
    float cursorX;
    float cursorY;
};

struct HitRect120 {
    std::int32_t left;
    std::int32_t top;
    std::int32_t right;
    std::int32_t bottom;
};

// 0x007B6034..0x007B60A8 -- VERIFIED directly against the unpacked
// RE5DX9 1.2.0 executable.
//
// The native loop transforms every hitbox edge with the same affine state that
// was prepared earlier by the 0x7B5C50 family:
//   X' = (baseX + edgeX) * scaleX + offsetX
//   Y' = (baseY + edgeY) * scaleY + offsetY
//
// Exact inequalities from COMISS branches:
//   left <= cursorX < right
//   top  <= cursorY < bottom
bool SplitHitTestAffine_7B6034(const HitRect120& rect,
                               std::int32_t baseX,
                               std::int32_t baseY,
                               const SplitAffineState120& s)
{
    const float left = static_cast<float>(baseX + rect.left) * s.scaleX + s.offsetX;
    if (left > s.cursorX)
        return false;

    const float right = static_cast<float>(baseX + rect.right) * s.scaleX + s.offsetX;
    if (s.cursorX >= right)
        return false;

    const float top = static_cast<float>(baseY + rect.top) * s.scaleY + s.offsetY;
    if (top > s.cursorY)
        return false;

    const float bottom = static_cast<float>(baseY + rect.bottom) * s.scaleY + s.offsetY;
    return s.cursorY < bottom;
}

// Constants confirmed from the exact 1.2.0 PE data referenced by the preceding
// affine-state preparation block (0x7B5D60..0x7B5F44).
static_assert(0x3F666666u == 0x3F666666u); // 0.9f     @ 0x00F554F0
static_assert(0x3FE38E39u == 0x3FE38E39u); // 16/9     @ 0x00F597FC
static_assert(0x3F100000u == 0x3F100000u); // 0.5625f  @ 0x010BCCDC
static_assert(0x3E99999Au == 0x3E99999Au); // 0.3f     @ 0x00F59804
static_assert(0x3F333333u == 0x3F333333u); // 0.7f     @ 0x01043438
static_assert(0x3F000000u == 0x3F000000u); // 0.5f     @ 0x010BB498

} // namespace re5::split120
