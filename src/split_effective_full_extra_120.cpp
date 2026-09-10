#include <cstdint>
namespace re5::split120 {
struct DisplaySplitFlags120 { std::uint8_t _0000[0x3064]; std::uint8_t splitActive; std::uint8_t _3065[0x3078-0x3065]; float field3078; std::uint8_t _307C[0x3084-0x307C]; std::uint8_t full; std::uint8_t transient; };
extern DisplaySplitFlags120* gSplit_123457C;
extern int SessionDisplayStatus_C42D90(void*);
extern std::uint8_t* gRoot_12340A4;
static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);} static inline bool EffectiveFull(const DisplaySplitFlags120*s){return s->full!=0&&s->transient==0;}
// 75CA8B VERIFIED destructor/reset fragment.
void ClearTransientSplitFlag_75CA8B(){gSplit_123457C->transient=0;}
// 75E88B VERIFIED transition setup fragment.
void SetTransientSplitFlag_75E88B(){gSplit_123457C->transient=1;}
// 75F09E VERIFIED destructor/reset fragment.
void ClearTransientSplitFlag_75F09E(){gSplit_123457C->transient=0;}
// 764BBC..764BFA VERIFIED: status1 uses the split horizontal offset unless Full is effective.
bool UseSplitHorizontalOffset_764BBC(){return SessionDisplayStatus_C42D90(Session())==1&&!EffectiveFull(gSplit_123457C);}
// 9E3C80..9E3CC6 VERIFIED: status1 + non-effective-Full returns trunc([+3078]*1280.0f); otherwise 1280.
int SplitUiLogicalWidth_9E3C80(){if(SessionDisplayStatus_C42D90(Session())!=1||EffectiveFull(gSplit_123457C))return 0x500;return static_cast<int>(gSplit_123457C->field3078*1280.0f);}
// 9F961B VERIFIED constructor fragment: +160 caches (full && !transient).
std::uint8_t InitialEffectiveFullCache_9F961B(){return EffectiveFull(gSplit_123457C)?1u:0u;}
// 9FA86C VERIFIED: in status1 a cached effective-Full mismatch forces refresh.
bool EffectiveFullCacheChanged_9FA86C(std::uint8_t cached){if(SessionDisplayStatus_C42D90(Session())!=1)return false;return cached!=(EffectiveFull(gSplit_123457C)?1u:0u);}
// 76C57F..76C599 VERIFIED: effective Full cancels horizontal crop and uses render width.
bool SplitGeometryUsesUncroppedWidth_76C57F(){return EffectiveFull(gSplit_123457C);}
// 76C45A..76C47E VERIFIED transient/active override.
bool ApplyTransientResourceVisibilityOverride_76C45A(bool current){if(gSplit_123457C->transient&&gSplit_123457C->splitActive&&SessionDisplayStatus_C42D90(Session())!=1)return true;return current;}
} // namespace re5::split120
