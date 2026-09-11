#include <cstdint>

namespace re5::split120 {

struct InputManagerKeyboardPlayerView120 {
    std::uint8_t _000[0x614];
    std::int32_t keyboardPlayerNo;
};
static_assert(__builtin_offsetof(InputManagerKeyboardPlayerView120,keyboardPlayerNo)==0x614,"mKeyboardPlayerNo offset");

// Native 0x79A970 first forwards the registry argument through 0x4384E0,
// then builds a 0x28-byte member descriptor on the stack and passes it to
// registry->0x422C00. Keep the descriptor layout explicit rather than using
// the earlier invented high-level registration signature.
struct ReflectedIntMemberDescriptor120 {
    const char* name;                   // +0x00
    std::uint16_t type;                 // +0x04 = 0x000A
    std::uint16_t flags;                // +0x06 = 0
    InputManagerKeyboardPlayerView120* owner; // +0x08
    std::int32_t* field;                // +0x0C = self+0x614
    std::uint32_t zero10;               // +0x10
    std::uint32_t zero14;               // +0x14
    std::uint32_t zero18;               // +0x18
    std::uint32_t zero1C;               // +0x1C
    std::uint32_t zero20;               // +0x20
    std::uint32_t zero24;               // +0x24
};
static_assert(sizeof(ReflectedIntMemberDescriptor120)==0x28,"native descriptor size");

extern void RegisterInputManagerBase_4384E0(InputManagerKeyboardPlayerView120* self, void* registry);
extern void RegisterReflectedDescriptor_422C00(void* registry, const ReflectedIntMemberDescriptor120* descriptor);

// 0x0079A970..0x0079A9CE -- exact mKeyboardPlayerNo reflection registration.
// Native property-name pointer is 0x00F5FD18 and resolves to
// "mKeyboardPlayerNo". The registered field is exactly InputManager+0x614.
void RegisterKeyboardPlayerNo_79A970(InputManagerKeyboardPlayerView120* self, void* registry)
{
    RegisterInputManagerBase_4384E0(self, registry);

    const ReflectedIntMemberDescriptor120 descriptor{
        "mKeyboardPlayerNo",
        0x000A,
        0,
        self,
        &self->keyboardPlayerNo,
        0,0,0,0,0,0
    };
    RegisterReflectedDescriptor_422C00(registry, &descriptor);
}

const char* KeyboardPlayerNoPropertyName(){return reinterpret_cast<const char*>(0x00F5FD18);}

} // namespace re5::split120
