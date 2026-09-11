#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gInput_1249C40;

struct UiRouteManagerBase120 {
    std::uint8_t bytes[0x2F9];
};

// 0x007B4330..0x007B435E -- exact native owner-aware free-route predicate.
// 0x7B4339 is direct caller #3 of native 799B50.
bool __thiscall UiInputReady_7B4330(void* selfRaw)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return false;
    auto* self=static_cast<UiRouteManagerBase120*>(selfRaw);
    for(int i=0;i<4;++i)
        if(self->bytes[0x20+i*0x70]==0)return true;
    return false;
}

// 0x007B4370..0x007B43B0 -- exact native command writer.
// ABI: ECX=self, stack(command,index), RET 8. 0x7B4379 is a direct 799B50
// caller; non-owner leaves all records untouched.
void __thiscall UiCommand4370_7B4370(void* selfRaw,int command,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return;
    auto* self=static_cast<UiRouteManagerBase120*>(selfRaw);
    std::uint8_t* r=self->bytes+index*0x70;
    self->bytes[0x2F8]=1;
    r[0x20]=0;
    *reinterpret_cast<std::int32_t*>(r+0x48)=command;
    *reinterpret_cast<std::int32_t*>(r+0x50)=*reinterpret_cast<const std::int32_t*>(0x012B0468);
    *reinterpret_cast<std::int32_t*>(r+0x54)=*reinterpret_cast<const std::int32_t*>(0x012B046C);
}

// 0x007B4480..0x007B44AA -- exact native route offset writer.
// ABI: ECX=self, stack(x,y,index), RET 0x0C. 0x7B4489 is direct caller #6 of
// 799B50; a rejected owner performs no write.
void __thiscall UiCommand4480_7B4480(void* selfRaw,int x,int y,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return;
    auto* self=static_cast<UiRouteManagerBase120*>(selfRaw);
    std::uint8_t* r=self->bytes+index*0x70;
    *reinterpret_cast<std::int32_t*>(r+0x50)=x;
    *reinterpret_cast<std::int32_t*>(r+0x54)=y;
}

} // namespace re5::split120
