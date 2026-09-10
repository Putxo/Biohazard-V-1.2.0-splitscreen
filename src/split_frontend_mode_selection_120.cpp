#include <cstdint>

namespace re5::split120 {

struct Session120;

static inline Session120* SessionFromRoot_12340A4()
{
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
    return *reinterpret_cast<Session120**>(root + 0x1042C);
}

static inline std::uint8_t* Bytes(Session120* session)
{
    return reinterpret_cast<std::uint8_t*>(session);
}

// Normal Mercenaries frontend family.
// Vtable update entry 0xA47CE0 shares its class with init method 0xA41410;
// 0xA41410 directly requests "id\\jpn\\merce\\merce_00" (0xFA9C04).
// 0xA48051..0xA480A0 is the exact selection commit branch:
//   selection 0..2 -> session+0x5CC = selection, session+0x79E = 0
//   selection 3    -> session+0x5CC = 0,         session+0x79E = 1
// Thus physical row 2 is preserved as +0x5CC==2, while row 3 is the
// distinct special/no-mercy path carried by +0x79E rather than mode 3.
void CommitMercenariesModeSelection_A48051(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    auto* b = Bytes(session);
    if (selected == 3) {
        *reinterpret_cast<std::int32_t*>(b + 0x5CC) = 0;
        b[0x79E] = 1;
    } else {
        *reinterpret_cast<std::int32_t*>(b + 0x5CC) = selected;
        b[0x79E] = 0;
    }
}

// Reunion / merce_dc frontend family.
// Vtable update entry 0xA49720 is paired with init method 0xA45D80.
// 0xA45D80 directly loads:
//   0xFA9F58 -> "id\\jpn\\merce_dc\\merce_00"
//   0xFA9F3C -> "id\\jpn\\merce_dc\\merce_dc01"
// This makes the class identity instruction-backed rather than inferred from
// the shape of the selector alone.
// 0xA499DB..0xA49B27 is the VERIFIED three-way selector path: self+0x18C is
// navigated with count 3, validated through self+0x19C, and committed exactly
// to session+0x5CC at 0xA49AE4. The same branch clears session+0x79E.
void CommitReunionThreeWaySelection_A49AE4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    auto* b = Bytes(session);
    *reinterpret_cast<std::int32_t*>(b + 0x5CC) = selected;
    b[0x79E] = 0;
}

// Four-way session mode selector in the neighboring frontend family.
// Kept semantically neutral until its resource/class identity is closed.
// 0xA49FD2..0xA4A073 constrains self+0x1A0 to four entries and 0xA4A0C0
// validates self+0x1A8 before the exact write at 0xA4A0FA.
void CommitFourWaySessionModeSelection_A4A0FA(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

// Extra/DLC frontend family containing LIN/DE menu resources.
// Vtable update entry 0xA52970 belongs to the class initialized through the
// A4F500/A4F543 resource path. 0xA4F543 directly requests 0xFAA688:
// "id\\jpn\\extra\\extra_03". The same vtable is at 0xFAA64C and is followed
// in rdata by extra_03/top50/top54/extra_01/extra_02 resource names.
// 0xA52CB0..0xA52DFC is its VERIFIED independent three-way selector; self+0x190
// is navigated with count 3, checked against self+0x194, and stored to +0x5CC.
void CommitExtraDlcThreeWaySelection_A52DD4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

} // namespace re5::split120
