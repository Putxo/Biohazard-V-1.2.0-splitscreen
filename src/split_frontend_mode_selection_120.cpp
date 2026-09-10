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

// 0x00A49993..0x00A49B1F -- VERIFIED selection/load/commit path.
// self+0x1A0 is loaded from session+0x5CC. The preceding navigation path
// 0xA49FD2..0xA4A073 constrains it to four entries through 0x7B5870(...,4,...).
// Confirmation at 0xA4A0C0 validates the selected bit in self+0x1A8 and then
// commits self+0x1A0 back to session+0x5CC at 0xA4A0FA.
void CommitFourWaySessionModeSelection_A4A0FA(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

// 0x00A499DB..0x00A49B27 -- VERIFIED three-way selector path.
// self+0x18C is navigated with count 3 (0xA49A40/0xA49A54) and confirmation
// writes that exact value to session+0x5CC at 0xA49AE4 after checking
// self+0x19C bit(selected).
void CommitThreeWaySessionModeSelection_A49AE4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

// 0x00A52CB0..0x00A52DFC -- VERIFIED independent three-way selector.
// self+0x190 is navigated with count 3 via 0x7B5870 and confirmation checks
// self+0x194 bit(selected) before storing selected to session+0x5CC at 0xA52DD4.
void CommitThreeWaySessionModeSelection_A52DD4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

} // namespace re5::split120
