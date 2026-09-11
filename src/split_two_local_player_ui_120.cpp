#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4; extern std::uint8_t* gInput_1249C40; extern void* gUiInput_11B20C4;
extern void* gFrontend_11B1D0C; extern void* gPlayerManager_11B27DC; extern std::uint8_t* gDisplay_12345D4; extern std::uint8_t* gFrame_11B209C;
extern int QueryGameStatus_C42D90(void*); extern int PollFrontendEntry_79F720(void*,void*,int,int); extern void EmitFrontendEvent_79F790(void*,int,void*);
extern std::uint32_t PhysicalInputMask_799820(void*,int); extern std::uint32_t OwnerInputMask_79B340(void*,int);
extern bool UiConfirmQuery_7B63C0(void*,int,int,int); extern bool OwnerPhysicalInputA_79AF10(void*,int); extern bool OwnerPhysicalInputB_79B010(void*,int);
extern void* FindPlayerActorByNo_7E6220(void*,int); extern void ClearPreferredOwner_799B30(void*,int); extern void ResetLocalUiSlot_7D9B30(void*,int); extern void DrawLocalPlayerOverlay_7DEA10(void*,void*,int);
#pragma pack(push,1)
struct LocalUiRecord120 {
    std::uint8_t type;          // +0x00
    std::uint8_t _01[2];        // +0x01
    std::uint16_t cursor;       // +0x03
    std::uint8_t _05[2];        // +0x05
    std::int32_t count;         // +0x07
    float timer;                // +0x0B
    std::uint8_t flag0F;        // +0x0F
    std::uint8_t asyncState;    // +0x10
    std::uint8_t _11[2];        // +0x11
    std::int32_t a13;           // +0x13
    std::int32_t a17;           // +0x17
    std::uint8_t _1B;           // +0x1B; native stride = 0x1C
};
#pragma pack(pop)
static_assert(sizeof(LocalUiRecord120)==0x1C,"native local UI record stride");
static_assert(__builtin_offsetof(LocalUiRecord120,cursor)==0x03,"cursor offset");
static_assert(__builtin_offsetof(LocalUiRecord120,count)==0x07,"count offset");
static_assert(__builtin_offsetof(LocalUiRecord120,timer)==0x0B,"timer offset");
static_assert(__builtin_offsetof(LocalUiRecord120,asyncState)==0x10,"async state offset");
static_assert(__builtin_offsetof(LocalUiRecord120,a13)==0x13,"a13 offset");
static_assert(__builtin_offsetof(LocalUiRecord120,a17)==0x17,"a17 offset");
static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
static inline bool NavUp(std::uint32_t mask){return ((*reinterpret_cast<std::uint32_t*>(gInput_1249C40+0x5B4)&mask)!=0)||UiRoute5620_7B5620(gUiInput_11B20C4,0,0,0)||UiRoute5620_7B5620(gUiInput_11B20C4,2,0,0)||OwnerPhysicalInputA_79AF10(gInput_1249C40,-1);}
static inline bool Confirm(std::uint32_t mask){return ((*reinterpret_cast<std::uint32_t*>(gInput_1249C40+0x5B8)&mask)!=0)||UiConfirmQuery_7B63C0(gUiInput_11B20C4,1,0,0)||OwnerPhysicalInputB_79B010(gInput_1249C40,-1);}
// 0x007DFBA0..0x007DFF5A -- exact two-slot local-player UI/input update family.
void UpdateTwoLocalPlayerUi_7DFBA0(void* selfRaw){
 auto*self=static_cast<std::uint8_t*>(selfRaw); if(*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10434)&0x500u)return;
 auto*session=Session(); auto*records=reinterpret_cast<LocalUiRecord120*>(self+0x2F671);
 for(int slot=0;slot<2;++slot){ auto&r=records[slot]; *reinterpret_cast<int*>(gInput_1249C40+0x618)=slot;
   if(r.asyncState>=2&&*reinterpret_cast<void**>(self+0x2F6E8)){int q=PollFrontendEntry_79F720(gFrontend_11B1D0C,*reinterpret_cast<void**>(self+0x2F6E8),r.a13,r.a17);if(q==1)r.asyncState=3;else if(q==0)r.asyncState=1;}
   if((self[0x2F6E0]&(1u<<slot))==0)continue;
   const int device=*reinterpret_cast<int*>(session+0x490+slot*4);
   if((*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x1043C)&0x02000000u)!=0){
     if(device>=0)DrawLocalPlayerOverlay_7DEA10(self,reinterpret_cast<std::uint8_t*>(&r)-1,slot);
     continue;
   }
   if(r.type==5){
     UiCommand4370_7B4370(gUiInput_11B20C4,0xCE,0);
     if(QueryGameStatus_C42D90(session)==1){int v=(*reinterpret_cast<int*>(gDisplay_12345D4+0x54)*720)/(*reinterpret_cast<int*>(gDisplay_12345D4+0x50));v/=4; UiCommand4480_7B4480(gUiInput_11B20C4,0,static_cast<int>(float(v)*-0.25f),0);}
     if(device<0){ResetLocalUiSlot_7D9B30(self,slot);continue;}
     std::uint32_t mask=0; if((*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10438)&0x1000u)==0){std::uint32_t physical=PhysicalInputMask_799820(gInput_1249C40,device);mask=(slot>=0&&slot!=*reinterpret_cast<int*>(gInput_1249C40+0x614))?physical:(physical|OwnerInputMask_79B340(gInput_1249C40,1));}
     int cursor=r.cursor; const int next=cursor+1;
     if(next>=r.count&&NavUp(mask)){EmitFrontendEvent_79F790(gFrontend_11B1D0C,0x1D,self); void*a=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,slot); *reinterpret_cast<std::uint32_t*>(static_cast<std::uint8_t*>(a)+0x2D84)&=~0x10u; UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0); self[0x2F6E0]&=std::uint8_t(~(1u<<slot)); continue;}
     int updated=cursor;
     if(NavUp(mask)){updated=next; if(updated>=r.count)updated=r.count-1;}
     if(Confirm(mask)){if(--updated<0)updated=0;}
     if(updated!=cursor){int msg=(r.a17?0x1B:0x11);if((*reinterpret_cast<std::uint32_t*>(gInput_1249C40+0x5B8)&mask)!=0)++msg;EmitFrontendEvent_79F790(gFrontend_11B1D0C,msg,self);r.cursor=static_cast<std::uint16_t>(updated);UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);}
     DrawLocalPlayerOverlay_7DEA10(self,reinterpret_cast<std::uint8_t*>(&r)-1,slot); continue;
   }
   if(r.timer>0.0f){r.timer-=*reinterpret_cast<float*>(gFrame_11B209C+0x28);if(r.timer<0.0f)r.timer=0.0f;}
   const bool expired=r.timer<=0.0f; if(r.flag0F){if(r.asyncState==2)continue;if(r.asyncState==3){if(device>=0)DrawLocalPlayerOverlay_7DEA10(self,reinterpret_cast<std::uint8_t*>(&r)-1,slot);continue;}}
   if(expired)self[0x2F6E0]&=std::uint8_t(~(1u<<slot)); else if(device>=0)DrawLocalPlayerOverlay_7DEA10(self,reinterpret_cast<std::uint8_t*>(&r)-1,slot);
 }
 ClearPreferredOwner_799B30(gInput_1249C40,-1); ++*reinterpret_cast<std::uint32_t*>(self+0x2F6E4);
}
}
