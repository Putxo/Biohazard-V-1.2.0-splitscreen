#include <cstdint>

namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t InputOwnerModeGate_799AF0(void*,int);

struct SplitDrawOwner120 {
    std::uint8_t _00[0x34];
    std::int32_t owner34;
};

// 0x009E5876..0x009E58A6 -- VERIFIED exact ownership preguard of 0x9E5870.
// 1) global/owner input gate must accept player -1;
// 2) the draw object must belong to current mKeyboardPlayerNo (+0x614),
//    unless the object explicitly uses owner -1 (global/unowned).
bool SplitTableOwnerPreguard_9E5876(const SplitDrawOwner120* self)
{
    if (InputOwnerModeGate_799AF0(gInput_1249C40,-1) != 0)
        return false;
    const int keyboardPlayer = *reinterpret_cast<const std::int32_t*>(
        gInput_1249C40 + 0x614);
    return self->owner34 == keyboardPlayer || self->owner34 == -1;
}

} // namespace re5::split120
