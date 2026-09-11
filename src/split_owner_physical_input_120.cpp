#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPhysicalInput_1249C74;
extern void* gUiInput_11B20C4;
extern bool __thiscall UiFallbackInput_7B4710(void*,int);

static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}
static inline bool OwnerGate(std::uint8_t* self,int player){
    if(IsNativeLocalCoopActive_C43BB0(Session())){
        const int preferred=*reinterpret_cast<const int*>(self+0x618);
        if(preferred>=0&&preferred!=*reinterpret_cast<const int*>(gInput_1249C40+0x614))return false;
    }
    if(player>=0&&player!=*reinterpret_cast<const int*>(gInput_1249C40+0x614))return false;
    return gInput_1249C40[0x5F0]!=1;
}
static inline std::uint8_t* PhysicalRecord(std::uint8_t*self){return gPhysicalInput_1249C74+*reinterpret_cast<const int*>(self+0x5E8)*0x8C0;}

// 0x0079AF10..0x0079B005 -- exact thiscall, one logical-player stack arg,
// ret 4. The 0x10/0x20 suppression bits, 0x2000 press bit and +0x164 bit0
// are evaluated only while this instance is unblocked and not in mode 1.
bool __thiscall OwnerPhysicalInputA_79AF10(void* selfRaw,int player){
    auto*self=static_cast<std::uint8_t*>(selfRaw);
    if(!OwnerGate(self,player))return false;
    const std::uint8_t blocked=self[0x5DC];
    if(blocked!=0)return false;
    if(self[0x5F0]==1)return false;
    auto*r=PhysicalRecord(self);
    if((r[0x134]&0x10)!=0)return false;
    if((r[0x134]&0x20)!=0)return false;
    if((*reinterpret_cast<const std::uint32_t*>(r+0x160)&0x2000u)!=0)return true;
    return (r[0x164]&1u)!=0;
}

// 0x0079B010..0x0079B126 -- exact companion thiscall, ret 4. 0x10/0x20 are
// hard-false paths; when direct physical input is unavailable/blocked, native
// falls back through 0x7B4710(command=1).
bool __thiscall OwnerPhysicalInputB_79B010(void* selfRaw,int player){
    auto*self=static_cast<std::uint8_t*>(selfRaw);
    if(!OwnerGate(self,player))return false;
    const std::uint8_t blocked=self[0x5DC];
    auto*r=PhysicalRecord(self);
    if(blocked==0&&self[0x5F0]!=1){
        if((r[0x134]&0x10)!=0)return false;
        if((r[0x134]&0x20)!=0)return false;
        if((*reinterpret_cast<const std::uint32_t*>(r+0x160)&0x08000000u)!=0)return true;
        if((*reinterpret_cast<const std::uint32_t*>(r+0x160)&0x100u)!=0)return true;
    }
    return UiFallbackInput_7B4710(gUiInput_11B20C4,1);
}

} // namespace re5::split120
