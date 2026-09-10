#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gProfileState_11B2158;
extern std::uint8_t* gRoot_12340A4;

extern bool SessionHasLocalCoop_C43BB0(void* session);
extern int TranslateInputCommand_7B6E10(void* profile,int command);
extern bool QueryInputA_799FB0(void* input,int command);
extern bool QueryInputB_79A090(void* input,int command);
extern bool QueryInputC_79A170(void* input,int command);
extern bool QueryInputD_79A240(void* input,int command);
extern bool QueryInputE_79A310(void* input,int command);

static inline int KeyboardPlayer(){return *reinterpret_cast<const int*>(gInput_1249C40+0x614);}
static inline bool OwnerOrGlobal(int player){return player<0||player==KeyboardPlayer();}
static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}

// 0x00799AF0..0x00799B08 -- VERIFIED.
// Non-owner is hard-blocked (1). Owner/global follows +5F0 exactly.
std::uint8_t InputOwnerModeGate_799AF0(void* inputRaw,int player){auto* in=static_cast<std::uint8_t*>(inputRaw);if(player>=0&&player!=*reinterpret_cast<int*>(in+0x614))return 1;return in[0x5F0];}

// 0x00799B30..0x00799B4B -- VERIFIED.
void ClearPreferredOwner_799B30(void* inputRaw,int player){auto* in=static_cast<std::uint8_t*>(inputRaw);int& p=*reinterpret_cast<int*>(in+0x618);if(player==-1||player==p)p=-1;}

// 0x00799B50..0x00799B89 -- VERIFIED.
bool PreferredOwnerAllowsCurrentKeyboard_799B50(void* inputRaw){auto* in=static_cast<std::uint8_t*>(inputRaw);if(!SessionHasLocalCoop_C43BB0(Session()))return true;const int p=*reinterpret_cast<int*>(in+0x618);return p<0||p==KeyboardPlayer();}

static bool RoutedOwnerQuery(int command,int player,bool(*fn)(void*,int)){
    if(!OwnerOrGlobal(player)||gInput_1249C40[0x5F0]==1)return false;
    int translated=TranslateInputCommand_7B6E10(gProfileState_11B2158,command);
    return fn(gInput_1249C40,translated);
}

// 0x0079A3E0..0x0079A560 -- VERIFIED five native owner-gated wrappers.
bool OwnerInputQueryA_79A3E0(int command,int player){return RoutedOwnerQuery(command,player,QueryInputA_799FB0);}
bool OwnerInputQueryB_79A430(int command,int player){return RoutedOwnerQuery(command,player,QueryInputB_79A090);}
bool OwnerInputQueryC_79A480(int command,int player){return RoutedOwnerQuery(command,player,QueryInputC_79A170);}
bool OwnerInputQueryD_79A4D0(int command,int player){return RoutedOwnerQuery(command,player,QueryInputD_79A240);}
bool OwnerInputQueryE_79A520(int command,int player){return RoutedOwnerQuery(command,player,QueryInputE_79A310);}

} // namespace re5::split120
