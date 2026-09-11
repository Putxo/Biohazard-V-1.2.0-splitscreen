#include <cstdint>
namespace re5::split120 {

#pragma pack(push,1)
struct StoryPauseRecordState7View120 {
    std::uint8_t active;             // +00
    std::uint8_t _01;
    std::uint8_t routeChanged;       // +02
    std::uint8_t _03;
    std::uint8_t changed;            // +04
    std::uint8_t _05[3];
    std::int32_t playerIndex;        // +08
    std::int32_t device;             // +0C
    std::uint32_t routedMask;        // +10
    std::uint32_t inputMask;         // +14
    std::uint8_t _18[0x34-0x18];
    std::int32_t state;              // +34
    std::int32_t previousState;      // +38
    std::uint8_t _3C[0x58-0x3C];
    std::uint8_t substate;           // +58
    std::uint8_t aux;                // +59
    std::uint8_t _5A[2];
    float timer;                      // +5C
    std::int32_t result;             // +60
    std::uint8_t _64[4];
};
#pragma pack(pop)
static_assert(sizeof(StoryPauseRecordState7View120)==0x68,"state7 record stride");

extern void __thiscall UiConfirmSound_9E4A90(void* owner);
extern void __thiscall UiCancelSound_9E4A70(void* owner);
extern void* __thiscall ResolveCurrentContext_768C10(void* manager);
extern void __thiscall SplitLocalTransition_76B520(void* split);
extern void __thiscall UpdateSplitViewportState_76CC60(void* split);
extern void __thiscall SetInputTransition_794FF0(void* inputState,int value);
extern void __thiscall RefreshLocalJoinOwner_7F1280(void* owner);
extern void __thiscall FinalizeCurrentContext_723420(void* context);

static inline float StoryFrameDelta120(){
    auto* frame=*reinterpret_cast<std::uint8_t**>(0x011B209C);
    return *reinterpret_cast<float*>(frame+0x28);
}
static inline void* StoryContextManager120(){return *reinterpret_cast<void**>(0x01234578);}

// 0x00A2E770..0x00A2E959 -- VERIFIED complete Story pause state 7.
// The outer state dispatcher at 0xA306AC maps state index 7 to this routine.
// Native ABI: ECX=owner, one stack argument=record, RET 4.
void __thiscall UpdateStoryPauseState7_A2E770(void* owner,StoryPauseRecordState7View120* r){
    switch(r->substate){
    case 0:
        r->timer=*reinterpret_cast<const float*>(0x010BCA6C); // 0.25f
        ++r->substate;
        return;

    case 1:
        if(r->timer>0.0f){
            r->timer-=StoryFrameDelta120();
            if(r->timer>0.0f)return;
            r->timer=0.0f;
            return; // native waits one extra update before reading input
        }
        {
            auto* input=*reinterpret_cast<std::uint8_t**>(0x01249C40);
            if((r->inputMask&*reinterpret_cast<std::uint32_t*>(input+0x5B4))!=0){
                UiCancelSound_9E4A70(owner);
                r->result=0;
            }else if((r->inputMask&*reinterpret_cast<std::uint32_t*>(input+0x5B8))!=0){
                UiConfirmSound_9E4A90(owner);
                r->result=1;
            }else{
                return;
            }
        }
        ++r->substate;
        r->routeChanged=1;
        r->timer=*reinterpret_cast<const float*>(0x010BCA6C);
        return;

    case 2:
        r->timer-=StoryFrameDelta120();
        if(r->timer>0.0f)return;
        if(r->result==0){
            auto* ownerBytes=static_cast<std::uint8_t*>(owner);
            ownerBytes[0x4C+(r->playerIndex^1)*0x68]=1;
            r->substate=0x0A;
            r->aux=0;
            r->routeChanged=0;
        }else{
            r->substate=0x5A;
            r->routeChanged=0;
        }
        return;

    case 0x0A:
        r->substate=0x14;
        return;

    case 0x14: {
        void* context=ResolveCurrentContext_768C10(StoryContextManager120());
        SetLocalPlayerActive_716720(context,false,r->playerIndex,r->device);
        void* split=*reinterpret_cast<void**>(0x0123457C);
        SplitLocalTransition_76B520(split);
        UpdateSplitViewportState_76CC60(split);
        SetInputTransition_794FF0(*reinterpret_cast<void**>(0x01249C1C),0);
        r->active=0;
        return;
    }

    case 0x1E: {
        // vtable+0x30 receives (0.5f, 0x20), then native advances to 0x1F.
        void* gate=*reinterpret_cast<void**>(0x011E8804);
        auto** vt=*reinterpret_cast<void***>(gate);
        using BeginFn=void(__thiscall*)(void*,float,int);
        reinterpret_cast<BeginFn>(vt[0x30/sizeof(void*)])(
            gate,*reinterpret_cast<const float*>(0x010BB498),0x20);
        ++r->substate;
        return;
    }

    case 0x1F: {
        void* gate=*reinterpret_cast<void**>(0x011E8804);
        auto** vt=*reinterpret_cast<void***>(gate);
        using BusyFn=int(__thiscall*)(void*);
        if(reinterpret_cast<BusyFn>(vt[0x38/sizeof(void*)])(gate)!=0)return;

        void* context=ResolveCurrentContext_768C10(StoryContextManager120());
        SetLocalPlayerActive_716720(context,false,r->playerIndex,r->device);
        RefreshLocalJoinOwner_7F1280(*reinterpret_cast<void**>(0x011B1DD8));
        RootSetFlags_726120(*reinterpret_cast<void**>(0x012340A4),0x4000u);
        void* active=ResolveCurrentContext_768C10(StoryContextManager120());
        FinalizeCurrentContext_723420(active);
        ++r->substate;
        return;
    }

    case 0x5A:
        r->state=r->previousState;
        r->substate=0;
        return;

    default:
        return;
    }
}

// Existing exact state 8: owns uIdOption05 UI and does not activate J2.
struct StoryPauseRecordState8View120 { std::uint8_t _00[8]; std::int32_t playerIndex; std::uint8_t _0C[0x34-0x0C]; std::int32_t state; std::int32_t previousState; std::uint8_t _3C[0x58-0x3C]; std::uint8_t substate; std::uint8_t _59[0x64-0x59]; void* child; };
extern void* CreatePauseOption05_A258C0(void* context);
extern void RegisterPauseChild_428B80(void* manager,int slot,void* child);
static void DestroyState8Child(StoryPauseRecordState8View120& r){ if(!r.child)return; auto* b=static_cast<std::uint8_t*>(r.child); *reinterpret_cast<std::uint32_t*>(b+4)&=~0x2000u; auto** vt=*reinterpret_cast<void***>(r.child); using F=void(__thiscall*)(void*,int); reinterpret_cast<F>(vt[0x30/sizeof(void*)])(r.child,1); r.child=nullptr; }
// 0x00A2D740 -- VERIFIED. State 8 owns uIdOption05 UI; it does not activate J2.
void UpdateStoryPauseState8_A2D740(StoryPauseRecordState8View120& r){ switch(r.substate){ case 0:{ DestroyState8Child(r); void* child=CreatePauseOption05_A258C0(*reinterpret_cast<void**>(0x011F54D0)); r.child=child; if(!child)return; auto* b=static_cast<std::uint8_t*>(child); *reinterpret_cast<std::uint32_t*>(b+4)|=0x2000u; *reinterpret_cast<std::int32_t*>(b+0x34)=r.playerIndex; RegisterPauseChild_428B80(*reinterpret_cast<void**>(0x012340B0),0x1B,child); ++r.substate; return;} case 1: if(!r.child||*(static_cast<std::uint8_t*>(r.child)+0x3C)==0)return; DestroyState8Child(r); ++r.substate; return; case 2:r.state=r.previousState;r.substate=0;return; default:return; }}
} // namespace re5::split120
