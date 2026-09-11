#include <cstdint>
namespace re5::split120 {

extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gProfileState_11B2158;
extern void* gUiInput_11B20C4;
extern std::uint32_t __thiscall OwnerPressedA_79B340(std::uint8_t*,int);
extern std::uint32_t __thiscall OwnerPressedB_79B530(std::uint8_t*,int);
extern int __thiscall TranslateInputCommand_7B6E10(void*,int);
extern bool __thiscall UiRoute5620_7B5620(void*,int,int,int);
extern bool __thiscall UiRoute5750_7B5750(void*,int,int,int,int);
extern void __thiscall UiRoute43C0_7B43C0(void*,int,int);
extern void __thiscall UiCommand4370_7B4370(void*,int,int);
extern bool __thiscall InputCommand_799B90(std::uint8_t*,int,int);

struct GameplayActor120 { std::uint8_t _0000[0x2D9C]; std::uint8_t playerNo; };
struct GameplayUiState120 {
    std::uint8_t _00[0xC8];
    std::int32_t activeUiRoute; // +C8
    std::int32_t _CC;
    std::int32_t routeGroupD0; // +D0
};
static_assert(__builtin_offsetof(GameplayUiState120,activeUiRoute)==0xC8,"gameplay ui route");
static_assert(__builtin_offsetof(GameplayUiState120,routeGroupD0)==0xD0,"route group");
static inline bool KeyboardOwned(const GameplayActor120* a){return int(a->playerNo)==*reinterpret_cast<const int*>(gInput_1249C40+0x614);}

// C699D0..C6D05D repeatedly gates keyboard/UI work with actor+2D9C ==
// InputManager+614. The helpers below reconstruct every instruction whose
// execution is specifically controlled by that ownership comparison; generic
// gameplay state-machine work after the gate remains in its parent subsystem.

// 0xC69A2D owner-only merge into the parent's physical input mask.
std::uint32_t MergeOwnerPressA_C69A2D(const GameplayActor120*a,std::uint32_t base){
    return KeyboardOwned(a)?(base|OwnerPressedA_79B340(gInput_1249C40,1)):base;
}

// 0xC69C60..0xC69CB3 -- exact thiscall helper, ret 0x0C.
// args: actor, command, parameter. Non-owner returns false. Owner resets the
// shared UI route, clears self+C8, emits the caller-supplied command and returns true.
bool __thiscall OwnerUiCommand_C69C60(GameplayUiState120*self,
                                      const GameplayActor120*actor,
                                      int command,int parameter){
    if(!KeyboardOwned(actor))return false;
    UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);
    self->activeUiRoute=-1;
    UiCommand4370_7B4370(gUiInput_11B20C4,command,parameter);
    return true;
}

// 0xC69CC0..0xC69CFE -- exact thiscall helper, ret 4.
bool __thiscall OwnerUiReset_C69CC0(GameplayUiState120*self,const GameplayActor120*actor){
    if(!KeyboardOwned(actor))return false;
    UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);
    self->activeUiRoute=-1;
    return true;
}

// 0xC69D10..0xC69D7B -- exact owner-aware route dispatcher, ret 0x10.
// actor is arg0; a/b/c are the three route selectors used by native code.
bool __thiscall OwnerUiRouteDispatch_C69D10(void*,const GameplayActor120*actor,
                                            int a,int b,int c){
    if(!KeyboardOwned(actor))return false;
    if(a<0)return UiRoute5620_7B5620(gUiInput_11B20C4,-1,c,0);
    if(b<0)return UiRoute5620_7B5620(gUiInput_11B20C4,a,c,0);
    return UiRoute5750_7B5750(gUiInput_11B20C4,a,b,c,0);
}

bool OwnerCommand5A_C69DE5(const GameplayActor120*a){return KeyboardOwned(a)&&InputCommand_799B90(gInput_1249C40,0x5A,0);}
bool OwnerCommand58_C69EDD(const GameplayActor120*a){return KeyboardOwned(a)&&InputCommand_799B90(gInput_1249C40,0x58,0);}

// 0xC69F15 is an ownership branch into the remainder of a generic handler.
// This predicate is the complete local-J2-specific semantic at that branch.
bool OwnerGatesWholeHandler_C69F15(const GameplayActor120*a){return KeyboardOwned(a);}

// 0xC6A0BA..0xC6A0F2 -- exact owner-conditioned prologue of the large UI
// geometry helper. The following rectangle construction is generic once entered.
bool OwnerRouteSetup_C6A0BA(GameplayUiState120*self,const GameplayActor120*a){
    if(!KeyboardOwned(a))return false;
    UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);
    self->activeUiRoute=-1;
    UiCommand4370_7B4370(gUiInput_11B20C4,0xC9,0);
    return true;
}

