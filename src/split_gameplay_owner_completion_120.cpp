#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t InputOwnerModeGate_799AF0(void*,int);
extern void* __thiscall PlayerInteractionObject_B5C450(void* actor);

// 0x008456D7..0x0084572D -- exact local-keyboard interaction gate inside
// parent 0x8454E0. Native requires, in order: actor+2DA8==0, actor logical
// player +2D9C equals InputManager+614, B5C450(actor) returns non-null, and
// 799AF0(input,player)==0. Only then does the surrounding generic interaction
// state evaluation continue.
bool GameplayActorCanEnterKeyboardInteraction_8456D7(void* actorRaw){
    auto* actor=static_cast<std::uint8_t*>(actorRaw);
    if(*reinterpret_cast<const int*>(actor+0x2DA8)!=0)return false;
    const int player=actor[0x2D9C];
    if(player!=*reinterpret_cast<const int*>(gInput_1249C40+0x614))return false;
    if(PlayerInteractionObject_B5C450(actorRaw)==nullptr)return false;
    return InputOwnerModeGate_799AF0(gInput_1249C40,player)==0;
}

extern std::uint8_t* gRoot_12340A4;
extern bool __thiscall SessionHasLocalCoop_C43BB0(void*);
static inline void* Session_(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}
// 0x00B8A51D..0x00B8A551 and 0x00B8CA92..0x00B8CAC2 -- twin gameplay
// local-owner gates. In native local coop, the shared keyboard state is only
// consumed for the actor whose +2D9C equals InputManager+614.
bool GameplayLocalOwnerGate_B8A53E(const void* actorRaw){auto*a=static_cast<const std::uint8_t*>(actorRaw);return !SessionHasLocalCoop_C43BB0(Session_())||a[0x2D9C]==*reinterpret_cast<const int*>(gInput_1249C40+0x614);}
bool GameplayLocalOwnerGate_B8CAB3(const void* actorRaw){return GameplayLocalOwnerGate_B8A53E(actorRaw);}
} // namespace re5::split120
