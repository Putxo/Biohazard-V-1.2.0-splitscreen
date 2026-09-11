#include <cstdint>

namespace re5::split120 {

struct Session120;

static inline Session120* SessionFromRoot_12340A4()
{
    auto* root = *reinterpret_cast<std::uint8_t**>(0x012340A4);
    return *reinterpret_cast<Session120**>(root + 0x1042C);
}

static inline std::uint8_t* Bytes(Session120* session)
{
    return reinterpret_cast<std::uint8_t*>(session);
}

extern void* gUiInput_11B20C4;
extern bool __thiscall IsMode1Route23_C42E10(Session120* session);
// Normal Mercenaries frontend family.
// Vtable update entry 0xA47CE0 shares its class with init method 0xA41410;
// 0xA41410 directly requests "id\\jpn\\merce\\merce_00" (0xFA9C04).
// 0xA48051..0xA480A0 is the exact selection commit branch:
//   selection 0..2 -> session+0x5CC = selection, session+0x79E = 0
//   selection 3    -> session+0x5CC = 0,         session+0x79E = 1
void CommitMercenariesModeSelection_A48051(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    auto* b = Bytes(session);
    if (selected == 3) {
        *reinterpret_cast<std::int32_t*>(b + 0x5CC) = 0;
        b[0x79E] = 1;
    } else {
        *reinterpret_cast<std::int32_t*>(b + 0x5CC) = selected;
        b[0x79E] = 0;
    }
}

// Reunion / merce_dc frontend family.
// Vtable update entry 0xA49720 is paired with init method 0xA45D80.
// 0xA45D80 directly loads:
//   0xFA9F58 -> "id\\jpn\\merce_dc\\merce_00"
//   0xFA9F3C -> "id\\jpn\\merce_dc\\merce_dc01"
// 0xA499DB..0xA49B27 is its VERIFIED three-way selector path.
void CommitReunionThreeWaySelection_A49AE4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    auto* b = Bytes(session);
    *reinterpret_cast<std::int32_t*>(b + 0x5CC) = selected;
    b[0x79E] = 0;
}

// View of the Reunion update object used by its later four-way selector.
struct ReunionFourWaySelector120 {
    std::uint8_t _000[0x1A0];
    std::int32_t selected;       // +0x1A0
    std::uint8_t _1A4[4];
    std::uint32_t enabledMask;   // +0x1A8
    std::uint8_t _1AC;
    std::uint8_t cachedRoute23;  // +0x1AD
};
static_assert(__builtin_offsetof(ReunionFourWaySelector120,selected)==0x1A0,"Reunion four-way selected");
static_assert(__builtin_offsetof(ReunionFourWaySelector120,enabledMask)==0x1A8,"Reunion four-way mask");
static_assert(__builtin_offsetof(ReunionFourWaySelector120,cachedRoute23)==0x1AD,"Reunion route cache");

// 0x00C42E10..0x00C42E28 -- exact predicate consumed by the Reunion branch:
// true iff session+0x58 == 1 and session+0x5CC is 2 or 3.
bool __thiscall IsMode1Route23_C42E10(Session120* session)
{
    const auto* b=Bytes(session);
    if(*reinterpret_cast<const std::int32_t*>(b+0x58)!=1)
        return false;
    const int route=*reinterpret_cast<const std::int32_t*>(b+0x5CC);
    return route==2 || route==3;
}

// 0x00A4A0C0..0x00A4A1C8 -- VERIFIED later four-way selector of the SAME
// Reunion/merce_dc update function A49720. Earlier source represented only the
// single write at A4A0FA and missed its local-slot topology side effect.
//
// After validating selected against self+0x1A8, native commits +0x5CC and
// evaluates C42E10. If that predicate differs from the cached byte +0x1AD,
// it rewrites the per-slot dword table session+0x4C0[0..3]:
//   route23 true  -> [0,1,1,0]
//   route23 false -> [0,1,2,3]
// This is why the four-way selector is local-player relevant even though its
// localized row label is resource-driven.
bool CommitReunionFourWaySelection_A4A0C0(ReunionFourWaySelector120* self)
{
    const int selected=self->selected;
    const std::uint32_t bit=1u<<selected;
    if((self->enabledMask & bit)==0)
        return false; // native branches to A49EC1 invalid-selection handling

    UiRoute43C0_7B43C0(gUiInput_11B20C4,1,0);

    auto* session=SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session)+0x5CC)=selected; // A4A0FA

    const bool route23=IsMode1Route23_C42E10(session);
    if(static_cast<std::uint8_t>(route23)==self->cachedRoute23)
        return true; // native A4A110 -> A4A1C8

    if(route23){
        SetSessionAuxPair_C42AB0(session,0,0);
        SetSessionAuxPair_C42AB0(session,1,1);
        SetSessionAuxPair_C42AB0(session,2,1);
        SetSessionAuxPair_C42AB0(session,3,0);
    }else{
        SetSessionAuxPair_C42AB0(session,0,0);
        SetSessionAuxPair_C42AB0(session,1,1);
        SetSessionAuxPair_C42AB0(session,2,2);
        SetSessionAuxPair_C42AB0(session,3,3);
    }
    return true;
}

// Exact single-write helper retained for address-level reference to A4A0FA.
void CommitFourWaySessionModeSelection_A4A0FA(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

// Extra/DLC frontend family containing LIN/DE menu resources.
// Vtable update entry 0xA52970 belongs to the class initialized through the
// A4F500/A4F543 resource path. 0xA4F543 directly requests 0xFAA688:
// "id\\jpn\\extra\\extra_03".
void CommitExtraDlcThreeWaySelection_A52DD4(std::int32_t selected)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x5CC) = selected;
}

} // namespace re5::split120
