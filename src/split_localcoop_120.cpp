#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// 0x00763170 — exact recovered predicate.
bool IsFullSplitEffective(const SplitRenderState120* self)
{
    return self->fullScreenSplitMode != 0 && self->transientSplitFlag == 0;
}

// -----------------------------------------------------------------------------
// 0x00716720 — PARTIAL semantic reconstruction.
//
// Recovered behavior:
// active=true:
//   session+0x47C |= (1 << slot)
//   native slot-mode setter(slot, 0)
//   native device setter(slot, device)
//   refresh player/input ownership
//   emit player-added notification
//
// active=false:
//   session+0x47C &= ~(1 << slot)
//   native slot-mode setter(slot, 1)
//   native device setter(slot, -1)
//   refresh player/input ownership
//   emit player-removed notification
// -----------------------------------------------------------------------------

struct Session120;
extern Session120* GetSession_12340A4_1042C();
extern std::uint32_t& SessionActivePlayerMask_47C(Session120*);
extern void SessionSetSlotMode_C42A30(Session120*, int slot, int mode);
extern void SessionSetDevice_C42A50(Session120*, int slot, int device);
extern void RefreshPlayerDevice_7E62F0(int slot);
extern void NotifyLocalPlayerState_7F1610(int eventId, int value);
extern int GetPlayerNotificationValue();

void SetLocalPlayerActive(bool active, int slot, int device)
{
    Session120* session = GetSession_12340A4_1042C();
    std::uint32_t& activeMask = SessionActivePlayerMask_47C(session);
    const std::uint32_t bit = 1u << slot;

    if (active) {
        activeMask |= bit;
        SessionSetSlotMode_C42A30(session, slot, 0);
        SessionSetDevice_C42A50(session, slot, device);
        RefreshPlayerDevice_7E62F0(slot);
        NotifyLocalPlayerState_7F1610(4, GetPlayerNotificationValue());
    } else {
        activeMask &= ~bit;
        SessionSetSlotMode_C42A30(session, slot, 1);
        SessionSetDevice_C42A50(session, slot, -1);
        RefreshPlayerDevice_7E62F0(slot);
        NotifyLocalPlayerState_7F1610(3, GetPlayerNotificationValue());
    }
}

// -----------------------------------------------------------------------------
// 0x00720CE0 — PARTIAL semantic reconstruction.
// Exact durable writes recovered so far:
//   self+0x350 = device
//   self+0x354 = slot
// End-state transition includes session+0x570 = 2 and aGame state = 13.
// Intermediate transition helpers remain unresolved and must not be guessed.
// -----------------------------------------------------------------------------

extern void NativeBeginAddPlayerTransition_Pre(AGame120*, int slot, int device,
                                                bool sourceIsKeyboard);
extern void SetSessionTransition570(int value);
extern void SetAGameMainState(AGame120*, std::uint8_t state);

void BeginAddPlayer(AGame120* self, int slot, int device, bool sourceIsKeyboard)
{
    self->pendingDevice = device;
    self->pendingSlot = slot;

    NativeBeginAddPlayerTransition_Pre(self, slot, device, sourceIsKeyboard);

    SetSessionTransition570(2);
    SetAGameMainState(self, 13); // native "Add Player" state
}

// -----------------------------------------------------------------------------
// 0x00723460 — PARTIAL high-level reconstruction of the recurring local-join
// detector. The original 1.2.0 safety/mode/player-count gates are preserved in
// this representation; this is not a forced J2 spawn path.
// -----------------------------------------------------------------------------

extern bool GlobalJoinGuardsPass();
extern bool SessionModeAllowsLocalJoin();
extern bool SessionLocalJoinStateClear();
extern bool LocalJoinEnvironmentAllowed_716310(AGame120*);
extern bool LocalJoinBusy_716340(AGame120*);
extern int  CountActivePlayers_C42B60();
extern int  CurrentPreferredJoinSlot();
extern bool DetectControllerJoin_79ADA0(int* device);
extern bool DetectKeyboardJoin_79AE20(int* device);
extern bool DetectFallbackJoin_79C1E0();
extern int  FallbackKeyboardDevice();
extern void InputSetSelectedJoinSlot614(int slot);
extern void PrepareKeyboardDeviceMapping(int slot, int device);

void PollLocalJoin_723460(AGame120* self)
{
    if (!GlobalJoinGuardsPass())
        return;
    if (!SessionModeAllowsLocalJoin())
        return;
    if (!SessionLocalJoinStateClear())
        return;
    if (!LocalJoinEnvironmentAllowed_716310(self))
        return;
    if (LocalJoinBusy_716340(self))
        return;
    if (CountActivePlayers_C42B60() >= 2)
        return;

    const int slot = CurrentPreferredJoinSlot();
    int device = -1;

    if (DetectControllerJoin_79ADA0(&device)) {
        InputSetSelectedJoinSlot614(slot);
        BeginAddPlayer(self, slot, device, false);
        return;
    }

    if (DetectKeyboardJoin_79AE20(&device)) {
        PrepareKeyboardDeviceMapping(slot, device);
        InputSetSelectedJoinSlot614(slot);
        BeginAddPlayer(self, slot, device, true);
        return;
    }

    if (DetectFallbackJoin_79C1E0()) {
        device = FallbackKeyboardDevice();
        PrepareKeyboardDeviceMapping(slot, device);
        InputSetSelectedJoinSlot614(slot);
        BeginAddPlayer(self, slot, device, false);
    }
}

} // namespace re5::split120
