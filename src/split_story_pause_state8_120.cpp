#include <cstdint>
namespace re5::split120 {
struct StoryPauseRecordState8View120 { std::uint8_t _00[8]; std::int32_t playerIndex; std::uint8_t _0C[0x34-0x0C]; std::int32_t state; std::int32_t previousState; std::uint8_t _3C[0x58-0x3C]; std::uint8_t substate; std::uint8_t _59[0x64-0x59]; void* child; };
extern void* CreatePauseOption05_A258C0(void* context);
extern void RegisterPauseChild_428B80(void* manager,int slot,void* child);
static void DestroyState8Child(StoryPauseRecordState8View120& r){ if(!r.child)return; auto* b=static_cast<std::uint8_t*>(r.child); *reinterpret_cast<std::uint32_t*>(b+4)&=~0x2000u; auto** vt=*reinterpret_cast<void***>(r.child); using F=void(__thiscall*)(void*,int); reinterpret_cast<F>(vt[0x30/sizeof(void*)])(r.child,1); r.child=nullptr; }
// 0x00A2D740 -- VERIFIED. State 8 owns uIdOption05 UI; it does not activate J2.
void UpdateStoryPauseState8_A2D740(StoryPauseRecordState8View120& r){ switch(r.substate){ case 0:{ DestroyState8Child(r); void* child=CreatePauseOption05_A258C0(*reinterpret_cast<void**>(0x011F54D0)); r.child=child; if(!child)return; auto* b=static_cast<std::uint8_t*>(child); *reinterpret_cast<std::uint32_t*>(b+4)|=0x2000u; *reinterpret_cast<std::int32_t*>(b+0x34)=r.playerIndex; RegisterPauseChild_428B80(*reinterpret_cast<void**>(0x012340B0),0x1B,child); ++r.substate; return;} case 1: if(!r.child||*(static_cast<std::uint8_t*>(r.child)+0x3C)==0)return; DestroyState8Child(r); ++r.substate; return; case 2:r.state=r.previousState;r.substate=0;return; default:return; }}
} // namespace re5::split120
