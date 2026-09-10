#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// 0x0076C1F0 — PARTIAL semantic reconstruction.
//
// Confirmed behavior from the 1.2.0 binary analysis used by the previous
// split-screen port:
//   * recomputes whether split rendering is active;
//   * the general aspect test is aspect < 0.5625;
//   * status 7 has an additional/special 0.425 threshold path;
//   * writes the result to SplitRenderState120::splitActive (+0x3064);
//   * when split becomes active and +0x3008 is null, allocates a native
//     0x230-byte resource, constructs it, sets flag 0x2000 in resource+4,
//     stores it in +0x3008, and registers it with the native UI/resource
//     manager using id 0x1E.
//
// The exact status/aspect getter call graph is deliberately kept abstract
// until its callees are decompiled; no unproven names are hard-coded here.

extern float GetCurrentAspectRatio_120();
extern int GetSplitStatus_120();
extern bool EvaluateStatus7SplitGate_120(float aspect, float threshold);
extern void* NativeAlloc_120(std::uint32_t bytes);
extern SplitResource120* ConstructSplitResource_120(void* storage);
extern void RegisterSplitResource_120(SplitResource120* resource, int id);

static bool ComputeSplitActive_76C1F0()
{
    const float aspect = GetCurrentAspectRatio_120();
    const int status = GetSplitStatus_120();

    if (status == 7)
        return EvaluateStatus7SplitGate_120(aspect, 0.425f);

    return aspect < 0.5625f;
}

void UpdateSplitActivationAndResource_76C1F0(SplitRenderState120* self)
{
    const bool active = ComputeSplitActive_76C1F0();
    self->splitActive = active ? 1 : 0;

    if (!active || self->secondarySplitResource != nullptr)
        return;

    void* storage = NativeAlloc_120(0x230);
    if (storage == nullptr)
        return;

    SplitResource120* resource = ConstructSplitResource_120(storage);
    if (resource == nullptr)
        return;

    self->secondarySplitResource = resource;
    resource->flags |= 0x2000u;
    RegisterSplitResource_120(resource, 0x1E);
}

// Cleanup counterpart for the +0x3008 resource.
// Confirmed lifecycle behavior: clear 0x2000, call the object's virtual
// release method at vtable +0x30, then clear the owning pointer.
extern void VirtualReleaseAtVtable30_120(SplitResource120* resource);

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
