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

struct InputManagerJoinView120 {
    std::uint8_t _0000[0x5D8];
    std::int32_t preferredDevice;
    std::uint8_t inputBlocked;
    std::uint8_t _5DD[0x5F0 - 0x5DD];
    std::uint8_t keyboardMode;
    std::uint8_t _5F1[0x5FC - 0x5F1];
    float joinTimer;
    std::uint8_t _600[0x614 - 0x600];
    std::int32_t selectedJoinPlayer; // +0x614
    std::uint8_t _618[0x61C - 0x618];
    std::uint8_t controllerScanArmed;
};

extern std::uint8_t* gPlayerInputState_1249C40;

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
        self->joinTimer = *reinterpret_cast<const float*>(0x00F52D5C);
    }
    return false;
}

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

bool DetectFallbackJoin_79C1E0(InputManagerJoinView120* self)
{
    if (self->inputBlocked != 0)
        return false;
    if (self->keyboardMode == 0)
        return false;
    return FallbackInputBitGate_79AEA0(self);
}

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

extern bool LocalJoinGateObject_A2CF60(void* object);
bool LocalJoinEnvironmentAllowed_716310(AGame120* self)
{
    const auto* root = *reinterpret_cast<std::uint8_t* const*>(0x012340A4);
    const auto flags = *reinterpret_cast<const std::uint32_t*>(root + 0x10438);
    if ((flags & 0x1000u) == 0)
        return true;

    void* object = *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(self) + 0xE8);
    if (object == nullptr)
        return true;
    return LocalJoinGateObject_A2CF60(object);
}

bool LocalJoinBusy_716340(AGame120* self)
{
    if (*reinterpret_cast<const std::int32_t*>(
            reinterpret_cast<const std::uint8_t*>(self) + 0xD4) != 0)
        return true;

    const auto* root = *reinterpret_cast<std::uint8_t* const*>(0x012340A4);
    const std::uint8_t low = *(root + 0x10438);
    if ((low & 0x80u) != 0)
        return true;
    return (low & 0x40u) != 0;
}

extern int CountActivePlayers_C42B60();
extern void SessionSetPair_C42A50(Session120*, int key, int value);
extern void InputRoute_7996B0(InputManagerJoinView120*, int value);

// Exact native 0x00723460 gate chain and branch wiring.
// Names are intentionally address/field based where semantics are still unknown.
void PollLocalJoin_723460(AGame120* self)
{
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);

    if ((*(root + 0x10434) & 0x03u) != 0)
        return;

    auto* session = *reinterpret_cast<std::uint8_t**>(root + 0x1042C);
    if (*reinterpret_cast<std::int32_t*>(session + 0x58) != 0)
        return;

    auto* global1249C1C = *reinterpret_cast<std::uint8_t**>(0x01249C1C);
    if (*reinterpret_cast<std::int32_t*>(global1249C1C + 0x48) > 1)
        return;

    if (*(session + 0x54) != 0)
        return;

    // 0x11E8804 -> virtual method vtable+0x38 must return zero.
    auto* gateObj = *reinterpret_cast<std::uint8_t**>(0x011E8804);
    auto** gateVtable = *reinterpret_cast<void***>(gateObj);
    using GateFn = int(__thiscall*)(void*);
    if (reinterpret_cast<GateFn>(gateVtable[0x38 / sizeof(void*)])(gateObj) != 0)
        return;

    auto flags10438 = *reinterpret_cast<std::uint32_t*>(root + 0x10438);
    if ((flags10438 & 0x20000000u) != 0)
        return;
    if (!LocalJoinEnvironmentAllowed_716310(self))
        return;

    // Native code keeps this value in EDX across 0x716340 and later requires it
    // to be non-negative in addition to the 0x700 mask being clear.
    flags10438 = *reinterpret_cast<std::uint32_t*>(root + 0x10438);
    if ((flags10438 & 0x700u) != 0)
        return;
    if (LocalJoinBusy_716340(self))
        return;
    if (static_cast<std::int32_t>(flags10438) < 0)
        return;

    if (CountActivePlayers_C42B60() >= 2)
        return;

    auto* playerArrayRoot = *reinterpret_cast<std::uint8_t**>(0x011B2158);
    const int playerArrayIndex = *reinterpret_cast<std::int32_t*>(playerArrayRoot + 0x20);
    const int selectedPlayer = *reinterpret_cast<std::int32_t*>(
        playerArrayRoot + playerArrayIndex * 0x5940 + 0x273C4);

    const int slot = FindJoinableSessionSlot_C42CB0(session);
    int detectedDevice = -1;
    auto* input = reinterpret_cast<InputManagerJoinView120*>(
        *reinterpret_cast<void**>(0x01249C40));

    if (DetectControllerJoin_79ADA0(input, &detectedDevice)) {
        input->selectedJoinPlayer = selectedPlayer;
        BeginAddPlayer(self, slot, detectedDevice, false);
        return;
    }

    if (DetectKeyboardJoin_79AE20(input, &detectedDevice)) {
        const int nonPositive = detectedDevice <= 0 ? 1 : 0;
        SessionSetPair_C42A50(reinterpret_cast<Session120*>(session), selectedPlayer, nonPositive);
        SessionSetPair_C42A50(reinterpret_cast<Session120*>(session), slot, detectedDevice);
        input->selectedJoinPlayer = selectedPlayer;
        BeginAddPlayer(self, slot, detectedDevice, true);
        return;
    }

    if (DetectFallbackJoin_79C1E0(input)) {
        const int preferredDevice = input->preferredDevice;
        const int nonPositive = preferredDevice <= 0 ? 1 : 0;
        SessionSetPair_C42A50(reinterpret_cast<Session120*>(session), selectedPlayer, preferredDevice);
        SessionSetPair_C42A50(reinterpret_cast<Session120*>(session), slot, nonPositive);
        input->selectedJoinPlayer = slot;
        InputRoute_7996B0(input, 0);
        BeginAddPlayer(self, slot, nonPositive, false);
    }
}

} // namespace re5::split120
