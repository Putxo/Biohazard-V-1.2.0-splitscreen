#include <cstdint>

namespace re5::split120 {

struct MercsFrontend120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;              // +0x14
    std::uint8_t _0015[0x188-0x15];
    std::int32_t currentPlayer;      // +0x188
    std::uint8_t _018c[0x5A0-0x18C];
    std::int32_t pendingUiState;     // +0x5A0
    std::uint8_t _05a4[0x6AC-0x5A4];
    std::int32_t localJoinComplete;  // +0x6AC
};

static inline std::uint8_t* Input120()
{
    return *reinterpret_cast<std::uint8_t**>(0x01249C40);
}

// 0x00A4A5B3 is the exact dispatcher target for main frontend state 3.
// It is NOT a one-shot gameplay-load state. It repeatedly services frontend
// interaction and, once +0x6AC is set, temporarily routes the global input
// context to player 1 around A43D80 before restoring player 0.
void RouteJoinedSecondPlayerInState3_A4A5E0(MercsFrontend120& self)
{
    auto* input = Input120();
    if (self.localJoinComplete != 0)
        *reinterpret_cast<std::int32_t*>(input + 0x618) = 1;

    // Native call at 0xA4A5F5: A43D80(self).

    if (self.localJoinComplete != 0)
        *reinterpret_cast<std::int32_t*>(input + 0x618) = 0;
}

// Exact dispatch-table recovery at 0xA4AF20 / 0xA4AEA8.
// These are the split-relevant state destinations confirmed byte-for-byte.
std::uint32_t MercsFrontendStateTarget(std::uint8_t state)
{
    switch (state) {
    case 0x03: return 0x00A4A5B3;
    case 0x0A: return 0x00A4A68E;
    case 0x0B: return 0x00A4A7C1;
    case 0x14: return 0x00A4A905;
    case 0x15: return 0x00A4A9DD;
    case 0x1E: return 0x00A4AAA2;
    case 0x32: return 0x00A4AD13;
    case 0x3C: return 0x00A4AD1F;
    case 0x46: return 0x00A4ADAA;
    case 0x50: return 0x00A4ADF0;
    case 0x5A: return 0x00A4AE41;
    case 0x5B: return 0x00A4AE4E;
    case 0x5C: return 0x00A4AE73;
    default:   return 0;
    }
}

// 0xA44820 is a per-player status renderer. The split-specific branch is
// exact: when C42EC0(session) is true and the player's status value is 1,
// the renderer remaps it to visual/status variant 6. This is presentation of
// the local-split J2 state, not the function that activates the player.
std::int32_t RemapMercsSplitPlayerVisual_A44883(bool isLocalSplit,
                                               std::int32_t status)
{
    if (isLocalSplit && status == 1)
        return 6;
    return status;
}

// 0xA43950 is one of the state-3 input handlers. For currentPlayer==0 a valid
// confirm immediately prepares the common exit sequence: pendingUiState=2,
// exit variant +0x6A4=1, and main state 0x5A. State 0x5A then calls A17240(0),
// state 0x5B polls A17280(0), and state 0x5C is the completed exit state.
void BeginMercsFrontendExitFromPrimary_A4398F(MercsFrontend120& self)
{
    self.pendingUiState = 2;
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(&self) + 0x6A4) = 1;
    self.state = 0x5A;
}

} // namespace re5::split120
