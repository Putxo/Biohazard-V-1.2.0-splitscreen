#include <cstdint>

namespace re5::split120 {

struct SessionPopulationView120 {
    std::uint8_t _0000[0x58];
    std::int32_t mode;
    std::uint8_t _005c[0x5cc-0x5c];
    std::int32_t route;
    std::uint8_t _05d0[0x620-0x5d0];
    std::int32_t state0;
    std::int32_t state1;
    std::int32_t state2;
    std::int32_t state3;
    std::int32_t enabled0;
    std::int32_t enabled1;
    std::int32_t enabled2;
    std::int32_t enabled3;
    std::uint8_t _0640[0x6b8-0x640];
    std::int32_t metric0;
    std::uint8_t _06bc[0x6f8-0x6bc];
    std::int32_t metric1;
    std::uint8_t _06fc[0x738-0x6fc];
    std::int32_t metric2;
    std::uint8_t _073c[0x778-0x73c];
    std::int32_t metric3;
};

static bool IsLowPopulationState(std::int32_t v) { return v == 1 || v == 2; }
static bool IsHighPopulationState(std::int32_t v) { return v == 3; }

// 0x00C43010..0x00C430E9 -- VERIFIED exact control flow.
// Important correction for split-screen: native local Mercenaries/Reunion
// (mode 2/5, route +0x5CC == 2) is explicitly rejected at 0xC43043..4A.
// Therefore this helper is NOT a post-join/local-split completion gate.
bool PopulationGate_C43010(const SessionPopulationView120* s)
{
    const auto mode = s->mode;
    if (mode == 0 || mode == 3 || mode == 4)
        return false;

    if ((mode == 2 || mode == 5) && s->route == 0)
        return false;
    if ((mode == 2 || mode == 5) && s->route == 2)
        return false;

    std::int32_t low = 0;
    std::int32_t high = 0;
    const std::int32_t states[4] = {s->state0, s->state1, s->state2, s->state3};
    for (std::int32_t v : states) {
        if (IsLowPopulationState(v)) ++low;
        else if (IsHighPopulationState(v)) ++high;
    }

    if (low <= 1)
        return true;

    if (mode == 1 && static_cast<std::uint32_t>(s->route - 2) <= 1u && high > 0)
        return true;

    return false;
}

// 0x00C430F0..0x00C431E7 -- VERIFIED exact ranking predicate.
// Only mode 1 participates. In the route 2/3 special case it compares the
// paired metrics +0x6B8/+0x6F8. Otherwise only enabled slots whose state is 2
// contribute metrics. The requested slot wins only if its metric is strictly
// smaller than every other valid metric.
bool IsStrictLowestMetric_C430F0(const SessionPopulationView120* s, std::int32_t slot)
{
    if (s->mode != 1 || static_cast<std::uint32_t>(slot) > 3u)
        return false;

    if (static_cast<std::uint32_t>(s->route - 2) <= 1u) {
        const std::int32_t pair[2] = {s->metric0, s->metric1};
        if (slot > 1)
            return false;
        return pair[slot] < pair[slot ^ 1];
    }

    std::int32_t metric[4] = {-1, -1, -1, -1};
    const std::int32_t enabled[4] = {s->enabled0, s->enabled1, s->enabled2, s->enabled3};
    const std::int32_t states[4] = {s->state0, s->state1, s->state2, s->state3};
    const std::int32_t values[4] = {s->metric0, s->metric1, s->metric2, s->metric3};
    for (int i = 0; i < 4; ++i)
        if (enabled[i] != 0 && states[i] == 2)
            metric[i] = values[i];

    for (int i = 0; i < 4; ++i) {
        if (metric[i] < 0 || i == slot)
            continue;
        if (metric[slot] >= metric[i])
            return false;
    }
    return true;
}

} // namespace re5::split120
