#include "re5/split_localcoop_120.hpp"

#include <cstddef>
#include <cstdint>

namespace re5::split120 {

struct SplitFallbackRecord120 {
    std::int32_t type;
    std::int32_t flags;
    std::int32_t x;
    std::int32_t y;
    std::int32_t eventId;
    std::int32_t resourceId;
};

// Exact 1.2.0 records recovered from the 55-entry table at 0x01121C98.
// These are the 11 event IDs that are not handled by the older Dev switch.
static constexpr SplitFallbackRecord120 kMissingSplitFallbackRecords[] = {
    {3, 0, 1114, 633, 0x2CC, 2472},
    {3, 0, 1114, 633, 0x126, 2407},
    {3, 0, 1114, 633, 0x127, 2407},
    {3, 0, 1114, 633, 0x134,   -1},
    {3, 0, 1114, 633, 0x113, 2407},
    {3, 0,  256, 633, 0x14D,   -1},
    {0, 0,  100, 600, 0x950, 2418},
    {3, 0,  957, 633, 0x270F,2406},
    {0, 0,  128, 618, 0x951, 2419},
    {3, 0,  924, 633, 0x952, 2386},
    {3, 0,  924, 633, 0x974, 2420},
};

const SplitFallbackRecord120* FindMissingSplitFallbackRecord_9E5B0C(int eventId)
{
    for (const auto& r : kMissingSplitFallbackRecords) {
        if (r.eventId == eventId)
            return &r;
    }
    return nullptr;
}

bool IsMissingSplitFallbackHandledNoDraw_9E5C78(int eventId)
{
    const auto* r = FindMissingSplitFallbackRecord_9E5B0C(eventId);
    return r != nullptr && r->resourceId < 0;
}

} // namespace re5::split120
