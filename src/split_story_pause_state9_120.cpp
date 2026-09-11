#include <cstdint>
namespace re5::split120 {
#pragma pack(push,1)
struct StoryPauseRecordState9View120 {
    std::uint8_t _00[2];
    std::uint8_t routeChanged;        // +02
    std::uint8_t _03;
    std::uint8_t changed;             // +04
    std::uint8_t _05[3];
    std::int32_t playerIndex;         // +08
    std::int32_t device;              // +0C
    std::uint32_t inputMask;          // +10
    std::uint8_t _14[0x34-0x14];
    std::int32_t state;               // +34
    std::int32_t previousState;       // +38
    std::uint8_t _3C[0x58-0x3C];
    std::uint8_t substate;            // +58
    std::uint8_t aux;                 // +59
    std::uint8_t _5A[2];
    float timer;                       // +5C
    std::int32_t result;              // +60
    std::uint8_t _64[4];              // native record stride = 0x68
};
#pragma pack(pop)
static_assert(sizeof(StoryPauseRecordState9View120)==0x68,"uIdOption 1.2 record stride");
static_assert(__builtin_offsetof(StoryPauseRecordState9View120,state)==0x34,"state offset");
static_assert(__builtin_offsetof(StoryPauseRecordState9View120,substate)==0x58,"substate offset");
static_assert(__builtin_offsetof(StoryPauseRecordState9View120,timer)==0x5C,"timer offset");
static_assert(__builtin_offsetof(StoryPauseRecordState9View120,result)==0x60,"result offset");

extern void __thiscall DrawStoryLeavePrompt_A2D7F0(void*,int,int);
extern bool __thiscall UiCancelFallback_7B5620(void*,int,int,int);
extern bool __thiscall UiConfirmFallback_7B63C0(void*,int,int,int);
extern void __thiscall UiConfirmSound_9E4A90(void*);
extern void __thiscall UiCancelSound_9E4A70(void*);
extern void* __thiscall ResolveCurrentContext_768C10(void*);
extern void __thiscall FinalizeCurrentContext_723420(void*);

static inline float FrameDelta_A2EC50(){
    auto* frame=*reinterpret_cast<std::uint8_t**>(0x011B209C);
    return *reinterpret_cast<float*>(frame+0x28);
}
static inline void* UiContextManager_A2EC50(){return *reinterpret_cast<void**>(0x01234578);}

// 0x00A2EC50..0x00A2EE55 -- exact native state-9 update shape.
// Native ABI: ECX=owner, one stack argument = record, ret 4.
// State 9 is the Story local-player leave/handback path.
void __thiscall UpdateStoryPauseState9_A2EC50(void* owner,StoryPauseRecordState9View120* r){
    switch(r->substate){
    case 0:
        r->timer=*reinterpret_cast<const float*>(0x010BCA6C); // 0.25f
        ++r->substate;
        return;

    case 1:
        // Native clamps and returns when the timer first crosses zero. The
        // prompt is therefore emitted on the next update, not that same frame.
        if(r->timer>0.0f){
            r->timer-=FrameDelta_A2EC50();
            if(r->timer>0.0f)return;
            r->timer=0.0f;
            return;
        }
        DrawStoryLeavePrompt_A2D7F0(owner,0x5B,0);
        {
            auto* input=*reinterpret_cast<std::uint8_t**>(0x01249C40);
            auto* ui=*reinterpret_cast<void**>(0x011B20C4);
            bool cancel=(r->inputMask&*reinterpret_cast<std::uint32_t*>(input+0x5B4))!=0;
            if(!cancel)cancel=UiCancelFallback_7B5620(ui,-1,0,0);
            if(cancel){
                UiCancelSound_9E4A70(owner);
                r->result=0;
            }else{
                bool confirm=(r->inputMask&*reinterpret_cast<std::uint32_t*>(input+0x5B8))!=0;
                if(!confirm)confirm=UiConfirmFallback_7B63C0(ui,-1,0,0);
                if(!confirm)return;
                UiConfirmSound_9E4A90(owner);
                r->result=1;
            }
        }
        ++r->substate;
        r->routeChanged=1;
        r->timer=*reinterpret_cast<const float*>(0x010BCA6C);
        return;

    case 2:
        r->timer-=FrameDelta_A2EC50();
        if(r->timer>0.0f)return;
        if(r->result==0){
            auto* ownerBytes=static_cast<std::uint8_t*>(owner);
            const int sibling=r->playerIndex^1;
            ownerBytes[0x4C+sibling*0x68]=1;
            r->substate=0x14;
            r->aux=0;
            r->routeChanged=0;
        }else{
            r->substate=0x5A;
            r->routeChanged=0;
        }
        return;

    case 0x14: {
        void* context=ResolveCurrentContext_768C10(UiContextManager_A2EC50());
        SetLocalPlayerActive_716720(context,false,r->playerIndex,r->device);

        auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4);
        auto* session=*reinterpret_cast<std::uint8_t**>(root+0x1042C);
        auto* input=*reinterpret_cast<std::uint8_t**>(0x01249C40);
        const int preferred=*reinterpret_cast<std::int32_t*>(input+0x5D8);
        auto* profile=*reinterpret_cast<std::uint8_t**>(0x011B2158);
        const int idx=*reinterpret_cast<std::int32_t*>(profile+0x20);
        const int activePlayer=*reinterpret_cast<std::int32_t*>(profile+idx*0x5940+0x273C4);
        SessionSetDevice_C42A50(session,activePlayer,preferred);
        *reinterpret_cast<std::int32_t*>(input+0x614)=activePlayer;
        RootSetFlags_726120(root,0x4000u);

        void* active=ResolveCurrentContext_768C10(UiContextManager_A2EC50());
        FinalizeCurrentContext_723420(active);
        ++r->substate;
        r->changed=1;
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
} // namespace re5::split120
