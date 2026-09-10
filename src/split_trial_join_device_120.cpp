#include <cstdint>

namespace re5::split120 {

struct TrialSplitJoinView120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;                 // +0x14
    std::uint8_t _0015[0x44 - 0x15];
    float timer44;                      // +0x44
    std::uint8_t _0048[0x188 - 0x48];
    std::int32_t localJoinAccepted;      // +0x188
};

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gTrialJoinOwner_11B1DD8;
extern void CancelTrialState_9E4A50(void* self);

// Split-specific state 2 of 0x00A5B0A0, directly recovered from
// 0x00A5B0EF..0x00A5B17A. inputMask is EAX returned by 0x9E3420.
// It waits for a non-preferred physical device, stores it as the native
// secondary binding session+0x658 and advances the trial_split state machine.
bool UpdateTrialSplitSecondDevice_A5B0EF(TrialSplitJoinView120* self,
                                          std::uint32_t inputMask)
{
    if ((*reinterpret_cast<const std::uint32_t*>(gInput_1249C40 + 0x5B8) & inputMask) != 0) {
        CancelTrialState_9E4A50(self);
        reinterpret_cast<std::uint8_t*>(self)[0x3E] = 1;
        self->state = 0x5A;
        return false;
    }

    const int preferredDevice = *reinterpret_cast<const std::int32_t*>(gInput_1249C40 + 0x5D8);
    int device = 0;
    const std::uint8_t* deviceState = gInput_1249C40 + 0x198;
    for (; device < 2; ++device, deviceState += 0x2C0) {
        if (device == preferredDevice)
            continue;
        if ((*deviceState & 0x08u) != 0)
            break;
    }

    if (device >= 2)
        return false;

    auto* session = *reinterpret_cast<std::uint8_t**>(gRoot_12340A4 + 0x1042C);
    *reinterpret_cast<std::int32_t*>(session + 0x658) = device;
    *reinterpret_cast<std::int32_t*>(gTrialJoinOwner_11B1DD8 + 0x6C) = device;
    ++self->state;
    self->localJoinAccepted = 1;
    self->timer44 = *reinterpret_cast<const float*>(0x00F554EC);
    return true;
}

} // namespace re5::split120
