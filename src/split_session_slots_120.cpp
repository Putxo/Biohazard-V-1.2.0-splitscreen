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
    std::uint8_t _4D0[0xAC];
    std::uint32_t pendingMask;   // +0x57C
};

// 0x00C43A00 constructor fragment -- VERIFIED slot/local-player setup.
void InitLocalCoopSessionSlots_C43A00(SessionSlotState120* s, int preferredDevice)
{
    s->availableMask = 0x3;
    for (int i=0;i<4;++i) { s->slotMode[i]=(i==0)?0:1; s->device[i]=-1; }
    s->device[0]=preferredDevice; s->activeMask=0x1;
}

// 0x00C42A30 / 0x00C42A50 -- VERIFIED.
void SetSessionSlotMode_C42A30(SessionSlotState120* s,int slot,int mode){if(static_cast<unsigned>(slot)<=3u)s->slotMode[slot]=mode;}
void SetSessionSlotDevice_C42A50(SessionSlotState120* s,int slot,int device){if(static_cast<unsigned>(slot)<=3u)s->device[slot]=device;}

// 0x00C42A70 / A90 / AB0 -- VERIFIED per-slot local-coop relationship fields.
void SetSessionLocalPair_C42A70(SessionSlotState120* s,int slot,int value){if(static_cast<unsigned>(slot)<=3u)s->localPair[slot]=value;}
void SetSessionPartnerPair_C42A90(SessionSlotState120* s,int slot,int value){if(static_cast<unsigned>(slot)<=3u)s->partnerPair[slot]=value;}
void SetSessionAuxPair_C42AB0(SessionSlotState120* s,int slot,int value){if(static_cast<unsigned>(slot)<=3u)s->auxPair[slot]=value;}

static int Pop4(std::uint32_t v){v&=0xFu;return int(v&1u?1:0)+int(v&2u?1:0)+int(v&4u?1:0)+int(v&8u?1:0);}
// 0x00C42B30 / B60 -- VERIFIED.
int CountAvailableSessionSlots_C42B30(const SessionSlotState120* s){return Pop4(s->availableMask);}
int CountActiveSessionSlots_C42B60(const SessionSlotState120* s){return Pop4(s->activeMask);}

// 0x00C42B90 / BC0 / BF0 -- VERIFIED exact available+mode predicates.
bool IsLocalActiveSlot_C42B90(const SessionSlotState120*s,int slot){return (s->availableMask&(1u<<slot))!=0&&s->slotMode[slot]==0;}
bool IsJoinableSlot_C42BC0(const SessionSlotState120*s,int slot){return (s->availableMask&(1u<<slot))!=0&&s->slotMode[slot]==1;}
bool IsMode2Slot_C42BF0(const SessionSlotState120*s,int slot){return (s->availableMask&(1u<<slot))!=0&&s->slotMode[slot]==2;}

// 0x00C42C20..C42C5D -- VERIFIED. Find a local(mode0) slot mapped to device.
int FindLocalSlotForDevice_C42C20(const SessionSlotState120*s,int device){for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))&&s->slotMode[i]==0&&s->device[i]==device)return i;return -1;}

// 0x00C42C60..C42CA9 -- VERIFIED. Prefer first available mode0 slot; if none,
// return first unavailable slot. Native returns 0 if all four are available.
int FindLocalOrFreeSlot_C42C60(const SessionSlotState120*s){for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))&&s->slotMode[i]==0)return i;for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))==0)return i;return 0;}

// 0x00C42CB0..C42CE2 -- VERIFIED.
int FindJoinableSessionSlot_C42CB0(const SessionSlotState120*s){for(int i=0;i<4;++i)if((s->availableMask&(1u<<i))&&s->slotMode[i]==1)return i;return -1;}

// 0x00C42CF0 and 0x00C42D20 are instruction-identical popcounts of +0x57C.
int CountPendingSlots_C42CF0(const SessionSlotState120*s){return Pop4(s->pendingMask);}
int CountPendingSlotsAlt_C42D20(const SessionSlotState120*s){return Pop4(s->pendingMask);}

} // namespace re5::split120
