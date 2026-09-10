#include <cstdint>
namespace re5::split120 {
std::uint32_t StoryPauseUpdateTarget_A305C3(int state){ static constexpr std::uint32_t t[13]={0x00A305D6,0x00A305E3,0x00A305EC,0x00A305EC,0x00A305F6,0x00A30600,0x00A3060A,0x00A30614,0x00A30652,0x00A3065C,0x00A3061E,0x00A3063E,0x00A30648}; return(state>=0&&state<13)?t[state]:0; }
std::uint32_t StoryPauseRenderTarget_A307E1(int state){ static constexpr std::uint32_t t[13]={0x00A307F4,0x00A30878,0x00A3080A,0x00A3081B,0x00A3082C,0x00A30878,0x00A30878,0x00A3083D,0x00A30878,0x00A30870,0x00A3084E,0x00A30878,0x00A3085F}; return(state>=0&&state<13)?t[state]:0; }
// VERIFIED tables: update state8->A2D740, state9->A2EC50; render state9->A30230.
bool IsStoryPauseLocalExtensionState(int state){return state==8||state==9;}
} // namespace re5::split120
