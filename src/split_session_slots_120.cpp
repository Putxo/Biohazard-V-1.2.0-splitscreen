#include <cstdint>

namespace re5::split120 {

struct SessionSlotState120 {
    std::uint8_t _0000[0x478];
    std::uint32_t availableMask; // +0x478
    std::uint32_t activeMask;    // +0x47C
    std::int32_t slotMode[4];    // +0x480
    std::int32_t device[4];      // +0x490
    std::int32_t localPair[4];   // +0x4A0
    std::int32_t partnerPair[4]; // +0x4B0
    std::int32_t auxPair[4];     // +0x4C0
    std::uint8_t _4D0[0x88];
    std::int32_t displayStatus;  // +0x558
    std::int32_t routeSlot[4];   // +0x55C
    std::uint8_t localModeFlag;  // +0x56C
    std::uint8_t _56D[3];
    std::int32_t transition570;  // +0x570
    std::int32_t transition574;  // +0x574
    std::int32_t transition578;  // +0x578
    std::uint32_t pendingMask;   // +0x57C
    std::int32_t transition580;  // +0x580
};
static_assert(__builtin_offsetof(SessionSlotState120,availableMask)==0x478,"available mask");
static_assert(__builtin_offsetof(SessionSlotState120,activeMask)==0x47C,"active mask");
static_assert(__builtin_offsetof(SessionSlotState120,device)==0x490,"slot devices");
static_assert(__builtin_offsetof(SessionSlotState120,displayStatus)==0x558,"display status");
static_assert(__builtin_offsetof(SessionSlotState120,pendingMask)==0x57C,"pending mask");

extern std::uint8_t* gInput_1249C40;
extern void* gSplitState_123457C;
extern void __thiscall SetSplitPlayerFlag_76A140(void* split,int value,int force);

static inline SessionSlotState120* S(void* p){return static_cast<SessionSlotState120*>(p);}

// 0x00C43A00..0x00C43BA1 -- exact local/session initialization facts used by
// J1/J2 routing. This is a thiscall with no preferred-device stack argument.
void __thiscall InitLocalCoopSessionSlots_C43A00(SessionSlotState120* s)
{
    s->availableMask=0x3;
    for(int i=0;i<4;++i){
        s->slotMode[i]=1;
        s->device[i]=-1;
        s->localPair[i]=0;
        s->partnerPair[i]=0;
        s->auxPair[i]=0;
    }
    s->slotMode[0]=0;
    s->device[0]=*reinterpret_cast<const int*>(gInput_1249C40+0x5D8);
    s->activeMask=0x1;
    s->displayStatus=0;
    SetSplitPlayerFlag_76A140(gSplitState_123457C,1,1);
    for(int i=0;i<4;++i)s->routeSlot[i]=-1;
    s->localModeFlag=0;
    s->transition570=0;
    s->transition574=0;
    s->transition578=0;
    s->pendingMask=0;
    s->transition580=0;
}

// 0x00C42A30 / 0x00C42A50 -- typed readability wrappers around the canonical
// opaque implementations in split_session_native_core_120.cpp.
void SetSessionSlotModeView_C42A30(SessionSlotState120* s,int slot,int mode){SessionSetSlotMode_C42A30(s,slot,mode);}
void SetSessionSlotDeviceView_C42A50(SessionSlotState120* s,int slot,int device){SessionSetDevice_C42A50(s,slot,device);}

// 0x00C42A70 / A90 / AB0 -- exact native thiscall setters, RET 8.
// Use opaque ECX in the external ABI so all decomp units refer to the same
// native symbol instead of creating artificial C++ overloads by view type.
void __thiscall SetSessionLocalPair_C42A70(void* session,int slot,int value){auto*s=S(session);if(static_cast<unsigned>(slot)<=3u)s->localPair[slot]=value;}
void __thiscall SetSessionPartnerPair_C42A90(void* session,int slot,int value){auto*s=S(session);if(static_cast<unsigned>(slot)<=3u)s->partnerPair[slot]=value;}
void __thiscall SetSessionAuxPair_C42AB0(void* session,int slot,int value){auto*s=S(session);if(static_cast<unsigned>(slot)<=3u)s->auxPair[slot]=value;}

static int Pop4(std::uint32_t v){v&=0xFu;return int(v&1u?1:0)+int(v&2u?1:0)+int(v&4u?1:0)+int(v&8u?1:0);}
// 0x00C42B30 / B60 -- exact ECX-only thiscall counters.
int __thiscall CountAvailableSessionSlots_C42B30(const SessionSlotState120* s){return Pop4(s->availableMask);}
int __thiscall CountActiveSessionSlots_C42B60(const void* session){
    const auto* s=static_cast<const SessionSlotState120*>(session);
    return Pop4(s->activeMask);
}

// 0x00C42B90 / BC0 / BF0 -- exact thiscall predicates, ret 4.
bool __thiscall IsLocalActiveSlot_C42B90(const SessionSlotState120*s,int slot){return (s->availableMask&(1u<<slot))!=0&&s->slotMode[slot]==0;}
bool __thiscall IsJoinableSlot_C42BC0(const SessionSlotState120*s,int slot){return (s->availableMask&(1u<<slot))!=0&&s->slotMode[slot]==1;}
bool __thiscall IsMode2Slot_C42BF0(const SessionSlotState120*s,int slot){return (s->availableMask&(1u<<slot))!=0&&s->slotMode[slot]==2;}

// 0x00C42C20..C42C5D -- exact thiscall, ret 4.
int __thiscall FindLocalSlotForDevice_C42C20(const SessionSlotState120*s,int device){for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))&&s->slotMode[i]==0&&s->device[i]==device)return i;return -1;}

// 0x00C42C60..C42CA9 -- ECX-only thiscall.
int __thiscall FindLocalOrFreeSlot_C42C60(const SessionSlotState120*s){for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))&&s->slotMode[i]==0)return i;for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))==0)return i;return 0;}

// 0x00C42CB0..C42CE2 -- ECX-only thiscall.
int __thiscall FindJoinableSessionSlot_C42CB0(const SessionSlotState120*s){for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))&&s->slotMode[i]==1)return i;return -1;}

// 0x00C42CF0 and 0x00C42D20 -- ECX-only popcounts of +0x57C.
int __thiscall CountPendingSlots_C42CF0(const SessionSlotState120*s){return Pop4(s->pendingMask);}
int __thiscall CountPendingSlotsAlt_C42D20(const SessionSlotState120*s){return Pop4(s->pendingMask);}

} // namespace re5::split120
