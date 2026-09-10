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

// 0x00A4F4E0..0x00A4F5AA -- VERIFIED.
// The common Extras/DLC frontend distinguishes the two DLC game modes by
// session+0x58. Native sets self+0x19C to 0 only for mode 3, and to 1 for
// every other mode reaching this frontend. The surrounding loader uses that
// bit to select the paired Extra resources; in the actual caller family those
// modes are 3 (Lost in Nightmares) and 4 (Desperate Escape).
std::uint32_t ResolveDlcFrontendVariant_A4F4E0()
{
    const auto* session = SessionFromRoot_12340A4();
    const auto mode = *reinterpret_cast<const std::int32_t*>(
        reinterpret_cast<const std::uint8_t*>(session) + 0x58);
    return mode == 3 ? 0u : 1u;
}

// 0x00A52D1F..0x00A52DD9 -- VERIFIED.
// self+0x190 is navigated with an exact count of three. Confirmation validates
// bit(selected) in self+0x194 and writes the selected row directly to
// session+0x5CC. This is the common LIN/DE three-row mode selector.
void CommitDlcThreeWayModeSelection_A52DD4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

// 0x00C43BB0..0x00C43BF0 -- exact behavior relevant to DLC.
// Unlike modes 2/5, modes 3/4 have no +0x5CC==2 early special-case here.
// They continue through the normal local-player-count path (unless +0x56C is
// set), so +0x5CC remains a frontend selection value while local readiness is
// decided by the session's active local-player state.
bool DlcUsesGenericLocalCountPath_C43BB0(std::int32_t mode)
{
    return mode == 3 || mode == 4;
}

} // namespace re5::split120
