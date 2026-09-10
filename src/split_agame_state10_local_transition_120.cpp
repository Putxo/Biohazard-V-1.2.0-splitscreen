#include <cstdint>

namespace re5::split120 {

struct GameRoot120;
struct AGameState10_120 {
    std::uint8_t _00[5];
    std::uint8_t substate5;
    std::uint8_t _06;
    std::uint8_t delay7;
    std::uint8_t _08[0x354];
    std::int32_t state35C;
};
struct Session120 { std::uint8_t _00[0x24]; std::int32_t transition24; };

extern GameRoot120* gRoot_12340A4;
extern void* gAsyncGate_11E8804;
extern std::uint8_t* gProfileState_11B2158;
extern void* gEventManager_1234578;

extern void OrRootStateFlags_726120(GameRoot120*, std::uint32_t);
extern void ClearRootStateFlags_715A30(GameRoot120*, std::uint32_t);
extern void OrRootIndexedFlags_726170(GameRoot120*, std::uint32_t, std::uint8_t);
extern void RootTransition_780A80(GameRoot120*, int, float);
extern void RefreshFrontend_7A40D0();
extern void StartDelay_716DB0(AGameState10_120*, float);
extern int  QueryAsyncGate_11E8804();
extern void UpdateState9_71A500(AGameState10_120*);
extern bool TransitionReady_71DF70(AGameState10_120*);
extern void FinalizeRoute_723870(AGameState10_120*, int);
extern int  QueryRoute_716AD0(AGameState10_120*, int);
extern void AlternateFinalize_71A190(AGameState10_120*);
extern Session120* RootSession_1042C(GameRoot120*);
extern void DispatchEvent_11AFF64(void* eventManager);
extern std::uint32_t CurrentStageId_11B2158();

// 0x00725940..0x00725AEC -- VERIFIED.
// aGame main state 10. The outer dispatcher at 0x725E80 indexes self+4 and
// entry 10 (0x725F07) calls this routine. Native state remap table at 0x725B08:
//   substate 0 -> init, 1 -> async wait, 2..8 -> no-op,
//   9 -> route helper, 10 -> readiness wait, 11 -> finalization.
void UpdateLocalTransitionState10_725940(AGameState10_120* self)
{
    switch (self->substate5) {
    case 0:
        OrRootStateFlags_726120(gRoot_12340A4, 0x00000800u);
        OrRootIndexedFlags_726170(gRoot_12340A4, 0xFFE02000u, 1);
        OrRootIndexedFlags_726170(gRoot_12340A4, 0x00008000u, 1);
        OrRootStateFlags_726120(gRoot_12340A4, 0x00000001u);
        OrRootStateFlags_726120(gRoot_12340A4, 0x00000200u);
        RootTransition_780A80(gRoot_12340A4, 0, 0.0f);
        RefreshFrontend_7A40D0();
        StartDelay_716DB0(self, 0.5f);
        ++self->substate5;
        return;

    case 1:
        if (QueryAsyncGate_11E8804() != 0)
            return;
        self->substate5 = 10;
        self->delay7 = 0;
        return;

    case 9:
        if (self->state35C != 5) {
            ++self->substate5;
            self->delay7 = 0;
        }
        UpdateState9_71A500(self);
        return;

    case 10:
        if (TransitionReady_71DF70(self))
            ++self->substate5;
        return;

    case 11: {
        ClearRootStateFlags_715A30(gRoot_12340A4, 0x00004000u);
        ClearRootStateFlags_715A30(gRoot_12340A4, 0x08000000u);
        ClearRootStateFlags_715A30(gRoot_12340A4, 0x00000001u);
        ClearRootStateFlags_715A30(gRoot_12340A4, 0x00040000u);
        FinalizeRoute_723870(self, 0);

        const std::uint32_t stage = CurrentStageId_11B2158();
        if (stage != 0x263u && stage != 0x267u) {
            if (QueryRoute_716AD0(self, 0) != 0)
                OrRootStateFlags_726120(gRoot_12340A4, 0x00040000u);
            else
                AlternateFinalize_71A190(self);
        }

        Session120* session = RootSession_1042C(gRoot_12340A4);
        if (session->transition24 != 0)
            session->transition24 = 2;
        DispatchEvent_11AFF64(gEventManager_1234578);
        return;
    }

    default:
        return;
    }
}

} // namespace re5::split120
