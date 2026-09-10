#include <cstdint>
namespace re5::split120 {
extern void SessionSetSlotMode_C42A30(void*,int,int);
extern void SessionSetDevice_C42A50(void*,int,int);
extern void RefreshPlayerDevice_7E62F0(void*,int);
extern void SetEventRequest_7F1610(void*,int,int);
// 0x00716720..0x00716802 -- VERIFIED exact local-player activation/deactivation. ECX is not consumed; stack args are active, slot, device.
void SetLocalPlayerActive_716720(void*,bool active,int slot,int device){auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4);auto* session=*reinterpret_cast<std::uint8_t**>(root+0x1042C);const std::uint32_t bit=1u<<slot;if(active){*reinterpret_cast<std::uint32_t*>(session+0x47C)|=bit;SessionSetSlotMode_C42A30(session,slot,0);SessionSetDevice_C42A50(session,slot,device);}else{*reinterpret_cast<std::uint32_t*>(session+0x47C)&=~bit;SessionSetSlotMode_C42A30(session,slot,1);SessionSetDevice_C42A50(session,slot,-1);}RefreshPlayerDevice_7E62F0(*reinterpret_cast<void**>(0x011B27DC),slot);auto* profiles=*reinterpret_cast<std::uint8_t**>(0x011B2158);const int idx=*reinterpret_cast<std::int32_t*>(profiles+0x20);const int value=*reinterpret_cast<std::int32_t*>(profiles+idx*0x5940+0x273D0);SetEventRequest_7F1610(*reinterpret_cast<void**>(0x011B1DD8),active?4:3,value);}
} // namespace re5::split120
