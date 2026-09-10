#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct CursorUpdate120 {
    std::uint8_t _00[0x1F54];
    std::int32_t field1F54;           // +0x1F54
    std::uint8_t _1F58[0x2910 - 0x1F58];
    float cursorX;                    // +0x2910
    float cursorY;                    // +0x2914
    std::uint8_t inputActive;         // +0x2918
    std::uint8_t previousInputActive; // +0x2919
    std::uint8_t absoluteRefresh;     // +0x291A
    std::uint8_t _291B[0x2920 - 0x291B];
    std::uint8_t field2920;           // +0x2920
    std::uint8_t _2921[0x2D9C - 0x2921];
    std::uint8_t playerIndex;         // +0x2D9C
    std::uint8_t _2D9D[0x2DA8 - 0x2D9D];
    std::int32_t cursorDisabled;      // +0x2DA8
};

struct CursorInputSource120 {
    std::uint8_t _00[0x1F8];
    std::int32_t absoluteX;           // +0x1F8
    std::int32_t absoluteY;           // +0x1FC
    std::uint8_t _200[0x20C - 0x200];
    float deltaX;                     // +0x20C
    float deltaY;                     // +0x210
};

extern void CursorUpdatePrelude_84CF30(CursorUpdate120* self);
extern SplitRenderState120* gSplitRenderState_123457C;
extern int ResolveSplitX_76A1E0(const SplitRenderState120* split, int playerIndex);

// 0x00B793B0..0x00B794A9 -- VERIFIED cursor acquisition/pre-clamp slice.
//
// The native parent clears transient cursor state first, then either:
//  * refreshes cursorX/Y from integer absolute coordinates (+1F8/+1FC), or
//  * accumulates float deltas (+20C/+210) when inputActive stayed active.
//
// It snapshots *truncated* X/Y before the split clamp. Those raw integer
// snapshots are later reused by 0xB79659 for the final hard 0..width/height
// screen clamp, even if the split-specific clamp changed the float cursor in
// between. The 0x76A1E0 call at 0xB79472 is preserved although its return is
// not used by the following block.
bool AcquireCursorForSplitClamp_B793B0(CursorUpdate120* self,
                                       CursorInputSource120* source,
                                       int& rawX,
                                       int& rawY)
{
    CursorUpdatePrelude_84CF30(self);

    self->field1F54 = 0;
    self->field2920 = 0;

    if (self->cursorDisabled != 0)
        return false;

    if (source == nullptr)
        return false;

    if (self->inputActive != 0 && self->previousInputActive == 1) {
        self->cursorX += source->deltaX;
        self->cursorY += source->deltaY;
    } else {
        if (self->absoluteRefresh == 0)
            return false;

        self->cursorX = static_cast<float>(source->absoluteX);
        self->cursorY = static_cast<float>(source->absoluteY);
        self->absoluteRefresh = 0;
    }

    ResolveSplitX_76A1E0(gSplitRenderState_123457C,
                          static_cast<int>(self->playerIndex));

    rawX = static_cast<int>(self->cursorX); // CVTTSS2SI
    rawY = static_cast<int>(self->cursorY); // CVTTSS2SI
    return true;
}

// 0x00B79659..0x00B796A1 -- VERIFIED final hard screen-bound clamp.
// Uses the raw integer cursor snapshots captured before B794AD, not a fresh
// conversion of the already split-clamped cursor values.
void ApplyCursorScreenBounds_B79659(CursorUpdate120* self,
                                    int rawX,
                                    int rawY,
                                    int renderWidth,
                                    int renderHeight)
{
    if (rawX < 0)
        self->cursorX = 0.0f;
    if (rawX > renderWidth)
        self->cursorX = static_cast<float>(renderWidth);

    if (rawY < 0)
        self->cursorY = 0.0f;
    if (rawY > renderHeight)
        self->cursorY = static_cast<float>(renderHeight);
}

} // namespace re5::split120
