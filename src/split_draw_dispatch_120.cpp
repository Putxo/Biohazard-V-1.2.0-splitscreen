#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

struct DrawGate120 {
    std::uint8_t _00[3];
    std::uint8_t enabled; // +0x03
};

// Native 1.2.0 dispatcher reached by several UI builders.
// The exact low-level renderer argument pack at 0x9E5E1E..0x9E5E82 is kept
// behind a dedicated ABI helper until its compound stack object has a stable
// recovered type. The routing itself is instruction-exact.
extern bool SplitUiTableFallback_9E5870(void* self,
                                        int a1,
                                        int a2,
                                        int a3,
                                        DrawGate120* gate,
                                        int a5,
                                        int a6,
                                        int a7);
extern void DrawLegacyPrimitivePack_9E5E1E(void* self,
                                           int a1,
                                           int a2,
                                           int a3,
                                           DrawGate120* gate,
                                           int a5,
                                           int a6,
                                           int a7);

// 0x009E5DE0..0x009E5E8B, thiscall, RET 0x1C.
//
// Verified control flow:
//   - gate+3 == 0: no draw, return immediately;
//   - gate+3 != 0: call 0x9E5870 with the same seven stack arguments;
//   - if 0x9E5870 returns nonzero, the event was consumed;
//   - otherwise fall back to the ordinary renderer pack at 0x9E5E1E.
void DrawSplitAwareDispatch_9E5DE0(void* self,
                                   int a1,
                                   int a2,
                                   int a3,
                                   DrawGate120* gate,
                                   int a5,
                                   int a6,
                                   int a7)
{
    if (gate->enabled == 0)
        return;

    if (SplitUiTableFallback_9E5870(self, a1, a2, a3, gate, a5, a6, a7))
        return;

    DrawLegacyPrimitivePack_9E5E1E(self, a1, a2, a3, gate, a5, a6, a7);
}

// 0x009E5E90..0x009E5F0E, thiscall, RET 0x18.
// This wrapper measures the resource through 0x7DC6B0, centers the requested
// X/Y around half the returned extents, then forwards to 0x9E5DE0.
struct UiExtent120 {
    float x;
    float y;
    float z;
    float w;
};

extern void MeasureUiExtent_7DC6B0(int resource,
                                   UiExtent120* outExtent,
                                   int zero0,
                                   int zero1,
                                   int sameA,
                                   int sameB,
                                   int minusOne);

void DrawCenteredSplitAware_9E5E90(void* self,
                                    int resource,
                                    int x,
                                    int y,
                                    int a4,
                                    int a5,
                                    int a6)
{
    UiExtent120 extent{};
    MeasureUiExtent_7DC6B0(resource, &extent, 0, 0, a6, a6, -1);

    const int centeredX = static_cast<int>(static_cast<float>(x) - extent.x * 0.5f);
    const int centeredY = static_cast<int>(static_cast<float>(y) - extent.y * 0.5f);

    DrawSplitAwareDispatch_9E5DE0(self,
                                  resource,
                                  centeredX,
                                  centeredY,
                                  reinterpret_cast<DrawGate120*>(static_cast<std::uintptr_t>(a4)),
                                  a5,
                                  a6,
                                  -1);
}

} // namespace re5::split120
