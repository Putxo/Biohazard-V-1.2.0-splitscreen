#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

extern void InputPrepare_7965E0();
extern void UiPlayerJoinBegin_7BA180(void* ui, int zero, int one);
extern void UiPlayerJoinReset_7BA1F0(void* ui, int zero);
extern void AudioOrUiJoinPrepare_7A40D0(void* obj);
extern void BeginState13Delay_716DB0(AGame120* self, float seconds);
extern bool IsState13ExternalGateClear_11E8804();
extern bool State13Ready_71DF70();
extern void RootSetFlags_715A30(std::uint32_t mask);
extern void RootSetFlags_726120(std::uint32_t mask);
extern void ResetJoinTransition_723870(AGame120* self, int value);
extern void BindPlayerToUi_7B8370(void* ui, int slot, int device);
extern void SetJoinUiSelection_7F12C0(void* obj, int value);
extern void RefreshJoinUi_7B7FC0(void* ui);
extern void DispatchJoinEvent_1234578(const void* eventObject);

struct AGameState13View120 {
    std::uint8_t _00[0x06];
    std::uint8_t substate;   // +0x06
    std::uint8_t subflag;    // +0x07
    std::uint8_t _08[0x350 - 0x08];
    std::int32_t device;     // +0x350
    std::int32_t slot;       // +0x354
};

// 0x00725B20..0x00725C7F -- VERIFIED state-13 (Add Player) handler.
// Four substates are selected by byte +0x06 through jump table 0x725C80.
void UpdateAddPlayerState13_725B20(AGame120* opaque)
{
    auto* self = reinterpret_cast<AGameState13View120*>(opaque);

    switch (self->substate) {
    case 0: {
        auto* global = *reinterpret_cast<std::uint8_t**>(0x01249C1C);
        if (global[0x131C] != 0)
            return;

        InputPrepare_7965E0();
        auto* ui = *reinterpret_cast<void**>(0x011B2158);
        UiPlayerJoinBegin_7BA180(ui, 0, 1);
        UiPlayerJoinReset_7BA1F0(ui, 0);
        AudioOrUiJoinPrepare_7A40D0(*reinterpret_cast<void**>(0x011B1D0C));
        BeginState13Delay_716DB0(opaque, 0.5f);
        ++self->substate;
        return;
    }

    case 1:
        if (!IsState13ExternalGateClear_11E8804())
            return;
        ++self->substate;
        self->subflag = 0;
        return;

    case 2:
        if (!State13Ready_71DF70())
            return;
        ++self->substate;
        return;

    case 3: {
        RootSetFlags_715A30(0x00004000u);
        RootSetFlags_715A30(0x08000000u);
        RootSetFlags_715A30(0x00000001u);
        RootSetFlags_715A30(0x00000010u);
        RootSetFlags_726120(0x00040000u);

        ResetJoinTransition_723870(opaque, 0);
        SetLocalPlayerActive(true, self->slot, self->device);

        auto* ui = *reinterpret_cast<void**>(0x011B2158);
        BindPlayerToUi_7B8370(ui, self->slot, self->device);
        SetJoinUiSelection_7F12C0(*reinterpret_cast<void**>(0x011B1DD8), -1);
        RefreshJoinUi_7B7FC0(ui);

        auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
        auto* session = *reinterpret_cast<std::uint8_t**>(root + 0x1042C);
        *reinterpret_cast<std::int32_t*>(session + 0x24) = 2;

        DispatchJoinEvent_1234578(reinterpret_cast<const void*>(0x011AFF64));
        return;
    }

    default:
        return;
    }
}

} // namespace re5::split120
