#include <cstdint>
namespace re5::split120 {
struct AGameRootState120 { std::uint8_t _00[5]; std::uint8_t state5; std::uint8_t substate6; std::uint8_t delay7; };
struct GameRoot120;
extern GameRoot120* gRoot_12340A4;
extern void OrRootStateFlags_726120(GameRoot120*, std::uint32_t);

// 0x0071A4C0..0x0071A4FA -- VERIFIED.
// Called by aGame state-0 dispatch (0x725E20 -> 0x725E33). Native performs a
// two-step substate transition, calls synchronized helper 0x726120 with 0x10,
// then moves the owning aGame object to state 5.
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
    OrRootStateFlags_726120(gRoot_12340A4, 0x10u);
    self->state5 = 5;
    self->substate6 = 0;
}

// 0x00725E20 state table: state0 -> 0x71A4C0, state5 -> 0x725B20.
bool AGameDispatchesLocalJoinOrAddPlayer_725E20(std::uint8_t state) {
    return state == 0 || state == 5;
}
}
