#include <cstdint>

namespace re5::split120 {

struct Session120;
static inline const std::uint8_t* Bytes(const Session120* s){return reinterpret_cast<const std::uint8_t*>(s);}
static inline std::int32_t Mode(const Session120* s){return *reinterpret_cast<const std::int32_t*>(Bytes(s)+0x58);}
static inline std::int32_t Selection(const Session120* s){return *reinterpret_cast<const std::int32_t*>(Bytes(s)+0x5CC);}
extern int __thiscall CountActiveSessionSlots_C42B60(const Session120* session);

// 0x00C42E80..0x00C42EDA -- exact Mercenaries/Reunion row predicates.
bool IsMercsRow0_C42E80(const Session120*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==0;}
bool IsMercsRow1_C42EA0(const Session120*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==1;}
bool IsMercsLocalSplitRow_C42EC0(const Session120*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==2;}

// 0x00C432E0..0x00C4333A -- exact LIN/DE row predicates.
bool IsDlcRow0_C432E0(const Session120*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==0;}
bool IsDlcRow1_C43300(const Session120*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==1;}
bool IsDlcLocalSplitRow_C43320(const Session120*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==2;}

// 0x00C43BB0..0x00C43BF0 -- exact native local-coop-active predicate.
// The mode/selection decision is read from the globally active session, while
// +0x56C and the final active-slot count operate on ECX (self). Native calls
// C42B60 directly rather than inlining/recomputing the popcount here.
bool __thiscall IsNativeLocalCoopActive_C43BB0(const Session120* self){
    extern std::uint8_t* gRoot_12340A4;
    const auto* active=*reinterpret_cast<const std::uint8_t* const*>(gRoot_12340A4+0x1042C);
    const int mode=*reinterpret_cast<const int*>(active+0x58);
    if((mode==2||mode==5)&&*reinterpret_cast<const int*>(active+0x5CC)==2)return true;
    if(mode==1)return false;
    if(Bytes(self)[0x56C]!=0)return false;
    return CountActiveSessionSlots_C42B60(self)>=2;
}

} // namespace re5::split120
