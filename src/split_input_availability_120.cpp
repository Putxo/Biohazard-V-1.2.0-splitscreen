#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4; extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gInputAux_1249C78; extern void* gUiInput_11B20C4;
extern void* gEventManager_1234578; extern void* gPlayerManager_11B27DC;
extern std::uint8_t* gDisplay_12345D4;
extern bool UiInputReady_7B4330(void*); extern void* QueryEventState_768C10(void*);
extern void* FindDefaultPlayerActor_7E6290(void*); extern bool EventBlocksPlayer_7162D0(void*,int);
extern bool RootRuntimeReady_77F4B0(void*); extern void RefreshDeviceVector_665620(void*,int);
static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
// 0x0079AB10..0x0079AD94 -- full native availability/ownership synchronizer.
void UpdateLocalInputAvailability_79AB10(void* selfRaw){
 auto*s=static_cast<std::uint8_t*>(selfRaw); auto rootFlags=*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10438);
 if(rootFlags&0x10000000u)s[0x600]=0; if(rootFlags&0x200u)s[0x600]=0;
 const bool uiReady=UiInputReady_7B4330(gUiInput_11B20C4);
 if(!uiReady){
   if(rootFlags&0x400u)s[0x600]=0;
   if(void* ev=QueryEventState_768C10(gEventManager_1234578)) if(static_cast<std::uint8_t*>(ev)[0x138]) s[0x600]=0;
   if(gPlayerManager_11B27DC){if(void* actor=FindDefaultPlayerActor_7E6290(gPlayerManager_11B27DC)){
      if(void* ev=QueryEventState_768C10(gEventManager_1234578))
        if(EventBlocksPlayer_7162D0(ev,static_cast<std::uint8_t*>(actor)[0x2D9C]))s[0x600]=0;
   }}
 }
 constexpr std::uint8_t one=1;
 rootFlags=*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10438);
 if((rootFlags&0x40000000u)&&UiInputReady_7B4330(gUiInput_11B20C4))s[0x600]=one;
 auto*session=Session();
 if(session[0x471]&&(*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10434)&0x20000u))s[0x600]=one;
 if(session[0x79C])s[0x600]=0;
 const int localOwner=*reinterpret_cast<int*>(s+0x614); const int keyboardOwner=*reinterpret_cast<int*>(gInput_1249C40+0x614);
 if((localOwner>=0&&localOwner!=keyboardOwner)||gInput_1249C40[0x5F0]!=1)s[0x600]=0;
 if(*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x10434)&0x1000u)s[0x600]=one;
 if(!RootRuntimeReady_77F4B0(gRoot_12340A4))s[0x600]=one;
 const int index=*reinterpret_cast<int*>(gInput_1249C40+0x5EC); auto*aux=gInputAux_1249C78+index*0x70;
 s[0x601]=aux[0x28]; aux[0x28]=s[0x600]; aux[0x29]=(session[0x28]!=0);
 if(!RootRuntimeReady_77F4B0(gRoot_12340A4))aux[0x29]=0; if(gDisplay_12345D4[0x21])aux[0x29]=1;
 if(gRoot_12340A4[0x1054C]==0&&gRoot_12340A4[0x1054D]==0&&s[0x601]!=s[0x600]){
   if(aux[0x28]){RefreshDeviceVector_665620(s+0x608,index);s[0x600]=one;return;}
   *reinterpret_cast<std::uint32_t*>(s+0x608)=*reinterpret_cast<std::uint32_t*>(aux+0x20);
   *reinterpret_cast<std::uint32_t*>(s+0x60C)=*reinterpret_cast<std::uint32_t*>(aux+0x24);
 }
 s[0x600]=one;
}
}
