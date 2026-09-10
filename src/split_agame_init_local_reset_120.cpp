#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void ClearRootGateFlags_7159E0(void*,std::uint32_t);
extern void ClearRootStateFlags_715A30(void*,std::uint32_t);

static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}

// Split/local-coop-relevant fragments of the full aGame initializer
// 0x00715450..0x0071597B. Generic profile/render initialization surrounding
// these writes is deliberately outside this repository's scope.

// 0x00715495..0x007154B2 -- VERIFIED keyboard-owner initialization.
void InitAGameKeyboardOwner_715495(){if(*reinterpret_cast<const int*>(Session()+0x18)!=6)*reinterpret_cast<int*>(gInput_1249C40+0x614)=0;}

// 0x00715508..0x00715518 / 0x0071557E / 0x00715598 -- VERIFIED.
// Native session slot constructor is invoked for initial session types 0,1,8.
bool AGameInitUsesLocalSlotConstructor_715508(int sessionType){return sessionType==0||sessionType==1||sessionType==8;}

// 0x007156F7..0x007157A1 -- VERIFIED root gate cleanup used by local transitions.
void ResetAGameLocalGateBits_7156F7(){
    const std::uint32_t f=*reinterpret_cast<const std::uint32_t*>(gRoot_12340A4+0x10434);
    if((f&0x20u)!=0)ClearRootGateFlags_7159E0(gRoot_12340A4,0x20u);
    auto*s=Session();
    if((f&0x20u)!=0){*reinterpret_cast<std::uint32_t*>(s+0x04)=0;*reinterpret_cast<std::int32_t*>(s+0x08)=-1;}
    if((f&0x02u)!=0){*reinterpret_cast<std::uint32_t*>(s+0x04)=0;*reinterpret_cast<std::int32_t*>(s+0x08)=-1;}
}

// 0x00715805..0x00715977 -- VERIFIED split/local state reset tail.
// The native code wraps every write in the same root synchronization callbacks;
// using the already reconstructed helpers preserves those semantics for +10438.
void ResetAGameSplitLocalState_715805(){
    *reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10440)=0;
    *reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10448)=0;
    ClearRootStateFlags_715A30(gRoot_12340A4,0x10000000u);
    ClearRootStateFlags_715A30(gRoot_12340A4,0x00000200u);
    ClearRootStateFlags_715A30(gRoot_12340A4,0x00000400u);
    ClearRootStateFlags_715A30(gRoot_12340A4,0x40000000u);
}

} // namespace re5::split120