// 0xC6A380..0xC6A3BC -- exact helper, ret 8. If the owning actor's routed
// query succeeds, native ORs 0x200 directly into the caller output mask.
void __thiscall MergeOwnerRoute7_C6A380(GameplayUiState120*self,
                                        const GameplayActor120*a,
                                        std::uint32_t* outMask){
    if(!KeyboardOwned(a))return;
    if(UiRoute5620_7B5620(gUiInput_11B20C4,self->routeGroupD0,7,0))
        *outMask|=0x200u;
}

// C6A3D5 gates an otherwise generic owner-only menu handler.
bool OwnerRoute4360_C6A3D5(const GameplayActor120*a){return KeyboardOwned(a);}

// Exact repeated owner-reset idiom at C6A719, C6A79C and C6BDBE.
static bool OwnerResetRouteAndC8(GameplayUiState120*self,const GameplayActor120*a){
    if(!KeyboardOwned(a))return false;
    UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);
    self->activeUiRoute=-1;
    return true;
}
bool OwnerRouteSetup_C6A719(GameplayUiState120*self,const GameplayActor120*a){return OwnerResetRouteAndC8(self,a);}
bool OwnerRouteSetup_C6A79C(GameplayUiState120*self,const GameplayActor120*a){return OwnerResetRouteAndC8(self,a);}
bool OwnerRouteReset_C6BDBE(GameplayUiState120*self,const GameplayActor120*a){return OwnerResetRouteAndC8(self,a);}

// Exact repeated C8-command setup at C6A81D/C6A87D. The parent performs its
// own state writes before/after this owner-conditioned block.
static bool OwnerResetRouteCommandC8(GameplayUiState120*self,const GameplayActor120*a){
    if(!KeyboardOwned(a))return false;
    UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);
    self->activeUiRoute=-1;
    UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);
    return true;
}
bool OwnerRouteC8_C6A81D(GameplayUiState120*self,const GameplayActor120*a){return OwnerResetRouteCommandC8(self,a);}
bool OwnerRouteC8_C6A87D(GameplayUiState120*self,const GameplayActor120*a){return OwnerResetRouteCommandC8(self,a);}

std::uint32_t MergeOwnerPressB_C6AA16(const GameplayActor120*a,std::uint32_t base){return KeyboardOwned(a)?(base|OwnerPressedB_79B530(gInput_1249C40,1)):base;}
std::uint32_t MergeOwnerPressB_C6AB89(const GameplayActor120*a,std::uint32_t base){return KeyboardOwned(a)?(base|OwnerPressedB_79B530(gInput_1249C40,1)):base;}
void MergeOwnerPressAB_C6AF58(const GameplayActor120*a,std::uint32_t& aMask,std::uint32_t& bMask){if(!KeyboardOwned(a))return;aMask|=OwnerPressedA_79B340(gInput_1249C40,1);bMask|=OwnerPressedB_79B530(gInput_1249C40,1);}

// These queries are reached only after the parent's physical-mask path failed.
bool OwnerRoute2Variant1_C6B5C8(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5750_7B5750(gUiInput_11B20C4,1,2,0,0);}
bool OwnerRoute2Variant0_C6B635(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,2,0,0);}

// 0xC6BBA7 owner-specific query; parent additionally tests self+CC before
// accepting the result, which is generic state not caused by +614 itself.
bool OwnerRoute11_7_C6BBA7(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,0x11,7,0);}

// 0xC6BD63 owner-only merge of both shared keyboard masks into the two parent
// masks. Native uses variant 1 at this site.
void MergeOwnerPressAB_C6BD63(const GameplayActor120*a,std::uint32_t& aMask,std::uint32_t& bMask){
    if(!KeyboardOwned(a))return;
    aMask|=OwnerPressedA_79B340(gInput_1249C40,1);
    bMask|=OwnerPressedB_79B530(gInput_1249C40,1);
}

// 0xC6BFF5 owner-only zero/zero route query after the physical +5B4 path.
bool OwnerRoute0_C6BFF5(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,0);}

// 0xC6C562..0xC6C5BA -- exact owner-conditioned mask augmentation. Native
// selects variant 0 only when profile command 0x0C translates to 0x63 or 0x62;
// otherwise variant 1. Both A/B owner masks are then merged.
void MergeTranslatedOwnerPressAB_C6C562(const GameplayActor120*a,
                                        std::uint32_t& aMask,
                                        std::uint32_t& bMask){
    if(!KeyboardOwned(a))return;
    int variant=1;
    int translated=TranslateInputCommand_7B6E10(gProfileState_11B2158,0x0C);
    if(translated==0x63)
        variant=0;
    else if(TranslateInputCommand_7B6E10(gProfileState_11B2158,0x0C)==0x62)
        variant=0;
    aMask|=OwnerPressedA_79B340(gInput_1249C40,variant);
    bMask|=OwnerPressedB_79B530(gInput_1249C40,variant);
}

// C6C7A0 and C6CCDB: exact owner-conditioned UI C8 command emission.
bool OwnerCommandC8_C6C7A0(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);return true;}
bool OwnerCommandC8_C6CCDB(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);return true;}

constexpr unsigned GameplayKeyboardOwnershipSites(){return 26u;}
} // namespace re5::split120
