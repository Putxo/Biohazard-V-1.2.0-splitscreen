#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern bool InputOwnerModeGate_799AF0(void*,int);
// 0x008456EE..0x0084574F -- VERIFIED gameplay owner gate inside 0x8454E0.
// Only the actor whose logical player number (+2D9C) equals mKeyboardPlayerNo
// can enter the keyboard-owned interaction path. 0x799AF0 must then report
// unblocked before player state is evaluated.
bool GameplayActorOwnsKeyboard_8456EE(const void* actorRaw){
    auto* actor=static_cast<const std::uint8_t*>(actorRaw);
    if(*reinterpret_cast<const int*>(actor+0x2DA8)!=0)return false;
    const int player=actor[0x2D9C];
    if(player!=*reinterpret_cast<const int*>(gInput_1249C40+0x614))return false;
    return !InputOwnerModeGate_799AF0(gInput_1249C40,player);
}

extern std::uint8_t* gRoot_12340A4;
extern bool SessionHasLocalCoop_C43BB0(void*);
static inline void* Session_(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}
// 0x00B8A51D..0x00B8A551 and 0x00B8CA92..0x00B8CAC2 -- VERIFIED twin
// gameplay gates. In native local coop, an actor may clear/use the shared
// keyboard mode byte +0x600 only when actor+0x2D9C owns input+0x614.
bool GameplayLocalOwnerGate_B8A53E(const void* actorRaw){auto*a=static_cast<const std::uint8_t*>(actorRaw);return !SessionHasLocalCoop_C43BB0(Session_())||a[0x2D9C]==*reinterpret_cast<const int*>(gInput_1249C40+0x614);}
bool GameplayLocalOwnerGate_B8CAB3(const void* actorRaw){return GameplayLocalOwnerGate_B8A53E(actorRaw);}
} // namespace re5::split120
