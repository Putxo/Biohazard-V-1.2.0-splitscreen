#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

struct UiMetricBox120 {
    float v0;
    float v1;
    float extentX;
    float extentY;
};

extern void* gUiRenderer_11B26F0;
extern int MeasureUiMetric_7DC6B0(void* renderer,
                                  int a0,
                                  int metricId,
                                  UiMetricBox120* out,
                                  int a3,
                                  int value0,
                                  int value1,
                                  int finalArg);

static inline int HalfTowardZero(int value)
{
    return value >= 0 ? value / 2 : -((-value) / 2);
}

// 0x009E3EE0..0x009E3FDC -- full instruction-derived reconstruction.
// Calling convention in the original is thiscall-like with six stack arguments
// and RET 0x18. The routine always returns 0 and only mutates the supplied X/Y
// coordinates for modes 1, 2 and 3.
int AdjustUiCoordinates_9E3EE0(int mode,
                               int value,
                               int* x,
                               int* y,
                               int metricId,
                               int finalArg)
{
    switch (mode) {
    case 3: {
        UiMetricBox120 metric{};
        MeasureUiMetric_7DC6B0(gUiRenderer_11B26F0,
                               0, metricId, &metric, 0,
                               value, value, -1);

        // 0x10BB498 is exactly 0.5f in the 1.2.0 image.
        *x = static_cast<int>(static_cast<float>(*x) - metric.extentX * 0.5f);
        *y = static_cast<int>(static_cast<float>(*y) - metric.extentY * 0.5f);
        break;
    }

    case 2: {
        UiMetricBox120 metric{};
        MeasureUiMetric_7DC6B0(gUiRenderer_11B26F0,
                               0, metricId, &metric, 0,
                               value, value, finalArg);
        *x -= static_cast<int>(metric.extentX);
        break;
    }

    case 1: {
        UiMetricBox120 metric{};
        MeasureUiMetric_7DC6B0(gUiRenderer_11B26F0,
                               0, metricId, &metric, 0,
                               value, value, -1);

        // Original sequence: CVTTSS2SI / CDQ / SUB EAX,EDX / SAR EAX,1.
        // This is signed division by two truncated toward zero.
        *x -= HalfTowardZero(static_cast<int>(metric.extentX));
        break;
    }

    default:
        break;
    }

    return 0;
}

} // namespace re5::split120
