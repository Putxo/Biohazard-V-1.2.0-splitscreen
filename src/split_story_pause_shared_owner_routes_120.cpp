#include <cstdint>
namespace re5::split120 {
struct PauseOwnerView120 { std::uint8_t _00[0x34]; std::int32_t playerIndex; };
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern bool __thiscall UiRoute5750_7B5750(void*,int,int,int,int);
extern bool __thiscall UiRoute5620_7B5620(void*,int,int,int);
extern int __thiscall UiSelectionStep_7B5900(void*,int,int,int,int,int);
extern int __thiscall UiGetCursor_7B52A0(void*,int);
static inline bool Own(const PauseOwnerView120* s){return s->playerIndex==*reinterpret_cast<std::int32_t*>(gInput_1249C40+0x614);}
// Exact InputManager+0x614 ownership gates from shared Story pause option families.
bool OwnerRoute7_A29DAC(const PauseOwnerView120*s){return Own(s)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,7,0,0);}
bool OwnerRoute8_A29E3D(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,8,0,0);}
bool OwnerWildcard_A2A173(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,-1,0,0);}
bool OwnerRoute1_A2A391(const PauseOwnerView120*s){return Own(s)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,1,0,0);}
bool OwnerRoute4_A2A6DC(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,4,0,0);}
bool OwnerNavUp_A2A754(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1);}
bool OwnerNavDown_A2A78E(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1);}
int OwnerClampSelection_A2A7BB(const PauseOwnerView120*s,int v,int row,int maxv){return Own(s)?UiSelectionStep_7B5900(gUiInput_11B20C4,v,row,maxv,0,0):v;}
bool OwnerRoute1Simple_A2AA81(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,0);}
bool OwnerNavUp4_A2AACA(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,4,1);}
bool OwnerNavDown4_A2AB00(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,4,1);}
int OwnerSelection4_A2AB2F(const PauseOwnerView120*s,int v,int row,int maxv){return Own(s)?UiSelectionStep_7B5900(gUiInput_11B20C4,v,row,maxv,0,0):v;}
bool OwnerHasTwoRoutes_A2AB57(const PauseOwnerView120*s){return Own(s)&&UiGetCursor_7B52A0(gUiInput_11B20C4,1)==2;}
// A2AC1D: exact player-0/player-1 visual branch driven by keyboardPlayerNo.
bool KeyboardOwnerIsPlayer0_A2AC1D(){return *reinterpret_cast<std::int32_t*>(gInput_1249C40+0x614)==0;}
bool OwnerRoute2_A2AF99(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,2,0,0);}
bool OwnerNavUp5_A2AFE1(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1);}
bool OwnerNavDown5_A2B018(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1);}
bool KeyboardOwnerIsPlayer0_A2B102(){return *reinterpret_cast<std::int32_t*>(gInput_1249C40+0x614)==0;}
bool OwnerRoute4B_A2B42F(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,4,0,0);}
bool OwnerNavUpB_A2B4A8(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1);}
bool OwnerNavDownB_A2B4E2(const PauseOwnerView120*s){return Own(s)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1);}
int OwnerSelectionB_A2B50E(const PauseOwnerView120*s,int v,int row,int maxv){return Own(s)?UiSelectionStep_7B5900(gUiInput_11B20C4,v,row,maxv,0,0):v;}
constexpr unsigned SharedStoryPauseOwnershipSiteCount(){return 22u;}
} // namespace re5::split120
