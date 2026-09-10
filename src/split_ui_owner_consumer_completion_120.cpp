#include <cstdint>
#include <cstddef>

namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern bool SessionHasLocalCoop_C43BB0(void*);
extern int SessionDisplayStatus_C42D90(void*);
extern bool UiEventBlocked_728C90(void*);
extern void UiCommand4370_7B4370(void*,int,int);
extern void UiCommand4480_7B4480(void*,const int*,int);
extern bool PreferredOwnerAllowsCurrentKeyboard_799B50(void*);
extern std::uint8_t InputOwnerModeGate_799AF0(void*,int);
extern bool InputCommand_799B90(std::uint8_t*,int,int);
extern bool InputCommandAlt_799C80(std::uint8_t*,int,int);
extern int UiCurrentCode_7B4360(void*,int);
extern bool UiRoute5750_7B5750(void*,int,int,int,int);
extern int UiGetSelection_7B5360(void*,int);
extern bool UiSelectionChanged_7B4660(void*,int);
extern int UiGetCursor_7B52A0(void*,int);
extern std::uint8_t* gUiManager_1234578;
static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
static inline int KeyboardPlayer(){return *reinterpret_cast<int*>(gInput_1249C40+0x614);}
void RouteLocalCoopUiCommand_A02DC0(void* selfRaw,int command,int arg){
    auto* self=static_cast<std::uint8_t*>(selfRaw);
    if(!SessionHasLocalCoop_C43BB0(Session())){UiCommand4370_7B4370(gUiInput_11B20C4,command,arg);return;}
    auto* keys=reinterpret_cast<const int*>(0x01129190); auto* vals=reinterpret_cast<const int*>(0x01129194);
    for(int i=0;i<11;++i){if(command==keys[i*2]){const int mapped=vals[i*2];UiCommand4370_7B4370(gUiInput_11B20C4,mapped,arg);int route=(mapped==0xBC)?0x46:0;if(KeyboardPlayer()==1)route+=0x210;const int pair[2]={route,0};UiCommand4480_7B4480(gUiInput_11B20C4,pair,arg);return;}}
    if(command==0x1B || command==0x1D){const int wanted=(command==0x1B)?0xB6:0xB7;auto* r=self+0x1F6;for(int i=0;i<2;++i,r+=0xA64){if(r[0]!=0){UiCommand4370_7B4370(gUiInput_11B20C4,wanted,arg);if(KeyboardPlayer()==1){const int pair[2]={0x203,0};UiCommand4480_7B4480(gUiInput_11B20C4,pair,arg);} return;}}}
    UiCommand4370_7B4370(gUiInput_11B20C4,command,arg);
}
bool OwnerFilteredUiCommand_A02F20(int command,int player){return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40)&&InputCommand_799B90(gInput_1249C40,command,player);}
bool OwnerFilteredUiCommandAlt_A02F40(int command,int player){return PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40)&&InputCommandAlt_799C80(gInput_1249C40,command,player);}
bool ArbitrateSiblingSelection_A01ABF(std::int8_t recordPlayer,int recordSelection,int siblingSelection,int selected,bool helperNonZero){if(!SessionHasLocalCoop_C43BB0(Session())||!helperNonZero)return true;if(recordSelection==selected||siblingSelection!=selected)return true;return recordPlayer!=KeyboardPlayer();}
struct OwnerRecord120{std::uint8_t _0[2];std::int8_t player;std::uint8_t _3[0x51];int selection54;};
struct OwnerManager120{std::uint8_t _0[0x5B80];int pending;std::uint8_t changed;};
bool UpdateOwnedSelector_A037C0(OwnerManager120* self,OwnerRecord120* rec){
    if(rec->player!=KeyboardPlayer())return false;const int code=UiCurrentCode_7B4360(gUiInput_11B20C4,0);if(code!=0x1B&&code!=0xDB&&code!=0xDD&&code!=0xB6){self->pending=-1;self->changed=0;return false;}const bool state=Session()[0x56C]!=0;bool route;if(!state)route=UiRoute5750_7B5750(gUiInput_11B20C4,0,0,0x11,0);else if(rec->player==0)route=UiRoute5750_7B5750(gUiInput_11B20C4,0,0,8,0);else route=UiRoute5750_7B5750(gUiInput_11B20C4,9,0x11,0,0);if(route)self->pending=UiGetSelection_7B5360(gUiInput_11B20C4,0);bool changed=false;if(self->pending!=-1&&UiSelectionChanged_7B4660(gUiInput_11B20C4,0)){const int current=UiGetCursor_7B52A0(gUiInput_11B20C4,0);if(current!=self->pending){changed=true;rec->selection54=(self->pending>=9)?self->pending-9:self->pending;}}self->changed=changed?1u:0u;self->pending=-1;return changed;
}
int OwnerPromptResource_A06A5A(int base,int index){const int owner=KeyboardPlayer();const bool blocked=InputOwnerModeGate_799AF0(gInput_1249C40,owner)!=0;return base+index*2+(blocked?0x11B:0x500);}
struct UiOwnerBase120{std::uint8_t _0[0x34];int owner34;std::uint8_t _38[0x150];int field188;int field18C;std::uint8_t _190[0x0C];int field19C;};
static inline void CaptureOwner(UiOwnerBase120* s){s->owner34=KeyboardPlayer();}
void CaptureMenuOwner_A23028(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A238B8(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A24112(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A24732(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A24D52(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A253C2(UiOwnerBase120*s){CaptureOwner(s);} void CaptureMenuOwner_A25902(UiOwnerBase120*s){CaptureOwner(s);}
bool MenuOwnerEventPredicate_A23090(UiOwnerBase120*self,int eventId){if(eventId==1){if(!SessionHasLocalCoop_C43BB0(Session()))return false;return KeyboardPlayer()!=self->owner34;}if(eventId==5){if(SessionDisplayStatus_C42D90(Session())!=1)return false;auto* obj=*reinterpret_cast<void**>(gUiManager_1234578+0x3830);return !UiEventBlocked_728C90(obj);}if(eventId==6)return Session()[0x56C]!=0;return false;}
} // namespace re5::split120
