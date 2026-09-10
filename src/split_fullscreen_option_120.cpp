#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SystemSettingsOwner120 {
    std::uint8_t _00[0x104D0];
    std::int32_t menuSelection[11]; // +0x104D0..+0x104F8, slot 10 is SplitModeQuality
    std::uint8_t _104FC[0x10540 - 0x104FC];
    std::int32_t splitModeQuality;  // +0x10540, registered as "mSplitModeQuality"
};

struct MenuEnum120;

extern SplitRenderState120* gSplitRenderState_123457C;
extern int MenuEnumCurrent_424F30(MenuEnum120* menu);
extern void RebuildRuntimeSplitUi_76CC10(SplitRenderState120* split);
extern int QueryGameStatus_C42D90();

// 0x0077EAD0 -- exact getter registered for SplitModeQuality.
int GetSplitModeQuality_77EAD0(const SystemSettingsOwner120* self)
{
    return self->splitModeQuality;
}

// 0x0077EAE0 -- exact setter registered for SplitModeQuality, RET 4.
void SetSplitModeQuality_77EAE0(SystemSettingsOwner120* self, int value)
{
    self->splitModeQuality = value;
}

// 0x00780EC0..0x00780ED6 -- VERIFIED load/apply path at the end of 0x780CA0.
// 0x780CA0 iterates the 11 graphics/system settings. Slot 10 is the resource
// named "SplitModeQuality" (0xF5A034). Its cached selected index is therefore
// menuSelection[10] at +0x104F8. Native copies strict (index == 1) to +0x3084.
void LoadFullSplitOption_780EC0(SystemSettingsOwner120* root)
{
    gSplitRenderState_123457C->fullScreenSplitMode =
        (root->menuSelection[10] == 1) ? 1u : 0u;
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

// 0x00781695..0x0078171E -- VERIFIED registration block.
// The graphics settings descriptor is registered under the exact name
// "SplitModeQuality" and uses getter/setter callbacks 0x77EAD0/0x77EAE0.
//
// 0x00782D72..0x00782DB1 -- VERIFIED serialization binding.
// The serializer binds the exact field address self+0x10540 under key
// "mSplitModeQuality" (0xF5F824). Thus the persisted backing field and the
// runtime +0x3084 flag are now tied through a concrete native path.

} // namespace re5::split120
