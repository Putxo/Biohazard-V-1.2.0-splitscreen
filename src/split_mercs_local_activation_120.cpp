#include <cstdint>

namespace re5::split120 {

struct Session120;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern void __thiscall SessionSetSlotAux_C42AB0(Session120*, int, int);
extern bool IsMercsRow1_C42EA0(const Session120*);
extern bool IsMercsLocalSplitRow_C42EC0(const Session120*);

static inline std::uint8_t* Bytes(Session120* p) { return reinterpret_cast<std::uint8_t*>(p); }

// 0xBF53AC..0xBF5401 -- exact per-slot profile-state reset performed by the
// Mercs/Reunion setup loop. The native address expression is preserved instead
// of replacing it with a guessed high-level structure.
void ResetMercsPerSlotProfileFloat_BF53AC(int slot)
{
    const int profileIndex = *reinterpret_cast<const int*>(gPlayerRoot_11B2158 + 0x20);
    const std::uint32_t linear = static_cast<std::uint32_t>(profileIndex * 0x3B8 + slot);
    *reinterpret_cast<float*>(gPlayerRoot_11B2158 + 0x273F8 + linear * 24u) = 0.0f;
}

// 0xBF5261..0xBF5334 -- exact local/session part of the four-slot setup loop.
void SetupMercsSlotLocalState_BF5261(Session120* session, int slot)
{
    auto* b = Bytes(session);
    SessionSetDevice_C42A50(session, slot, -1);

    const int slotState = *reinterpret_cast<const int*>(b + 0x620 + slot * 4);
    if (slotState == 0)
        return;

    const std::uint32_t bit = 1u << slot;
    if (slotState != 2)
        *reinterpret_cast<std::uint32_t*>(b + 0x30) |= bit;

    *reinterpret_cast<std::uint32_t*>(b + 0x478) |= bit;
    SessionSetSlotMode_C42A30(session, slot, 2);
    SessionSetSlotAux_C42AB0(session, slot, 0);

    if (IsMercsLocalSplitRow_C42EC0(session) && slot != 0) {
        const int secondaryDevice = *reinterpret_cast<const int*>(b + 0x658);
        if (secondaryDevice >= 0) {
            *reinterpret_cast<std::uint32_t*>(b + 0x47C) |= bit;
            SessionSetSlotMode_C42A30(session, slot, 0);
            SessionSetDevice_C42A50(session, slot, secondaryDevice);
        }
    }

    ResetMercsPerSlotProfileFloat_BF53AC(slot);
}

// 0xBF522E..0xBF5261 -- exact mask/reset head for the slot loop.
void BeginMercsLocalSlotSetup_BF522E(Session120* session)
{
    auto* b = Bytes(session);
    *reinterpret_cast<std::uint32_t*>(b + 0x478) = 0;
    *reinterpret_cast<std::uint32_t*>(b + 0x47C) = 0;
    *reinterpret_cast<std::uint32_t*>(b + 0x30) = 0;
}

// 0xBF546A..0xBF5502 -- exact primary-slot activation and local-split binding
// tail. primarySlot is the value captured from session+0x5DC at BF505E.
void ActivateMercsPrimaryLocalSlot_BF546A(Session120* session,
                                          int primarySlot,
                                          int preferredDevice)
{
    auto* b = Bytes(session);
    const std::uint32_t bit = 1u << primarySlot;
    *reinterpret_cast<std::uint32_t*>(b + 0x47C) |= bit;
    SessionSetSlotMode_C42A30(session, primarySlot, 0);
    SessionSetDevice_C42A50(session, primarySlot, preferredDevice);

    if (IsMercsLocalSplitRow_C42EC0(session)) {
        const int primaryLocalDevice = *reinterpret_cast<const int*>(b + 0x654);
        SessionSetDevice_C42A50(session, primarySlot, primaryLocalDevice);
    }

    // Row 1 (not row 2) explicitly assigns keyboard/UI ownership to the
    // selected primary slot at BF54F6..BF54FC.
    if (IsMercsRow1_C42EA0(session))
        *reinterpret_cast<int*>(gInput_1249C40 + 0x614) = primarySlot;

    *reinterpret_cast<int*>(b + 0x18) = 6;
    *reinterpret_cast<int*>(b + 0x24) = 2;
    *reinterpret_cast<int*>(b + 0x2C) = 9;
}

void PrepareMercsLocalPlayers_BF522E(Session120* session,
                                     int primarySlot,
                                     int preferredDevice)
{
    BeginMercsLocalSlotSetup_BF522E(session);
    for (int slot = 0; slot < 4; ++slot)
        SetupMercsSlotLocalState_BF5261(session, slot);
    ActivateMercsPrimaryLocalSlot_BF546A(session, primarySlot, preferredDevice);
}

} // namespace re5::split120
