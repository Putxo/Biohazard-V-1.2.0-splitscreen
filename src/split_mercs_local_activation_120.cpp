#include <cstdint>

namespace re5::split120 {

struct Session120;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern void SessionSetSlotMode_C42A30(Session120*, int, int);
extern void SessionSetDevice_C42A50(Session120*, int, int);
extern void SessionSetSlotAux_C42AB0(Session120*, int, int);

static inline std::uint8_t* Bytes(Session120* p) { return reinterpret_cast<std::uint8_t*>(p); }
static inline const std::uint8_t* Bytes(const Session120* p) { return reinterpret_cast<const std::uint8_t*>(p); }

// 0xC42EA0..0xC42EBA and 0xC42EC0..0xC42EDA -- exact row predicates.
bool IsMercsOrReunionRow1_C42EA0(const Session120* session)
{
    const auto* b = Bytes(session);
    const std::int32_t mode = *reinterpret_cast<const std::int32_t*>(b + 0x58);
    return (mode == 2 || mode == 5) &&
           *reinterpret_cast<const std::int32_t*>(b + 0x5CC) == 1;
}

bool IsMercsOrReunionLocalSplit_C42EC0(const Session120* session)
{
    const auto* b = Bytes(session);
    const std::int32_t mode = *reinterpret_cast<const std::int32_t*>(b + 0x58);
    return (mode == 2 || mode == 5) &&
           *reinterpret_cast<const std::int32_t*>(b + 0x5CC) == 2;
}

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
// session+0x620[slot] controls whether the slot participates. Participating
// slots are made available and start in mode 2, not mode 1. In local-split row
// 2, every participating non-zero slot whose secondary binding is valid is
// promoted to active mode 0 and bound to session+0x658.
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

    if (IsMercsOrReunionLocalSplit_C42EC0(session) && slot != 0) {
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

    if (IsMercsOrReunionLocalSplit_C42EC0(session)) {
        const int primaryLocalDevice = *reinterpret_cast<const int*>(b + 0x654);
        SessionSetDevice_C42A50(session, primarySlot, primaryLocalDevice);
    }

    // Row 1 (not row 2) explicitly assigns keyboard/UI ownership to the
    // selected primary slot at BF54F6..BF54FC.
    if (IsMercsOrReunionRow1_C42EA0(session))
        *reinterpret_cast<int*>(gInput_1249C40 + 0x614) = primarySlot;

    *reinterpret_cast<int*>(b + 0x18) = 6;
    *reinterpret_cast<int*>(b + 0x24) = 2;
    *reinterpret_cast<int*>(b + 0x2C) = 9;
}

// Convenience reconstruction of the complete local/session slot mechanics of
// BF522E..BF5502. Generic stage/profile configuration in the same parent
// routine remains in its own subsystem and is intentionally not mislabeled as
// split-screen code.
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
