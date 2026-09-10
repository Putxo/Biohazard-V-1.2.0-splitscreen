#include <cstdint>

namespace re5::split120 {

extern "C" bool SplitCapabilityAlways_76A410();
extern "C" bool PlatformOnlineUnavailable_794D40();

// 0x00A529AC..0x00A529DE -- VERIFIED from direct 1.2 disassembly.
// DLC three-row selector availability mask at self+0x194:
//   bit0 (row 0) starts enabled unconditionally,
//   bit2 (row 2) is ORed when 0x76A410 returns true,
//   bit1 (row 1) is ORed only when 0x794D40 returns false.
// 0x76A410 is itself a two-instruction function: mov al,1 / ret, therefore
// row 2 is unconditionally available in this build.
std::uint32_t BuildDlcModeAvailability_A529AC()
{
    std::uint32_t mask = 1u;
    if (SplitCapabilityAlways_76A410())
        mask |= 4u;
    if (!PlatformOnlineUnavailable_794D40())
        mask |= 2u;
    return mask;
}

// Exact consequence of the native 0x76A410 implementation in 1.2.0.
bool IsDlcRow2AlwaysAvailable_76A410()
{
    return true;
}

} // namespace re5::split120
