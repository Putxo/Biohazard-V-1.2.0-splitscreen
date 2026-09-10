#include <cstdint>
namespace re5::split120 {
struct InputManagerKeyboardPlayerView120 { std::uint8_t _000[0x614]; std::int32_t keyboardPlayerNo; };
static_assert(__builtin_offsetof(InputManagerKeyboardPlayerView120,keyboardPlayerNo)==0x614,"mKeyboardPlayerNo offset");
extern void RegisterReflectedMember_422C00(void*,const void*,void*,unsigned short);
// 0x0079A970 family; exact name xref at 0x0079A9B1. VERIFIED identity: mKeyboardPlayerNo points to InputManager+0x614, the same field used by Story/Mercs player routing.
void RegisterKeyboardPlayerNo_79A970(InputManagerKeyboardPlayerView120* self,void* owner,const void* descriptor){RegisterReflectedMember_422C00(owner,descriptor,&self->keyboardPlayerNo,0x000A);}
const char* KeyboardPlayerNoPropertyName(){return reinterpret_cast<const char*>(0x00F5FD18);}
} // namespace re5::split120
