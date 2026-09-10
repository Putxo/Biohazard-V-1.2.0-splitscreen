#include <cstdint>

namespace re5::split120 {

struct MercFrontend120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;          // +0x14 low byte dispatch state
    std::uint8_t _0015[0x5a0-0x15];
    std::int32_t pendingState;   // +0x5A0
    std::uint8_t _05a4[0x6ac-0x5a4];
    std::int32_t localJoinReady; // +0x6AC
};

struct Input120 {
    std::uint8_t _0000[0x614];
    std::int32_t selectedPlayer; // +0x614
    std::int32_t routePlayer;    // +0x618
};

static inline Input120* GlobalInput_1249C40()
{
    return *reinterpret_cast<Input120**>(0x01249C40);
}

// 0x00A4A5B3..0x00A4A610 -- VERIFIED.
// Before calling the local-join detector A43D80 the frontend runs three
// independent blockers (A43460, A437C0, A438D0). If none blocks and +0x6AC
// is nonzero, routePlayer (+0x618) is temporarily forced to 1. A43D80 is then
// called, after which routePlayer is restored to 0. This proves +0x6AC is a
// persistent completion/readiness state that changes which local player the
// shared input precheck observes; it is not itself a transition request.
bool CallJoinDetectorWithCompletionRoute_A4A5E0(
    MercFrontend120& self,
    bool (*joinDetector)(MercFrontend120*))
{
    auto* input = GlobalInput_1249C40();
    if (self.localJoinReady != 0)
        input->routePlayer = 1;

    const bool handled = joinDetector(&self);

    if (self.localJoinReady != 0)
        input->routePlayer = 0;

    return handled;
}

// 0x00A48BE1..0x00A48C19 -- VERIFIED consumer in the recurring frontend
// refresh. For each of players 0 and 1, +0x6AC!=0 causes input+0x618 to be set
// to that player before A443E0/A44820/799B30 run. Thus after local J2 join is
// finalized, both player contexts are refreshed independently each frame.
void SelectPerPlayerRouteWhenJoinComplete_A48BE1(
    const MercFrontend120& self,
    std::int32_t player)
{
    if (self.localJoinReady != 0)
        GlobalInput_1249C40()->routePlayer = player;
}

// A44370 sets +0x6AC=1 after slot-1 setup. The two direct consumers above
// establish its split-screen meaning more precisely: it gates per-player input
// routing/refresh after the second local player has been finalized.
bool IsLocalJoinPerPlayerRoutingEnabled(const MercFrontend120& self)
{
    return self.localJoinReady != 0;
}

} // namespace re5::split120
