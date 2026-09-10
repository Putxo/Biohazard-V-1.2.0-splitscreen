#include <cstdint>
namespace re5::split120 {
struct GameRoot120;
extern GameRoot120* gRoot_12340A4;
extern void OrRootIndexedFlags_726170(GameRoot120*, std::uint32_t, std::uint8_t);
extern void ClearRootIndexedFlags_7261C0(GameRoot120*, std::uint32_t, std::uint8_t);

// 0x0071AD10..0x0071AD74 -- VERIFIED.
// Native inlines the synchronized clear path when disabled; semantically this
// is the exact set/clear route for indexed root word 1, mask 0xFFE02000.
void SetRootIndex1FFE02000_71AD10(bool enabled) {
    if (enabled) OrRootIndexedFlags_726170(gRoot_12340A4, 0xFFE02000u, 1);
    else ClearRootIndexedFlags_7261C0(gRoot_12340A4, 0xFFE02000u, 1);
}

// 0x0071AD80..0x0071ADE4 -- VERIFIED.
// Same synchronized route for indexed root word 1, mask 0x001F9000.
void SetRootIndex1_1F9000_71AD80(bool enabled) {
    if (enabled) OrRootIndexedFlags_726170(gRoot_12340A4, 0x001F9000u, 1);
    else ClearRootIndexedFlags_7261C0(gRoot_12340A4, 0x001F9000u, 1);
}

// 0x0071ADF0..0x0071AE54 -- VERIFIED.
// Same synchronized route for indexed root word 1, mask 0x00008000.
void SetRootIndex1_8000_71ADF0(bool enabled) {
    if (enabled) OrRootIndexedFlags_726170(gRoot_12340A4, 0x00008000u, 1);
    else ClearRootIndexedFlags_7261C0(gRoot_12340A4, 0x00008000u, 1);
}
}
