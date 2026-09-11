#pragma once

#include <cstdint>

namespace re5::split120 {

struct AGame120 {
    std::uint8_t _00[0x350];
    std::int32_t pendingDevice; // +0x350
    std::int32_t pendingSlot;   // +0x354
};
static_assert(__builtin_offsetof(AGame120,pendingDevice)==0x350,"AGame pending device");
static_assert(__builtin_offsetof(AGame120,pendingSlot)==0x354,"AGame pending slot");

struct SplitResource120 {
    std::uint32_t vtable;
    std::uint32_t flags;
    std::uint8_t _08[0x230 - 0x08];
};

// Canonical RE5DX9 1.2.0 split renderer view. These offsets are referenced
// directly throughout the executable and are locked here so every translation
// unit including this header is compile-time checked against the same layout.
struct SplitRenderState120 {
    std::uint8_t _00[0x3008];
    SplitResource120* secondarySplitResource; // +0x3008
    std::uint8_t _300C[0x3064 - 0x300C];
    std::uint8_t splitActive;                  // +0x3064
    std::uint8_t _3065[3];
    std::int32_t splitI0;                      // +0x3068
    std::int32_t splitI1;                      // +0x306C
    float splitScale;                          // +0x3070
    float splitParam;                          // +0x3074
    float splitScale2;                         // +0x3078
    float splitOffset;                         // +0x307C
    float splitVertical;                       // +0x3080
    std::uint8_t fullScreenSplitMode;          // +0x3084
    std::uint8_t transientSplitFlag;           // +0x3085
};
static_assert(__builtin_offsetof(SplitRenderState120,secondarySplitResource)==0x3008,"split secondary resource");
static_assert(__builtin_offsetof(SplitRenderState120,splitActive)==0x3064,"split active");
static_assert(__builtin_offsetof(SplitRenderState120,splitI0)==0x3068,"split i0");
static_assert(__builtin_offsetof(SplitRenderState120,splitI1)==0x306C,"split i1");
static_assert(__builtin_offsetof(SplitRenderState120,splitScale)==0x3070,"split scale");
static_assert(__builtin_offsetof(SplitRenderState120,splitParam)==0x3074,"split parameter");
static_assert(__builtin_offsetof(SplitRenderState120,splitScale2)==0x3078,"split scale2");
static_assert(__builtin_offsetof(SplitRenderState120,splitOffset)==0x307C,"split x offset");
static_assert(__builtin_offsetof(SplitRenderState120,splitVertical)==0x3080,"split vertical");
static_assert(__builtin_offsetof(SplitRenderState120,fullScreenSplitMode)==0x3084,"full split mode");
static_assert(__builtin_offsetof(SplitRenderState120,transientSplitFlag)==0x3085,"transient split flag");

bool IsFullSplitEffective(const SplitRenderState120* self);
void SetLocalPlayerActive(bool active, int slot, int device);
void BeginAddPlayer(AGame120* self, int slot, int device, bool sourceIsKeyboard);
void __thiscall PollLocalJoin_723460(AGame120* self);
void RecalculateSplitGeometry_76C0C0(SplitRenderState120* self);
void UpdateSplitActivationAndResource_76C1F0(SplitRenderState120* self);
void ReleaseSecondarySplitResource(SplitRenderState120* self);

void SplitDrawPrimary_A23A00(void* self, int itemIndex);
void DrawSecondarySplitMenu_A23B40(void* self);
void SplitTop22_A24850(void* self);

} // namespace re5::split120
