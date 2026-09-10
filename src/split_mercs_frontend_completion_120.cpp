#include <cstdint>

namespace re5::split120 {

struct MercsFrontendCompletion120 {
    std::uint8_t _0000[0x3C];
    std::uint8_t complete; // +0x3C
};

// 0x00A4AE73 -- VERIFIED from direct 1.2.0 disassembly.
// State 0x5C raises the standard completion byte, but the original routine does
// one more operation before returning: it calls 0x7B43C0 on the global manager
// at 0x11B20C4 with routeByte=1 and startSlot=0.
//
// Exact tail:
//   push 0
//   mov  ecx,[0x11B20C4]
//   push 1
//   mov  byte ptr [esi+0x3C],1
//   call 0x7B43C0
//
// Therefore the earlier reconstruction that treated A4AE73 as only a flag
// write was incomplete. The frontend is marked complete AND its routed UI/input
// records are reset/re-enabled through the native 7B43C0 helper.
void MarkMercsFrontendComplete_A4AE73(MercsFrontendCompletion120& self)
{
    self.complete = 1;
    // Native side effect: ResetUiRouteRange_7B43C0(*[0x11B20C4], 1, 0).
}

bool MercsFrontendHasCompleted(const MercsFrontendCompletion120& self)
{
    return self.complete != 0;
}

} // namespace re5::split120
