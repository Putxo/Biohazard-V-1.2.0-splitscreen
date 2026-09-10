#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

namespace {

template <class T>
static inline T& field(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(p) + off);
}

extern int QueryGameStatus_C42D90();
extern SplitRenderState120* GetSplitState_123457C();
extern int FullCoordTransform_76A460(int x);
extern int PanelTransform_76C2A0(int x);
extern void DrawUiItem_9E3E70(void* self, int width, int x, int y,
                              int* localValue, int stringOrId,
                              int height, int minusOne);
extern std::uint16_t LookupUiWord_112C128(int index);

static inline int div10_trunc(int v) { return v / 10; }
static inline int div8_trunc(int v)  { return v / 8; }
static inline int div4_trunc(int v)  { return v / 4; }

} // namespace

// 0x00A23A00
// thiscall; stack argument: itemIndex; returns with RET 4.
// Reconstructed directly from the 1.2.0 instruction stream.
void SplitDrawPrimary_A23A00(void* self, int itemIndex)
{
    int width;
    if (QueryGameStatus_C42D90() == 1) {
        width = static_cast<int>(GetSplitState_123457C()->splitScale * 30.0f);
    } else {
        // Exact branch: 0x1C when self+0x20 != 0, 0x1E when it is zero.
        width = field<std::int32_t>(self, 0x20) == 0 ? 0x1E : 0x1C;
    }

    int uiId = static_cast<int>(LookupUiWord_112C128(itemIndex));
    int localValue = -1;

    int x;
    if (uiId == 0x952 || uiId == 0x974) {
        x = div4_trunc(field<std::int32_t>(self, 0x24) * 3);
    } else {
        x = div10_trunc(field<std::int32_t>(self, 0x24) * 9);
    }

    int y = div8_trunc(field<std::int32_t>(self, 0x28) * 7);

    if (QueryGameStatus_C42D90() == 1) {
        auto* split = GetSplitState_123457C();
        if (split->fullScreenSplitMode != 0 && split->transientSplitFlag == 0) {
            x = FullCoordTransform_76A460(x);
        } else {
            x = static_cast<int>(static_cast<float>(x) + split->splitOffset);
        }

        int halfWidth = (width - (width >> 31)) >> 1;
        y = PanelTransform_76C2A0(y + halfWidth);
    } else {
        int halfWidth = (width - (width >> 31)) >> 1;
        y -= halfWidth;
    }

    y -= field<std::int32_t>(self, 0x30);
    x -= field<std::int32_t>(self, 0x2C);

    DrawUiItem_9E3E70(self, width, x, y, &localValue, uiId, 0x60, -1);
}

} // namespace re5::split120
