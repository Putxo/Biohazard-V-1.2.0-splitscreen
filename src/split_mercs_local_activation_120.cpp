#include <cstdint>

namespace re5::split120 {

struct Session120;

static inline std::uint8_t* Bytes(Session120* p) { return reinterpret_cast<std::uint8_t*>(p); }

// 0xC42EC0..0xC42EDA -- VERIFIED exact predicate.
bool IsMercsOrReunionLocalSplit_C42EC0(const Session120* session)
{
    const auto* b = reinterpret_cast<const std::uint8_t*>(session);
    const std::int32_t mode = *reinterpret_cast<const std::int32_t*>(b + 0x58);
    if (mode != 2 && mode != 5) return false;
    return *reinterpret_cast<const std::int32_t*>(b + 0x5CC) == 2;
}

// Split-specific portion of 0xBF5050 setup, exact branch 0xBF52E5..0xBF5334.
// Native has already marked the current slot available and in slotMode=2.
// When the local-split predicate is true, a non-primary eligible slot with a
// valid secondary binding at session+0x658 is promoted to active/local mode 0
// and receives that binding through C42A50.
void PromoteMercsSecondaryLocalSlot_BF52E5(Session120* session,
                                           int slot,
                                           int primarySlot,
                                           int secondaryDevice)
{
    if (!IsMercsOrReunionLocalSplit_C42EC0(session)) return;
    if (slot == primarySlot) return;
    if (secondaryDevice < 0) return;

    auto* b = Bytes(session);
    *reinterpret_cast<std::uint32_t*>(b + 0x47C) |= (1u << slot);
    // Native continuation:
    //   C42A30(session, slot, 0)
    //   C42A50(session, slot, secondaryDevice)
}

// 0xBF546A..0xBF54DC -- VERIFIED primary-slot activation tail.
// primarySlot is session+0x5DC. Native initially assigns input+0x5D8, then
// overwrites it with session+0x654 when the local-split predicate is true.
// Together +0x654/+0x658 are therefore the primary/secondary local bindings
// consumed by the Mercenaries/Reunion split setup.
int ResolveMercsPrimaryDevice_BF54BF(const Session120* session,
                                     int preferredDevice)
{
    const auto* b = reinterpret_cast<const std::uint8_t*>(session);
    if (IsMercsOrReunionLocalSplit_C42EC0(session))
        return *reinterpret_cast<const std::int32_t*>(b + 0x654);
    return preferredDevice;
}

} // namespace re5::split120
