#include <cstdint>
namespace re5::split120 {
struct GameRoot120;
struct AGameLocalJoin120 { std::uint8_t _00[0x3C]; std::uint32_t cachedBit800; };
extern GameRoot120* gRoot_12340A4;
extern void* gFrontend_11B1D0C;
extern void* gInputAux_1249C44;
extern std::uint32_t ReadRootStateFlags10438(GameRoot120*);
extern void OrRootIndexedFlags_726170(GameRoot120*, std::uint32_t, std::uint8_t);
extern void ClearRootIndexedFlags_7261C0(GameRoot120*, std::uint32_t, std::uint8_t);
extern void EnableLocalSplitFrontend_7A4070(void*);
extern void RefreshLocalInputAux_7A7630(void*);
extern void DisableLocalSplitFrontend_7A4060(void*);

// 0x0071B0E0..0x0071B1DE -- VERIFIED.
// Mirrors root+0x10438 bit 0x800 into self+0x3C and only performs the expensive
// native refresh when that state changes. The disabled branch in the binary
// inlines the same synchronized AND-NOT writes represented by 0x7261C0 here.
void RefreshLocalJoinBit800_71B0E0(AGameLocalJoin120* self) {
    const std::uint32_t bit800 = ReadRootStateFlags10438(gRoot_12340A4) & 0x800u;
    if (self->cachedBit800 == bit800) return;

    if (bit800 != 0) {
        OrRootIndexedFlags_726170(gRoot_12340A4, 0xFFE02000u, 1);
        OrRootIndexedFlags_726170(gRoot_12340A4, 0x00008000u, 1);
        EnableLocalSplitFrontend_7A4070(gFrontend_11B1D0C);
        RefreshLocalInputAux_7A7630(gInputAux_1249C44);
    } else {
        ClearRootIndexedFlags_7261C0(gRoot_12340A4, 0xFFE02000u, 1);
        ClearRootIndexedFlags_7261C0(gRoot_12340A4, 0x001F9000u, 1);
        DisableLocalSplitFrontend_7A4060(gFrontend_11B1D0C);
    }
    self->cachedBit800 = bit800;
}
}
