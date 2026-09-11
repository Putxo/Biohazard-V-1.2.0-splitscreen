#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gProfileState_11B2158;
extern std::uint8_t* gSplitState_123457C;
extern int __thiscall QueryGameStatus_C42D90(void* session);
extern bool __thiscall IsNativeLocalCoopActive_C43BB0(const void* session);
extern bool __thiscall PreferredOwnerAllowsCurrentKeyboard_799B50(void* input);
extern int __thiscall ResolveSplitLogicalPlayer_76A1E0(void* split,int index);
extern int __thiscall TranslateInputCommand_7B6E10(void* profile,int command);
extern int __thiscall ResolveTranslatedInput_79A660(void* self,int translated);

static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
static inline int KeyboardPlayer(){return *reinterpret_cast<const int*>(gInput_1249C40+0x614);}

// 0x0079A570..0x0079A614 -- VERIFIED exact command-2/3 owner remapper.
int MapOwnedLocalUiCommand_79A570(int command,int player){
    int ownerCandidate=-1;
    if(QueryGameStatus_C42D90(Session())==1){
        ownerCandidate=(player!=ResolveSplitLogicalPlayer_76A1E0(gSplitState_123457C,0))?1:0;
    }else if(IsNativeLocalCoopActive_C43BB0(Session())){
        const bool preferredOk=PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40);
        ownerCandidate=KeyboardPlayer();
        if(!preferredOk) ownerCandidate^=1;
    }
    if(ownerCandidate>=0 && ownerCandidate!=KeyboardPlayer()) return command;
    if(gInput_1249C40[0x5F0]!=0) return command;
    if(command==2) return 0x2A;
    if(command==3) return 0x32;
    return command;
}

static int TranslateAndResolve(void* self,int original,int profileCommand){
    const int translated=TranslateInputCommand_7B6E10(gProfileState_11B2158,profileCommand);
    if(translated==0) return 0;
    const int resolved=ResolveTranslatedInput_79A660(self,translated);
    return resolved!=0?resolved:original;
}

// 0x0079B8F0..0x0079BBC6 -- VERIFIED against the native 29-entry jump table
// at 0x79BB54. args: command, variant, logicalPlayer; native RET 0x0C.
int __thiscall MapOwnedLocalInputCommand_79B8F0(void* self,int command,int variant,int logicalPlayer){
    int candidate=logicalPlayer;
    if(candidate!=-1 && QueryGameStatus_C42D90(Session())==1){
        candidate=(candidate!=ResolveSplitLogicalPlayer_76A1E0(gSplitState_123457C,0))?1:0;
        if(candidate!=KeyboardPlayer() || gInput_1249C40[0x5F0]==1) return command;
    }else{
        if(IsNativeLocalCoopActive_C43BB0(Session())){
            const int routePlayer=*reinterpret_cast<const int*>(gInput_1249C40+0x618);
            if(routePlayer>=0 && routePlayer!=KeyboardPlayer()) candidate=KeyboardPlayer()^1;
            else candidate=KeyboardPlayer();
            if(candidate>=0 && candidate!=KeyboardPlayer()) return command;
        }
        if(gInput_1249C40[0x5F0]==1) return command;
    }

    if(command==0x1E || command==0x1F) return command;
    switch(command){
    case 1: case 3: if(variant==0) return TranslateAndResolve(self,command,0x0F); return 0;
    case 4: if(variant==0) return TranslateAndResolve(self,command,0x10); return 0;
    case 6: if(variant==0) return TranslateAndResolve(self,command,0x0C); return 0;
    case 9: case 11:
        if(variant==0) return 0x95;
        if(variant==1) return TranslateAndResolve(self,command,0x0F);
        return 0;
    case 12:
        if(variant==0) return 0x95;
        if(variant==1) return TranslateAndResolve(self,command,0x10);
        return 0;
    case 14: case 15:
        if(variant==0) return 0x96;
        if(variant==1) return TranslateAndResolve(self,command,0x0F);
        if(variant==2) return TranslateAndResolve(self,command,0x10);
        return 0;
    case 17: case 19:
        if(variant==0) return 0x96;
        if(variant==1) return TranslateAndResolve(self,command,2);
        if(variant==2) return TranslateAndResolve(self,command,3);
        return 0;
    case 20:
        if(variant==0) return 0x98;
        if(variant==1) return TranslateAndResolve(self,command,2);
        if(variant==2) return TranslateAndResolve(self,command,3);
        return 0;
    case 21: case 26: case 28:
        if(variant==0) return 0x99;
        if(variant==1) return TranslateAndResolve(self,command,2);
        if(variant==2) return TranslateAndResolve(self,command,3);
        return 0;
    case 22: case 24:
        if(variant==0) return 0x97;
        if(variant==1) return TranslateAndResolve(self,command,0x0F);
        return 0;
    case 25:
        if(variant==0) return 0x97;
        if(variant==1) return TranslateAndResolve(self,command,0x10);
        return 0;
    case 27:
        if(variant==0) return 0x99;
        if(variant==1) return TranslateAndResolve(self,command,0);
        if(variant==2) return TranslateAndResolve(self,command,1);
        return 0;
    case 29:
        if(variant==0) return TranslateAndResolve(self,command,0x26);
        return 0;
    default:
        if(variant==0) return TranslateAndResolve(self,command,4);
        return 0;
    }
}

} // namespace re5::split120
