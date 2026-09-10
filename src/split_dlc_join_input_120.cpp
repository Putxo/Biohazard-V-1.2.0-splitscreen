#include <cstdint>

namespace re5::split120 {

struct Session120;
struct DlcLocalJoinUi120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;                 // +0x14
    std::uint8_t stateAux;              // +0x15
    std::uint8_t _0016[0x188 - 0x16];
    std::int32_t localJoinAccepted;      // +0x188
    std::int32_t selectedJoinDevice;     // +0x18C
};

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern bool DetectControllerJoin_79ADA0(std::uint8_t*, int*);
extern bool DetectKeyboardJoin_79AE20(std::uint8_t*, int*);
extern bool DetectFallbackJoin_79C1E0(std::uint8_t*);
extern void RefreshInputMode_7996B0(std::uint8_t*, int value);

static inline Session120* Session()
{
    return *reinterpret_cast<Session120**>(gRoot_12340A4 + 0x1042C);
}

static inline int CurrentSelectedPlayer()
{
    const int index = *reinterpret_cast<const std::int32_t*>(gPlayerRoot_11B2158 + 0x20);
    return *reinterpret_cast<const std::int32_t*>(
        gPlayerRoot_11B2158 + index * 0x5940 + 0x273C4);
}

static inline void SetSessionBinding(Session120* session, std::uint32_t offset, int value)
{
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(session) + offset) = value;
}

// 0x00A545B6..0x00A54743 -- VERIFIED split/local-input branch of the
// LIN/DE frontend update 0x00A54560. This is the native producer of
// session+0x654/+0x658 consumed by the C43320/BF59DE local two-slot backend.
bool UpdateDlcLocalJoinInput_A545B6(DlcLocalJoinUi120* self)
{
    Session120* session = Session();
    int detectedDevice = -1;

    if (DetectControllerJoin_79ADA0(gInput_1249C40, &detectedDevice)) {
        const int preferredDevice = *reinterpret_cast<const std::int32_t*>(gInput_1249C40 + 0x5D8);
        SetSessionBinding(session, 0x654, preferredDevice);
        self->selectedJoinDevice = detectedDevice;
        SetSessionBinding(session, 0x658, detectedDevice);
        *reinterpret_cast<std::int32_t*>(gInput_1249C40 + 0x614) = CurrentSelectedPlayer();
    }
    else if (DetectKeyboardJoin_79AE20(gInput_1249C40, &detectedDevice)) {
        const int complementaryDevice = (detectedDevice <= 0) ? 1 : 0;
        SetSessionBinding(session, 0x654, complementaryDevice);
        self->selectedJoinDevice = detectedDevice;
        SetSessionBinding(session, 0x658, detectedDevice);
        *reinterpret_cast<std::int32_t*>(gInput_1249C40 + 0x614) = CurrentSelectedPlayer();
    }
    else if (DetectFallbackJoin_79C1E0(gInput_1249C40)) {
        const int preferredDevice = *reinterpret_cast<const std::int32_t*>(gInput_1249C40 + 0x5D8);
        const int complementaryDevice = (preferredDevice <= 0) ? 1 : 0;
        self->selectedJoinDevice = complementaryDevice;
        SetSessionBinding(session, 0x654, preferredDevice);
        SetSessionBinding(session, 0x658, complementaryDevice);

        const int selectedPlayer = CurrentSelectedPlayer();
        *reinterpret_cast<std::int32_t*>(gInput_1249C40 + 0x614) =
            (selectedPlayer >= 1) ? 0 : 1;
        RefreshInputMode_7996B0(gInput_1249C40, 0);
    }
    else {
        return false;
    }

    self->localJoinAccepted = 1;
    self->state = 0x0A;
    self->stateAux = 0;
    return true;
}

} // namespace re5::split120
