#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct CursorClampContext120 {
    std::uint8_t _00[0x2910];
    float cursorX;                 // +0x2910
    float cursorY;                 // +0x2914
};

struct InputManager120 {
    std::uint8_t _00[0x614];
    std::int32_t selectedJoinSlot; // +0x614 (0 = upper/J1, nonzero = lower/J2)
};

// Split-specific clamp block recovered from 0x00B794AD..0x00B79659 inside
// the larger 0x00B793B0 routine. This helper intentionally models only that
// verified block; the parent routine performs unrelated cursor/update work
// before and after it.
void ApplySplitCursorClamp_B794AD(CursorClampContext120* self,
                                 const SplitRenderState120* split,
                                 const InputManager120* input,
                                 int runtimeStatus,
                                 int renderWidth,
                                 int renderHeight)
{
    if (runtimeStatus == 1) {
        const bool fullEffective =
            split->fullScreenSplitMode != 0 && split->transientSplitFlag == 0;

        if (!fullEffective) {
            const bool active = split->splitActive != 0;
            const int splitWidth = split->splitI0;

            const int effectiveWidth = active
                ? static_cast<int>(static_cast<float>(renderHeight) * 1.7777777910232544f)
                : renderWidth;

            const int margin = static_cast<int>(
                static_cast<float>(effectiveWidth - splitWidth) *
                (input->selectedJoinSlot == 0 ? 0.30000001192092896f
                                              : 0.69999998807907104f));

            int minX = margin;
            float xScale = 1.0f;

            if (active) {
                xScale = (static_cast<float>(renderWidth) /
                          static_cast<float>(renderHeight)) * 0.5625f;
                const int centered = (renderWidth - effectiveWidth) / 2;
                minX = static_cast<int>(
                    static_cast<float>(centered + margin) / xScale);
            }

            const float minXf = static_cast<float>(minX);
            if (self->cursorX < minXf)
                self->cursorX = minXf;

            const float maxXf = static_cast<float>(splitWidth) * xScale + minXf;
            if (self->cursorX > maxXf)
                self->cursorX = maxXf;
        }

        const float halfHeight = static_cast<float>(renderHeight / 2);
        if (input->selectedJoinSlot == 0) {
            if (self->cursorY > halfHeight)
                self->cursorY = halfHeight;
        } else {
            if (self->cursorY < halfHeight)
                self->cursorY = halfHeight;
        }
    } else if (split->splitActive != 0 && split->transientSplitFlag != 0) {
        const int width16x9 = static_cast<int>(
            static_cast<float>(renderHeight) * 1.7777777910232544f);
        const int minX = (renderWidth - width16x9) / 2;
        const int maxX = minX + width16x9;

        if (self->cursorX < static_cast<float>(minX))
            self->cursorX = static_cast<float>(minX);
        if (self->cursorX > static_cast<float>(maxX))
            self->cursorX = static_cast<float>(maxX);
    }
}

} // namespace re5::split120
