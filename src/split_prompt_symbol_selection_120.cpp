#include <cstdint>

namespace re5::split120 {

extern int MapOwnedLocalUiCommand_79A570(int command,int logicalPlayer);

static inline std::uint16_t PromptCode0_10E9C60(){return *reinterpret_cast<const std::uint16_t*>(0x010E9C60);}
static inline std::uint16_t PromptCode1_10E9C64(){return *reinterpret_cast<const std::uint16_t*>(0x010E9C64);}

struct PromptDestination120 {
    std::uint8_t _00[0x40];
    std::uint32_t flags40;
    std::uint8_t _44[0x60-0x44];
    std::uint16_t promptCode60;
};
static_assert(__builtin_offsetof(PromptDestination120,flags40)==0x40,"prompt flags");
static_assert(__builtin_offsetof(PromptDestination120,promptCode60)==0x60,"prompt code");

// 0x007DA67C..0x007DA6CA -- exact local-player prompt-symbol selection block.
// Selector 0 replaces destination+0x60 with native code 2; selector 1 replaces
// it with native code 3. Any other selector leaves destination+0x60 untouched.
// In all cases the resulting/current destination code is then routed through
// 0x79A570 for the requested logical player (2/3 may become 0x2A/0x32).
std::uint16_t ApplyLocalPromptSymbolSelection_7DA67C(PromptDestination120* dst,
                                                     std::uint16_t sourceSelector,
                                                     int logicalPlayer)
{
    dst->flags40 |= 0x40000000u;
    if(sourceSelector==0)
        dst->promptCode60=PromptCode0_10E9C60();
    else if(sourceSelector==1)
        dst->promptCode60=PromptCode1_10E9C64();

    const std::uint16_t routed=static_cast<std::uint16_t>(
        MapOwnedLocalUiCommand_79A570(static_cast<int>(dst->promptCode60),logicalPlayer));
    dst->promptCode60=routed;
    return routed;
}

} // namespace re5::split120
