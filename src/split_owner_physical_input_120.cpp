#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPhysicalInput_1249C74;
extern void* gUiInput_11B20C4;
extern bool SessionHasLocalCoop_C43BB0(void* session);
extern bool UiFallbackInput_7B4710(void*,int);

static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}
static inline bool OwnerGate(std::uint8_t* self,int player){
    if(SessionHasLocalCoop_C43BB0(Session())){const int preferred=*reinterpret_cast<int*>(self+0x618);if(preferred>=0&&preferred!=*reinterpret_cast<int*>(gInput_1249C40+0x614))return false;}
    if(player>=0&&player!=*reinterpret_cast<int*>(gInput_1249C40+0x614))return false;
    return gInput_1249C40[0x5F0]!=1;
}
static inline std::uint8_t* PhysicalRecord(std::uint8_t*self){return gPhysicalInput_1249C74+*reinterpret_cast<int*>(self+0x5E8)*0x8C0;}

// 0x0079AF10..0x0079B005 -- VERIFIED owner-aware local-player physical input gate.
bool OwnerPhysicalInputA_79AF10(void* selfRaw,int player){
    auto*self=static_cast<std::uint8_t*>(selfRaw);if(!OwnerGate(self,player)||self[0x5DC]!=0)return false;
    auto*r=PhysicalRecord(self);if(self[0x5F0]!=1){if((r[0x134]&0x10)!=0)return false;if((r[0x134]&0x20)!=0)return false;if((*reinterpret_cast<std::uint32_t*>(r+0x160)&0x2000u)!=0)return true;if((r[0x164]&1u)!=0)return true;}return false;
}

// 0x0079B010..0x0079B126 -- VERIFIED companion owner-aware input gate.
bool OwnerPhysicalInputB_79B010(void* selfRaw,int player){
    auto*self=static_cast<std::uint8_t*>(selfRaw);if(!OwnerGate(self,player))return false;
    const bool blocked=self[0x5DC]!=0;auto*r=PhysicalRecord(self);
    if(!blocked&&self[0x5F0]!=1){if((r[0x134]&0x10)!=0)return false;if((r[0x134]&0x20)!=0)return false;if((*reinterpret_cast<std::uint32_t*>(r+0x160)&0x08000000u)!=0)return true;if((*reinterpret_cast<std::uint32_t*>(r+0x160)&0x100u)!=0)return true;}
    return UiFallbackInput_7B4710(gUiInput_11B20C4,1);
}

} // namespace re5::split120
