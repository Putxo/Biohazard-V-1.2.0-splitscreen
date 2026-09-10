#include <cstdint>
namespace re5::split120 {
struct StoryPauseSelectView120 { std::uint8_t _00[8]; std::int32_t playerIndex; std::uint8_t _0C[0x34-0x0C]; std::int32_t state; std::int32_t previousState; std::int32_t selectedRow; std::int32_t optionCount; std::int32_t optionIds[5]; std::uint8_t substate; };
extern void PauseSelectionError_9E4AC0(); extern bool PauseSpecial13Gate_C42960(void*); extern void PauseSpecial13Apply_C42A10(void*);
// Split-specific acceptance semantics inside 0x00A2E0F0 -- VERIFIED. +0x44 is an inline five-DWORD option array, not a pointer.
bool CommitStoryPauseSelection_A2E0F0(StoryPauseSelectView120& r){ if(r.selectedRow<0||r.selectedRow>=r.optionCount)return false; const int id=r.optionIds[r.selectedRow]; auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4); auto* session=*reinterpret_cast<std::uint8_t**>(root+0x1042C); auto* input=*reinterpret_cast<std::uint8_t**>(0x01249C40); if(id==8&&r.playerIndex!=*reinterpret_cast<std::int32_t*>(input+0x614)){PauseSelectionError_9E4AC0();return false;} if(id==9&&*reinterpret_cast<std::int32_t*>(session+0x58)!=0){PauseSelectionError_9E4AC0();return false;} if(id==13){if(!PauseSpecial13Gate_C42960(session))return false;PauseSpecial13Apply_C42A10(session);return true;} r.previousState=r.state;r.state=id;r.substate=0;return true; }
} // namespace re5::split120
