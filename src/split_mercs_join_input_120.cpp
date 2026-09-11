#include <cstdint>

namespace re5::split120 {

struct Session120;
struct MercsJoinUi120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;          // +0x14
    std::uint8_t stateAux15;     // +0x15
    std::uint8_t stateAux16;     // +0x16
    std::uint8_t _0017[0x6AC-0x17];
    std::int32_t uiState6AC;     // +0x6AC
    std::int32_t selectedJoinDevice; // +0x6B0
};
static_assert(__builtin_offsetof(MercsJoinUi120,state)==0x14,"state");
static_assert(__builtin_offsetof(MercsJoinUi120,uiState6AC)==0x6AC,"ui state");
static_assert(__builtin_offsetof(MercsJoinUi120,selectedJoinDevice)==0x6B0,"join device");

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern bool __thiscall IsMercsLocalSplit_C42EC0(Session120*);
extern bool __thiscall DetectControllerJoin_79ADA0(std::uint8_t*,int*);
extern bool __thiscall DetectKeyboardJoin_79AE20(std::uint8_t*,int*);
extern bool __thiscall DetectFallbackJoin_79C1E0(std::uint8_t*);
extern void __thiscall SessionSetDevice_C42A50(Session120*,int,int);
extern void __thiscall RefreshInputMode_7996B0(std::uint8_t*,int);

static inline Session120* Session(){return *reinterpret_cast<Session120**>(gRoot_12340A4+0x1042C);}
static inline void SetBinding(Session120*s,std::uint32_t off,int v){*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(s)+off)=v;}
static inline void AcceptJoin(MercsJoinUi120*self){self->state=0x32;self->stateAux15=0;self->stateAux16=0;}

// 0x00A43DF4..0x00A43F8A -- exact local-join input-assignment branch inside
// parent 0xA43D80. The parent first checks C42EC0, processes an existing +6B0
// device via 0x9E3420/0x9E3480, and dispatches on +0x6AC before reaching this
// state branch. This helper deliberately represents this branch, not the whole
// generic parent state machine.
bool UpdateMercsLocalJoinInput_A43DF4(MercsJoinUi120*self){
 Session120*session=Session();
 const int preferred=*reinterpret_cast<int*>(gInput_1249C40+0x5D8);
 int detected=-1;
 if(DetectControllerJoin_79ADA0(gInput_1249C40,&detected)){
   SetBinding(session,0x654,preferred);self->selectedJoinDevice=detected;SetBinding(session,0x658,detected);
   *reinterpret_cast<int*>(gInput_1249C40+0x614)=0;AcceptJoin(self);return true;
 }
 if(DetectKeyboardJoin_79AE20(gInput_1249C40,&detected)){
   const int other=(detected<=0)?1:0;
   SetBinding(session,0x654,other);self->selectedJoinDevice=detected;SetBinding(session,0x658,detected);
   *reinterpret_cast<int*>(gInput_1249C40+0x614)=0;
   SessionSetDevice_C42A50(session,0,other);SessionSetDevice_C42A50(session,1,detected);
   AcceptJoin(self);return true;
 }
 if(DetectFallbackJoin_79C1E0(gInput_1249C40)){
   const int other=(preferred<=0)?1:0;
   self->selectedJoinDevice=other;SetBinding(session,0x654,preferred);SetBinding(session,0x658,other);
   *reinterpret_cast<int*>(gInput_1249C40+0x614)=1;
   RefreshInputMode_7996B0(gInput_1249C40,0);
   SessionSetDevice_C42A50(session,0,preferred);SessionSetDevice_C42A50(session,1,other);
   AcceptJoin(self);return true;
 }
 return false;
}

} // namespace re5::split120
