#include <cstdint>

namespace re5::split120 {

struct GameRequestState120 {
    std::uint8_t _0000[0xF8];
    std::int32_t request;   // +0xF8
    std::int32_t argument;  // +0xFC
};

// 0x007F1610..0x007F1624 -- VERIFIED exact setter.
// This routine is not itself the request/event receiver: it only stores the
// two values. The consumer must be traced separately from the owner/state
// machine that services this global object.
void SetGameRequest_7F1610(GameRequestState120& self,
                           std::int32_t request,
                           std::int32_t argument)
{
    self.request = request;
    self.argument = argument;
}

// 0x00BF2E6A..0x00BF2E96 -- VERIFIED Mercs/Reunion frontend-owner completion.
// Mode 2 (Mercenaries) emits request 0x0B with argument 0; the paired mode
// emits request 0x1E with argument 0. Both calls target the global object at
// [0x011B1DD8].
void EmitMercsFrontendCompletedRequest_BF2E6A(GameRequestState120& requests,
                                             std::int32_t mode)
{
    SetGameRequest_7F1610(requests, mode == 2 ? 0x0B : 0x1E, 0);
}

// 0x00BF2ED3..0x00BF2EF3 -- VERIFIED paired request immediately before the
// owner creates/registers its child frontend: Mercs uses 0x0C, paired mode
// uses 0x1F, argument 0. This brackets the child lifetime with distinct
// enter/leave requests rather than proving either request directly loads
// gameplay.
void EmitMercsFrontendEnterRequest_BF2ED3(GameRequestState120& requests,
                                         std::int32_t mode)
{
    SetGameRequest_7F1610(requests, mode == 2 ? 0x0C : 0x1F, 0);
}

} // namespace re5::split120
