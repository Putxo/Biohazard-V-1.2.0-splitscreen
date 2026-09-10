#include <cstdint>

namespace re5::split120 {

struct StoryPauseRecord120 {
    std::uint8_t _00[0x08];
    std::int32_t playerIndex; std::int32_t device;
    std::uint32_t cancelMask; std::uint32_t confirmMask;
    std::uint8_t _18[0x1C-0x18];
    std::int32_t splitAccessorA; std::int32_t splitAccessorB;
    std::int32_t uiMetricA; std::int32_t uiMetricB;
    std::uint8_t _2C[0x30-0x2C];
    std::int32_t splitGeometry; std::int32_t state;
    std::int32_t previousState; std::int32_t selectedRow;
    std::int32_t optionCount; std::int32_t optionIds[5];
    std::uint8_t substate; std::uint8_t substateAux; std::uint8_t _5A[2];
    float timer; std::int32_t result; void* child;
};
static_assert(sizeof(StoryPauseRecord120) == 0x68, "1.2.0 pause record stride");

// 0x00A2D710 -- VERIFIED local/split reset fragment.
void ResetStoryPauseRecordIfNotLocal_A2D710(StoryPauseRecord120* records, int index)
{
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
    if ((*reinterpret_cast<std::uint32_t*>(root + 0x10438) & 0x10u) != 0) return;
    StoryPauseRecord120& current = records[index];
    current.state = 0;
    reinterpret_cast<std::uint8_t*>(&current)[6] = 0;
    StoryPauseRecord120& peer = records[index ^ 1];
    reinterpret_cast<std::uint8_t*>(&peer)[4] = 0;
}

// Exact native option-id tables at 0x112C224..0x112C27C.
static constexpr std::int32_t kStandard[4] = {1,2,5,3};
static constexpr std::int32_t kLocalA[4] = {1,6,8,9};
static constexpr std::int32_t kLocalB[4] = {1,6,8,9};
static constexpr std::int32_t kFive[5] = {1,2,5,4,3};
static constexpr std::int32_t kThree[3] = {1,5,3};
static constexpr std::int32_t kTwo[2] = {1,5};
static constexpr std::int32_t kAlt[3] = {1,13,3};
const std::int32_t* PauseOptionTable112C224(){return kStandard;}
const std::int32_t* PauseOptionTable112C234(){return kLocalA;}
const std::int32_t* PauseOptionTable112C244(){return kLocalB;}
const std::int32_t* PauseOptionTable112C254(){return kFive;}
const std::int32_t* PauseOptionTable112C268(){return kThree;}
const std::int32_t* PauseOptionTable112C274(){return kTwo;}
const std::int32_t* PauseOptionTable112C27C(){return kAlt;}

} // namespace re5::split120
