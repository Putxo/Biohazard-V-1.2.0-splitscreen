#pragma once

#include <cstdint>

namespace re5::split120 {

struct AGame120 {
    std::uint8_t _00[0x350];
    std::int32_t pendingDevice;
    std::int32_t pendingSlot;
};

struct SplitResource120 {
    std::uint32_t vtable;
    std::uint32_t flags;
    std::uint8_t _08[0x230 - 0x08];
};

struct SplitRenderState120 {
    std::uint8_t _00[0x3008];
    SplitResource120* secondarySplitResource;
    std::uint8_t _300C[0x3064 - 0x300C];
    std::uint8_t splitActive;
    std::uint8_t _3065[3];
    std::int32_t splitI0;
    std::int32_t splitI1;
    float splitScale;
    float splitParam;
    float splitScale2;
    float splitOffset;
    float splitVertical;
    std::uint8_t fullScreenSplitMode;
    std::uint8_t transientSplitFlag;
};

bool IsFullSplitEffective(const SplitRenderState120* self);
void SetLocalPlayerActive(bool active, int slot, int device);
void BeginAddPlayer(AGame120* self, int slot, int device, bool sourceIsKeyboard);
void PollLocalJoin_723460(AGame120* self);
void RecalculateSplitGeometry_76C0C0(SplitRenderState120* self);
void UpdateSplitActivationAndResource_76C1F0(SplitRenderState120* self);
void ReleaseSecondarySplitResource(SplitRenderState120* self);

void SplitDrawPrimary_A23A00(void* self, int itemIndex);
void DrawSecondarySplitMenu_A23B40(void* self);
void SplitTop22_A24850(void* self);

} // namespace re5::split120
