#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

extern void __thiscall InputPrepare_7965E0(void* inputState);
extern void __thiscall UiPlayerJoinBegin_7BA180(void* ui,int zero,int one);
extern void __thiscall UiPlayerJoinReset_7BA1F0(void* ui,int zero);
extern void __thiscall AudioOrUiJoinPrepare_7A40D0(void* obj);
extern void __thiscall BeginState13Delay_716DB0(AGame120* self,float seconds);
extern bool State13Ready_71DF70();
extern void __thiscall ClearRootStateFlags_715A30(void* root,std::uint32_t mask);
extern void __thiscall OrRootStateFlags_726120(void* root,std::uint32_t mask);
extern void __thiscall ResetJoinTransition_723870(AGame120* self,int value);
extern void __thiscall BindPlayerToUi_7B8370(void* ui,int slot,int device);
extern void __thiscall SetJoinUiSelection_7F12C0(void* obj,int value);
extern void __thiscall RefreshJoinUi_7B7FC0(void* ui);

struct AGameState13View120 {
    std::uint8_t _00[0x06];
    std::uint8_t substate;   // +0x06
    std::uint8_t subflag;    // +0x07
    std::uint8_t _08[0x350 - 0x08];
    std::int32_t device;     // +0x350
    std::int32_t slot;       // +0x354
};
static_assert(__builtin_offsetof(AGameState13View120,device)==0x350,"device offset");
static_assert(__builtin_offsetof(AGameState13View120,slot)==0x354,"slot offset");

static bool State13ExternalGateBusy_725B8D(){
    void* gate=*reinterpret_cast<void**>(0x011E8804);
    auto** vt=*reinterpret_cast<void***>(gate);
    using Fn=int(__thiscall*)(void*);
    return reinterpret_cast<Fn>(vt[0x38/sizeof(void*)])(gate)!=0;
}

static void DispatchState13JoinEvent_725C6C(){
    void* mgr=*reinterpret_cast<void**>(0x01234578);
    auto** vt=*reinterpret_cast<void***>(mgr);
    using Fn=void(__thiscall*)(void*,const void*);
    reinterpret_cast<Fn>(vt[0x24/sizeof(void*)])(mgr,reinterpret_cast<const void*>(0x011AFF64));
}

// 0x00725B20..0x00725C7F -- exact Add Player state-13 handler.
// ECX=AGame, no stack arguments, four substates selected by byte +0x06.
void __thiscall UpdateAddPlayerState13_725B20(AGame120* opaque)
{
    auto* self=reinterpret_cast<AGameState13View120*>(opaque);

    switch(self->substate){
    case 0: {
        void* inputState=*reinterpret_cast<void**>(0x01249C1C);
        if(*reinterpret_cast<std::uint8_t*>(static_cast<std::uint8_t*>(inputState)+0x131C)!=0)
            return;
        InputPrepare_7965E0(inputState);
        void* ui=*reinterpret_cast<void**>(0x011B2158);
        UiPlayerJoinBegin_7BA180(ui,0,1);
        UiPlayerJoinReset_7BA1F0(ui,0);
        AudioOrUiJoinPrepare_7A40D0(*reinterpret_cast<void**>(0x011B1D0C));
        BeginState13Delay_716DB0(opaque,*reinterpret_cast<const float*>(0x010BB498)); // 0.5f
        ++self->substate;
        return;
    }

    case 1:
        if(State13ExternalGateBusy_725B8D())
            return;
        ++self->substate;
        self->subflag=0;
        return;

    case 2:
        if(!State13Ready_71DF70())
            return;
        ++self->substate;
        return;

    case 3: {
        void* root=*reinterpret_cast<void**>(0x012340A4);
        ClearRootStateFlags_715A30(root,0x00004000u);
        ClearRootStateFlags_715A30(root,0x08000000u);
        ClearRootStateFlags_715A30(root,0x00000001u);
        ClearRootStateFlags_715A30(root,0x00000010u);
        OrRootStateFlags_726120(root,0x00040000u);

        ResetJoinTransition_723870(opaque,0);
        // Native 0x725C21 loads ECX=AGame before calling 0x716720.
        SetLocalPlayerActive_716720(opaque,true,self->slot,self->device);

        void* ui=*reinterpret_cast<void**>(0x011B2158);
        BindPlayerToUi_7B8370(ui,self->slot,self->device);
        SetJoinUiSelection_7F12C0(*reinterpret_cast<void**>(0x011B1DD8),-1);
        RefreshJoinUi_7B7FC0(ui);

        auto* rootBytes=static_cast<std::uint8_t*>(root);
        auto* session=*reinterpret_cast<std::uint8_t**>(rootBytes+0x1042C);
        *reinterpret_cast<std::int32_t*>(session+0x24)=2;
        DispatchState13JoinEvent_725C6C();
        return;
    }

    default:
        return;
    }
}

} // namespace re5::split120
