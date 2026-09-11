#include <cstdint>

namespace re5::split120 {
struct Session120;
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern void* gInputService_11B1DD8;
extern bool UiDeviceReady_7F1390(void*, int, int);
extern bool UiRoute5620_7B5620(void*, int, int, int);
extern bool InputRoute_799B90(std::uint8_t*, int, int);
extern void RefreshMercsPlayerDevice_A43360(void*, int);
extern void RenderMercsLocalDualPlayer_A44BF0(void*);
extern void DestroyCurrentContext_715300(void*);
extern void* ResolveCurrentContext_768BC0(void*, int);
extern void EventServiceAdvance_7F1280(void*);
static inline Session120* Session(){return *reinterpret_cast<Session120**>(gRoot_12340A4+0x1042C);} static inline std::uint8_t* SBytes(Session120* s){return reinterpret_cast<std::uint8_t*>(s);}
using PlayerMaskMapper=std::uint32_t(*)(void*,int);
// A20702..A20739 VERIFIED: local row2 ORs the player-1 mask into the normal session+5DC mask.
std::uint32_t ExpandMercsLocalPlayerMask_A20721(void* owner,PlayerMaskMapper mapper){Session120*s=Session();int mode=*reinterpret_cast<int*>(SBytes(s)+0x58);if(mode!=2&&mode!=5)return 0;int p=*reinterpret_cast<int*>(SBytes(s)+0x5DC);std::uint32_t m=mapper(owner,p);if(IsMercsLocalSplitRow_C42EC0(s))m|=mapper(owner,1);return m;}
// A2CFC8 VERIFIED: local Mercs/Reunion takes the A2D0A8 early path and skips the generic network branch.
bool SkipGenericPauseNetworkBranch_A2CFC8(){return IsMercsLocalSplitRow_C42EC0(Session());}
using SessionPredicate=bool(__thiscall*)(const void*);
// A2F914/A2F929 VERIFIED shared preservation guard for Mercs/Reunion and LIN/DE local split.
bool UseLocalSplitPreservationPath_A2F914(SessionPredicate isDlcLocal){Session120*s=Session();return IsMercsLocalSplitRow_C42EC0(s)||isDlcLocal(s);}
// A42A0D..A42AF5 VERIFIED: player1 can use special resource 0x7CA when +658 exists but its device is unavailable.
bool UseMercsSecondPlayerDevicePrompt_A42A0D(int player){Session120*s=Session();if(!IsMercsLocalSplitRow_C42EC0(s)||player==0)return false;int d=*reinterpret_cast<int*>(SBytes(s)+0x658);return d>=0&&!UiDeviceReady_7F1390(gInputService_11B1DD8,d,0);}
// A432C0..A43313 VERIFIED per-player device substitution.
int ResolveMercsPerPlayerDevice_A432E8(void*self,int requested){auto*b=static_cast<std::uint8_t*>(self);int d=*reinterpret_cast<int*>(gInput_1249C40+0x5D8);int cur=*reinterpret_cast<int*>(b+0x188);if(requested==cur||!IsMercsLocalSplitRow_C42EC0(Session()))return d;int local=*reinterpret_cast<int*>(b+0x6B0);if(local>=0&&UiDeviceReady_7F1390(gInputService_11B1DD8,local,0))d=local;return d;}
// A434EF VERIFIED prompt id: player0 local=0xBF, player0 generic=0x6C, player1=0x6D.
int MercsFrontendPromptId_A434EF(int player){if(player!=0)return 0x6D;return IsMercsLocalSplitRow_C42EC0(Session())?0xBF:0x6C;}
// A4359F VERIFIED local cancel route override.
int MercsCancelRoute_A4359F(int route){return IsMercsLocalSplitRow_C42EC0(Session())?0:(route-2);}
// A439BF VERIFIED local split adds route0 as an extra cancel source.
bool MercsLocalExtraCancel_A439BF(){return IsMercsLocalSplitRow_C42EC0(Session())&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,0);}
// A43BAE VERIFIED: generic helper is wholly suppressed in local split.
bool MercsGenericControlHelperAllowed_A43BAE(){return !IsMercsLocalSplitRow_C42EC0(Session());}
// A43FC8 VERIFIED route override.
int MercsUiRouteAfterLocalOverride_A43FC8(int route){return IsMercsLocalSplitRow_C42EC0(Session())?0:(route-2);}
// A4497B VERIFIED: status-5 auxiliary prompts 29C/298/29D/299 are hidden in local split.
bool DrawMercsStatus5AuxPrompts_A4497B(int status){return status==5&&!IsMercsLocalSplitRow_C42EC0(Session());}
// A44B7E VERIFIED: local split returns before the generic A18130 routing body.
bool MercsA44B70ReturnsImmediately_A44B7E(){return IsMercsLocalSplitRow_C42EC0(Session());}
// A46D42 VERIFIED: local split skips A46150 child creation, refreshes player1, increments +15.
bool HandleMercsLocalChildBypass_A46D42(void*self){if(!IsMercsLocalSplitRow_C42EC0(Session()))return false;RefreshMercsPlayerDevice_A43360(self,1);++*(static_cast<std::uint8_t*>(self)+0x15);return true;}
// A47081 VERIFIED extra generic prompt is omitted locally.
bool DrawMercsNonLocalExtraPrompt_A47081(bool f){return !IsMercsLocalSplitRow_C42EC0(Session())&&f;}
// A47106 VERIFIED dedicated dual-player helper.
bool DispatchMercsLocalDualPlayer_A47106(void*self){if(!IsMercsLocalSplitRow_C42EC0(Session()))return false;RenderMercsLocalDualPlayer_A44BF0(self);return true;}
// A48372 VERIFIED frontend routes current player via InputManager+618.
void RouteMercsFrontendToCurrentPlayer_A48372(void*self){if(IsMercsLocalSplitRow_C42EC0(Session()))*reinterpret_cast<int*>(gInput_1249C40+0x618)=*reinterpret_cast<int*>(static_cast<std::uint8_t*>(self)+0x188);}
// A49E54 VERIFIED shared row0/local context cleanup condition.
bool MercsNeedsContextCleanup_A49E54(bool self40){Session120*s=Session();return !self40&&(IsMercsRow0_C42E80(s)||IsMercsLocalSplitRow_C42EC0(s));}
void CleanupMercsContextIfNeeded_A49E54(bool self40,void*m){if(!MercsNeedsContextCleanup_A49E54(self40))return;DestroyCurrentContext_715300(ResolveCurrentContext_768BC0(m,0));}
// A4A3F7 VERIFIED local input-mask ownership: only keyboardPlayerNo==0 runs 5A/route1 tests; accepted input ORs 0x8000.
std::uint32_t ApplyMercsLocalInputMask_A4A3F7(std::uint32_t mask){if(!IsMercsLocalSplitRow_C42EC0(Session())||*reinterpret_cast<int*>(gInput_1249C40+0x614)!=0)return mask;if(InputRoute_799B90(gInput_1249C40,0x5A,0)||UiRoute5620_7B5620(gUiInput_11B20C4,1,0,0))mask|=0x8000u;return mask;}
// A4A83B VERIFIED local timed transition: state +=3, +16C=0x11.
bool ApplyMercsLocalTimedTransition_A4A83B(void*self){if(!IsMercsLocalSplitRow_C42EC0(Session()))return false;auto*b=static_cast<std::uint8_t*>(self);b[0x14]=static_cast<std::uint8_t>(b[0x14]+3);*reinterpret_cast<int*>(b+0x16C)=0x11;return true;}
// A4AA38 VERIFIED local terminal service transition.
bool AdvanceMercsLocalTerminalService_A4AA38(){if(!IsMercsLocalSplitRow_C42EC0(Session()))return false;EventServiceAdvance_7F1280(gInputService_11B1DD8);return true;}
} // namespace re5::split120
