#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void* gPlayerManager_11B27DC;
extern bool SessionHasLocalCoop_C43BB0(void* session);
extern void* FindPlayerActorByNo_7E6220(void* manager,int playerNo);
extern void* FindDefaultPlayerActor_7E6290(void* manager);

static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}

// 0x0079BC53..0x0079BCDB -- VERIFIED local-coop-specific head of 0x79BC50.
// In local coop the selected keyboard player (+614) first chooses the player
// actor; if no actor is available, the same logical player is translated to
// its native per-slot input device through session+0x490[player]. Outside local
// coop the original preferred device +0x5D8 is used.
int ResolveSelectedLocalDevice_79BC53(void* selfRaw){
    auto*self=static_cast<std::uint8_t*>(selfRaw);self[0x61C]=0;
    const bool local=SessionHasLocalCoop_C43BB0(Session());
    void* actor=local?FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,*reinterpret_cast<int*>(gInput_1249C40+0x614))
                     :FindDefaultPlayerActor_7E6290(gPlayerManager_11B27DC);
    if(actor)return static_cast<int>(static_cast<std::uint8_t*>(actor)[0x2DB1]);
    if(local){const int player=*reinterpret_cast<int*>(gInput_1249C40+0x614);return *reinterpret_cast<int*>(Session()+0x490+player*4);}
    return *reinterpret_cast<int*>(self+0x5D8);
}

// 0x00715495..0x007154B2 -- VERIFIED aGame initialization ownership reset.
// Session type 6 preserves the current owner; every other session resets
// keyboard ownership to logical player 0 before the local-coop state machine.
void ResetKeyboardPlayerOnAGameInit_715495(){
    if(*reinterpret_cast<const int*>(Session()+0x18)!=6)*reinterpret_cast<int*>(gInput_1249C40+0x614)=0;
}

} // namespace re5::split120
