#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct RenderDimensionsA2D120 {
    std::uint8_t _00[0x50];
    std::int32_t width;
    std::int32_t height;
};

extern void* gUiInput_11B20C4;
extern RenderDimensionsA2D120* gRenderDimensions_12345D4;
extern int QueryGameStatus_C42D90();
extern void InputBaseUpdate_7B4370(void* manager, int arg1, int arg2);
extern void InputSplitOffset_7B4480(void* manager, int zero, int value, int arg2);

static inline int NativeDiv4TowardZero(int v)
{
    // CDQ; AND EDX,3; ADD EAX,EDX; SAR EAX,2
    return v >= 0 ? (v >> 2) : ((v + 3) >> 2);
}

// 0x00A2D7F0..0x00A2D864 -- VERIFIED
// Native ABI: two stack args, RET 8. ECX itself is not consumed; both
// downstream calls use the global UI/input manager at 0x11B20C4.
void UpdateSplitInputAspect_A2D7F0(int arg1, int arg2)
{
    InputBaseUpdate_7B4370(gUiInput_11B20C4, arg1, arg2);

    if (QueryGameStatus_C42D90() != 1)
        return;

    const int width = gRenderDimensions_12345D4->width;
    const int height = gRenderDimensions_12345D4->height;

    // Native sequence: (height*720)/width, then signed truncation toward zero /4.
    const int ratio720 = (height * 720) / width;
    const int quarter = NativeDiv4TowardZero(ratio720);

    // 0xF650E4 = 0xBE800000 = exactly -0.25f; CVTTSS2SI truncates toward zero.
    const int value = static_cast<int>(static_cast<float>(quarter) * -0.25f);

    // Native stack at 0xA2D830..0xA2D85E is {0,value,arg2} for 0x7B4480.
    InputSplitOffset_7B4480(gUiInput_11B20C4, 0, value, arg2);
}

} // namespace re5::split120
