#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

struct SplitHitTransform120 {
    std::int32_t cursorX;
    std::int32_t cursorY;
    float scaleX;
    float scaleY;
    std::int32_t offsetX;
    std::int32_t offsetY;
};

extern int GetRuntimeStatus_C42D90();
extern int GetCurrentSplitPlayerSelector_1249C40_614();
extern std::int32_t GetUiBaseX_12B0468();
extern std::int32_t GetUiBaseY_12B046C();

// 0x007B5D60..0x007B5F44 -- VERIFIED split-aware precompute extracted from
// the larger 0x007B5C50 family.
//
// The four coordinate values correspond to the exact native stack mapping
// around the preceding 0x5DD230 conversion call:
//   convertedX / convertedY = helper-produced output coordinates
//   inputX / inputY         = source coordinates used by the explicit
//                             rescale branches below
//
// The result is consumed by the hit-test block at 0x007B6034..0x007B60A8.
SplitHitTransform120 ComputeSplitHitTransform_7B5D60(
    const SplitRenderState120* split,
    std::int32_t physicalWidth,
    std::int32_t physicalHeight,
    std::int32_t convertedX,
    std::int32_t convertedY,
    std::int32_t inputX,
    std::int32_t inputY)
{
    SplitHitTransform120 out{
        convertedX,
        convertedY,
        1.0f,
        1.0f,
        GetUiBaseX_12B0468(),
        GetUiBaseY_12B046C(),
    };

    if (GetRuntimeStatus_C42D90() != 1)
        return out;

    std::int32_t effectiveWidth = physicalWidth;
    const float aspect = static_cast<float>(physicalHeight) /
                         static_cast<float>(physicalWidth);

    // 0x7B5DC0: COMISS aspect, 0.9 ; JBE <= path.
    if (aspect > 0.8999999761581421f) {
        out.cursorX = (inputX * 1280) / physicalWidth;

        const std::int32_t nineTenthsWidth = (physicalWidth * 9) / 10;
        const std::int32_t crop = (physicalHeight - nineTenthsWidth) / 2;
        const std::int32_t cropScaled = (crop * 1280) / physicalWidth;

        out.cursorY = (((inputY * 720) / physicalWidth) * 9) / 10;
        out.cursorY -= cropScaled;
    } else if (split->splitActive != 0) {
        // Native 0x7B5E34..47: helper outputs remain intact; only the width
        // used by the affine transform becomes trunc(H * 16/9).
        effectiveWidth = static_cast<std::int32_t>(
            static_cast<float>(physicalHeight) * 1.7777777910232544f);
    } else {
        out.cursorX = (inputX * 1280) / physicalWidth;
        out.cursorY = (inputY * 720) / physicalHeight;
    }

    const float scale = split->splitScale;
    out.scaleX = scale;
    out.scaleY = scale * static_cast<float>(effectiveWidth) * 0.5625f /
                 static_cast<float>(physicalHeight);

    // Exact arithmetic shape of 0x7B5E7D..0x7B5EE3. C++ signed division
    // truncates toward zero, matching IDIV and the explicit SAR adjustment.
    const std::int32_t scaledHeight = static_cast<std::int32_t>(
        static_cast<float>(effectiveWidth) * 0.5625f);
    const std::int32_t xAdj =
        -((scaledHeight / 16) * 1280) / effectiveWidth;
    out.offsetY += xAdj;

    const std::int32_t delta = static_cast<std::int32_t>(
        (1.0f - scale) * 1280.0f);

    if (GetCurrentSplitPlayerSelector_1249C40_614() == 0) {
        out.offsetX = static_cast<std::int32_t>(
            static_cast<float>(delta) * 0.30000001192092896f +
            static_cast<float>(GetUiBaseX_12B0468()));
    } else {
        out.offsetX = static_cast<std::int32_t>(
            static_cast<float>(delta) * 0.699999988079071f +
            static_cast<float>(GetUiBaseX_12B0468()));
        out.offsetY += 360;
    }

    if (IsFullSplitEffective(split)) {
        out.offsetX = static_cast<std::int32_t>(
            static_cast<float>(delta) * 0.5f);
    }

    return out;
}

} // namespace re5::split120
