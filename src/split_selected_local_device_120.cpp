#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void* gPlayerManager_11B27DC;
extern bool __thiscall IsNativeLocalCoopActive_C43BB0(const void* session);
extern void* __thiscall FindPlayerActorByNo_7E6220(void* manager,int playerNo);
extern void* __thiscall FindDefaultPlayerActor_7E6290(void* manager);

static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}

// 0x0079BC50..0x0079BCDB -- exact selected-device head of the native input
// update. ECX=self, no stack args. Native intentionally calls C43BB0 a second
// time after a missing actor rather than caching the earlier result.
int __thiscall ResolveSelectedLocalDevice_79BC50(void* selfRaw){
    auto*self=static_cast<std::uint8_t*>(selfRaw);
    self[0x61C]=0;

    void* actor=nullptr;
    if(IsNativeLocalCoopActive_C43BB0(Session())){
        const int player=*reinterpret_cast<const int*>(gInput_1249C40+0x614);
        actor=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,player);
    }else{
        actor=FindDefaultPlayerActor_7E6290(gPlayerManager_11B27DC);
    }

    if(actor!=nullptr)
        return static_cast<int>(static_cast<std::uint8_t*>(actor)[0x2DB1]);

    if(IsNativeLocalCoopActive_C43BB0(Session())){
        const int player=*reinterpret_cast<const int*>(gInput_1249C40+0x614);
        return *reinterpret_cast<const int*>(Session()+0x490+player*4);
    }
    return *reinterpret_cast<const int*>(self+0x5D8);
}

// 0x00715495..0x007154B2 -- exact aGame-init ownership write embedded in the
// parent initializer. Session type 6 preserves the current owner; all others
// set InputManager+0x614 to logical player 0.
void ResetKeyboardPlayerOnAGameInit_715495(){
    if(*reinterpret_cast<const int*>(Session()+0x18)!=6)
        *reinterpret_cast<int*>(gInput_1249C40+0x614)=0;
}

} // namespace re5::split120
