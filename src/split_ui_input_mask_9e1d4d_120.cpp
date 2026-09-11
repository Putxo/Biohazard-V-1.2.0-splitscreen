#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gProfileState_11B2158;
extern std::uint32_t __thiscall PhysicalInputMask_799820(void* input,int device);
extern std::uint32_t __thiscall OwnerInputMask_79B340(void* input,int variant);

static inline std::uint8_t* Session_9E1D4D(){
    return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);
}

// 0x009E1D4D..0x009E1DF7 -- exact local/J2 input-mask construction block.
// This contains the last direct 799B50 caller from the native 40-caller audit:
//   0x009E1DDF -> 0x00799B50.
//
// Native behavior:
// - when self+0x5C is usable and root flag 0x400 is clear, select one physical
//   input mask; under C43BB0 the device comes from the current profile's session
//   slot (+273C4 -> session+0x490), otherwise self+0x5C is the device;
// - otherwise OR physical masks for devices 0 and 1;
// - only when 799B50 accepts the current keyboard/preferred owner, OR the
//   owner channel returned by 79B340(input,1).
std::uint32_t BuildLocalUiInputMask_9E1D4D(void* selfRaw)
{
    auto* self=static_cast<std::uint8_t*>(selfRaw);
    std::uint32_t mask=0;

    const int selfDevice=*reinterpret_cast<const int*>(self+0x5C);
    const std::uint32_t rootFlags=*reinterpret_cast<const std::uint32_t*>(gRoot_12340A4+0x10434);

    if(selfDevice>=0 && (rootFlags&0x400u)==0){
        auto* session=Session_9E1D4D();
        int device=selfDevice;

        if(IsNativeLocalCoopActive_C43BB0(session)){
            const int profileIndex=*reinterpret_cast<const int*>(gProfileState_11B2158+0x20);
            const int slot=*reinterpret_cast<const int*>(
                gProfileState_11B2158 + profileIndex*0x5940 + 0x273C4);
            device=*reinterpret_cast<const int*>(session+0x490+slot*4);
        }

        mask=PhysicalInputMask_799820(gInput_1249C40,device);
    }else{
        mask=PhysicalInputMask_799820(gInput_1249C40,0);
        mask|=PhysicalInputMask_799820(gInput_1249C40,1);
    }

    if(PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))
        mask|=OwnerInputMask_79B340(gInput_1249C40,1);

    return mask;
}

} // namespace re5::split120
