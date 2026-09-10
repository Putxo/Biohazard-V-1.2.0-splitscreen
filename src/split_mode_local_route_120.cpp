#include <cstdint>
#include <cstddef>

namespace re5::split120 {

struct SessionLocalRouteView120 {
    std::uint8_t _0000[0x58];
    std::int32_t mode;              // +0x58
    std::uint8_t _005C[0x56C - 0x5C];
    std::uint8_t blockLocalRoute;   // +0x56C
    std::uint8_t _056D[0x5CC - 0x56D];
    std::int32_t localRouteKind;    // +0x5CC
};

struct InputLocalRouteView120 {
    std::uint8_t _0000[0x614];
    std::int32_t selectedPlayer;    // +0x614
    std::int32_t routePlayer;       // +0x618
};

static_assert(__builtin_offsetof(SessionLocalRouteView120, mode) == 0x58);
static_assert(__builtin_offsetof(SessionLocalRouteView120, blockLocalRoute) == 0x56C);
static_assert(__builtin_offsetof(SessionLocalRouteView120, localRouteKind) == 0x5CC);
static_assert(__builtin_offsetof(InputLocalRouteView120, selectedPlayer) == 0x614);
static_assert(__builtin_offsetof(InputLocalRouteView120, routePlayer) == 0x618);

extern int CountActivePlayers_C42B60(SessionLocalRouteView120* session);

// 0x00C43BB0..0x00C43BF0 -- VERIFIED.
// Native mode/local-route predicate used by 0x799B50.
bool IsNativeLocalSplitRoute_C43BB0(SessionLocalRouteView120* session)
{
    const auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
    const auto* nativeSession = *reinterpret_cast<SessionLocalRouteView120* const*>(root + 0x1042C);
    const int mode = nativeSession->mode;

    if ((mode == 2 || mode == 5) && nativeSession->localRouteKind == 2)
        return true;

    if (mode == 1)
        return false;

    if (session->blockLocalRoute != 0)
        return false;

    return CountActivePlayers_C42B60(session) >= 2;
}

// 0x00799B50..0x00799B89 -- VERIFIED.
// Returns whether the caller may continue through the local-split/input path.
bool LocalSplitInputPrecheck_799B50(InputLocalRouteView120* self)
{
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
    auto* session = *reinterpret_cast<SessionLocalRouteView120**>(root + 0x1042C);

    if (!IsNativeLocalSplitRoute_C43BB0(session))
        return true;

    const int routePlayer = self->routePlayer;
    if (routePlayer < 0)
        return true;

    auto* input = *reinterpret_cast<InputLocalRouteView120**>(0x01249C40);
    return routePlayer == input->selectedPlayer;
}

} // namespace re5::split120
