#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SystemSettingsOwner120 {
    std::uint8_t _00[0x104F8];
    std::int32_t fullSplitSelection; // +0x104F8
};

struct MenuEnum120;

extern SplitRenderState120* gSplitRenderState_123457C;
extern int MenuEnumCurrent_424F30(MenuEnum120* menu);
extern void RebuildRuntimeSplitUi_76CC10(SplitRenderState120* split);
extern int QueryGameStatus_C42D90();

// 0x00780EC0..0x00780ED6 -- VERIFIED load/apply path at the end of 0x780CA0.
// The native system-settings loader copies root+0x104F8 into split+0x3084
// using strict equality to 1.
void LoadFullSplitOption_780EC0(SystemSettingsOwner120* root)
{
    gSplitRenderState_123457C->fullScreenSplitMode =
        (root->fullSplitSelection == 1) ? 1u : 0u;
}

// 0x00781210..0x00781274 -- VERIFIED option-change path for menu slot index 10.
// Jump table 0x781344 maps edi==10 to 0x781210. After committing the enum
// selection, 0x424F30 returns the active value; only value 1 means Full.
// In runtime status 1 the native code immediately calls 0x76CC10.
void ApplyFullSplitOptionSlot10_781210(MenuEnum120* option)
{
    const int selected = MenuEnumCurrent_424F30(option);
    gSplitRenderState_123457C->fullScreenSplitMode = (selected == 1) ? 1u : 0u;

    if (QueryGameStatus_C42D90() == 1)
        RebuildRuntimeSplitUi_76CC10(gSplitRenderState_123457C);
}

} // namespace re5::split120
