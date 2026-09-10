#include <cstdint>

namespace re5::split120 {

struct MercsFrontendCompletion120 {
    std::uint8_t _0000[0x3C];
    std::uint8_t complete; // +0x3C
};

// 0x00A4AE73 -- VERIFIED from the recovered Mercenaries/Reunion frontend
// state dispatcher. State 0x5C is the terminal frontend state and raises
// the object's standard completion byte at +0x3C.
void MarkMercsFrontendComplete_A4AE73(MercsFrontendCompletion120& self)
{
    self.complete = 1;
}

// The important boundary for the split-screen reconstruction is therefore:
// local J2 join and final confirmation are fully resolved before +0x3C is
// raised. Destruction / next game-state selection belongs to the owner of
// this frontend object, not to A17240/A17280 or the local-join machinery.
bool MercsFrontendHasCompleted(const MercsFrontendCompletion120& self)
{
    return self.complete != 0;
}

} // namespace re5::split120
