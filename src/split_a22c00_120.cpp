#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct SplitGeometryNode120 {
    std::uint8_t _00[0x18];
    std::int32_t x;              // +0x1C
    std::int32_t y;              // +0x20
    std::int32_t width;          // +0x24
    std::int32_t height;         // +0x28
    std::int32_t xOrigin;        // +0x2C
    std::int32_t scratch;        // +0x30
    std::int32_t playerIndex;    // +0x34
    std::uint8_t _38[0x188-0x38];
    std::int32_t mappedDevice;   // +0x188
    void* resource;              // +0x18C
};

struct RuntimeRootA22C120 {
    std::uint8_t _0000[0x1042C];
    void* session;               // +0x1042C
};

struct RenderDimensionsA22C120 {
    std::uint8_t _00[0x50];
    std::int32_t width;          // +0x50
    std::int32_t height;         // +0x54
};

extern RuntimeRootA22C120* gRuntimeRoot_12340A4;
extern SplitRenderState120* gSplitRenderState_123457C;
extern RenderDimensionsA22C120* gRenderDimensions_12345D4;

extern int GetRuntimeStatus_C42D90(void* session);
extern int ResolveSplitX_76A1E0(SplitRenderState120* split, int playerIndex);
extern int ResolveSplitY_76A250(SplitRenderState120* split, int xIndex);
extern int GetUiWidth_9E3C80(int splitIndex);
extern int GetUiHeight_9E3CD0(int splitIndex);

// 0x00A22C00..0x00A22CF6 -- VERIFIED against direct disassembly of the
// unpacked RE5DX9 1.2.0 executable.
//
// Native ABI is thiscall with ECX=self and NO stack arguments (plain RET).
// The previous reconstruction incorrectly modeled split/render dimensions as
// caller-supplied parameters; the native function reads all of them from the
// global objects at 0x123457C and 0x12345D4.
void UpdateSplitGeometry_A22C00(SplitGeometryNode120* self)
{
    if (!self->resource)
        return;

    auto* rootBytes = reinterpret_cast<std::uint8_t*>(gRuntimeRoot_12340A4);
    auto* session = *reinterpret_cast<std::uint8_t**>(rootBytes + 0x1042C);

    // A22C1C..A22C26: session+0x490[playerIndex].
    self->mappedDevice = *reinterpret_cast<std::int32_t*>(
        session + 0x490 + self->playerIndex * 4);

    SplitRenderState120* split = gSplitRenderState_123457C;

    self->x = ResolveSplitX_76A1E0(split, self->playerIndex);
    if (self->x == -1)
        self->x = 0;

    self->y = ResolveSplitY_76A250(split, self->x);
    self->width  = GetUiWidth_9E3C80(self->x);
    self->height = GetUiHeight_9E3CD0(self->x);

    // Exact CDQ/SUB/SAR signed divide-by-two sequence.
    self->xOrigin = (self->width - 0x500) / 2;

    if (GetRuntimeStatus_C42D90(session) == 1 && split->splitActive) {
        // Native effective-FULL gate:
        //   +3084 != 0 && +3085 == 0  => skip correction.
        if (!IsFullSplitEffective(split)) {
            const RenderDimensionsA22C120* dims = gRenderDimensions_12345D4;
            constexpr float kAspect16By9 = 1.7777777910232544f; // 0x3FE38E39

            // CVTTSS2SI truncates the floating excess before signed halving.
            const int excess = static_cast<int>(
                static_cast<float>(dims->width) -
                static_cast<float>(dims->height) * kAspect16By9);
            const int halfExcess = excess / 2;

            // A22CE2..A22CEB: NEG; IMUL width; IDIV renderWidth.
            const int correction = (-halfExcess * self->width) / dims->width;
            self->xOrigin += correction;
        }
    }

    self->scratch = 0;
}

} // namespace re5::split120
