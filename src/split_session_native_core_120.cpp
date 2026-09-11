#include <cstdint>

namespace re5::split120 {

// Canonical opaque session layout slices used by the native local-coop core.
// Keeping the implementation on raw offsets avoids C++ type-overload symbols
// for the same executable VA.

// 0x00C42A30..0x00C42A4D -- ECX=session, stack slot/value, RET 8.
void __thiscall SessionSetSlotMode_C42A30(void* session,int slot,int value)
{
    if(static_cast<unsigned>(slot)>3u)return;
    auto* b=static_cast<std::uint8_t*>(session);
    *reinterpret_cast<std::int32_t*>(b+0x480+slot*4)=value;
}

// 0x00C42A50..0x00C42A6D -- ECX=session, stack slot/device, RET 8.
void __thiscall SessionSetDevice_C42A50(void* session,int slot,int device)
{
    if(static_cast<unsigned>(slot)>3u)return;
    auto* b=static_cast<std::uint8_t*>(session);
    *reinterpret_cast<std::int32_t*>(b+0x490+slot*4)=device;
}

// 0x00C42D90..0x00C42D96 -- ECX=session, no stack arguments.
// Native body: MOV EAX,[ECX+558] / RET.
int __thiscall QueryGameStatus_C42D90(void* session)
{
    return *reinterpret_cast<const std::int32_t*>(
        static_cast<const std::uint8_t*>(session)+0x558);
}

} // namespace re5::split120
