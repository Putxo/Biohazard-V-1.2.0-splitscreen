#include <cstdint>

namespace re5::split120 {

struct Session120;

struct MercsFrontend120 {
    std::uint8_t _0000[0x14];
    std::uint8_t state;             // +0x14
    std::uint8_t joinSubstate;      // +0x15
    std::uint8_t joinSubstate2;     // +0x16
    std::uint8_t _0017[0x690 - 0x17];
    std::uint32_t playerFlags;      // +0x690
    std::uint32_t playerFlagsCopy;  // +0x694
    std::uint8_t _0698[0x6AC - 0x698];
    std::int32_t localJoinStage;     // +0x6AC
};

static inline Session120* SessionFromRoot_12340A4()
{
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
    return *reinterpret_cast<Session120**>(root + 0x1042C);
}

extern void SessionCopySlotDescriptor_C42AF0(Session120* session,
                                              int slot,
                                              const void* descriptor32);
extern void FinalizeMercsLocalJoin_A44370(MercsFrontend120* self);

// 0xA43E49 and 0xA43F79 -- VERIFIED.
// Every successful local-input assignment path enters frontend state 0x32 and
// clears both byte substates.  State 0x32 is dispatched by A4A4B7 through the
// byte table at A4AF20 to case A4AD13, which calls A48B50 every update.
void EnterMercsJoinState32_A43E49(MercsFrontend120& self)
{
    self.state = 0x32;
    self.joinSubstate = 0;
    self.joinSubstate2 = 0;
}

// 0xC42AF0 -- VERIFIED exact 32-byte per-slot descriptor copier.
// slot must be 0..3; destination = session + 0x4D4 + slot*0x20.
//
// 0xA46F20 -- VERIFIED state-32 substate-0 initialization.  It copies the
// native 32-byte descriptor at 0x10974F4 into session slot 1, snapshots
// self+0x690 to self+0x694, then tail-calls A44370.
void InitializeMercsJ2Slot_A46F20(MercsFrontend120& self)
{
    const auto* nativeSlot1Descriptor = reinterpret_cast<const void*>(0x010974F4);
    SessionCopySlotDescriptor_C42AF0(SessionFromRoot_12340A4(),
                                     1,
                                     nativeSlot1Descriptor);
    self.playerFlagsCopy = self.playerFlags;
    FinalizeMercsLocalJoin_A44370(&self);
}

// 0xA48B50..0xA48B88 -- VERIFIED state-32 substate dispatcher.
// Jump table A48B8C contains:
//   substate 0 -> A48B77 -> A46F20
//   substate 1 -> A48B87 (return)
//   substate 2 -> A48B87 (return)
//   substate 3 -> A48B87 (return)
//   substate 4 -> A48B7F -> A44370
// Any value >4 also returns.
void UpdateMercsJoinState32_A48B50(MercsFrontend120& self)
{
    switch (self.joinSubstate) {
        case 0:
            InitializeMercsJ2Slot_A46F20(self);
            break;
        case 4:
            FinalizeMercsLocalJoin_A44370(&self);
            break;
        default:
            break;
    }
}

} // namespace re5::split120
