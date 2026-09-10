#include <cstdint>

namespace re5::split120 {

struct Session120;
static inline const std::uint8_t* Bytes(const Session120* s){return reinterpret_cast<const std::uint8_t*>(s);}
static inline std::int32_t Mode(const Session120* s){return *reinterpret_cast<const std::int32_t*>(Bytes(s)+0x58);}
static inline std::int32_t Selection(const Session120* s){return *reinterpret_cast<const std::int32_t*>(Bytes(s)+0x5CC);}
static inline int Pop4(std::uint32_t v){v&=0xFu;return int(v&1u?1:0)+int(v&2u?1:0)+int(v&4u?1:0)+int(v&8u?1:0);}

// 0x00C42E80..0x00C42EDA -- VERIFIED Mercenaries/Reunion row predicates.
bool IsMercsRow0_C42E80(const Session120*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==0;}
bool IsMercsRow1_C42EA0(const Session120*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==1;}
bool IsMercsLocalSplitRow_C42EC0(const Session120*s){const auto m=Mode(s);return(m==2||m==5)&&Selection(s)==2;}

// 0x00C432E0..0x00C4333A -- VERIFIED LIN/DE row predicates.
bool IsDlcRow0_C432E0(const Session120*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==0;}
bool IsDlcRow1_C43300(const Session120*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==1;}
bool IsDlcLocalSplitRow_C43320(const Session120*s){const auto m=Mode(s);return(m==3||m==4)&&Selection(s)==2;}

// 0x00C43BB0..0x00C43BF0 -- VERIFIED native "local coop effectively active" predicate.
// Mercs/Reunion explicitly become local when row 2 is selected. Mode 1 never
// qualifies. Other modes require +56C==0 and at least two active session slots.
bool IsNativeLocalCoopActive_C43BB0(const Session120* self){
    extern std::uint8_t* gRoot_12340A4;
    auto* session=*reinterpret_cast<const std::uint8_t* const*>(gRoot_12340A4+0x1042C);
    const int mode=*reinterpret_cast<const int*>(session+0x58);
    if((mode==2||mode==5)&&*reinterpret_cast<const int*>(session+0x5CC)==2)return true;
    if(mode==1)return false;
    if(Bytes(self)[0x56C]!=0)return false;
    return Pop4(*reinterpret_cast<const std::uint32_t*>(Bytes(self)+0x47C))>=2;
}

} // namespace re5::split120
