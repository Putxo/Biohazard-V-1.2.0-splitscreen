#include <cstdint>

namespace re5::split120 {

struct AGameDispatch120 { std::uint8_t _00[4]; std::uint32_t mainState; std::uint8_t _08[0x340]; std::uint8_t* currentInput; std::uint8_t* previousInput; };
extern std::uint8_t* gRoot_12340A4;
extern void UpdateDispatchPrelude_718D90(AGameDispatch120*);
extern void State0_725D00(AGameDispatch120*); extern void State1_725D20(AGameDispatch120*);
extern void UpdateStoryLocalJoinDriver_724A20(AGameDispatch120*); extern void State3_722A90(AGameDispatch120*);
extern void State4_725DB0(AGameDispatch120*); extern void State5_722D70(AGameDispatch120*);
extern void State6_71F840(AGameDispatch120*); extern void State7_723380(AGameDispatch120*);
extern void State8_7233E0(AGameDispatch120*); extern void State9_725030(AGameDispatch120*);
extern void UpdateLocalTransitionState10_725940(AGameDispatch120*); extern void TrialSplitState11_720A50(AGameDispatch120*);
extern void TrialSplitState12_720BF0(AGameDispatch120*); extern void UpdateAddPlayerOuterState13_725E20(AGameDispatch120*);
extern void State14_720E90(AGameDispatch120*); extern void State15_7210C0(AGameDispatch120*);
extern void UpdateDispatchPostA_7196E0(AGameDispatch120*); extern void UpdateDispatchPostB_71FD70(AGameDispatch120*); extern void UpdateDispatchPostC_716260(AGameDispatch120*);

// 0x00725E80..0x00725F8F -- VERIFIED complete top-level aGame dispatcher.
// Native local-coop states are 2 (recurring Story join), 10 (local transition),
// 11/12 (trial_split states already reconstructed), and 13 (Add Player).
void DispatchAGameState_725E80(AGameDispatch120* self) {
    UpdateDispatchPrelude_718D90(self);
    if ((*reinterpret_cast<const std::uint32_t*>(gRoot_12340A4 + 0x10434) & 0x500u) != 0) return;
    if (*reinterpret_cast<const std::int32_t*>(gRoot_12340A4 + 0x60) == 0 && gRoot_12340A4[0x64] != 0) return;

    switch (self->mainState & 0xFFu) {
    case 0: State0_725D00(self); break; case 1: State1_725D20(self); break;
    case 2: UpdateStoryLocalJoinDriver_724A20(self); break; case 3: State3_722A90(self); break;
    case 4: State4_725DB0(self); break; case 5: State5_722D70(self); break;
    case 6: State6_71F840(self); break; case 7: State7_723380(self); break;
    case 8: State8_7233E0(self); break; case 9: State9_725030(self); break;
    case 10: UpdateLocalTransitionState10_725940(self); break;
    case 11: TrialSplitState11_720A50(self); break; case 12: TrialSplitState12_720BF0(self); break;
    case 13: UpdateAddPlayerOuterState13_725E20(self); break;
    case 14: State14_720E90(self); break; case 15: State15_7210C0(self); break;
    default: break;
    }

    UpdateDispatchPostA_7196E0(self);
    UpdateDispatchPostB_71FD70(self);
    UpdateDispatchPostC_716260(self);
    self->previousInput[0]=self->currentInput[0]; self->previousInput[1]=self->currentInput[1];
    self->previousInput[2]=self->currentInput[2]; self->previousInput[3]=self->currentInput[3];
}

} // namespace re5::split120
