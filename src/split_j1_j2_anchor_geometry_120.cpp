#include <cstdint>
namespace re5::split120 {
struct SplitAnchorState120 { std::uint8_t _0[0x3070]; float splitScale; };
static inline int truncf(float v){return static_cast<int>(v);}

// 0x00A2ABC7..0x00A2AC70 and twin 0x00A2B0AD..0x00A2B153.
// Exact status-1 local-J1/J2 anchor calculation. `base` is the measured left
// edge and `right` its paired edge from 0x7B5B00. `deltaOther` is the second
// measured coordinate delta retained in the native local stack.
int ComputeLocalPlayerAnchoredX_A2ABC7(const SplitAnchorState120* split,
                                      int keyboardPlayerNo,
                                      bool effectiveFull,
                                      int base,
                                      int right,
                                      int deltaOther)
{
    const float s=split->splitScale;
    const int margin=truncf((1.0f-s)*1280.0f);
    const int scaledBase=truncf(float(base)*s);
    const float anchor=effectiveFull?0.5f:(keyboardPlayerNo==0?0.30000001192092896f:0.699999988079071f);
    const int anchored=truncf(float(scaledBase)+float(margin)*anchor);
    const float scaledDelta=float(right-base)*s;
    return truncf(float(anchored)+scaledDelta+float(deltaOther));
}
}
