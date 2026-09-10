#include <cstdint>
namespace re5::split120 {
struct SessionResetView120 { std::uint8_t bytes[0x660]; };
extern std::uint8_t* gInput_1249C40;
extern void SessionSetSlotMode_C42A30(void*,int,int);
extern void SessionSetDevice_C42A50(void*,int,int);
extern void SessionField_C42A70(void*,int,int);
extern void SessionField_C42A90(void*,int,int);
extern void SessionField_C42AB0(void*,int,int);
extern void SessionPtr_C42AF0(void*,int,const void*);
extern void SessionField_C42AD0(void*,int,int);
extern void SessionField_C42D70(void*,int,int);
extern void SessionField_C42F10(void*,int);
// 0x00BF2670..0x00BF28E3 -- VERIFIED local/session reset core.
// This source reconstructs the fields which own native local-player routing;
// unrelated service/network reset calls at the function head are intentionally
// represented only by their resulting session fields.
void ResetSessionLocalBindings_BF2670(SessionResetView120* session) {
    auto* b=session->bytes;
    *reinterpret_cast<std::int32_t*>(b+0x578)=0;
    *reinterpret_cast<std::int32_t*>(b+0x574)=0;
    *reinterpret_cast<std::int32_t*>(b+0x64)=0;
    *reinterpret_cast<std::int32_t*>(b+0x5C8)=0;
    *reinterpret_cast<std::int32_t*>(b+0x5CC)=0;
    *reinterpret_cast<std::int32_t*>(b+0x5D0)=0;
    *reinterpret_cast<std::int32_t*>(b+0x5D4)=0;
    *reinterpret_cast<std::int32_t*>(b+0x5DC)=0;
    *reinterpret_cast<std::int32_t*>(b+0x654)=-1;
    *reinterpret_cast<std::int32_t*>(b+0x658)=-1;
    for(int slot=0;slot<4;++slot){
        SessionSetSlotMode_C42A30(session,slot,1);
        SessionSetDevice_C42A50(session,slot,-1);
        SessionField_C42A70(session,slot,0);
        SessionField_C42A90(session,slot,0);
        SessionField_C42AB0(session,slot,0);
        SessionPtr_C42AF0(session,slot,reinterpret_cast<const void*>(0x010974F4));
        SessionField_C42AD0(session,slot,0);
        SessionField_C42D70(session,slot,0);
        SessionField_C42F10(session,slot);
        *reinterpret_cast<std::int32_t*>(b+0x610+slot*4)=6;
        *reinterpret_cast<std::int32_t*>(b+0x630+slot*4)=0;
        *reinterpret_cast<std::int32_t*>(b+0x620+slot*4)=0;
    }
    *reinterpret_cast<std::int32_t*>(gInput_1249C40+0x614)=0;
    const int preferred=*reinterpret_cast<const std::int32_t*>(gInput_1249C40+0x5D8);
    SessionSetDevice_C42A50(session,0,preferred);
}
}
