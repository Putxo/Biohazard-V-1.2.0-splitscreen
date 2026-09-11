#include <cstdint>
#include <cstddef>

namespace re5::split120 {

// Layout audit for the two native objects consumed by C43BB0 and 799B50.
// The actual executable VAs have one canonical C++ definition each:
//   C43BB0 -> split_session_mode_predicates_120.cpp
//   799B50 -> split_input_owner_queries_120.cpp
// This unit intentionally contains no second implementation of either VA.
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

// Typed audit adapters only. They deliberately omit native VA suffixes so they
// cannot masquerade as additional executable functions in the static archive.
bool AuditNativeLocalSplitRouteView_120(SessionLocalRouteView120* session)
{
    return IsNativeLocalCoopActive_C43BB0(session);
}

bool AuditLocalSplitInputPrecheckView_120(InputLocalRouteView120* input)
{
    return PreferredOwnerAllowsCurrentKeyboard_799B50(input);
}

} // namespace re5::split120
