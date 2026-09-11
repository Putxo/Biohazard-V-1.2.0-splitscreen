#include <cstdint>

namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void __thiscall SetSessionSlotDevice_C42A50(void* session,int slot,int device);

static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
static inline int KeyboardPlayer(){return *reinterpret_cast<const std::int32_t*>(gInput_1249C40+0x614);}
static inline int PreferredDevice(){return *reinterpret_cast<const std::int32_t*>(gInput_1249C40+0x5D8);}

// 0x00A57A6B..0x00A57A8F -- after 0x799A60 accepts the preferred physical
// device, native binds input+0x5D8 to the logical keyboard owner input+0x614.
void BindPreferredDeviceToKeyboardOwner_A57A76(){SetSessionSlotDevice_C42A50(Session(),KeyboardPlayer(),PreferredDevice());}

// 0x00A57B89..0x00A57BDE -- scan the two 0x2C0 physical records. A record is
// usable only when +0x38 and +0x3B are both non-zero. Sentinel 2 survives if
// neither record is available. Native stores it at +0x5D8 and binds it to +614.
int SelectAndBindAvailableDevice_A57B89(){
    int selected=2;
    for(int device=0;device<2;++device){
        const std::uint8_t* record=gInput_1249C40+device*0x2C0;
        if(record[0x38]!=0&&record[0x3B]!=0){selected=device;break;}
    }
    *reinterpret_cast<std::int32_t*>(gInput_1249C40+0x5D8)=selected;
    SetSessionSlotDevice_C42A50(Session(),KeyboardPlayer(),selected);
    return selected;
}

// 0x00A57C0C..0x00A57C1B -- exact preferred-device reset.
void ClearPreferredDevice_A57C0C(){*reinterpret_cast<std::int32_t*>(gInput_1249C40+0x5D8)=-1;}
} // namespace re5::split120
