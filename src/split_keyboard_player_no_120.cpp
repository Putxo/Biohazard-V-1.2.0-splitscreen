#include <cstdint>

namespace re5::split120 {

struct InputManagerKeyboardPlayerView120 {
    std::uint8_t _000[0x614];
    std::int32_t keyboardPlayerNo;
};
static_assert(__builtin_offsetof(InputManagerKeyboardPlayerView120,keyboardPlayerNo)==0x614,"mKeyboardPlayerNo offset");

struct ReflectedIntMemberDescriptor120 {
    const char* name;
    std::uint16_t type;
    std::uint16_t flags;
    InputManagerKeyboardPlayerView120* owner;
    std::int32_t* field;
    std::uint32_t zero10;
    std::uint32_t zero14;
    std::uint32_t zero18;
    std::uint32_t zero1C;
    std::uint32_t zero20;
    std::uint32_t zero24;
};
static_assert(sizeof(ReflectedIntMemberDescriptor120)==0x28,"native descriptor size");

// 0x4384E0 is invoked with ECX=self and one stack argument (registry).
extern void __thiscall RegisterInputManagerBase_4384E0(InputManagerKeyboardPlayerView120* self,void* registry);
// 0x422C00 is invoked with ECX=registry and one stack argument (descriptor).
extern void __thiscall RegisterReflectedDescriptor_422C00(void* registry,const ReflectedIntMemberDescriptor120* descriptor);

// 0x0079A970..0x0079A9CE -- exact mKeyboardPlayerNo reflection registration.
void __thiscall RegisterKeyboardPlayerNo_79A970(InputManagerKeyboardPlayerView120* self,void* registry)
{
    RegisterInputManagerBase_4384E0(self,registry);
    const ReflectedIntMemberDescriptor120 descriptor{
        reinterpret_cast<const char*>(0x00F5FD18),
        0x000A,0,self,&self->keyboardPlayerNo,0,0,0,0,0,0
    };
    RegisterReflectedDescriptor_422C00(registry,&descriptor);
}

const char* KeyboardPlayerNoPropertyName(){return reinterpret_cast<const char*>(0x00F5FD18);}

} // namespace re5::split120
