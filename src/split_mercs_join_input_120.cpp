#include <cstdint>

namespace re5::split120 {

struct Session120;

struct MercsJoinUi120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;                 // +0x14
    std::uint8_t _0015[0x697];
    std::int32_t uiState6AC;            // +0x6AC
    std::int32_t selectedJoinDevice;    // +0x6B0
};

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern bool IsMercsLocalSplit_C42EC0(Session120*);
extern bool DetectControllerJoin_79ADA0(std::uint8_t*, int*);
extern bool DetectKeyboardJoin_79AE20(std::uint8_t*, int*);
extern bool DetectFallbackJoin_79C1E0(std::uint8_t*);
extern void SessionSetDevice_C42A50(Session120*, int slot, int device);
extern void RefreshInputMode_7996B0(std::uint8_t*);

static inline Session120* Session()
{
    return *reinterpret_cast<Session120**>(gRoot_12340A4 + 0x1042C);
}

static inline void SetBinding(Session120* session, std::uint32_t offset, int value)
{
    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(session) + offset) = value;
}

// 0x00A43D80..0x00A43F8A -- VERIFIED split-specific input assignment.
// Native enters this body only when C42EC0 is true (Mercenaries/Reunion and
// session+0x5CC==2). It then resolves who owns player 1/player 2 from the
// preferred device and the device that produced the local-join input.
bool UpdateMercsLocalJoinInput_A43D80(MercsJoinUi120* self)
{
    Session120* session = Session();
    if (!IsMercsLocalSplit_C42EC0(session))
        return false;

    const int preferredDevice = *reinterpret_cast<int*>(gInput_1249C40 + 0x5D8);
    int detectedDevice = -1;

    // 0xA43DF4..0xA43E5A: a second controller/device joined.
    if (DetectControllerJoin_79ADA0(gInput_1249C40, &detectedDevice)) {
        SetBinding(session, 0x654, preferredDevice);
        self->selectedJoinDevice = detectedDevice;
        SetBinding(session, 0x658, detectedDevice);
        *reinterpret_cast<int*>(gInput_1249C40 + 0x614) = 0;
        self->state = 0x32;
        return true;
    }

    // 0xA43E5B..0xA43EDC: preferred-device/keyboard-side join. For the native
    // device ids used here (0/1), the other local player is the complementary
    // id. Native writes both bindings and immediately writes them to slot 0/1.
    if (DetectKeyboardJoin_79AE20(gInput_1249C40, &detectedDevice)) {
        const int otherDevice = (detectedDevice <= 0) ? 1 : 0;
        SetBinding(session, 0x654, otherDevice);
        self->selectedJoinDevice = detectedDevice;
        SetBinding(session, 0x658, detectedDevice);
        *reinterpret_cast<int*>(gInput_1249C40 + 0x614) = 0;
        SessionSetDevice_C42A50(session, 0, otherDevice);
        SessionSetDevice_C42A50(session, 1, detectedDevice);
        self->state = 0x32;
        return true;
    }

    // 0xA43EDD..0xA43F8A: fallback local-input path. Native pairs the current
    // preferred device with the complementary 0/1 device, marks input+0x614=1,
    // refreshes input mode and commits both slot-device assignments.
    if (DetectFallbackJoin_79C1E0(gInput_1249C40)) {
        const int otherDevice = (preferredDevice <= 0) ? 1 : 0;
        self->selectedJoinDevice = otherDevice;
        SetBinding(session, 0x654, preferredDevice);
        SetBinding(session, 0x658, otherDevice);
        *reinterpret_cast<int*>(gInput_1249C40 + 0x614) = 1;
        RefreshInputMode_7996B0(gInput_1249C40);
        SessionSetDevice_C42A50(session, 0, preferredDevice);
        SessionSetDevice_C42A50(session, 1, otherDevice);
        self->state = 0x32;
        return true;
    }

    return false;
}

} // namespace re5::split120
