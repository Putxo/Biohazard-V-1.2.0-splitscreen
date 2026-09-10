#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct Vec3f120 {
    float x;
    float y;
    float z;
};

struct SplitUiOwner120 {
    std::uint8_t _00[0x1AC];
};

extern int GetRuntimeStatus_C42D90();
extern void SetUiVector_633320(void* widget, const Vec3f120* value);

struct WidgetPosEntry120 {
    std::uint32_t offset;
    bool useSplitX;
    bool useSplitVerticalY;
    float xMultiplier;
    float yConstant;
};

// 0x00A1BE00 -- PARTIAL semantic reconstruction of the 1.2.0 split-aware
// geometry pass. The position formulas are established from the original
// 1.2.0 family and were semantically validated in the historical Stage27 work.
//
// Important boundary: the original 1.2.0 routine also participates in scale
// vector setup. The exact provenance of one reused local scale value was not
// proven, so this reconstruction intentionally covers the position-vector
// portion only instead of guessing the remaining scale semantics.
void ApplySplitWidgetPositions_A1BE00(SplitUiOwner120* owner,
                                      const SplitRenderState120* split)
{
    if (GetRuntimeStatus_C42D90() != 1)
        return;

    // 1.2.0 operates on 11 widgets, then repeats the same formulas for the
    // second player block at +0x1F8. The offsets below are native 1.2.0 object
    // offsets, not the older Dev-layout translations used during the port.
    static constexpr WidgetPosEntry120 kEntries[11] = {
        {0x1AC, true,  false,  16.0f,  64.0f},
        {0x1B0, false, false,   0.0f,   0.0f},
        {0x1D8, false, false,   0.0f,  52.0f},
        {0x1FC, false, false,   0.0f,  89.0f},
        {0x220, false, false,   0.0f, 102.0f},
        {0x2B0, true,  false, 723.0f,  30.0f},
        {0x270, true,  false,   0.0f,   0.0f},
        {0x28C, true,  false,   0.0f,   0.0f},
        {0x2C0, true,  false,   0.0f,   0.0f},
        {0x2EC, true,  false,  30.0f,  -9.0f},
        {0x244, true,  true,    0.0f,   0.0f},
    };

    auto* base = reinterpret_cast<std::uint8_t*>(owner);

    for (int player = 0; player < 2; ++player) {
        const std::uint32_t groupOffset = player ? 0x1F8u : 0u;

        for (const auto& e : kEntries) {
            void* widget = *reinterpret_cast<void**>(base + e.offset + groupOffset);
            if (!widget)
                continue;

            Vec3f120 pos{};
            if (e.useSplitX)
                pos.x = split->splitOffset + split->splitParam * e.xMultiplier;
            else
                pos.x = 0.0f;

            pos.y = e.useSplitVerticalY ? split->splitVertical : e.yConstant;
            pos.z = 0.0f;

            SetUiVector_633320(widget, &pos);
        }
    }
}

} // namespace re5::split120
