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
    float xMultiplier;
    float yConstant;
};

// 0x00A1BE00 -- VERIFIED against direct disassembly of the unpacked
// RE5DX9 1.2.0 executable (0xA1BE00..0xA1C396).
//
// Key verification that resolves the old Stage27 uncertainty:
//   A1BE32  movss xmm0,[split+0x3070]      ; splitScale
//   A1BE3C  push 0
//   A1BE3E  movss [esp+0x18],xmm0
//   A1BE42  call 0x76A420
// After the callee-cleaned push, this stored value is the persistent local at
// [esp+0x14]. Every scale-vector write later reloads [esp+0x14], proving that
// the reused scale value is exactly split->splitScale. No speculative value is
// needed.
//
// The function handles 11 widget slots and repeats the block twice by adding
// +0x1F8 to the widget base (A1C381) until two groups have been processed.
void RebuildSplitUiGeometry_A1BE00(SplitUiOwner120* owner,
                                   const SplitRenderState120* split)
{
    // Native function returns immediately when owner+0x4C == 1. That owner
    // state is deliberately not named beyond this structural condition here.
    auto* ownerBytes = reinterpret_cast<std::uint8_t*>(owner);
    if (*reinterpret_cast<const std::uint32_t*>(ownerBytes + 0x4C) == 1)
        return;

    // 0x76A420(0) => trunc(round(0 * splitParam) + splitOffset), i.e. the
    // native integer X origin used throughout this function.
    const int splitX0 = SplitXFromParam_76A420(split, 0);
    const float scale = split->splitScale;   // VERIFIED local [esp+0x14]
    const float param = split->splitParam;   // native local [esp+0x18]

    static constexpr WidgetEntry120 kEntries[11] = {
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

            // Direct instructions A1BED8, A1C0EA, A1C14D, A1C1BC,
            // A1C237, A1C2BE and A1C34A all reload the same [esp+0x14]
            // local and replicate it to XYZ before A1A890.
            const Vec3f120 scl{scale, scale, scale};
            SetWidgetScale_A1A890(widget, &scl);
        }
    }
}

} // namespace re5::split120
