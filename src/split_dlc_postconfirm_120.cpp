#include <cstdint>

namespace re5::split120 {

struct DlcFrontend120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;            // +0x14 low byte used by dispatcher
    std::uint8_t _0015[0x3d-0x15];
    std::uint8_t transitionFlag;   // +0x3D
    std::uint8_t _003e[0x44-0x3e];
    float timer;                   // +0x44
    std::uint8_t _0048[0x188-0x48];
    std::int32_t phase;            // +0x188
    std::uint8_t _018c[0x190-0x18c];
    std::int32_t modeRow;          // +0x190
    std::uint32_t modeMask;        // +0x194
    std::int32_t difficultyRow;    // +0x198
};

enum : std::uint8_t {
    DLC_STATE_INIT = 0,
    DLC_STATE_MODE_SELECT = 0x0A,
    DLC_STATE_MODE_CONFIRM_WAIT = 0x0B,
    DLC_STATE_DIFFICULTY_SELECT = 0x14,
    DLC_STATE_DIFFICULTY_CONFIRM_WAIT = 0x15,
    DLC_STATE_FINAL_CONFIRM = 0x1E,
    DLC_STATE_EXIT_BEGIN = 0x5A,
    DLC_STATE_EXIT_WAIT = 0x5B,
    DLC_STATE_EXIT_DONE = 0x5C,
};

// Exact dispatcher byte table at 0xA530B8 maps the reachable state values in
// 0xA52970 to cases 0,1,2,3,0x0A,0x0B,0x14,0x15,0x1E,0x5A,0x5B,0x5C.

// 0xA52DD4..0xA52E0B -- VERIFIED post-confirm transition.
// After a valid row is committed to session+0x5CC, native closes the selector,
// starts a short transition timer and increments state 0x0A -> 0x0B.
void AdvanceAfterDlcModeCommit_A52DD4(DlcFrontend120& self)
{
    self.transitionFlag = 1;
    self.state = static_cast<std::uint8_t>(self.state + 1);
}

// 0xA52E10..0xA52E64 -- VERIFIED route once the transition timer expires.
// Only row 1 enters the secondary difficulty/network-selection path.
// Rows 0 and 2 both bypass it and proceed to the common exit/start path.
std::uint8_t ResolveDlcPostModeRoute_A52E30(std::int32_t selectedRow)
{
    if (selectedRow == 1)
        return DLC_STATE_DIFFICULTY_SELECT;
    return DLC_STATE_EXIT_BEGIN;
}

// 0xA5303A..0xA53082 -- VERIFIED common exit sequence.
// 0x5A calls A17240(0) and advances immediately to 0x5B.
// 0x5B calls A17280(0) until complete, then advances to 0x5C.
// 0x5C returns from the frontend update.
bool DlcExitStateIsTerminal_A5307C(std::uint8_t state)
{
    return state == DLC_STATE_EXIT_DONE;
}

// The permanently available row 2 therefore commits +0x5CC=2 and takes the
// direct start/exit route rather than row 1's online/difficulty branch.
bool DlcLocalSplitRowTakesDirectStartRoute(std::int32_t selectedRow)
{
    return selectedRow == 2 &&
           ResolveDlcPostModeRoute_A52E30(selectedRow) == DLC_STATE_EXIT_BEGIN;
}

} // namespace re5::split120
