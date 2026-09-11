#include <cstdint>

namespace re5::split120 {

struct AGame120;
using CriticalFn120=void(__stdcall*)(void*);
static void OrRootFlagLocked(std::uint8_t* root,std::uint32_t off,std::uint32_t mask){const bool lock=root[0x1C]!=0||*reinterpret_cast<std::uint8_t*>(0x01249C2E)!=0;auto enter=*reinterpret_cast<CriticalFn120*>(0x00F4A0F8);auto leave=*reinterpret_cast<CriticalFn120*>(0x00F4A164);if(lock)enter(root+0x04);*reinterpret_cast<std::uint32_t*>(root+off)|=mask;if(lock)leave(root+0x04);}
static inline std::uint8_t* Root120(){return *reinterpret_cast<std::uint8_t**>(0x012340A4);}
static inline std::uint8_t* Session120Ptr(){auto*root=Root120();return *reinterpret_cast<std::uint8_t**>(root+0x1042C);}

extern void __thiscall PrepareJoinSlot_726D80(void* playerUi,int slot);
extern void __thiscall RouteJoinDevice_7F1A00(void* route,int device,int enabled);
extern void __thiscall SetInputJoinMode_7E63C0(void* object,int enabled);
extern void __thiscall SetRootFlag_726170(std::uint8_t* root,std::uint32_t flag,int enabled);
extern void __thiscall SetInputTransition_794EC0(void* input,int value);

// 0x00720CE0..0x00720E89 -- exact native Add Player transition.
// ECX=self; stack args=slot, device, sourceIsKeyboard/controller-branch flag;
// native returns ret 0x0C.
void __thiscall BeginAddPlayer_720CE0(AGame120* selfRaw,int slot,int device,bool routeDetectedDevice){
 auto*self=reinterpret_cast<std::uint8_t*>(selfRaw);
 *reinterpret_cast<std::int32_t*>(self+0x350)=device;
 *reinterpret_cast<std::int32_t*>(self+0x354)=slot;
 PrepareJoinSlot_726D80(*reinterpret_cast<void**>(0x011B2158),slot);
 if(!routeDetectedDevice)RouteJoinDevice_7F1A00(*reinterpret_cast<void**>(0x011B1DD8),device,1);
 auto*session=Session120Ptr();*reinterpret_cast<std::int32_t*>(session+0x2C)=0;
 auto*root=Root120();
 OrRootFlagLocked(root,0x10438,0x80000000u);
 OrRootFlagLocked(root,0x10440,0x00400000u);
 OrRootFlagLocked(root,0x10440,0x01000000u);
 OrRootFlagLocked(root,0x10440,0x00800000u);
 SetInputJoinMode_7E63C0(*reinterpret_cast<void**>(0x011B27DC),1);
 SetRootFlag_726170(root,0x00008000u,1);
 SetInputTransition_794EC0(*reinterpret_cast<void**>(0x01249C1C),0);
 *reinterpret_cast<std::int32_t*>(session+0x570)=2;
 *reinterpret_cast<std::uint32_t*>(self+0x04)=0;
 *(self+0x04)=0x0D;
}

} // namespace re5::split120
