#include <cstdint>

namespace re5::split120 {

struct SplitRenderState120Mini {
    std::uint8_t _00[0x3064];
    std::uint8_t splitActive;
    std::uint8_t _3065[0x1f];
    std::uint8_t fullScreenSplitMode; // +0x3084
    std::uint8_t transientSplitFlag;  // +0x3085
};

struct WidgetProjectionOwner120 {
    std::uint8_t _00[0xE0];
    std::int32_t projectionMode; // +0xE0
    std::uint8_t _E4[0x1AA - 0xE4];
    std::uint8_t sideSelector;   // +0x1AA
};

static bool IsFullSplitEffectiveMini(const SplitRenderState120Mini* split)
{
    return split->fullScreenSplitMode != 0 && split->transientSplitFlag == 0;
}

// 0x009F24E1..0x009F2661 -- VERIFIED isolated split-aware projection block
// inside the larger 0x009F2280 widget/update routine.
//
// Inputs metricX/metricY are the two floating extents already prepared by the
// parent at [esp+0x20]/[esp+0x24]. The native block emits integer-valued
// floats in XMM2/XMM4 after CVTTSS2SI -> CVTSI2SS.
void ComputeSplitWidgetProjection_9F24E1(
    const WidgetProjectionOwner120* owner,
    const SplitRenderState120Mini* split,
    int renderWidth,
    int renderHeight,
    float metricX,
    float metricY,
    float& outX,
    float& outY)
{
    constexpr float kOne = 1.0f;
    constexpr float kHalf = 0.5f;
    constexpr float kAspect16x9 = 1.7777777910232544f;
    constexpr float kHalfAspect = 0.5625f;
    constexpr float kUiWidth = 1280.0f;
    constexpr float kUiHeight = 720.0f;
    constexpr float kUpperEdge = 1.009374976158142f;
    constexpr float kLowerEdge = 0.9906250238418579f;

    float x;
    float y;

    if (split->splitActive != 0) {
        if (owner->projectionMode != 0 && !IsFullSplitEffectiveMini(split)) {
            const float edge = owner->sideSelector == 0 ? kUpperEdge : kLowerEdge;
            x = ((edge / static_cast<float>(renderWidth)) * metricX -
                 (edge - kOne) * kHalf) * kUiWidth;
            y = (metricY / static_cast<float>(renderHeight)) * kUiHeight;
        } else {
            const float normalized =
                (static_cast<float>(renderWidth) /
                 static_cast<float>(renderHeight)) * kHalfAspect;
            x = ((normalized / static_cast<float>(renderWidth)) * metricX -
                 (normalized - kOne) * kHalf) * kUiWidth;
            y = (metricY / static_cast<float>(renderHeight)) * kUiHeight;
        }
    } else {
        float verticalScale = kOne;
        if (owner->projectionMode == 0) {
            const float aspect = static_cast<float>(renderWidth) /
                                 static_cast<float>(renderHeight);
            verticalScale = kAspect16x9 / aspect;
        }

        x = (metricX / static_cast<float>(renderWidth)) * kUiWidth;
        y = ((verticalScale / static_cast<float>(renderHeight)) * metricY -
             (verticalScale - kOne) * kHalf) * kUiHeight;
    }

    outX = static_cast<float>(static_cast<int>(x));
    outY = static_cast<float>(static_cast<int>(y));
}

} // namespace re5::split120
