#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

extern bool LocalPlayerTransitionReady_715BB0(void* self);

// 0x00716810..0x0071688D -- direct reconstruction.
// Native 1.2.0 refuses this lifecycle path unless session+0x58 == 0.
void UpdateStoryLocalPlayerTransition_716810(void* selfRaw)
{
    auto* self = static_cast<std::uint8_t*>(selfRaw);
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);

    if ((*(root + 0x10434) & 0x03u) != 0)
        return;

    auto* session = *reinterpret_cast<std::uint8_t**>(root + 0x1042C);
    if (*reinterpret_cast<std::int32_t*>(session + 0x58) != 0)
        return;

    const std::int32_t phase = *reinterpret_cast<std::int32_t*>(session + 0x2C);
    const std::uint8_t lowFlags = *(root + 0x10438);

    std::uint32_t gateFlags = 0;
    if ((lowFlags & 0x80u) != 0)
        gateFlags |= 1u;
    if ((lowFlags & 0x40u) != 0)
        gateFlags |= 2u;

    auto& latched = *(self + 0x358);
    if (latched == 0) {
        if (gateFlags != 0 && phase == 2)
            latched = 1;
        return;
    }

    if (phase != 2) {
        latched = 0;
        return;
    }

    if (LocalPlayerTransitionReady_715BB0(selfRaw) && gateFlags == 0) {
        *reinterpret_cast<std::uint32_t*>(self + 0x04) = 0;
        *(self + 0x04) = 0x0A;
    }
}

} // namespace re5::split120
