#include <cstdint>

namespace re5::split120 {

struct FrontendTransitionHost120 {
    std::uint8_t _0000[0x18];
    std::int32_t uiObjectIndex;      // +0x18
    std::uint8_t _001c[0x64-0x1c];
    float transition[4];             // +0x64 + index*4
    std::uint8_t _0074[0x78-0x74];
    std::int32_t transitionStep[4];  // +0x78 + index*4
    std::uint8_t _0088[0x8c-0x88];
    std::int32_t transitionArmed[4]; // +0x8C + index*4
};

extern "C" void __attribute__((thiscall)) UpdateUiVisibility_9E3890(FrontendTransitionHost120* self, std::int32_t visible);
extern std::uint8_t* gFrameState_11B209C;

// 0xA17240 -- VERIFIED.
// Begins the standard nine-step frontend transition for one channel.
void __attribute__((thiscall)) BeginFrontendTransition_A17240(FrontendTransitionHost120* self,
                                                               std::int32_t channel)
{
    self->transition[channel] = 9.0f;
    self->transitionStep[channel] = 9;
    self->transitionArmed[channel] = 1;
    UpdateUiVisibility_9E3890(self, 1);
}

// 0xA17280 -- VERIFIED.
// Decrements the transition by the frame delta at global+0x20. When it reaches
// zero, the visible state is cleared and AL=1 is returned to the caller.
bool __attribute__((thiscall)) TickFrontendTransition_A17280(FrontendTransitionHost120* self,
                                                              std::int32_t channel)
{
    const float dt = *reinterpret_cast<const float*>(gFrameState_11B209C + 0x20);
    const float next = self->transition[channel] - dt;
    self->transition[channel] = next;
    if (next <= 0.0f) {
        self->transitionStep[channel] = 0;
        UpdateUiVisibility_9E3890(self, 0);
        return true;
    }
    self->transitionStep[channel] = static_cast<std::int32_t>(next);
    return false;
}

// 0xA172D0 -- VERIFIED helper used by this same family.
bool __attribute__((thiscall)) IsFrontendTransitionAtNine_A172D0(const FrontendTransitionHost120* self,
                                                                 std::int32_t channel)
{
    return self->transitionStep[channel] == 9;
}

// Important integration result: A17240/A17280 are generic UI/front-end fade
// helpers used by many unrelated menus. At A4AE41/A4AE4E they only animate the
// Mercenaries/Reunion state 0x5A->0x5B transition; they are NOT the gameplay
// loader themselves. The actual state machine advances after Tick returns true.

} // namespace re5::split120
