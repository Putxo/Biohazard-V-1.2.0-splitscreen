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
struct MenuEnumLookupResult120 {
    std::int32_t key;
    std::int32_t value;
};
static_assert(sizeof(MenuEnumLookupResult120)==8,"0x424EF0 pair size");

extern std::uint8_t* gRoot_12340A4;
extern SplitRenderState120* gSplitRenderState_123457C;
extern void* gSettingsRegistry_12337DC;
extern const MenuEnumLookupResult120* __thiscall MenuEnumLookup_424EF0(MenuEnum120* menu,int selectedIndex);
extern void __thiscall MenuEnumSet_424F00(MenuEnum120* menu,int value);
extern int __thiscall MenuEnumCurrent_424F30(MenuEnum120* menu);
extern void __thiscall RebuildRuntimeSplitUi_76CC10(SplitRenderState120* split);
static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}

// Exact 0x28-byte descriptor shape copied by the native compiler at both
// 0x781695 and 0x782D72. The two 16-bit fields are independently written.
struct NativeSettingDescriptor120 {
    const void* name;             // +00
    std::uint16_t kind;           // +04
    std::uint16_t flags;          // +06
    void* owner;                  // +08
    void* getterOrField;          // +0C
    void* aux10;                  // +10
    void* setter;                 // +14
    void* aux18;                  // +18
    void* aux1C;                  // +1C
    void* aux20;                  // +20
    void* aux24;                  // +24
};
static_assert(sizeof(NativeSettingDescriptor120)==0x28,"native settings descriptor size");
static_assert(__builtin_offsetof(NativeSettingDescriptor120,setter)==0x14,"native setter offset");

extern void __thiscall RegisterEnumSetting_425960(void* registry,
                                                  int valueCount,
                                                  NativeSettingDescriptor120 descriptor,
                                                  const void* enumTable,
                                                  int aux0,int aux1,int aux2);
extern void __thiscall RegisterSerializedSetting_422C00(void* serializer,
                                                        const NativeSettingDescriptor120* descriptor);

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

// 0x00781695..0x0078171E -- exact SplitModeQuality enum registration.
void RegisterSplitModeQuality_781695(SystemSettingsOwner120* self)
{
    NativeSettingDescriptor120 d{};
    d.name=reinterpret_cast<const void*>(0x00F5A034); // "SplitModeQuality"
    d.kind=6;
    d.flags=0x80;
    d.owner=self;
    d.getterOrField=reinterpret_cast<void*>(0x0077EAD0);
    d.aux10=nullptr;
    d.setter=reinterpret_cast<void*>(0x0077EAE0);
    d.aux18=d.aux1C=d.aux20=d.aux24=nullptr;

    RegisterEnumSetting_425960(gSettingsRegistry_12337DC,
                               2,d,
                               reinterpret_cast<const void*>(0x00F5F4B4),
                               0,0,0);
}

// 0x0078172A..0x0078174D: native default = HIGH (1).
void InitializeSplitModeQualityDefault_78174D(SystemSettingsOwner120* self){self->splitModeQuality=1;}

// 0x00780EC0..0x00780ED6: cached menu slot 10 -> split+0x3084.
void LoadFullSplitOption_780EC0(SystemSettingsOwner120* root){
    gSplitRenderState_123457C->fullScreenSplitMode=(root->menuSelection[10]==1)?1u:0u;
}

// 0x00781210..0x00781274 -- exact SplitModeQuality live-apply switch block.
// Native registers:
//   EBX = SystemSettingsOwner120*, EDI = setting slot (10 here),
//   ESI = MenuEnum120*.
// The original does NOT merely read 0x424F30: it resolves the cached menu
// index through 0x424EF0, compares its +4 value with the current enum value,
// commits through 0x424F00 when different, re-reads through 0x424F30, updates
// split+0x3084, and rebuilds runtime split UI only while session status == 1.
// Returns true here only to expose whether the native block took its change path;
// the original code is an inline switch case and falls through to 0x7812A6.
bool ApplyFullSplitOptionSlot10_781210(SystemSettingsOwner120* settings,
                                       int slotIndex,
                                       MenuEnum120* option)
{
    const MenuEnumLookupResult120* pair=
        MenuEnumLookup_424EF0(option,settings->menuSelection[slotIndex]);
    const int targetValue=pair->value;

    if(MenuEnumCurrent_424F30(option)==targetValue)
        return false; // 0x781231 -> 0x7812AB

    MenuEnumSet_424F00(option,targetValue);

    const int committed=MenuEnumCurrent_424F30(option);
    gSplitRenderState_123457C->fullScreenSplitMode=(committed==1)?1u:0u;

    if(QueryGameStatus_C42D90(Session())==1)
        RebuildRuntimeSplitUi_76CC10(gSplitRenderState_123457C);

    return true;
}

// Convenience entry matching the identified native slot. This does not replace
// the exact block above; it makes the slot-10 identity explicit to callers.
bool ApplyFullSplitOptionSlot10_781210(SystemSettingsOwner120* settings,
                                       MenuEnum120* option)
{
    return ApplyFullSplitOptionSlot10_781210(settings,10,option);
}

// 0x00782D72..0x00782DB1 -- exact serializer descriptor for +0x10540.
void SerializeSplitModeQuality_782D72(SystemSettingsOwner120* self,void* serializer)
{
    NativeSettingDescriptor120 d{};
    d.name=reinterpret_cast<const void*>(0x00F5F824); // "mSplitModeQuality"
    d.kind=6;
    d.flags=0;
    d.owner=self;
    d.getterOrField=&self->splitModeQuality;
    d.aux10=nullptr;
    d.setter=nullptr;
    d.aux18=d.aux1C=d.aux20=d.aux24=nullptr;
    RegisterSerializedSetting_422C00(serializer,&d);
}

// Executable identity summary:
//   option key     0xF5A034 = "SplitModeQuality"
//   saved key      0xF5F824 = "mSplitModeQuality"
//   enum table     0xF5F4B4 = LOW/HIGH
//   setting slot   10, cached at self+0x104F8
//   backing field  self+0x10540
//   runtime flag   split+0x3084
// The localized visible label comes from game language resources rather than
// the internal ASCII setting key.

} // namespace re5::split120
