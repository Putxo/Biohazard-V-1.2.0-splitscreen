#include <cstdint>

namespace re5::split120 {

// Session-local player slot state reconstructed from the native 1.2.0 session
// constructor and its small slot/device accessors.
struct SessionSlotState120 {
    std::uint8_t _0000[0x478];
    std::uint32_t availableMask; // +0x478
    std::uint32_t activeMask;    // +0x47C
    std::int32_t slotMode[4];    // +0x480
    std::int32_t device[4];      // +0x490
};

// 0x00C43A00 constructor fragment -- VERIFIED slot/local-player setup.
// Native defaults expose two local-capable session slots (bits 0 and 1), make
// slot 0 active/local and slot 1 joinable, and bind slot 0 to the preferred
// input device from inputManager+0x5D8.
void InitLocalCoopSessionSlots_C43A00(SessionSlotState120* s, int preferredDevice)
{
    s->availableMask = 0x3;

    for (int i = 0; i < 4; ++i) {
        s->slotMode[i] = (i == 0) ? 0 : 1;
        s->device[i] = -1;
    }

    s->device[0] = preferredDevice;
    s->activeMask = 0x1;
}

// 0x00C42A30 -- exact slot-mode setter.
void SetSessionSlotMode_C42A30(SessionSlotState120* s, int slot, int mode)
{
    if (static_cast<unsigned>(slot) <= 3u)
        s->slotMode[slot] = mode;
}

// 0x00C42A50 -- exact per-slot input-device setter.
void SetSessionSlotDevice_C42A50(SessionSlotState120* s, int slot, int device)
{
    if (static_cast<unsigned>(slot) <= 3u)
        s->device[slot] = device;
}

// 0x00C42B30 -- popcount of the four available-slot bits.
int CountAvailableSessionSlots_C42B30(const SessionSlotState120* s)
{
    const unsigned v = s->availableMask & 0xFu;
    return int((v & 1u) != 0) + int((v & 2u) != 0) +
           int((v & 4u) != 0) + int((v & 8u) != 0);
}

// 0x00C42B60 -- popcount of the four active-player bits.
int CountActiveSessionSlots_C42B60(const SessionSlotState120* s)
{
    const unsigned v = s->activeMask & 0xFu;
    return int((v & 1u) != 0) + int((v & 2u) != 0) +
           int((v & 4u) != 0) + int((v & 8u) != 0);
}

} // namespace re5::split120
