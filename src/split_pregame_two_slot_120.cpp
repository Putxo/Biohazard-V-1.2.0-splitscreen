#include <cstdint>

namespace re5::split120 {

struct SessionTwoSlotView120 {
    std::uint8_t _0000[0x478];
    std::uint32_t availableMask; // +0x478
    std::uint32_t activeMask;    // +0x47C
    std::int32_t slotMode[4];    // +0x480
    std::int32_t device[4];      // +0x490
    std::int32_t field4A0[4];    // +0x4A0
    std::int32_t field4B0[4];    // +0x4B0
    std::int32_t field4C0[4];    // +0x4C0
};

// 0x00769A63..0x00769B7F -- VERIFIED two-slot pre-game setup fragment.
// At entry to this branch EDI is known zero (0x769976). The native code
// prepares slots 0/1 as available, clears the active mask, marks both slot
// modes as 1, binds slot 0 to inputManager+0x5D8, leaves slot 1 unbound, and
// clears the adjacent per-slot state arrays for those two slots.
void PrepareTwoLocalSlots_769A63(SessionTwoSlotView120* s, int preferredDevice)
{
    s->availableMask = 0x3;
    s->activeMask = 0;

    s->slotMode[0] = 1;
    s->slotMode[1] = 1;

    s->device[0] = preferredDevice;
    s->device[1] = -1;

    s->field4A0[0] = 0;
    s->field4A0[1] = 0;
    s->field4B0[0] = 0;
    s->field4B0[1] = 0;
    s->field4C0[0] = 0;
    s->field4C0[1] = 0;
}

} // namespace re5::split120
