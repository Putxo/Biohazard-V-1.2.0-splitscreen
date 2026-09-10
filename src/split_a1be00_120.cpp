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

extern void SetWidgetPosition_A1A830(void* widget, const Vec3f120* value);
extern void SetWidgetScale_A1A890(void* widget, const Vec3f120* value);
extern int  SplitXFromParam_76A420(const SplitRenderState120* split, int value);
extern int  SplitYWithVertical_76A4A0(const SplitRenderState120* split, int value);

struct WidgetEntry120 {
    std::uint32_t offset;
    bool useSplitX;
    bool useSplitVerticalY;
    bool useSplitScale;
    float xMultiplier;
    float yConstant;
};

// 0x00A1BE00 -- VERIFIED against direct disassembly of the unpacked
// RE5DX9 1.2.0 executable (0xA1BE00..0xA1C396).
//
// Important correction from the direct audit:
// split->splitScale is NOT applied to all eleven widgets. Native code reloads
// the persistent [esp+0x14] splitScale local for seven scale vectors only.
// The four early widgets at +1B0/+1D8/+1FC/+220 explicitly load 1.0f before
// A1A890. This source preserves that distinction.
void RebuildSplitUiGeometry_A1BE00(SplitUiOwner120* owner,
                                   const SplitRenderState120* split)
{
    auto* ownerBytes = reinterpret_cast<std::uint8_t*>(owner);
    if (*reinterpret_cast<const std::uint32_t*>(ownerBytes + 0x4C) == 1)
        return;

    const int splitX0 = SplitXFromParam_76A420(split, 0);
    const float scale = split->splitScale;
    const float param = split->splitParam;

    static constexpr WidgetEntry120 kEntries[11] = {
        {0x1AC, true,  false, true,   16.0f,  64.0f},
        {0x1B0, false, false, false,   0.0f,   0.0f},
        {0x1D8, false, false, false,   0.0f,  52.0f},
        {0x1FC, false, false, false,   0.0f,  89.0f},
        {0x220, false, false, false,   0.0f, 102.0f},
        {0x2B0, true,  false, true,  723.0f,  30.0f},
        {0x270, true,  false, true,    0.0f,   0.0f},
        {0x28C, true,  false, true,    0.0f,   0.0f},
        {0x2C0, true,  false, true,    0.0f,   0.0f},
        {0x2EC, true,  false, true,   30.0f,  -9.0f},
        {0x244, true,  true,  true,    0.0f,   0.0f},
    };

    for (int player = 0; player < 2; ++player) {
        const std::uint32_t groupOffset = static_cast<std::uint32_t>(player) * 0x1F8u;

        for (const auto& e : kEntries) {
            void* widget = *reinterpret_cast<void**>(ownerBytes + e.offset + groupOffset);
            if (!widget)
                continue;

            Vec3f120 pos{};
            pos.x = e.useSplitX
                ? static_cast<float>(splitX0) + param * e.xMultiplier
                : 0.0f;
            pos.y = e.useSplitVerticalY
                ? static_cast<float>(SplitYWithVertical_76A4A0(split, 0))
                : e.yConstant;
            pos.z = 0.0f;
            SetWidgetPosition_A1A830(widget, &pos);

            const float widgetScale = e.useSplitScale ? scale : 1.0f;
            const Vec3f120 scl{widgetScale, widgetScale, widgetScale};
            SetWidgetScale_A1A890(widget, &scl);
        }
    }
}

} // namespace re5::split120
