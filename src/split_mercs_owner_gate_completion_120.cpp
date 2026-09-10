#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern bool InputCommand_799B90(std::uint8_t*,int,int);
extern bool InputOwnerModeGate_799AF0(void*,int);
extern bool UiRoute5620_7B5620(void*,int,int,int);
static inline int Owner(){return *reinterpret_cast<int*>(gInput_1249C40+0x614);}

// 0x00A43968..0x00A4398F -- exact player-0 exit gate.
bool MercsPrimaryOwnerExitGate_A43986(bool externalTrigger,bool route3){return externalTrigger||(route3&&Owner()==0);}

// 0x00A44022..0x00A4405A -- in the local join UI only logical player 1 may
// consume keyboard command 0x5A in this branch.
bool MercsSecondOwnerAccept_A4402D(){return Owner()==1&&InputCommand_799B90(gInput_1249C40,0x5A,0);}

// 0x00A44C5A..0x00A44C83 -- per-player prompt enable. Native loops EBX over
// the local slots and only the slot equal to mKeyboardPlayerNo may pass the
// global owner-mode gate.
bool MercsPerPlayerPromptEnabled_A44C60(int player){return Owner()==player&&!InputOwnerModeGate_799AF0(gInput_1249C40,-1);}

// 0x00A4A458..0x00A4A480 -- second local input bit in the state-3 poller.
// Only logical player 0 may convert keyboard command 0x58 into bit 0 of the
// routed mask; J2/non-owner does not consume that keyboard event.
std::uint32_t ApplyMercsOwnerCommand58_A4A45E(std::uint32_t mask){if(Owner()==0&&InputCommand_799B90(gInput_1249C40,0x58,0))mask|=1u;return mask;}
} // namespace re5::split120
