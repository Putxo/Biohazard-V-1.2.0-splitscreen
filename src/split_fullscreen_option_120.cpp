#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SystemSettingsOwner120 {
    std::uint8_t _00[0x104D0];
    std::int32_t menuSelection[11]; // +0x104D0..+0x104F8
    std::uint8_t _104FC[0x10540 - 0x104FC];
    std::int32_t splitModeQuality;  // +0x10540
};

struct MenuEnum120;

extern SplitRenderState120* gSplitRenderState_123457C;
extern int MenuEnumCurrent_424F30(MenuEnum120* menu);
extern void RebuildRuntimeSplitUi_76CC10(SplitRenderState120* split);
extern int QueryGameStatus_C42D90();

// Exact executable-side identity of the native split-screen option.
// Registration block: 0x00781695..0x0078171E
// Serialization block: 0x00782D72..0x00782DB1
struct SplitModeOptionMetadata120 {
    const char* settingName;
    const char* serializedName;
    const char* value0Name;
    const char* value1Name;
    std::uint32_t menuSlot;
    std::uint32_t cachedSelectionOffset;
    std::uint32_t backingFieldOffset;
    std::uint32_t runtimeFlagOffset;
    std::uint32_t getterVa;
    std::uint32_t setterVa;
};

static constexpr SplitModeOptionMetadata120 kSplitModeOption120{
    "SplitModeQuality",       // native string @ 0x00F5A034
    "mSplitModeQuality",      // native string @ 0x00F5F824
    "LOW",                    // enum string referenced by table @ 0x00F5F4B4
    "HIGH",                   // enum string referenced by table @ 0x00F5F4B4
    10u,
    0x104F8u,
    0x10540u,
    0x3084u,
    0x0077EAD0u,
    0x0077EAE0u
};

const SplitModeOptionMetadata120& GetNativeSplitModeOptionMetadata_781695()
{
    return kSplitModeOption120;
}

// 0x0077EAD0 -- exact getter registered by 0x781695.
int GetSplitModeQuality_77EAD0(const SystemSettingsOwner120* self)
{
    return self->splitModeQuality;
}

// 0x0077EAE0 -- exact setter registered by 0x781695, RET 4.
void SetSplitModeQuality_77EAE0(SystemSettingsOwner120* self, int value)
{
    self->splitModeQuality = value;
}

// 0x0078172A..0x0078174D -- native constructor/default tail.
// SplitModeQuality backing field is initialized to 1 (HIGH).
void InitializeSplitModeQualityDefault_78174D(SystemSettingsOwner120* self)
{
    self->splitModeQuality = 1;
}

// 0x00780EC0..0x00780ED6 -- exact load/apply path at the end of 0x780CA0.
// The 11-entry system/graphics setting array uses slot 10 for SplitModeQuality.
void LoadFullSplitOption_780EC0(SystemSettingsOwner120* root)
{
    gSplitRenderState_123457C->fullScreenSplitMode =
        (root->menuSelection[kSplitModeOption120.menuSlot] == 1) ? 1u : 0u;
}

// 0x00781210..0x00781274 -- exact option-change path for menu slot index 10.
// The native jump table at 0x781344 maps index 10 to this block. Value 1
// (HIGH) drives split+0x3084; value 0 (LOW) clears it.
void ApplyFullSplitOptionSlot10_781210(MenuEnum120* option)
{
    const int selected = MenuEnumCurrent_424F30(option);
    gSplitRenderState_123457C->fullScreenSplitMode = (selected == 1) ? 1u : 0u;

    if (QueryGameStatus_C42D90() == 1)
        RebuildRuntimeSplitUi_76CC10(gSplitRenderState_123457C);
}

// 0x00781695..0x0078171E registration facts represented above:
//   name      = 0xF5A034 -> "SplitModeQuality"
//   getter    = 0x77EAD0
//   setter    = 0x77EAE0
//   enum data = 0xF5F4B4, whose referenced labels are LOW/HIGH
//   generic registration helper = 0x425960
//
// 0x00782D72..0x00782DB1 serialization facts represented above:
//   field address = self + 0x10540
//   key           = 0xF5F824 -> "mSplitModeQuality"
//   generic serializer helper = 0x422C00
//
// The generic descriptor ABI is intentionally not guessed here: the exact
// executable identity, offsets, callbacks, enum labels and runtime binding are
// represented without inventing a higher-level signature for 0x425960/0x422C00.

} // namespace re5::split120
