#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gInput_1249C40;
extern void* gUiManager_1234578;
extern int __thiscall QueryUiOverlay_768C10(void* manager);

struct UiRouteManager120 {
    std::uint8_t bytes[0x2F9];
};

static inline std::int32_t& I32(std::uint8_t* p,std::uint32_t off){
    return *reinterpret_cast<std::int32_t*>(p+off);
}

// 0x007B43C0..0x007B4452 -- exact native function.
// ABI: ECX=self, stack(routeByte,startSlot), RET 8.
// 0x7B43C9 is one of the 40 direct native calls to 0x799B50. A rejected owner
// returns before any UI state is modified.
void __thiscall UiRoute43C0_7B43C0(void* selfRaw,int routeByte,int startSlot)
{
    auto* self=static_cast<UiRouteManager120*>(selfRaw);
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return;

    if(QueryUiOverlay_768C10(gUiManager_1234578)!=0)
        self->bytes[0x2F8]=0;

    if(startSlot>=4)return;

    const std::int32_t globalUiX=*reinterpret_cast<const std::int32_t*>(0x012B0468);
    const std::int32_t globalUiY=*reinterpret_cast<const std::int32_t*>(0x012B046C);
    const std::uint8_t route=static_cast<std::uint8_t>(routeByte);

    for(int slot=startSlot;slot<4;++slot){
        std::uint8_t* r=self->bytes+0x20+slot*0x70;
        r[0x00]=route;
        I32(r,0x28)=0;
        I32(r,0x10)=-1;
        I32(r,0x14)=-1;
        I32(r,0x1C)=-1;
        I32(r,0x08)=-1;
        I32(r,0x0C)=-1;
        I32(r,0x68)=-1;
        I32(r,0x64)=-1;
        I32(r,0x5C)=-1;
        I32(r,0x60)=-1;
        I32(r,0x30)=globalUiX;
        I32(r,0x34)=globalUiY;
    }
}

} // namespace re5::split120
