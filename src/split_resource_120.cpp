#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gDisplay_12345D4;
extern int __thiscall GetRuntimeStatus_C42D90(void* session);
extern void* NativeAlignedAlloc_11AF294(std::uint32_t bytes, std::uint32_t alignment);
extern SplitResource120* ConstructSplitResource_B8D6A0(void* storage);
extern void RegisterSplitResource_428B80(int id, SplitResource120* resource);
extern void VirtualReleaseAtVtable30_120(SplitResource120* resource);

static inline void* Session(){return *reinterpret_cast<void**>(gRoot_12340A4+0x1042C);}

// 0x0076C1F0..0x0076C29F -- exact split activation/resource block.
void UpdateSplitActivationAndResource_76C1F0(SplitRenderState120* self)
{
    const int width=*reinterpret_cast<const int*>(gDisplay_12345D4+0x50);
    const int height=*reinterpret_cast<const int*>(gDisplay_12345D4+0x54);
    const float aspect=static_cast<float>(height)/static_cast<float>(width);
    self->splitActive=0;
    if(aspect>=0.5625f)return;
    if(GetRuntimeStatus_C42D90(Session())==7&&aspect>=0.4250000119f)return;
    self->splitActive=1;
    if(self->secondarySplitResource!=nullptr)return;
    void* storage=NativeAlignedAlloc_11AF294(0x230,0x10);
    SplitResource120* resource=storage?ConstructSplitResource_B8D6A0(storage):nullptr;
    self->secondarySplitResource=resource;
    if(resource==nullptr)return;
    resource->flags|=0x2000u;
    RegisterSplitResource_428B80(0x1E,resource);
}

void ReleaseSecondarySplitResource(SplitRenderState120* self)
{
    SplitResource120* resource=self->secondarySplitResource;
    if(resource==nullptr)return;
    resource->flags&=~0x2000u;
    VirtualReleaseAtVtable30_120(resource);
    self->secondarySplitResource=nullptr;
}

} // namespace re5::split120
