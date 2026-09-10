#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

struct DisplaySize120_76C1F0 {
    int width;
    int height;
};

extern DisplaySize120_76C1F0 GetCurrentDisplaySize_12345D4();
extern int GetRuntimeStatus_C42D90();
extern void* NativeAlignedAlloc_11AF294(std::uint32_t bytes, std::uint32_t alignment);
extern SplitResource120* ConstructSplitResource_B8D6A0(void* storage);
extern void RegisterSplitResource_428B80(int id, SplitResource120* resource);
extern void VirtualReleaseAtVtable30_120(SplitResource120* resource);

// 0x0076C1F0..0x0076C29F -- VERIFIED against direct 1.2.0 disassembly.
//
// The activation gate is exactly:
//   aspect = physicalHeight / physicalWidth
//   active when aspect < 0.5625
//   except runtime status 7 requires the narrower aspect < 0.425
//
// When activation succeeds and +0x3008 is null, native code allocates
// 0x230 bytes aligned to 0x10, constructs with 0xB8D6A0, sets bit 0x2000
// in resource+4, stores it in +0x3008 and registers id 0x1E.
void UpdateSplitActivationAndResource_76C1F0(SplitRenderState120* self)
{
    const DisplaySize120_76C1F0 display = GetCurrentDisplaySize_12345D4();
    const float aspect = static_cast<float>(display.height) /
                         static_cast<float>(display.width);

    self->splitActive = 0;

    if (aspect >= 0.5625f)
        return;

    if (GetRuntimeStatus_C42D90() == 7 && aspect >= 0.4250000119f)
        return;

    self->splitActive = 1;

    if (self->secondarySplitResource != nullptr)
        return;

    void* storage = NativeAlignedAlloc_11AF294(0x230, 0x10);
    SplitResource120* resource = nullptr;
    if (storage != nullptr)
        resource = ConstructSplitResource_B8D6A0(storage);

    self->secondarySplitResource = resource;
    if (resource == nullptr)
        return;

    resource->flags |= 0x2000u;
    RegisterSplitResource_428B80(0x1E, resource);
}

// Cleanup counterpart observed elsewhere in the same lifecycle family.
void ReleaseSecondarySplitResource(SplitRenderState120* self)
{
    SplitResource120* resource = self->secondarySplitResource;
    if (resource == nullptr)
        return;

    resource->flags &= ~0x2000u;
    VirtualReleaseAtVtable30_120(resource);
    self->secondarySplitResource = nullptr;
}

} // namespace re5::split120
