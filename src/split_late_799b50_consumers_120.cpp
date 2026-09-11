#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gInput_1249C40;
extern bool __thiscall InputCommand_799B90(std::uint8_t* input,int command,int player);

// The five functions below are intentionally feature-block reconstructions,
// not claims that the very large enclosing menu functions are fully decompiled.
// Each one corresponds exactly to a remaining direct CALL 0x799B50 in 1.2.0.

// 0x00A04BD4..0x00A04BF5 inside parent 0x00A04B90.
// The 799B50 result gates command 0x54. A rejected owner skips that command and
// falls through to the following A02F20(type=0x09) path; an accepted 0x54
// command takes the parent's immediate-success branch.
bool OwnerCommand54Branch_A04BDA()
{
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40) &&
           InputCommand_799B90(gInput_1249C40,0x54,0);
}

// 0x00A08804..0x00A0882A inside parent 0x00A08750.
// Only the current preferred owner may consume command 0x5A. If the owner gate
// is false, native skips 0x5A and continues with the physical/UI fallback path.
bool OwnerCommand5ABranch_A0880A()
{
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40) &&
           InputCommand_799B90(gInput_1249C40,0x5A,0);
}

// 0x00A0B2D5..0x00A0B2FB inside parent 0x00A0B210.
// Exact same owner-gated command-0x58 block later reused by the sister parent
// at A0BFBB. True takes the parent's 0x1000/success destination; false proceeds
// to the remaining controller/UI tests.
bool OwnerCommand58Branch_A0B2DB()
{
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40) &&
           InputCommand_799B90(gInput_1249C40,0x58,0);
}

// 0x00A0B7EB..0x00A0B7FE inside parent 0x00A0B6E0.
// This call exists only after 7B6330 returned exactly 1. Native immediately
// accepts the route when 799B50 is true; a rejected owner continues to 7B63C0.
bool PreferredOwnerImmediateRoute_A0B7F1()
{
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40);
}

// 0x00A0BFB5..0x00A0BFDB inside parent 0x00A0BEF0.
// Sister of A0B2DB: preferred-owner gate followed by command 0x58/player 0.
bool OwnerCommand58Branch_A0BFBB()
{
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40) &&
           InputCommand_799B90(gInput_1249C40,0x58,0);
}

} // namespace re5::split120
