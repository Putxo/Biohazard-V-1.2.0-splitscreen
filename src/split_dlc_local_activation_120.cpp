#include <cstdint>

namespace re5::split120 {

struct Session120;
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void SessionSetSlotMode_C42A30(Session120*, int, int);
extern void SessionSetSlotBinding_C42A50(Session120*, int, int);
extern void SessionSetSlotAux_C42AB0(Session120*, int, int);
extern bool IsDlcLocalSplitSelection_C43320(Session120*);

static inline Session120* Session()
{
    return *reinterpret_cast<Session120**>(gRoot_12340A4 + 0x1042C);
}

static inline std::uint8_t* Bytes(Session120* session)
{
    return reinterpret_cast<std::uint8_t*>(session);
}

// 0x00BF5888..0x00BF5A4A -- VERIFIED local-player/session setup used by the
// LIN/DE start path. The parent routine clears the session slot masks, resets
// all four slot bindings, makes all four slots available, then activates the
// selected primary slot using the preferred input binding from input+0x5D8.
//
// When 0xC43320 is true (mode 3/4 and +0x5CC==2), native overrides the
// primary binding with session+0x654, activates the paired slot (slot^1), and
// assigns session+0x658 to that partner. This is the concrete second-local-
// player activation branch for the DLC local-coop row.
void PrepareDlcLocalPlayers_BF5888(int primarySlot)
{
    auto* session = Session();
    auto* bytes = Bytes(session);

    *reinterpret_cast<std::uint32_t*>(bytes + 0x478) = 0;
    *reinterpret_cast<std::uint32_t*>(bytes + 0x47C) = 0;
    *reinterpret_cast<std::uint32_t*>(bytes + 0x30) = 0;

    for (int slot = 0; slot < 4; ++slot) {
        SessionSetSlotBinding_C42A50(session, slot, -1);
        *reinterpret_cast<std::uint32_t*>(bytes + 0x478) |= (1u << slot);
        SessionSetSlotMode_C42A30(session, slot, 1);
        SessionSetSlotAux_C42AB0(session, slot, 0);
    }

    *reinterpret_cast<std::uint32_t*>(bytes + 0x47C) |= (1u << primarySlot);
    SessionSetSlotMode_C42A30(session, primarySlot, 0);

    const int preferredBinding =
        *reinterpret_cast<const int*>(gInput_1249C40 + 0x5D8);
    SessionSetSlotBinding_C42A50(session, primarySlot, preferredBinding);

    if (IsDlcLocalSplitSelection_C43320(session)) {
        const int firstLocalBinding =
            *reinterpret_cast<const int*>(bytes + 0x654);
        const int secondLocalBinding =
            *reinterpret_cast<const int*>(bytes + 0x658);

        SessionSetSlotBinding_C42A50(session, primarySlot, firstLocalBinding);

        const int partnerSlot = primarySlot ^ 1;
        *reinterpret_cast<std::uint32_t*>(bytes + 0x47C) |=
            (1u << partnerSlot);
        SessionSetSlotMode_C42A30(session, partnerSlot, 0);
        SessionSetSlotBinding_C42A50(session, partnerSlot, secondLocalBinding);
    }

    *reinterpret_cast<std::uint32_t*>(bytes + 0x18) = 9;
    *reinterpret_cast<std::uint32_t*>(bytes + 0x24) = 2;
}

} // namespace re5::split120
