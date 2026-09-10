#include <cstdint>
namespace re5::split120 {
struct AGameRootState120 { std::uint8_t _00[5]; std::uint8_t state5; std::uint8_t substate6; std::uint8_t delay7; };
extern std::uint8_t* gRoot_12340A4;
static inline void RootOrFlag(std::uint32_t flag) {
    *reinterpret_cast<std::uint32_t*>(gRoot_12340A4 + 0x10438) |= flag;
}
// 0x0071A4C0..0x0071A4FA -- VERIFIED.
// Called by aGame state-0 dispatch (0x725E20 -> 0x725E33). Native performs a
// two-step substate transition, then raises root+0x10438 bit 0x10 and moves
// the owning aGame object to state 5. This is the executable-side enabling
// point for the local-join state bit consumed by 0x716310/0x723460.
void EnableLocalJoinRootState_71A4C0(AGameRootState120* self) {
    const std::uint8_t sub = self->substate6;
    if (sub == 0) {
        self->substate6 = static_cast<std::uint8_t>(sub + 1);
        self->delay7 = 1;
    } else if (sub != 1) {
        return;
    }
    self->delay7 = static_cast<std::uint8_t>(self->delay7 - 1);
    if (self->delay7 != 0) return;
    RootOrFlag(0x10u);
    self->state5 = 5;
    self->substate6 = 0;
}
// 0x00725E20 state table: state0 -> 0x71A4C0, state5 -> 0x725B20.
// States1..4 take the common non-AddPlayer path. This compact predicate keeps
// the split/local topology explicit without reconstructing unrelated states.
bool AGameDispatchesLocalJoinOrAddPlayer_725E20(std::uint8_t state) {
    return state == 0 || state == 5;
}
}
