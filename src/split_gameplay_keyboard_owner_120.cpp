#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern std::uint32_t OwnerPressedA_79B340(std::uint8_t*,int);
extern std::uint32_t OwnerPressedB_79B530(std::uint8_t*,int);
extern bool UiRoute5620_7B5620(void*,int,int,int);
extern bool UiRoute5750_7B5750(void*,int,int,int,int);
extern void UiRoute43C0_7B43C0(void*,int,int);
extern void UiCommand4370_7B4370(void*,int,int);
extern bool InputCommand_799B90(std::uint8_t*,int,int);
struct GameplayActor120 { std::uint8_t _0000[0x2D9C]; std::uint8_t playerNo; };
static inline bool KeyboardOwned(const GameplayActor120* a){return int(a->playerNo)==*reinterpret_cast<int*>(gInput_1249C40+0x614);}
// Native gameplay/menu-controller family C699D0..C6D05D repeatedly uses actor+2D9C
// against InputManager+614. These are the exact local-J2 ownership branches; the
// surrounding actor state machines are unrelated gameplay and deliberately out of scope.
std::uint32_t MergeOwnerPressA_C69A2D(const GameplayActor120*a,std::uint32_t base){return KeyboardOwned(a)?(base|OwnerPressedA_79B340(gInput_1249C40,1)):base;}
bool OwnerOnlyRoute_C69C74(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);return true;}
bool OwnerOnlyRoute_C69CD4(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);return true;}
bool OwnerWildcard_C69D21(const GameplayActor120*a,int key){return KeyboardOwned(a)&&UiRoute5620_7B5620(gUiInput_11B20C4,-1,key,0);}
bool OwnerCommand5A_C69DE5(const GameplayActor120*a){return KeyboardOwned(a)&&InputCommand_799B90(gInput_1249C40,0x5A,0);}
bool OwnerCommand58_C69EDD(const GameplayActor120*a){return KeyboardOwned(a)&&InputCommand_799B90(gInput_1249C40,0x58,0);}
bool OwnerGatesWholeHandler_C69F15(const GameplayActor120*a){return KeyboardOwned(a);}
bool OwnerRouteSetup_C6A0BA(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);UiCommand4370_7B4370(gUiInput_11B20C4,0xC9,0);return true;}
bool OwnerRoute7_C6A390(const GameplayActor120*a,int group){return KeyboardOwned(a)&&UiRoute5620_7B5620(gUiInput_11B20C4,group,7,0);}
bool OwnerRoute4360_C6A3D5(const GameplayActor120*a){return KeyboardOwned(a);}
bool OwnerRouteSetup_C6A719(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);return true;}
bool OwnerRouteSetup_C6A79C(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);return true;}
bool OwnerRouteC8_C6A81D(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);return true;}
bool OwnerRouteC8_C6A87D(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);return true;}
std::uint32_t MergeOwnerPressB_C6AA16(const GameplayActor120*a,std::uint32_t base){return KeyboardOwned(a)?(base|OwnerPressedB_79B530(gInput_1249C40,1)):base;}
std::uint32_t MergeOwnerPressB_C6AB89(const GameplayActor120*a,std::uint32_t base){return KeyboardOwned(a)?(base|OwnerPressedB_79B530(gInput_1249C40,1)):base;}
void MergeOwnerPressAB_C6AF58(const GameplayActor120*a,std::uint32_t& aMask,std::uint32_t& bMask){if(!KeyboardOwned(a))return;aMask|=OwnerPressedA_79B340(gInput_1249C40,1);bMask|=OwnerPressedB_79B530(gInput_1249C40,1);}
bool OwnerRoute2Variant1_C6B5C8(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5750_7B5750(gUiInput_11B20C4,1,2,0,0);}
bool OwnerRoute2Variant0_C6B635(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,2,0,0);}
bool OwnerRoute11_7_C6BBA7(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,0x11,7,0);}
void MergeOwnerPressAB_C6BD63(const GameplayActor120*a,std::uint32_t& aMask,std::uint32_t& bMask){if(!KeyboardOwned(a))return;aMask|=OwnerPressedA_79B340(gInput_1249C40,1);bMask|=OwnerPressedB_79B530(gInput_1249C40,1);}
bool OwnerRouteReset_C6BDBE(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);return true;}
bool OwnerRoute0_C6BFF5(const GameplayActor120*a){return KeyboardOwned(a)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,0);}
bool OwnerSpecificBranch_C6C562(const GameplayActor120*a){return KeyboardOwned(a);}
bool OwnerCommandC8_C6C7A0(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);return true;}
bool OwnerCommandC8_C6CCDB(const GameplayActor120*a){if(!KeyboardOwned(a))return false;UiCommand4370_7B4370(gUiInput_11B20C4,0xC8,0);return true;}
constexpr unsigned GameplayKeyboardOwnershipSites(){return 26u;}
} // namespace re5::split120
