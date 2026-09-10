#include <cstdint>

namespace re5::split120 {

struct Session120;
struct StageController120 {
    std::uint8_t _0000[0x1849];
    std::uint8_t scenarioVariant; // +0x1849
};

extern std::uint8_t* gPlayerStateRoot_11B2158;
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void SessionSetSlotMode_C42A30(Session120*, int slot, int mode);
extern void SessionSetDevice_C42A50(Session120*, int slot, int device);
extern void SessionSetPair_C42A70(Session120*, int slot, int value);
extern void SessionSetPair_C42A90(Session120*, int slot, int value);
extern void SessionSetPair_C42AB0(Session120*, int slot, int value);
extern void SessionSetPair_C42D70(Session120*, int slot, int value);

static inline Session120* Session()
{
    return *reinterpret_cast<Session120**>(gRoot_12340A4 + 0x1042C);
}

// 0x76965C..0x7696AD: native maps selected scenario id 0x320..0x324
// from player-state +0x273D8 to a compact variant 0..4 at self+0x1849.
std::uint8_t ResolvePregameVariant_76965C(const std::uint8_t* playerRoot,
                                          std::uint8_t previous)
{
    const int index = *reinterpret_cast<const int*>(playerRoot + 0x20);
    const int scenario = *reinterpret_cast<const int*>(
        playerRoot + index * 0x5940 + 0x273D8);
    switch (scenario) {
        case 0x320: return 0;
        case 0x321: return 1;
        case 0x322: return 2;
        case 0x323: return 3;
        case 0x324: return 4;
        default:    return previous;
    }
}

// 0x769A63..0x769B7F: exact two-slot pregame session shape reached only
// when the compact scenario variant is below 4. Variant 4 takes the
// 0x769987 branch instead and clears availableMask.
void PreparePregameTwoLocalSlots_769A63()
{
    auto* session = Session();
    auto* bytes = reinterpret_cast<std::uint8_t*>(session);
    *reinterpret_cast<std::uint32_t*>(bytes + 0x478) = 0x3;
    *reinterpret_cast<std::uint32_t*>(bytes + 0x47C) = 0x0;

    SessionSetSlotMode_C42A30(session, 0, 1);
    SessionSetSlotMode_C42A30(session, 1, 1);

    const int preferredDevice = *reinterpret_cast<const int*>(gInput_1249C40 + 0x5D8);
    SessionSetDevice_C42A50(session, 0, preferredDevice);
    SessionSetDevice_C42A50(session, 1, -1);

    SessionSetPair_C42A70(session, 0, 0);
    SessionSetPair_C42A70(session, 1, 0);
    SessionSetPair_C42A90(session, 0, 0);
    SessionSetPair_C42A90(session, 1, 0);
    SessionSetPair_C42AB0(session, 0, 0);
    SessionSetPair_C42AB0(session, 1, 0);
    SessionSetPair_C42D70(session, 0, 0);
    SessionSetPair_C42D70(session, 1, 0);
}

bool PregameVariantUsesTwoLocalSlots_769971(std::uint8_t variant)
{
    return variant < 4;
}

} // namespace re5::split120
