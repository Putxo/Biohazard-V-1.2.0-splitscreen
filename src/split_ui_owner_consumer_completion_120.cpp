#include <cstdint>
#include <cstddef>

namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern bool __thiscall SessionHasLocalCoop_C43BB0(void*);
extern int __thiscall QueryGameStatus_C42D90(void*);
extern bool __thiscall UiEventBlocked_728C90(void*);
extern void __thiscall UiCommand4370_7B4370(void*,int,int);
extern void __thiscall UiCommand4480_7B4480(void*,int,int,int);
extern bool __thiscall PreferredOwnerAllowsCurrentKeyboard_799B50(void*);
extern std::uint8_t __thiscall InputOwnerModeGate_799AF0(void*,int);
extern bool __thiscall InputCommand_799B90(std::uint8_t*,int,int);
extern bool __thiscall InputCommandAlt_799C80(std::uint8_t*,int,int);
extern int __thiscall UiCurrentCode_7B4360(void*,int);
extern bool __thiscall UiRoute5750_7B5750(void*,int,int,int,int);
extern int __thiscall UiGetSelection_7B5360(void*,int);
extern bool __thiscall UiSelectionChanged_7B4660(void*,int);
extern int __thiscall UiGetCursor_7B52A0(void*,int);
extern std::uint8_t* gUiManager_1234578;

static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
static inline int KeyboardPlayer(){return *reinterpret_cast<int*>(gInput_1249C40+0x614);}

void RouteLocalCoopUiCommand_A02DC0(void* selfRaw,int command,int arg){
    auto* self=static_cast<std::uint8_t*>(selfRaw);
    if(!SessionHasLocalCoop_C43BB0(Session())){UiCommand4370_7B4370(gUiInput_11B20C4,command,arg);return;}
    auto* keys=reinterpret_cast<const int*>(0x01129190); auto* vals=reinterpret_cast<const int*>(0x01129194);
    for(int i=0;i<11;++i){
        if(command==keys[i*2]){
            const int mapped=vals[i*2];
            UiCommand4370_7B4370(gUiInput_11B20C4,mapped,arg);
            int route=(mapped==0xBC)?0x46:0;
            if(KeyboardPlayer()==1)route+=0x210;
            UiCommand4480_7B4480(gUiInput_11B20C4,route,0,arg);
            return;
        }
    }
    if(command==0x1B || command==0x1D){
        const int wanted=(command==0x1B)?0xB6:0xB7;
        auto* r=self+0x1F6;
        for(int i=0;i<2;++i,r+=0xA64){
            if(r[0]!=0){
                UiCommand4370_7B4370(gUiInput_11B20C4,wanted,arg);
                if(KeyboardPlayer()==1)UiCommand4480_7B4480(gUiInput_11B20C4,0x203,0,arg);
                return;
            }
        }
    }
    UiCommand4370_7B4370(gUiInput_11B20C4,command,arg);
}

// 0x00A02F20..0x00A02F3C -- exact stack-only wrapper, RET 8.
// Native ignores caller ECX, gates through 799B50, then tail-jumps 799B90.
bool __stdcall OwnerFilteredUiCommand_A02F20(int command,int player){
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40) &&
           InputCommand_799B90(gInput_1249C40,command,player);
}

// 0x00A02F40..0x00A02F5C -- exact stack-only wrapper, RET 8.
// Same owner precheck, but successful path tail-jumps 799C80.
bool __stdcall OwnerFilteredUiCommandAlt_A02F40(int command,int player){
    return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40) &&
           InputCommandAlt_799C80(gInput_1249C40,command,player);
}

bool ArbitrateSiblingSelection_A01ABF(std::int8_t recordPlayer,int recordSelection,int siblingSelection,int selected,bool helperNonZero){
    if(!SessionHasLocalCoop_C43BB0(Session())||!helperNonZero)return true;
    if(recordSelection==selected||siblingSelection!=selected)return true;
    return recordPlayer!=KeyboardPlayer();
}

struct OwnerRecord120{std::uint8_t _0[2];std::int8_t player;std::uint8_t _3[0x51];int selection54;};
struct OwnerManager120{std::uint8_t _0[0x5B80];int pending;std::uint8_t changed;};
static_assert(__builtin_offsetof(OwnerRecord120,selection54)==0x54,"owner record selection");
static_assert(__builtin_offsetof(OwnerManager120,pending)==0x5B80,"owner manager pending");
static_assert(__builtin_offsetof(OwnerManager120,changed)==0x5B84,"owner manager changed");

bool UpdateOwnedSelector_A037C0(OwnerManager120* self,OwnerRecord120* rec){
    if(rec->player!=KeyboardPlayer())return false;
    const int code=UiCurrentCode_7B4360(gUiInput_11B20C4,0);
    if(code!=0x1B&&code!=0xDB&&code!=0xDD&&code!=0xB6){self->pending=-1;self->changed=0;return false;}
    const bool sessionState=Session()[0x56C]!=0;
    bool routed;
    if(!sessionState)routed=UiRoute5750_7B5750(gUiInput_11B20C4,0,0,0x11,0);
    else if(rec->player==0)routed=UiRoute5750_7B5750(gUiInput_11B20C4,0,0,8,0);
    else routed=UiRoute5750_7B5750(gUiInput_11B20C4,9,0x11,0,0);
    if(routed)self->pending=UiGetSelection_7B5360(gUiInput_11B20C4,0);
    if(self->pending==-1 || !UiSelectionChanged_7B4660(gUiInput_11B20C4,0)){self->changed=0;self->pending=-1;return false;}
    const int pending=self->pending;
    const int current=UiGetCursor_7B52A0(gUiInput_11B20C4,0);
    if(current==pending)return false;
    rec->selection54=(pending>=9)?pending-9:pending;
    self->changed=1;
    self->pending=-1;
    return true;
}

int OwnerPromptResource_A06A5A(int base,int index){
    const int owner=KeyboardPlayer();
    const bool blocked=InputOwnerModeGate_799AF0(gInput_1249C40,owner)!=0;
    return base+index*2+(blocked?0x11B:0x500);
}

struct UiOwnerBase120{std::uint8_t _0[0x34];int owner34;std::uint8_t _38[0x150];int field188;int field18C;std::uint8_t _190[0x0C];int field19C;};
static inline void CaptureOwner(UiOwnerBase120* s){s->owner34=KeyboardPlayer();}
void CaptureMenuOwner_A23028(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A238B8(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A24112(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A24732(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A24D52(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A253C2(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A25902(UiOwnerBase120*s){CaptureOwner(s);}

bool MenuOwnerEventPredicate_A23090(UiOwnerBase120*self,int eventId){
    if(eventId==1){if(!SessionHasLocalCoop_C43BB0(Session()))return false;return KeyboardPlayer()!=self->owner34;}
    if(eventId==5){if(QueryGameStatus_C42D90(Session())!=1)return false;auto* obj=*reinterpret_cast<void**>(gUiManager_1234578+0x3830);return !UiEventBlocked_728C90(obj);}
    if(eventId==6)return Session()[0x56C]!=0;
    return false;
}
} // namespace re5::split120
