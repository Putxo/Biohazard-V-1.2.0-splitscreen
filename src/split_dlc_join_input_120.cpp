#include <cstdint>

namespace re5::split120 {

struct Session120;
struct DlcLocalJoinUi120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;
    std::uint8_t stateAux;
    std::uint8_t _0016[0x188 - 0x16];
    std::int32_t localJoinAccepted;
    std::int32_t selectedJoinDevice;
};
static_assert(__builtin_offsetof(DlcLocalJoinUi120,state)==0x14,"state");
static_assert(__builtin_offsetof(DlcLocalJoinUi120,localJoinAccepted)==0x188,"accepted");
static_assert(__builtin_offsetof(DlcLocalJoinUi120,selectedJoinDevice)==0x18C,"device");

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern bool __thiscall DetectControllerJoin_79ADA0(std::uint8_t*,int*);
extern bool __thiscall DetectKeyboardJoin_79AE20(std::uint8_t*,int*);
extern bool __thiscall DetectFallbackJoin_79C1E0(std::uint8_t*);
extern void __thiscall RefreshInputMode_7996B0(std::uint8_t*,int);

static inline Session120* Session(){return *reinterpret_cast<Session120**>(gRoot_12340A4+0x1042C);}
static inline int CurrentSelectedPlayer(){
    const int index=*reinterpret_cast<const std::int32_t*>(gPlayerRoot_11B2158+0x20);
    return *reinterpret_cast<const std::int32_t*>(gPlayerRoot_11B2158+index*0x5940+0x273C4);
}
static inline void SetSessionBinding(Session120*s,std::uint32_t off,int value){
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(s)+off)=value;
}

// 0x00A545B6..0x00A54743 -- exact split/local-input branch of the LIN/DE
// frontend update. Produces session+0x654/+0x658 and mKeyboardPlayerNo.
bool UpdateDlcLocalJoinInput_A545B6(DlcLocalJoinUi120*self){
    Session120*session=Session(); int detected=-1;
    if(DetectControllerJoin_79ADA0(gInput_1249C40,&detected)){
        const int preferred=*reinterpret_cast<const int*>(gInput_1249C40+0x5D8);
        SetSessionBinding(session,0x654,preferred);
        self->selectedJoinDevice=detected;
        SetSessionBinding(session,0x658,detected);
        *reinterpret_cast<int*>(gInput_1249C40+0x614)=CurrentSelectedPlayer();
    }else if(DetectKeyboardJoin_79AE20(gInput_1249C40,&detected)){
        const int complement=(detected<=0)?1:0; // native SETLE
        SetSessionBinding(session,0x654,complement);
        self->selectedJoinDevice=detected;
        SetSessionBinding(session,0x658,detected);
        *reinterpret_cast<int*>(gInput_1249C40+0x614)=CurrentSelectedPlayer();
    }else if(DetectFallbackJoin_79C1E0(gInput_1249C40)){
        const int preferred=*reinterpret_cast<const int*>(gInput_1249C40+0x5D8);
        const int complement=(preferred<=0)?1:0; // native SETLE
        self->selectedJoinDevice=complement;
        SetSessionBinding(session,0x654,preferred);
        SetSessionBinding(session,0x658,complement);
        // A54705: CMP 0,selected ; SBB EDX,EDX ; ADD EDX,1.
        // Therefore every nonzero bit pattern (including -1 sentinels) maps
        // to owner 0; only selected==0 maps to owner 1.
        const int selected=CurrentSelectedPlayer();
        *reinterpret_cast<int*>(gInput_1249C40+0x614)=(selected==0)?1:0;
        RefreshInputMode_7996B0(gInput_1249C40,0);
    }else return false;

    self->localJoinAccepted=1;
    self->state=0x0A;
    self->stateAux=0;
    return true;
}

} // namespace re5::split120
