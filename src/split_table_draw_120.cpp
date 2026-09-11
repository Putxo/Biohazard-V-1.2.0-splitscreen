#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SplitFallbackRecord120 {
    std::int32_t type;
    std::int32_t flags;
    std::int32_t x;
    std::int32_t y;
    std::int32_t eventId;
    std::int32_t resourceId;
};

struct DrawOwner120 {
    std::uint8_t _00[0x1C];
    std::int32_t uiA;      // +0x1C
    std::int32_t uiB;      // +0x20
    std::uint8_t _24[0x0C];
    std::int32_t originY;  // +0x30
};

struct DrawDescriptor120 {
    std::uint32_t first;
    float a;
    float b;
};

extern int AdjustUiCoordinates_9E3EE0(void* self,
                                      int type,
                                      int arg1,
                                      int* x,
                                      int* y,
                                      int resourceId,
                                      int arg7);
extern int MeasureUiWidth_9E3C80(int uiA);
extern SplitRenderState120* gSplitRenderState_123457C;
extern int PanelTransform_76C2A0(SplitRenderState120* split, int value);
extern void* gUiRenderer_11B26F0;
extern void LowLevelDraw_7DDFB0(void* renderer,
                                int zero0,
                                int resourceId,
                                float x,
                                float y,
                                int zero1,
                                DrawDescriptor120 descriptor,
                                int packed,
                                float neg1a,
                                float neg1b,
                                int arg6,
                                int arg7);

// 0x009E5C6E..0x009E5DD0 -- VERIFIED isolated table-driven draw path.
//
// This is the branch entered after 0x9E5870 finds a matching record in the
// 55-entry table at 0x1121C98. It preserves the native override rules,
// coordinate adjustment, split transform and final 0x7DDFB0 argument pack.
bool DrawFallbackRecord_9E5C6E(void* selfOpaque,
                               const SplitFallbackRecord120& record,
                               int arg1,
                               int& x,
                               int& y,
                               const std::uint32_t* arg4,
                               int arg6,
                               int arg7)
{
    // Native 0x9E5C78: negative resource id means "handled, no draw".
    if (record.resourceId < 0)
        return true;

    // Native record coordinates override caller coordinates only when >= 0.
    if (record.x >= 0)
        x = record.x;
    if (record.y >= 0)
        y = record.y;

    AdjustUiCoordinates_9E3EE0(selfOpaque,
                               record.type,
                               arg1,
                               &x,
                               &y,
                               record.resourceId,
                               arg7);

    auto* self = static_cast<DrawOwner120*>(selfOpaque);
    int transformedX = x;
    int transformedY = y;

    if (QueryGameStatus_C42D90() == 1) {
        SplitRenderState120* split = gSplitRenderState_123457C;

        // 0x10BCDF4 = 0x3A4CCCCD = ~1/1280.
        const int scaledBaseX = static_cast<int>(
            static_cast<float>(MeasureUiWidth_9E3C80(self->uiA)) *
            static_cast<float>(y) *
            0.00078125001164153218f);

        transformedX = IsFullSplitEffective(split)
            ? FullCoordTransform_76A460(split, scaledBaseX)
            : static_cast<int>(static_cast<float>(scaledBaseX) +
                               split->splitOffset);

        transformedY = PanelTransform_76C2A0(split, arg1 + y / 2);
    }

    const DrawDescriptor120 descriptor{
        *arg4,
        static_cast<float>(arg1),
        static_cast<float>(arg1),
    };

    const int packed = (self->uiA << 4) | self->uiB;

    LowLevelDraw_7DDFB0(gUiRenderer_11B26F0,
                        0,
                        record.resourceId,
                        static_cast<float>(transformedX),
                        static_cast<float>(self->originY + transformedY),
                        0,
                        descriptor,
                        packed,
                        -1.0f,
                        -1.0f,
                        arg6,
                        arg7);
    return true;
}

} // namespace re5::split120
