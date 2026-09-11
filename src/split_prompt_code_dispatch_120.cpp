#include <cstdint>

namespace re5::split120 {

extern std::uint16_t ResolveRoutedPromptCode_7D7500(void* self,
                                                     std::uint16_t code,
                                                     int logicalSelection);

// 0x007DA708..0x007DA9D3 -- split/local-player prompt-code selection block.
//
// Native marks the destination record with bit 0x40000000 at +0x40, decodes
// the source action id (0..0x27), writes the selected prompt code at +0x60,
// then routes every case through 0x7D7500.  Action ids 21..34 and ids >39
// preserve the pre-existing code.  Action id 2 uses the dynamic BX value.
//
// This helper isolates the prompt-code/routing semantics.  The caller's
// +0x40 flag write and the final store to its +0x60 record remain parent
// record operations rather than input-routing semantics.
std::uint16_t ResolveLocalPromptActionCode_7DA708(
    void* promptOwner,
    int actionId,
    std::uint16_t dynamicCodeForAction2,
    std::uint16_t currentCode,
    int logicalSelection)
{
    std::uint16_t code = currentCode;

    switch (actionId) {
    case 0:  code = 0x02; break;
    case 1:  code = 0x03; break;
    case 2:  code = dynamicCodeForAction2; break;
    case 3:  code = 0x05; break;
    case 4:  code = 0x0C; break;
    case 5:  code = 0x0D; break;
    case 6:  code = 0x0B; break;
    case 7:  code = 0x0A; break;
    case 8:  code = 0x06; break;
    case 9:  code = 0x07; break;
    case 10: code = 0x08; break;
    case 11: code = 0x09; break;
    case 12: code = 0x0E; break;
    case 13: code = 0x13; break;
    case 14: code = 0x14; break;
    case 15: code = 0x0F; break;
    case 16: code = 0x10; break;
    case 17: code = 0x11; break;
    case 18: code = 0x12; break;
    case 19: code = 0x15; break;
    case 20: code = 0x16; break;
    case 35: code = 0x23; break;
    case 36: code = 0x24; break;
    case 37: code = 0x25; break;
    case 38: code = 0x26; break;
    case 39: code = 0x27; break;
    default: break;
    }

    return ResolveRoutedPromptCode_7D7500(promptOwner, code, logicalSelection);
}

} // namespace re5::split120
