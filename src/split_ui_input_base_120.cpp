#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40; extern void* gUiInput_11B20C4; extern std::uint8_t* gRoot_12340A4;
extern std::uint32_t ReadRoutedInputMaskA_9E3420(int,int); extern bool InputCommand_799B90(std::uint8_t*,int,int);
extern bool UiConfirmFallback_7B63C0(void*,int,int,int); extern int UiSelection_7B5870(void*,int,int,int,int);
extern void UiCommand4370_7B4370(void*,int,int); extern void UiRoute43C0_7B43C0(void*,int,int); extern bool UiRoute5620_7B5620(void*,int,int,int); extern bool UiRoute5750_7B5750(void*,int,int,int,int);
struct UiBase120{std::uint8_t _00[0x2c];int xOffset;int yOffset;int playerNo;std::uint8_t _38[0x188-0x38];int device;int context;};
static inline bool Own(const UiBase120*s){return s->playerNo==*reinterpret_cast<int*>(gInput_1249C40+0x614);}
// A22BB0 VERIFIED constructor: captures current keyboard player as owner +34.
void InitSplitUiBase_A22BB0(UiBase120*s){s->device=0;s->context=0;s->xOffset=0;s->yOffset=0;s->playerNo=*reinterpret_cast<int*>(gInput_1249C40+0x614);}
// A22DF0 VERIFIED: only owning player emits command and split input transform.
bool EmitOwnedUiCommand_A22DF0(UiBase120*s,int command,int param){if(!Own(s))return false;UiCommand4370_7B4370(gUiInput_11B20C4,command,param);return true;}
// A22E70 VERIFIED: route reset exists only for owning player.
bool ResetOwnedUiRoutes_A22E70(UiBase120*s){if(!Own(s))return false;UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);UiRoute43C0_7B43C0(gUiInput_11B20C4,1,1);UiRoute43C0_7B43C0(gUiInput_11B20C4,1,2);return true;}
// A22EC0 VERIFIED generic owner-routed UI query.
bool QueryOwnedUiRoute_A22EC0(UiBase120*s,int a,int b,int c,int d){if(!Own(s))return false;if(a<0)return UiRoute5620_7B5620(gUiInput_11B20C4,-1,c,d);if(b<0)return UiRoute5620_7B5620(gUiInput_11B20C4,a,c,d);return UiRoute5750_7B5750(gUiInput_11B20C4,a,b,c,d);}
// A22F30 VERIFIED: controller/device mask first, then keyboard-owner command 0x5A fallback.
bool OwnedUiAccept_A22F30(UiBase120*s){if((ReadRoutedInputMaskA_9E3420(s->device,s->playerNo)&0x8000u)!=0)return true;return Own(s)&&InputCommand_799B90(gInput_1249C40,0x5A,0);}
// A22F70 VERIFIED: routed mask + global confirm bits, then owner-only UI fallback.
bool OwnedUiConfirm_A22F70(UiBase120*s){std::uint32_t m=ReadRoutedInputMaskA_9E3420(s->device,s->playerNo);if((*reinterpret_cast<std::uint32_t*>(gInput_1249C40+0x5B8)&m)!=0)return true;return Own(s)&&UiConfirmFallback_7B63C0(gUiInput_11B20C4,-1,0,0);}
// A22FC0 VERIFIED: non-owner preserves caller value; owner delegates to 7B5870.
int OwnedUiSelection_A22FC0(UiBase120*s,int fallback,int a,int b,int c){if(!Own(s))return fallback;return UiSelection_7B5870(gUiInput_11B20C4,fallback,a,b,c);}
} // namespace re5::split120
