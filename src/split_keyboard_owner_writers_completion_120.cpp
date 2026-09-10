#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern std::uint8_t* gRoot_12340A4;
static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
static inline int& KeyboardPlayer(){return *reinterpret_cast<int*>(gInput_1249C40+0x614);}
static inline int CurrentSelectedPlayer(){const int i=*reinterpret_cast<int*>(gPlayerRoot_11B2158+0x20);return *reinterpret_cast<int*>(gPlayerRoot_11B2158+i*0x5940+0x273C4);}

// 0x00A2EE05..0x00A2EE17 -- VERIFIED completion of the local-player UI state.
// Native copies the currently selected player directly to mKeyboardPlayerNo
// immediately after applying the same selected player to the session binding.
void SyncKeyboardOwnerToSelectedPlayer_A2EE11(){KeyboardPlayer()=CurrentSelectedPlayer();}

// 0x00A398D6..0x00A398EC -- VERIFIED menu transition case: EBX is zero in this
// state-machine path, therefore the transition explicitly hands keyboard/UI
// ownership back to logical player 0.
void ResetKeyboardOwner_A398E6(){KeyboardPlayer()=0;}

// 0x00A4B124..0x00A4B147 -- VERIFIED constructor/init route.
void ResetKeyboardOwner_A4B13D(){KeyboardPlayer()=0;}

// 0x00A4B72A..0x00A4B793 -- VERIFIED frontend init: session+5DC is the native
// current frontend/local slot; the same value becomes mKeyboardPlayerNo.
void SyncKeyboardOwnerToFrontendSlot_A4B78D(){KeyboardPlayer()=*reinterpret_cast<int*>(Session()+0x5DC);}

// 0x00BF28B1..0x00BF28E3 -- VERIFIED session/frontend initialization tail.
// After clearing the per-slot records native resets owner to J1 then binds the
// preferred physical device to that slot via C42A50.
void ResetKeyboardOwner_BF28B7(){KeyboardPlayer()=0;}

// 0x00BF54E1..0x00BF5502 -- VERIFIED activation tail. C42EA0 identifies the
// local row where the activated slot itself becomes keyboard/UI owner.
void SetKeyboardOwnerForActivatedSlot_BF54FC(int slot,bool localRow1){if(localRow1)KeyboardPlayer()=slot;}

// Repeated native transition idiom at BF6D6D/BF6EE9/BF840A/BF895F:
// owner = current selected-player field & 1. This normalizes all values to the
// two logical local players without changing the selected-player source.
void SyncKeyboardOwnerLowBit_BF6D6D(){KeyboardPlayer()=CurrentSelectedPlayer()&1;}
void SyncKeyboardOwnerLowBit_BF6EE9(){KeyboardPlayer()=CurrentSelectedPlayer()&1;}
void SyncKeyboardOwnerLowBit_BF840A(){KeyboardPlayer()=CurrentSelectedPlayer()&1;}
void SyncKeyboardOwnerLowBit_BF895F(){KeyboardPlayer()=CurrentSelectedPlayer()&1;}
} // namespace re5::split120
