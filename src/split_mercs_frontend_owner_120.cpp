#include <cstdint>

namespace re5::split120 {

struct FrontendChild120 {
    std::uint8_t _0000[0x3C];
    std::uint8_t complete;       // +0x3C
    std::uint8_t resultA;        // +0x3D
    std::uint8_t resultB;        // +0x3E
    std::uint8_t cancelled;      // +0x3F
};

struct FrontendOwner120 {
    std::uint8_t _0000[0x18];
    FrontendChild120* child;     // +0x18
    std::uint8_t _001c[0x25-0x1C];
    std::uint8_t substate;       // +0x25
    std::uint8_t _0026[2];
    std::int32_t result;         // +0x28
};

// 0x00BF2CF0..0x00BF2E23 -- VERIFIED owner dispatcher.
// Selector 3 constructs the Mercenaries/Reunion frontend through 0xA46690.
// 0xA46690 allocates exactly 0x6D0 bytes and tail-jumps to ctor 0xA431E0.
constexpr std::uint32_t FrontendFactoryForSelector_BF2D8E(std::uint32_t selector)
{
    switch (selector) {
    case 0: return 0x00A46150;
    case 1: return 0x00A464B0;
    case 2: return 0x00A42E50;
    case 3: return 0x00A46690;
    case 4: return 0x00A44EB0;
    case 5: return 0x00A45210;
    case 6: return 0x00A453E0;
    case 7: return 0x00A456F0;
    default: return 0;
    }
}

// 0x00BF2D11..0x00BF2D60 and 0x00BF2E9B..0x00BF2ECF consume the
// standard child completion byte at +0x3C, destroy the child through
// vtable+0x30, clear owner+0x18 and advance owner+0x25.
std::int32_t DecodeCompletedFrontendResult_BF2D11(const FrontendChild120& child)
{
    if (child.cancelled != 0)
        return -2;

    // Native sequence:
    //   cmp byte ptr [child+0x3E],0
    //   sete dl
    //   lea edx,[edx+edx-1]
    // Therefore +0x3E==0 -> +1, nonzero -> -1.
    return child.resultB == 0 ? 1 : -1;
}

bool OwnerCanConsumeFrontend_BF2E9B(const FrontendOwner120& owner)
{
    return owner.child != nullptr && owner.child->complete != 0;
}

// 0x00BF2E6A..0x00BF2E96 -- VERIFIED next owner substate.
// After the frontend object has been consumed, mode 2 dispatches event 0x0B;
// the paired non-mode-2 route dispatches event 0x1E.
std::uint32_t MercsOwnerCompletionEvent_BF2E6A(std::int32_t sessionMode)
{
    return sessionMode == 2 ? 0x0Bu : 0x1Eu;
}

// Recovered chain in the 1.2.0 executable:
// BF2D8E(selector=3) -> A46690 -> alloc 0x6D0 -> ctor A431E0
// A4AE73 -> child+0x3C=1 after the local split frontend has finished
// BF2E9B -> consumes completion, destroys child, owner substate advances
// BF2E6A -> mode 2 event 0x0B, paired route event 0x1E.

} // namespace re5::split120
