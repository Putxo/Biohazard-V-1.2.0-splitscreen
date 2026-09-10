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

struct SplitUiDispatchOwner120 {
    std::uint8_t _00[0x34];
    std::int32_t selectedJoinSlot; // +0x34
};

struct InputManagerSelectedSlot120 {
    std::uint8_t _00[0x614];
    std::int32_t selectedSlot; // +0x614
};

extern InputManagerSelectedSlot120* gInputManager_1249C40;
extern bool InputDispatchBlocked_799AF0(InputManagerSelectedSlot120* input,
                                        int selector);
extern bool DrawExplicitSplitFallbackCase_9E58A6(void* self,
                                                  int arg1, int x, int y,
                                                  int arg4, int eventId,
                                                  int arg6, int arg7);
extern const SplitFallbackRecord120* FindSplitFallbackRecord_9E5B0C(int eventId);
extern bool DrawFallbackRecord_9E5C6E(void* self,
                                      const SplitFallbackRecord120& record,
                                      int arg1,
                                      int& x,
                                      int& y,
                                      const std::uint32_t* arg4,
                                      int arg6,
                                      int arg7);

// 0x009E5870..0x009E5DD0 -- VERIFIED complete native dispatcher.
// Native ABI: thiscall, seven stack arguments, RET 0x1C.
// Args: (arg1, x, y, arg4, eventId, arg6, arg7).
bool SplitUiTableFallback_9E5870(void* selfOpaque,
                                 int arg1,
                                 int x,
                                 int y,
                                 const std::uint32_t* arg4,
                                 int eventId,
                                 int arg6,
                                 int arg7)
{
    auto* input = gInputManager_1249C40;

    // 0x9E5876..0x9E5885: the global input/UI gate rejects the event outright.
    if (InputDispatchBlocked_799AF0(input, -1))
        return false;

    // 0x9E588B..0x9E58A6: owner+0x34 must either match input+0x614 or be -1.
    const auto* self = static_cast<const SplitUiDispatchOwner120*>(selfOpaque);
    if (self->selectedJoinSlot != input->selectedSlot &&
        self->selectedJoinSlot != -1)
        return false;

    // 0x9E58A6..0x9E5C6B: hard-coded legacy/special event cases.
    if (DrawExplicitSplitFallbackCase_9E58A6(
            selfOpaque,
            arg1,
            x,
            y,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(arg4)),
            eventId,
            arg6,
            arg7))
        return true;

    // 0x9E5B00..0x9E5C6E: exact 55-record table search at 0x1121C98.
    const SplitFallbackRecord120* record =
        FindSplitFallbackRecord_9E5B0C(eventId);
    if (record == nullptr)
        return false;

    // 0x9E5C6E..0x9E5DD0: record override/coordinate transform/final draw.
    return DrawFallbackRecord_9E5C6E(
        selfOpaque, *record, arg1, x, y, arg4, arg6, arg7);
}

} // namespace re5::split120
