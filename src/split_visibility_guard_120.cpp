#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

struct VisibilityWidget120 {
    std::uint8_t _00[0x08];
    std::uint8_t visibleFlag; // +0x08
};

struct VisibilityOwner120 {
    std::uint8_t _00[0x30F0];
    std::int32_t type; // +0x30F0
};

// 0x009F2A80..0x009F2A9C -- VERIFIED split-specific branch.
//
// Direct 1.2.0 instructions:
//   cmp dword ptr [eax+0x30F0],8
//   jne skip
//   mov ecx,[0x123457C]
//   cmp byte ptr [ecx+0x3085],0
//   jne skip
//   mov byte ptr [esi+0x08],0
//
// Therefore the widget byte is cleared only for type==8 while the transient
// split flag is zero. A nonzero +0x3085 deliberately suppresses the clear.
void ApplyTransientVisibilityGuard_9F2A80(
    VisibilityWidget120* widget,
    const VisibilityOwner120* returnedObject,
    const SplitRenderState120* split)
{
    if (returnedObject == nullptr)
        return;

    if (returnedObject->type != 8)
        return;

    if (split->transientSplitFlag != 0)
        return;

    widget->visibleFlag = 0;
}

} // namespace re5::split120
