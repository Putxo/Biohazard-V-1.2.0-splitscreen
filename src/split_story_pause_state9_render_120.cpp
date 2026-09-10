#include <cstdint>
namespace re5::split120 {
struct StoryPauseRender9View120 { std::uint8_t _00[0x2C]; std::int32_t splitMetric; std::int32_t splitGeometry; std::uint8_t _34[0x5A-0x34]; std::uint8_t visualAlt; };
extern void SetPauseVisualStyle_9E3D80(int*,int); extern void DrawPauseState9Centered_A2D8B0(void*,const void*,int); extern int SessionStatus_C42D90(void*); extern void DrawPauseState9Overlay_A2F3C0(void*,int);
// 0x00A30230..0x00A302C2 -- VERIFIED State 9 renderer.
void RenderStoryPauseState9_A30230(void* owner,StoryPauseRender9View120& r){const int style=r.visualAlt?0x743:0x740;SetPauseVisualStyle_9E3D80(&r.splitGeometry,style);const int y=(0x24-r.splitGeometry)/2;DrawPauseState9Centered_A2D8B0(owner,reinterpret_cast<const void*>(0x0112C288),y);auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4);auto* session=*reinterpret_cast<void**>(root+0x1042C);if(SessionStatus_C42D90(session)==1)DrawPauseState9Overlay_A2F3C0(owner,r.splitMetric+y);else DrawPauseState9Overlay_A2F3C0(owner,0x18C);}
} // namespace re5::split120
