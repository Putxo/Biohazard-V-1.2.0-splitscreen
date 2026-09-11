#include <cstdint>

namespace re5::split120 {

struct Session120;
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern void __thiscall SessionSetSlotMode_C42A30(Session120*,int,int);
extern void __thiscall SessionSetSlotBinding_C42A50(Session120*,int,int);
extern void __thiscall SessionSetSlotAux_C42AB0(Session120*,int,int);
extern bool __thiscall IsDlcLocalSplitSelection_C43320(Session120*);
using CriticalFn120=void(__stdcall*)(void*);

static inline Session120* Session(){return *reinterpret_cast<Session120**>(gRoot_12340A4+0x1042C);}
static inline std::uint8_t* Bytes(Session120*s){return reinterpret_cast<std::uint8_t*>(s);}

static void ResetProfileSlotFloat_BF5946(int slot){
 auto*profile=gPlayerRoot_11B2158;
 const bool lock=profile[0x1C]!=0||*reinterpret_cast<std::uint8_t*>(0x01249C2E)!=0;
 auto enter=*reinterpret_cast<CriticalFn120*>(0x00F4A0F8);
 auto leave=*reinterpret_cast<CriticalFn120*>(0x00F4A164);
 if(lock)enter(profile+0x04);
 const int profileIndex=*reinterpret_cast<const int*>(profile+0x20);
 *reinterpret_cast<float*>(profile+0x273F8+profileIndex*0x5940+slot*0x18)=0.0f;
 if(lock)leave(profile+0x04);
}

// 0x00BF5888..0x00BF5A71 -- exact local-player/session setup block used by
// LIN/DE start. `primarySlot` is the parent's [esp+0x10] value at this block.
void PrepareDlcLocalPlayers_BF5888(int primarySlot){
 auto*session=Session();auto*bytes=Bytes(session);
 *reinterpret_cast<std::uint32_t*>(bytes+0x478)=0;
 *reinterpret_cast<std::uint32_t*>(bytes+0x47C)=0;
 *reinterpret_cast<std::uint32_t*>(bytes+0x30)=0;

 for(int slot=0;slot<4;++slot){
   SessionSetSlotBinding_C42A50(session,slot,-1);
   *reinterpret_cast<std::uint32_t*>(bytes+0x478)|=(1u<<slot);
   const int mode=(bytes[0x56C]!=0&&primarySlot!=slot)?2:1;
   SessionSetSlotMode_C42A30(session,slot,mode);
   SessionSetSlotAux_C42AB0(session,slot,0);
   ResetProfileSlotFloat_BF5946(slot);
 }

 *reinterpret_cast<std::uint32_t*>(bytes+0x47C)|=(1u<<primarySlot);
 SessionSetSlotMode_C42A30(session,primarySlot,0);
 const int preferred=*reinterpret_cast<const int*>(gInput_1249C40+0x5D8);
 SessionSetSlotBinding_C42A50(session,primarySlot,preferred);

 if(IsDlcLocalSplitSelection_C43320(session)){
   const int first=*reinterpret_cast<const int*>(bytes+0x654);
   const int second=*reinterpret_cast<const int*>(bytes+0x658);
   SessionSetSlotBinding_C42A50(session,primarySlot,first);
   const int partner=primarySlot^1;
   *reinterpret_cast<std::uint32_t*>(bytes+0x47C)|=(1u<<partner);
   SessionSetSlotMode_C42A30(session,partner,0);
   SessionSetSlotBinding_C42A50(session,partner,second);
 }
 *reinterpret_cast<std::uint32_t*>(bytes+0x18)=9;
 *reinterpret_cast<std::uint32_t*>(bytes+0x24)=2;
}

} // namespace re5::split120
