#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// 0x00763170 — exact recovered predicate.
bool IsFullSplitEffective(const SplitRenderState120* self)
{
    return self->fullScreenSplitMode != 0 && self->transientSplitFlag == 0;
}

struct Session120;
struct InputManager120;

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
    SetAGameMainState(self, 13);
}

// 0x00C42CB0 — VERIFIED.
// Returns the first slot 0..3 for which bit(slot) is set in session+0x478 and
// slotMode[slot] at +0x480 is exactly 1. Returns -1 when none matches.
int FindJoinableSessionSlot_C42CB0(const std::uint8_t* session)
{
    const auto mask = *reinterpret_cast<const std::uint32_t*>(session + 0x478);
    const auto* slotMode = reinterpret_cast<const std::int32_t*>(session + 0x480);
    for (int slot = 0; slot < 4; ++slot) {
        if ((mask & (1u << slot)) != 0 && slotMode[slot] == 1)
            return slot;
    }
    return -1;
}

extern InputManager120* gInputManager_1249C40;
extern SplitRenderState120* gSplitRenderState_123457C;
extern bool InputJoinTrigger_76A410(SplitRenderState120* split);

// The fields below are only the portions touched by the verified join helpers.
struct InputManagerJoinView120 {
    std::uint8_t _0000[0x5D8];
    std::int32_t preferredDevice;       // +0x5D8
    std::uint8_t inputBlocked;          // +0x5DC
    std::uint8_t _5DD[0x5F0 - 0x5DD];
    std::uint8_t keyboardMode;          // +0x5F0
    std::uint8_t _5F1[0x5FC - 0x5F1];
    float joinTimer;                    // +0x5FC
    std::uint8_t _600[0x61C - 0x600];
    std::uint8_t controllerScanArmed;   // +0x61C
};

extern std::uint8_t* gPlayerInputState_1249C40;

// 0x0079ADA0 — VERIFIED.
// Scans only controller indices 0 and 1 (stride 0x2C0, stop at 0x580).
// A candidate must differ from +0x5D8, have input-state flag 0x08 at +0x198,
// and pass 0x76A410. On success it writes the device index to outDevice.
bool DetectControllerJoin_79ADA0(InputManagerJoinView120* self, int* outDevice)
{
    if (self->inputBlocked != 0)
        return false;

    self->controllerScanArmed = 1;
    for (int device = 0, off = 0; off < 0x580; ++device, off += 0x2C0) {
        if (self->preferredDevice == device)
            continue;

        auto* state = reinterpret_cast<const std::uint8_t*>(gPlayerInputState_1249C40) + off;
        if ((state[0x198] & 0x08) == 0)
            continue;

        if (InputJoinTrigger_76A410(gSplitRenderState_123457C)) {
            *outDevice = device;
            return true;
        }

        // Exact native side effect on a failed trigger.
        self->joinTimer = *reinterpret_cast<const float*>(0x00F52D5C);
    }
    return false;
}

// 0x0079AE20 — VERIFIED.
// Same two-device scan, but only while keyboardMode==0 and candidate device is
// exactly +0x5D8. This is the complementary branch to 0x79ADA0.
bool DetectKeyboardJoin_79AE20(InputManagerJoinView120* self, int* outDevice)
{
    if (self->inputBlocked != 0 || self->keyboardMode != 0)
        return false;

    for (int device = 0, off = 0; off < 0x580; ++device, off += 0x2C0) {
        if (self->preferredDevice != device)
            continue;

        auto* state = reinterpret_cast<const std::uint8_t*>(gPlayerInputState_1249C40) + off;
        if ((state[0x198] & 0x08) == 0)
            continue;

        if (InputJoinTrigger_76A410(gSplitRenderState_123457C)) {
            *outDevice = device;
            return true;
        }

        self->joinTimer = *reinterpret_cast<const float*>(0x00F52D5C);
    }
    return false;
}

extern bool FallbackInputBitGate_79AEA0(InputManagerJoinView120* self);

// 0x0079C1E0 — VERIFIED.
// The fallback path is only available when input is not blocked and
// keyboardMode is non-zero; the final decision is delegated to 0x79AEA0.
bool DetectFallbackJoin_79C1E0(InputManagerJoinView120* self)
{
    if (self->inputBlocked != 0)
        return false;
    if (self->keyboardMode == 0)
        return false;
    return FallbackInputBitGate_79AEA0(self);
}

// 0x0079AEA0 — VERIFIED.
// Tests bit 0x08000000 in the per-input record selected by +0x5E8. If set,
// 0x76A410 must also succeed. Failed trigger refreshes +0x5FC exactly as the
// controller/keyboard scans do.
bool FallbackInputBitGate_79AEA0(InputManagerJoinView120* self)
{
    const int index = *reinterpret_cast<const std::int32_t*>(
        reinterpret_cast<const std::uint8_t*>(self) + 0x5E8);
    const auto* base = *reinterpret_cast<std::uint8_t* const*>(0x01249C74);
    const auto flags = *reinterpret_cast<const std::uint32_t*>(base + index * 0x8C0 + 0x120);
    if ((flags & 0x08000000u) == 0)
        return false;

    if (InputJoinTrigger_76A410(gSplitRenderState_123457C))
        return true;

    self->joinTimer = *reinterpret_cast<const float*>(0x00F52D5C);
    return false;
}

extern bool GlobalJoinGuardsPass();
extern bool SessionModeAllowsLocalJoin();
extern bool SessionLocalJoinStateClear();
extern bool LocalJoinEnvironmentAllowed_716310(AGame120*);
extern bool LocalJoinBusy_716340(AGame120*);
extern int  CountActivePlayers_C42B60();
extern int  CurrentPreferredJoinSlot();
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
    auto* input = reinterpret_cast<InputManagerJoinView120*>(gInputManager_1249C40);

    if (DetectControllerJoin_79ADA0(input, &device)) {
        InputSetSelectedJoinSlot614(slot);
        BeginAddPlayer(self, slot, device, false);
        return;
    }

    if (DetectKeyboardJoin_79AE20(input, &device)) {
        PrepareKeyboardDeviceMapping(slot, device);
        InputSetSelectedJoinSlot614(slot);
        BeginAddPlayer(self, slot, device, true);
        return;
    }

    if (DetectFallbackJoin_79C1E0(input)) {
        device = FallbackKeyboardDevice();
        PrepareKeyboardDeviceMapping(slot, device);
        InputSetSelectedJoinSlot614(slot);
        BeginAddPlayer(self, slot, device, false);
    }
}

} // namespace re5::split120
