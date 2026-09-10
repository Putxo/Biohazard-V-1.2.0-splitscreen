#include <cstdint>

namespace re5::split120 {

struct AGameState10_120;
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gProfileState_11B2158;
extern std::uint8_t* gInputManager_1249C40;
extern void* gEventInput_11B1DD8;

extern int  QueryProfileSlotValue_CEE100(void* profileSlot, int slot);
extern void SessionSetLocalPair_C42A70(void* session, int slot, int value);
extern void SessionSetPartnerPair_C42A90(void* session, int slot, int value);
extern int  InputDeviceRoute_799820(void* inputManager, int device);
extern int  QueryInputEvent_7F1390(void* eventInput, int device, int arg0);

static inline std::uint8_t* Session() {
    return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4 + 0x1042C);
}

// 0x00715AC0..0x00715AD5 -- VERIFIED.
bool SessionPhaseIs2Or5_715AC0(const void* sessionRaw) {
    const auto* s = static_cast<const std::uint8_t*>(sessionRaw);
    const int phase = *reinterpret_cast<const std::int32_t*>(s + 0x58);
    return phase == 2 || phase == 5;
}

// 0x00715AE0..0x00715AF5 -- VERIFIED.
bool SessionPhaseIs3Or4_715AE0(const void* sessionRaw) {
    const auto* s = static_cast<const std::uint8_t*>(sessionRaw);
    const int phase = *reinterpret_cast<const std::int32_t*>(s + 0x58);
    return phase == 3 || phase == 4;
}

// 0x00715B00..0x00715B19 -- VERIFIED.
bool SessionPhaseIs0Or3Or4_715B00(const void* sessionRaw) {
    const auto* s = static_cast<const std::uint8_t*>(sessionRaw);
    const int phase = *reinterpret_cast<const std::int32_t*>(s + 0x58);
    return phase == 0 || phase == 3 || phase == 4;
}

// 0x00715B20..0x00715B3A -- VERIFIED.
bool SessionPhaseIs1Or2Or5_715B20(const void* sessionRaw) {
    const auto* s = static_cast<const std::uint8_t*>(sessionRaw);
    const int phase = *reinterpret_cast<const std::int32_t*>(s + 0x58);
    return phase == 1 || phase == 2 || phase == 5;
}

// 0x00715BB0..0x00715BC4 -- VERIFIED.
// Predicate consumed by 0x716810 before switching aGame main state to 10.
bool LocalPlayerTransitionReady_715BB0(void* selfRaw) {
    const auto* self = static_cast<const std::uint8_t*>(selfRaw);
    return self[0x20] != 0 && self[0x04] == 2;
}

static inline std::uint32_t CurrentStageCode() {
    const int profile = *reinterpret_cast<const std::int32_t*>(gProfileState_11B2158 + 0x20);
    return *reinterpret_cast<const std::uint32_t*>(
        gProfileState_11B2158 + profile * 0x5940 + 0x273D8);
}

// 0x00715CE0..0x00715D0D -- VERIFIED.
bool StageUsesLocalTransitionClassA_715CE0() {
    const std::uint32_t v = CurrentStageCode();
    return (v >= 0x258u && v < 0x2BCu) || v == 0x323u;
}

// 0x00715D10..0x00715D35 -- VERIFIED.
bool StageUsesLocalTransitionClassB_715D10() {
    const std::uint32_t v = CurrentStageCode();
    return v >= 0x320u && v < 0x384u;
}

// 0x00716370..0x00716400 -- VERIFIED.
void UpdateLocalJoinBusyReasonA_716370(void* selfRaw) {
    auto* self = static_cast<std::uint8_t*>(selfRaw);
    auto* session = Session();
    session[0x29] = 0;
    session[0x2A] = 0;
    if ((gRoot_12340A4[0x10434] & 3u) != 0) return;
    const int phase = *reinterpret_cast<const std::int32_t*>(session + 0x58);
    if (phase == 1 || phase == 2 || phase == 5) return;
    if (StageUsesLocalTransitionClassA_715CE0() || StageUsesLocalTransitionClassB_715D10()) return;
    *reinterpret_cast<std::int32_t*>(self + 0xD4) = 1;
    if (SessionPhaseIs3Or4_715AE0(session))
        *reinterpret_cast<std::int32_t*>(self + 0xD4) = 2;
}

