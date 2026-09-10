#include "re5/split_localcoop_120.hpp"
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

// Exact RE5DX9 1.2.0 table at VA 0x01121C98.
// 55 records, stride 0x18. The following record is the native type==4
// sentinel: {4,0,0,0,-1,-1}.
static constexpr SplitFallbackRecord120 kSplitFallbackRecords_1121C98[55] = {
    {3,0,1114,633,  688,2407}, {3,0,1114,633,  689,2407},
    {3,0,1114,633,  690,2407}, {3,0,1114,633,  691,2407},
    {3,0,1114,633,  692,2407}, {3,0, 207,633,  697,2425},
    {3,0,1114,633,  698,2407}, {3,0,1114,633,  708,2407},
    {2,0,1114,633,  709,  -1}, {3,0, 207,633,  710,2425},
    {3,0, 957,633,  712,2406}, {3,0,1114,633,  716,2472},
    {3,0,1114,651,  276,2407}, {3,0, 904,651,  277,  -1},
    {3,0,1114,633,  280,  -1}, {3,0,1114,633,  294,2407},
    {3,0,1114,633,  295,2407}, {3,0,1114,633,  296,2407},
    {3,0,1114,633,  297,2407}, {3,0,1114,633,  308,  -1},
    {3,0, 256,633,  274,  -1}, {3,0,1114,633,  275,2407},
    {3,0, 256,633,  333,  -1}, {3,0, 957,633, 1534,2406},
    {3,0, 210,633, 1535,2485}, {3,0, 449,633, 1536,2413},
    {3,0, 449,633, 1537,2414}, {3,0, 675,633, 1538,2417},
    {3,0, 923,633, 1539,2415}, {3,0, 923,633, 1540,2416},
    {3,0,1114,633, 1541,2407}, {0,0,  -1, -1, 1553,  -1},
    {0,0, 100,600, 1684,2418}, {0,0, 100,600, 2384,2418},
    {3,0, 500,633,  399,2426}, {3,0, 640,633,  400,2427},
    {3,0, 717,633, 1740,2410}, {0,0,  -1, -1, 2453,2391},
    {0,0,  -1, -1,  366,  -1}, {3,0, 957,633, 9999,2406},
    {0,0, 128,618, 2385,2419}, {3,0, 924,633, 2386,2386},
    {3,0, 924,633, 2420,2420}, {2,0,  -1, -1,  696,2472},
    {0,0, 128,618,  677,2473}, {0,0,  -1, -1,  699,2474},
    {0,0,  -1, -1,  701,2476}, {0,0,  -1, -1,  704,2477},
    {0,0,  -1, -1,  705,2478}, {0,0,  -1, -1,  711,2479},
    {0,0,  -1, -1,  668,2480}, {0,0,  -1, -1,  669,2481},
    {2,0,1152,633,  702,  -1}, {1,0, 640,618, 1791,2483},
    {2,0,1152,618,  703,2484},
};

const SplitFallbackRecord120* FindSplitFallbackRecord_9E5B0C(int eventId)
{
    for (const auto& record : kSplitFallbackRecords_1121C98) {
        if (record.eventId == eventId)
            return &record;
    }
    return nullptr;
}

// Compatibility helper retained for callers interested specifically in
// records that consume the event without issuing a low-level draw.
bool IsSplitFallbackHandledNoDraw_9E5C78(int eventId)
{
    const auto* record = FindSplitFallbackRecord_9E5B0C(eventId);
    return record != nullptr && record->resourceId < 0;
}

} // namespace re5::split120
