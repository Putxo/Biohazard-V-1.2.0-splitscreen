#include <cstdint>

namespace re5::split120 {

extern void* gUiInput_11B20C4;

struct MercsFrontendCompletion120 {
    std::uint8_t _0000[0x3C];
    std::uint8_t complete; // +0x3C
};

// 0x00A4AE73 -- VERIFIED from direct 1.2.0 disassembly.
// State 0x5C raises the standard completion byte and then calls 0x7B43C0 on
// the global routed UI/input manager at 0x11B20C4 with routeByte=1/startSlot=0.
//
// Exact tail:
//   push 0
//   mov  ecx,[0x11B20C4]
//   push 1
//   mov  byte ptr [esi+0x3C],1
//   call 0x7B43C0
void MarkMercsFrontendComplete_A4AE73(MercsFrontendCompletion120& self)
{
    self.complete = 1;
    UiRoute43C0_7B43C0(gUiInput_11B20C4, 1, 0);
}

bool MercsFrontendHasCompleted(const MercsFrontendCompletion120& self)
{
    return self.complete != 0;
}

} // namespace re5::split120
