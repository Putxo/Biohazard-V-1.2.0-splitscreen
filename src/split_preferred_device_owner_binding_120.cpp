#include <cstdint>

namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void SessionSetDevice_C42A50(void* session,int slot,int device);

static inline std::uint8_t* Session(){
    return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);
}
static inline int KeyboardPlayer(){
    return *reinterpret_cast<const std::int32_t*>(gInput_1249C40+0x614);
}
static inline int PreferredDevice(){
    return *reinterpret_cast<const std::int32_t*>(gInput_1249C40+0x5D8);
}

// 0x00A57A6B..0x00A57A8F -- VERIFIED.
// When 0x799A60 reports that the preferred physical device is usable, the
// native frontend binds that device to the logical keyboard owner (+0x614).
void BindPreferredDeviceToKeyboardOwner_A57A76(){
    SessionSetDevice_C42A50(Session(),KeyboardPlayer(),PreferredDevice());
}

// 0x00A57B89..0x00A57BDE -- VERIFIED.
// Native scans two physical-input records (stride 0x2C0) and selects the first
// record whose +0x38/+0x3B availability bytes are both nonzero. If neither is
// available, the loop leaves device 2 as its sentinel result. The selected
// value is stored at InputManager+0x5D8, then bound to the current owner +0x614.
int SelectAndBindAvailableDevice_A57B89(){
    int selected=2;
    for(int device=0;device<2;++device){
        const std::uint8_t* record=gInput_1249C40+device*0x2C0;
        if(record[0x38]!=0 && record[0x3B]!=0){selected=device;break;}
    }
    *reinterpret_cast<std::int32_t*>(gInput_1249C40+0x5D8)=selected;
    SessionSetDevice_C42A50(Session(),KeyboardPlayer(),selected);
    return selected;
}

// 0x00A57C0C..0x00A57C1B -- VERIFIED end/reset path.
void ClearPreferredDevice_A57C0C(){
    *reinterpret_cast<std::int32_t*>(gInput_1249C40+0x5D8)=-1;
}
} // namespace re5::split120