// 0x00716410..0x00716471 -- VERIFIED.
void UpdateLocalJoinBusyReasonB_716410(void* selfRaw) {
    auto* self = static_cast<std::uint8_t*>(selfRaw);
    auto* session = Session();
    session[0x2A] = 0;
    if ((gRoot_12340A4[0x10434] & 3u) != 0) return;
    const int phase = *reinterpret_cast<const std::int32_t*>(session + 0x58);
    if (phase == 1 || phase == 2 || phase == 5) return;
    if (StageUsesLocalTransitionClassA_715CE0() || StageUsesLocalTransitionClassB_715D10()) return;
    *reinterpret_cast<std::int32_t*>(self + 0xD4) = 2;
}

// 0x00716AD0..0x00716BD7 -- VERIFIED.
// State-10 local pair resolver. Returns 0 blocked, 1 already phase 2, -1 after
// consuming/advancing the pending two-slot route (or on non-mutating request).
int QueryRoute_716AD0(AGameState10_120*, int nonMutating) {
    const std::uint32_t rootFlags = *reinterpret_cast<const std::uint32_t*>(gRoot_12340A4 + 0x10434);
    if ((rootFlags & 3u) != 0) return 0;
    auto* session = Session();
    if (*reinterpret_cast<const std::int32_t*>(session + 0x58) != 0) return 0;
    if (*reinterpret_cast<const std::int32_t*>(session + 0x574) !=
        *reinterpret_cast<const std::int32_t*>(session + 0x578)) return 0;
    const int phase = *reinterpret_cast<const std::int32_t*>(session + 0x2C);
    if (phase == 2) return 1;
    if (phase == 5) return 0;
    const std::uint32_t pendingMask = *reinterpret_cast<std::uint32_t*>(session + 0x57C);
    if (static_cast<std::int32_t>(pendingMask) <= 0) return 0;
    if (nonMutating != 0) return -1;
    int slot = -1;
    for (int i = 0; i < 2; ++i) if ((pendingMask & (1u << i)) != 0) { slot = i; break; }
    if (slot >= 0) {
        const int profile = *reinterpret_cast<const std::int32_t*>(gProfileState_11B2158 + 0x20);
        void* profileSlot = gProfileState_11B2158 + profile * 0x8C74 + 0x38;
        const int value = QueryProfileSlotValue_CEE100(profileSlot, slot);
        SessionSetLocalPair_C42A70(session, slot, value);
        const int partner = slot ^ 1;
        const int partnerValue = *reinterpret_cast<const std::int32_t*>(session + 0x4B0 + partner * 4);
        SessionSetPartnerPair_C42A90(session, slot, partnerValue);
    }
    *reinterpret_cast<std::uint32_t*>(session + 0x57C) = 0;
    *reinterpret_cast<std::int32_t*>(session + 0x2C) = 1;
    return -1;
}

// 0x00716E60..0x00716E9D -- VERIFIED.
int RouteLogicalPlayerToInputDevice_716E60(int player) {
    const std::uint32_t flags = *reinterpret_cast<const std::uint32_t*>(gRoot_12340A4 + 0x10438);
    if ((flags & 0x1000u) != 0 || static_cast<unsigned>(player) > 3u) return 0;
    auto* session = Session();
    const int device = *reinterpret_cast<const std::int32_t*>(session + 0x490 + player * 4);
    if (device < 0) return 0;
    return InputDeviceRoute_799820(gInputManager_1249C40, device);
}

// 0x00716EA0..0x00716EF0 -- VERIFIED.
bool LocalPlayerMappedDeviceEventReady_716EA0(int player) {
    const int preferred = *reinterpret_cast<const std::int32_t*>(gInputManager_1249C40 + 0x5D8);
    if (QueryInputEvent_7F1390(gEventInput_11B1DD8, preferred, 0) == 0) return false;
    auto* session = Session();
    const int device = *reinterpret_cast<const std::int32_t*>(session + 0x490 + player * 4);
    if (device < 0) return false;
    return QueryInputEvent_7F1390(gEventInput_11B1DD8, device, 0) != 0;
}

} // namespace re5::split120
