#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4; extern std::uint8_t* gInput_1249C40; extern void* gPlayerManager_11B27DC;
extern int __thiscall ResolveSplitPlayer_76A1E0(void*,int); extern void* gSplit_123457C;
extern std::uint8_t __thiscall InputOwnerModeGate_799AF0(void*,int);
extern void* __thiscall FindDefaultPlayerActor_7E6290(void*); extern void* __thiscall PlayerPromptObject_B5C430(void*); extern std::uint16_t __thiscall TranslatePlayerPrompt_C6DE10(void*,std::uint16_t,int,int);
static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}

// 0x007D7500..0x007D7615 -- exact local-player routed prompt resolver.
// This explicitly contains direct 799B50 caller 0x007D755F from the native
// 40-caller audit. If C43BB0 is active, 799B50 selects mKeyboardPlayerNo when
// true and the opposite logical player when false before the 799AF0 gate.
std::uint16_t ResolveRoutedPromptCode_7D7500(void* selfRaw,std::uint16_t code,int selection){
 auto*self=static_cast<std::uint8_t*>(selfRaw); if(self[0x2F65C]==0)return code; int owner=-1;
 if(QueryGameStatus_C42D90(Session())==1) owner=(selection!=ResolveSplitPlayer_76A1E0(gSplit_123457C,0));
 else if(IsNativeLocalCoopActive_C43BB0(Session())) owner=PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40)?*reinterpret_cast<int*>(gInput_1249C40+0x614):(*reinterpret_cast<int*>(gInput_1249C40+0x614)^1);
 if(InputOwnerModeGate_799AF0(gInput_1249C40,owner)==0){
   // Native 0x7D7595 loads gPlayerManager directly and calls 0x7E6290; there
   // is deliberately no pre-null check on the global manager pointer here.
   if(void*a=FindDefaultPlayerActor_7E6290(gPlayerManager_11B27DC)) if(void*p=PlayerPromptObject_B5C430(a)) return TranslatePlayerPrompt_C6DE10(p,code,0,0);
   return code;
 }
 if(gInput_1249C40[0x61C]==0||code!=0x0B)return code;
 int pairs=0; if(gInput_1249C40[0x38]&&gInput_1249C40[0x3B])++pairs; if(gInput_1249C40[0x2F8]&&gInput_1249C40[0x2FB])++pairs;
 return pairs==1?std::uint16_t(0x32):code;
}
}
