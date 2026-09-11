#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SystemSettingsOwner120 {
    std::uint8_t _00[0x104D0];
    std::int32_t menuSelection[11];
    std::uint8_t _104FC[0x10540 - 0x104FC];
    std::int32_t splitModeQuality;
};
static_assert(__builtin_offsetof(SystemSettingsOwner120,menuSelection)==0x104D0,"menu selection base");
static_assert(__builtin_offsetof(SystemSettingsOwner120,splitModeQuality)==0x10540,"SplitModeQuality backing field");

struct MenuEnum120;
extern std::uint8_t* gRoot_12340A4;
extern SplitRenderState120* gSplitRenderState_123457C;
extern int __thiscall MenuEnumCurrent_424F30(MenuEnum120* menu);
extern void __thiscall RebuildRuntimeSplitUi_76CC10(SplitRenderState120* split);
extern int __thiscall QueryGameStatus_C42D90(void* session);
static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}

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
    "SplitModeQuality", "mSplitModeQuality", "LOW", "HIGH",
    10u,0x104F8u,0x10540u,0x3084u,0x0077EAD0u,0x0077EAE0u
};

const SplitModeOptionMetadata120& GetNativeSplitModeOptionMetadata_781695(){return kSplitModeOption120;}

// 0x0077EAD0: ECX=self, no stack args.
int __thiscall GetSplitModeQuality_77EAD0(const SystemSettingsOwner120* self){return self->splitModeQuality;}
// 0x0077EAE0: ECX=self, one stack arg, ret 4.
void __thiscall SetSplitModeQuality_77EAE0(SystemSettingsOwner120* self,int value){self->splitModeQuality=value;}

// 0x0078172A..0x0078174D: native default = HIGH (1).
void InitializeSplitModeQualityDefault_78174D(SystemSettingsOwner120* self){self->splitModeQuality=1;}

// 0x00780EC0..0x00780ED6: cached menu slot 10 -> split+0x3084.
void LoadFullSplitOption_780EC0(SystemSettingsOwner120* root){
    gSplitRenderState_123457C->fullScreenSplitMode=(root->menuSelection[10]==1)?1u:0u;
}

// 0x00781210..0x00781274: slot-10 live apply block. In the parent routine this
// block is reached only after 0x424F30 differs from the cached value and the
// parent has first applied that value through 0x424F00.
void ApplyFullSplitOptionSlot10_781210(MenuEnum120* option){
    const int selected=MenuEnumCurrent_424F30(option);
    gSplitRenderState_123457C->fullScreenSplitMode=(selected==1)?1u:0u;
    if(QueryGameStatus_C42D90(Session())==1)
        RebuildRuntimeSplitUi_76CC10(gSplitRenderState_123457C);
}

// Registration 0x781695..0x78171E:
//   0xF5A034 = "SplitModeQuality"
//   getter=0x77EAD0, setter=0x77EAE0, enum table=0xF5F4B4 (LOW/HIGH)
//   generic descriptor registrar=0x425960.
// Serialization 0x782D72..0x782DB1:
//   self+0x10540 under key 0xF5F824 = "mSplitModeQuality", helper=0x422C00.
// The localized visible label (e.g. Spanish "Modo de pantalla partida") lives
// in game localization resources, not as this executable's internal ASCII key.

} // namespace re5::split120
