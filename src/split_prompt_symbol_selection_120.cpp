#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gInput_1249C40;
extern int MapOwnedLocalUiCommand_79A570(int command,int logicalPlayer);

// Native global prompt codes read by 0x7DA699/0x7DA6A8.
// The words at 0x10E9C60 and 0x10E9C64 are respectively 2 and 3.
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
// ESI is the destination record. The source action record supplies a small
// code: value 0 selects native word 0x10E9C60 (2), value 1 selects native word
// 0x10E9C64 (3). The selected code is then passed through 0x79A570 with the
// current logical-player selector before being stored at destination+0x60.
// 0x79A570 is the native owner-aware conversion that can turn 2/3 into
// 0x2A/0x32, which is the executable-side START/ESC/controller-symbol routing.
std::uint16_t ApplyLocalPromptSymbolSelection_7DA67C(PromptDestination120* dst,
                                                     std::uint16_t sourceSelector,
                                                     int logicalPlayer)
{
    dst->flags40 |= 0x40000000u;

    std::uint16_t code = sourceSelector;
    if(sourceSelector==0)
        code=PromptCode0_10E9C60();
    else if(sourceSelector==1)
        code=PromptCode1_10E9C64();

    const std::uint16_t routed=static_cast<std::uint16_t>(
        MapOwnedLocalUiCommand_79A570(static_cast<int>(code),logicalPlayer));
    dst->promptCode60=routed;
    return routed;
}

} // namespace re5::split120
