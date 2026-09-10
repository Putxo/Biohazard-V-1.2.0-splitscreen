#include <cstdint>

namespace re5::split120 {

struct AGame120 { std::uint8_t _00[4]; std::uint32_t mainState; std::uint8_t _08[0x38]; std::uint8_t flag40; std::uint8_t _41[0x1B]; std::int32_t state5C; std::uint8_t _60[0x0C]; std::int32_t state6C; };
extern std::uint8_t* gRoot_12340A4;

extern void UpdateAGameBase_71D4E0(AGame120*);
extern void RefreshLocalJoinBit800_71B0E0(void*);
extern bool EarlyStoryGate_71A940(AGame120*);
extern void HandleRoot4000_723420(AGame120*);
extern bool StoryTransitionGate_719F10(AGame120*);
extern bool StoryImmediateGate_7165F0(AGame120*);
extern void StoryImmediateRoute_7165D0(AGame120*);
extern void StoryFlag40Route_719020(AGame120*);
extern void StoryState5CRoute_716120(AGame120*);
extern void UpdateStoryRoute_724190(AGame120*);
extern bool SessionPhaseIs1Or2Or5_715B20(const void*);
extern void UpdateStoryPhase125_7245D0(AGame120*);
extern void UpdateStoryCommon_721370(AGame120*);
extern void PollLocalJoin_723460(AGame120*);
extern void UpdateState9_71A500(AGame120*);
extern void UpdateStoryLocalPlayerTransition_716810(AGame120*);
extern void UpdateStoryTail_71AC10(AGame120*, int);

// 0x00724A20..0x00724B46 -- VERIFIED full recurring Story/local-coop driver.
// This is aGame main state 2 (0x725E80 dispatcher). Crucially, PollLocalJoin
// is called every eligible frame, followed by the transition evaluator 716810.
void UpdateStoryLocalJoinDriver_724A20(AGame120* self) {
    UpdateAGameBase_71D4E0(self);
    RefreshLocalJoinBit800_71B0E0(self);
    if (EarlyStoryGate_71A940(self)) return;

    const std::uint32_t flags = *reinterpret_cast<const std::uint32_t*>(gRoot_12340A4 + 0x10438);
    if ((flags & 0x00010000u) != 0) {
        if ((self->mainState & 0xFFu) != 0x0Cu) {
            self->mainState = 0;
            reinterpret_cast<std::uint8_t*>(&self->mainState)[0] = 0x0C;
        }
        return;
    }
    if ((flags & 0x00004000u) != 0) { HandleRoot4000_723420(self); return; }
    if (StoryTransitionGate_719F10(self)) return;

    auto* session = *reinterpret_cast<std::uint8_t**>(gRoot_12340A4 + 0x1042C);
    const int phase = *reinterpret_cast<const std::int32_t*>(session + 0x58);
    if (phase == 0 || phase == 3 || phase == 4) {
        if (StoryImmediateGate_7165F0(self)) { StoryImmediateRoute_7165D0(self); return; }
        if (self->flag40 != 0) { StoryFlag40Route_719020(self); return; }
        if (session[0x56C] == 0 || self->state6C == 3) {
            if (self->state5C >= 0) { StoryState5CRoute_716120(self); return; }
        }
        if ((*reinterpret_cast<const std::uint32_t*>(gRoot_12340A4 + 0x10438) & 0x400u) != 0) {
            self->mainState = 0;
            reinterpret_cast<std::uint8_t*>(&self->mainState)[0] = 5;
            return;
        }
    }

    UpdateStoryRoute_724190(self);
    if (SessionPhaseIs1Or2Or5_715B20(session)) UpdateStoryPhase125_7245D0(self);
    UpdateStoryCommon_721370(self);
    PollLocalJoin_723460(self);
    UpdateState9_71A500(self);
    UpdateStoryLocalPlayerTransition_716810(self);
    UpdateStoryTail_71AC10(self, 0);
}

} // namespace re5::split120
