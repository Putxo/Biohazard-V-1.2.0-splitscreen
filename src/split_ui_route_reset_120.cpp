#include <cstdint>

namespace re5::split120 {

struct UiRouteManager120 {
    std::uint8_t bytes[0x2F9];
};

static inline std::int32_t& I32(std::uint8_t* p, std::uint32_t off)
{
    return *reinterpret_cast<std::int32_t*>(p + off);
}

// 0x007B43C0 -- VERIFIED instruction-level reconstruction of the route-range
// reset used by the Mercenaries/Reunion completion path.
//
// Native ABI: ECX=manager, arg0 is copied as a byte into each route record,
// arg1=startSlot, RET 8. Before touching the records the native function calls
// 0x799B50(input); if that gate fails it returns immediately. It then calls
// 0x768C10 and clears manager+0x2F8 when that overlay/state is present.
//
// The loop layout is exact: first record at manager+0x20, stride 0x70, and the
// current UI origin globals 0x12B0468 / 0x12B046C are copied to record+0x30/+0x34.
void ResetUiRouteRange_7B43C0(UiRouteManager120& self,
                              std::uint8_t routeByte,
                              std::int32_t startSlot,
                              bool gate799B50,
                              bool overlay768C10,
                              std::int32_t globalUiX,
                              std::int32_t globalUiY)
{
    if (!gate799B50)
        return;

    if (overlay768C10)
        self.bytes[0x2F8] = 0;

    if (startSlot >= 4)
        return;

    // Native callers supply a valid slot index in the range 0..3.
    for (std::int32_t slot = startSlot; slot < 4; ++slot) {
        std::uint8_t* r = self.bytes + 0x20 + slot * 0x70;

        r[0x00] = routeByte;
        I32(r, 0x28) = 0;
        I32(r, 0x10) = -1;
        I32(r, 0x14) = -1;
        I32(r, 0x1C) = -1;
        I32(r, 0x08) = -1;
        I32(r, 0x0C) = -1;
        I32(r, 0x68) = -1;
        I32(r, 0x64) = -1;
        I32(r, 0x5C) = -1;
        I32(r, 0x60) = -1;
        I32(r, 0x30) = globalUiX;
        I32(r, 0x34) = globalUiY;
    }
}

} // namespace re5::split120
