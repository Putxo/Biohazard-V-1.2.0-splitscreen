#pragma once

#include <cstdint>

namespace re5::split120 {

struct AGame120 {
    std::uint8_t _00[0x350];
    std::int32_t pendingDevice;       // +0x350
    std::int32_t pendingSlot;         // +0x354
};

struct SplitRenderState120 {
    std::uint8_t _00[0x3008];
    void* secondarySplitResource;     // +0x3008
    std::uint8_t _300C[0x3064 - 0x300C];
    std::uint8_t splitActive;         // +0x3064
    std::uint8_t _3065[3];
    std::int32_t splitI0;             // +0x3068
    std::int32_t splitI1;             // +0x306C
    float splitScale;                 // +0x3070
    float splitParam;                 // +0x3074
    float splitScale2;                // +0x3078
    float splitOffset;                // +0x307C
    float splitVertical;              // +0x3080
    std::uint8_t fullScreenSplitMode; // +0x3084
    std::uint8_t transientSplitFlag;  // +0x3085
};

bool IsFullSplitEffective(const SplitRenderState120* self);
void SetLocalPlayerActive(bool active, int slot, int device);
void BeginAddPlayer(AGame120* self, int slot, int device, bool sourceIsKeyboard);
void PollLocalJoin_723460(AGame120* self);
void RecalculateSplitGeometry_76C0C0(SplitRenderState120* self);

} // namespace re5::split120
