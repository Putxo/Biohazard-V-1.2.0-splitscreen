#include "re5/split_localcoop_120.hpp"

#include <cstdint>

namespace re5::split120 {

struct PlayerStateOwner120 {
    std::uint8_t _00[0x5C];
    std::int32_t state; // +0x5C
};

// 0x00CEE1B0..0x00CEE1CD -- VERIFIED.
// thiscall: ECX=self, one 4-byte stack argument, RET 4.
//
// Direct 1.2.0 control flow:
//   if ((byte)arg != 0) return self->state;
//   if (split->transientSplitFlag != 0) return 1;
//   return self->state;
//
// The special forced value therefore applies only to selector/argument zero
// during the transient split state.
std::int32_t PlayerStateAccessor_CEE1B0(
    const PlayerStateOwner120* self,
    std::uint32_t selector,
    const SplitRenderState120* split)
{
    if (static_cast<std::uint8_t>(selector) == 0 &&
        split->transientSplitFlag != 0) {
        return 1;
    }

    return self->state;
}

} // namespace re5::split120
