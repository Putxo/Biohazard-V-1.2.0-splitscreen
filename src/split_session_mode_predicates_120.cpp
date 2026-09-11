#include <cstdint>

namespace re5::split120 {

static inline const std::uint8_t* Bytes(const void* s){return static_cast<const std::uint8_t*>(s);}
static inline std::int32_t Mode(const void* s){return *reinterpret_cast<const std::int32_t*>(Bytes(s)+0x58);}
static inline std::int32_t Selection(const void* s){return *reinterpret_cast<const std::int32_t*>(Bytes(s)+0x5CC);}

// 0x00C42E80..0x00C42EDA -- exact Mercenaries/Reunion row predicates.
bool __thiscall IsMercsRow0_C42E80(const void*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==0;}
bool __thiscall IsMercsRow1_C42EA0(const void*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==1;}
bool __thiscall IsMercsLocalSplitRow_C42EC0(const void*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==2;}

// 0x00C432E0..0x00C4333A -- exact LIN/DE row predicates.
bool __thiscall IsDlcRow0_C432E0(const void*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==0;}
bool __thiscall IsDlcRow1_C43300(const void*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==1;}
bool __thiscall IsDlcLocalSplitRow_C43320(const void*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==2;}

// 0x00C43BB0..0x00C43BF0 -- canonical native local-coop-active predicate.
bool __thiscall IsNativeLocalCoopActive_C43BB0(const void* selfRaw){
    extern std::uint8_t* gRoot_12340A4;
    const auto* self=static_cast<const std::uint8_t*>(selfRaw);
    const auto* active=*reinterpret_cast<const std::uint8_t* const*>(gRoot_12340A4+0x1042C);
    const int mode=*reinterpret_cast<const int*>(active+0x58);
    if((mode==2||mode==5)&&*reinterpret_cast<const int*>(active+0x5CC)==2)return true;
    if(mode==1)return false;
    if(self[0x56C]!=0)return false;
    return CountActiveSessionSlots_C42B60(selfRaw)>=2;
}

} // namespace re5::split120
