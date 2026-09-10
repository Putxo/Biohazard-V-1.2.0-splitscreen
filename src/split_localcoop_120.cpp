#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

bool IsFullSplitEffective(const SplitRenderState120* self)
{
    return self->fullScreenSplitMode != 0 && self->transientSplitFlag == 0;
}

// -----------------------------------------------------------------------------
// 0x00716720..0x00716802 -- VERIFIED, thiscall, RET 0x0C.
// Native args: (bool active, int slot, int device).
// -----------------------------------------------------------------------------
struct Session120;
extern Session120* GetSession_12340A4_1042C();
extern std::uint32_t& SessionActivePlayerMask_47C(Session120*);
extern void SessionSetSlotMode_C42A30(Session120*, int slot, int mode);
extern void SessionSetDevice_C42A50(Session120*, int slot, int device);
extern void RefreshPlayerDevice_7E62F0(int slot);
extern void NotifyLocalPlayerState_7F1610(int eventId, int value);
extern int GetPlayerNotificationValue_11B2158_273D0();

void SetLocalPlayerActive(bool active, int slot, int device)
{
    Session120* session = GetSession_12340A4_1042C();
    const std::uint32_t bit = 1u << slot;

    if (active) {
        SessionActivePlayerMask_47C(session) |= bit;
        SessionSetSlotMode_C42A30(session, slot, 0);
        SessionSetDevice_C42A50(session, slot, device);
        RefreshPlayerDevice_7E62F0(slot);
        NotifyLocalPlayerState_7F1610(4, GetPlayerNotificationValue_11B2158_273D0());
        return;
    }

    SessionActivePlayerMask_47C(session) &= ~bit;
    SessionSetSlotMode_C42A30(session, slot, 1);
    SessionSetDevice_C42A50(session, slot, -1);
    RefreshPlayerDevice_7E62F0(slot);
    NotifyLocalPlayerState_7F1610(3, GetPlayerNotificationValue_11B2158_273D0());
}

// -----------------------------------------------------------------------------
// 0x00720CE0..0x00720E89 -- VERIFIED, thiscall, RET 0x0C.
// Native args are (slot, device, sourceIsKeyboard).
// The function performs the complete transition preparation before state 13.
// -----------------------------------------------------------------------------
extern void PrepareProfileSlot_726D80(int slot);
extern void ConfigureControllerSource_7F1A00(int device, int enabled);
extern void SessionClearField2C();
extern void NotifyRootFlagsChanged_120();
extern void RootOr10438(std::uint32_t mask);
extern void RootOr10440(std::uint32_t mask);
extern void RefreshJoinUi_7E63C0(int value);
extern void RootNotify_726170(std::uint32_t mask, int enabled);
extern void NetworkJoinReset_794EC0(int value);
extern void SessionSetTransition570(int value);

void BeginAddPlayer(AGame120* self, int slot, int device, bool sourceIsKeyboard)
{
    self->pendingDevice = device; // +0x350
    self->pendingSlot = slot;     // +0x354

    PrepareProfileSlot_726D80(slot);
    if (!sourceIsKeyboard)
        ConfigureControllerSource_7F1A00(device, 1);

    SessionClearField2C();

    // Exact native flag order from 0x720D51..0x720E35.  The original invokes
    // its notification callback around every mutation; represented explicitly
    // here rather than hiding the transition in one speculative helper.
    RootOr10438(0x80000000u);
    NotifyRootFlagsChanged_120();
    RootOr10440(0x00400000u);
    NotifyRootFlagsChanged_120();
    RootOr10440(0x01000000u);
    NotifyRootFlagsChanged_120();
    RootOr10440(0x00800000u);
    NotifyRootFlagsChanged_120();

    RefreshJoinUi_7E63C0(1);
    RootNotify_726170(0x8000u, 1);
    NetworkJoinReset_794EC0(0);

    SessionSetTransition570(2);
    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 4) = 0;
    *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(self) + 4) = 13;
}

// -----------------------------------------------------------------------------
// 0x00723460..0x00723658 -- directly disassembled recurring local-join detector.
// Still PARTIAL because helper semantics 0x79ADA0/0x79AE20/0x79C1E0 are being
// named conservatively, but argument/data flow below is instruction-derived.
// IMPORTANT: playerSelector (EDI) and joinSlot (ESI) are distinct native values.
// -----------------------------------------------------------------------------
extern bool GlobalJoinGuardsPass_723460();
extern bool LocalJoinEnvironmentAllowed_716310(AGame120*);
extern bool LocalJoinBusy_716340(AGame120*);
extern int  CountActivePlayers_C42B60();
extern int  ReadCurrentPlayerSelector_11B2158_273C4();
extern int  ResolveJoinSlot_C42CB0();
extern bool DetectControllerJoin_79ADA0(int* device);
extern bool DetectKeyboardJoin_79AE20(int* device);
extern bool DetectFallbackJoin_79C1E0();
extern int  ReadFallbackDevice_1249C40_5D8();
extern void SessionSetDeviceForSlot_C42A50(int slot, int device);
extern void InputSetSelectedPlayer614(int playerSelector);
extern void ResetFallbackInput_7996B0(int value);

void PollLocalJoin_723460(AGame120* self)
{
    if (!GlobalJoinGuardsPass_723460())
        return;
    if (!LocalJoinEnvironmentAllowed_716310(self))
        return;
    if (LocalJoinBusy_716340(self))
        return;
    if (CountActivePlayers_C42B60() >= 2)
        return;

    const int playerSelector = ReadCurrentPlayerSelector_11B2158_273C4();
    const int joinSlot = ResolveJoinSlot_C42CB0();
    int device = -1;

    if (DetectControllerJoin_79ADA0(&device)) {
        InputSetSelectedPlayer614(playerSelector);
        BeginAddPlayer(self, joinSlot, device, false);
        return;
    }

    if (DetectKeyboardJoin_79AE20(&device)) {
        // Native 0x723596..0x7235C7 updates two session device mappings before
        // entering state 13.  The first mapping uses playerSelector and a
        // boolean derived from device<=0; the second uses joinSlot/device.
        SessionSetDeviceForSlot_C42A50(playerSelector, device <= 0 ? 1 : 0);
        SessionSetDeviceForSlot_C42A50(joinSlot, device);
        InputSetSelectedPlayer614(playerSelector);
        BeginAddPlayer(self, joinSlot, device, true);
        return;
    }

    if (DetectFallbackJoin_79C1E0()) {
        device = ReadFallbackDevice_1249C40_5D8();
        const int mapped = device <= 0 ? 1 : 0;
        SessionSetDeviceForSlot_C42A50(playerSelector, device);
        SessionSetDeviceForSlot_C42A50(joinSlot, mapped);
        InputSetSelectedPlayer614(joinSlot);
        ResetFallbackInput_7996B0(0);
        BeginAddPlayer(self, joinSlot, mapped, false);
    }
}

} // namespace re5::split120
