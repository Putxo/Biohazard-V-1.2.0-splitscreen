#include <cstdint>

namespace re5::split120 {

struct Session120;

static inline const std::uint8_t* Bytes(const Session120* session)
{
    return reinterpret_cast<const std::uint8_t*>(session);
}

static inline std::int32_t Mode(const Session120* session)
{
    return *reinterpret_cast<const std::int32_t*>(Bytes(session) + 0x58);
}

static inline std::int32_t Selection(const Session120* session)
{
    return *reinterpret_cast<const std::int32_t*>(Bytes(session) + 0x5CC);
}

// 0x00C42E80..0x00C42EDA -- VERIFIED Mercenaries/Reunion row predicates.
bool IsMercsRow0_C42E80(const Session120* session)
{
    const auto mode = Mode(session);
    return (mode == 2 || mode == 5) && Selection(session) == 0;
}

bool IsMercsRow1_C42EA0(const Session120* session)
{
    const auto mode = Mode(session);
    return (mode == 2 || mode == 5) && Selection(session) == 1;
}

bool IsMercsLocalSplitRow_C42EC0(const Session120* session)
{
    const auto mode = Mode(session);
    return (mode == 2 || mode == 5) && Selection(session) == 2;
}

// 0x00C432E0..0x00C4333A -- VERIFIED LIN/DE row predicates.
// These three helpers mirror the three-row DLC frontend one-for-one.
bool IsDlcRow0_C432E0(const Session120* session)
{
    const auto mode = Mode(session);
    return (mode == 3 || mode == 4) && Selection(session) == 0;
}

bool IsDlcRow1_C43300(const Session120* session)
{
    const auto mode = Mode(session);
    return (mode == 3 || mode == 4) && Selection(session) == 1;
}

bool IsDlcLocalSplitRow_C43320(const Session120* session)
{
    const auto mode = Mode(session);
    return (mode == 3 || mode == 4) && Selection(session) == 2;
}

} // namespace re5::split120
