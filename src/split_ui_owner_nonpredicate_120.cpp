#include <cstdint>

namespace re5::split120 {

struct Session120;
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gPlayerRoot_11B2158;
extern int __thiscall FindLocalSlotForDevice_C42C20(Session120*,int);

static inline Session120* Session(){
    return *reinterpret_cast<Session120**>(gRoot_12340A4+0x1042C);
}
static inline int KeyboardOwner(){
    return *reinterpret_cast<const int*>(gInput_1249C40+0x614);
}
static inline int CurrentSelectedPlayer(){
    const int i=*reinterpret_cast<const int*>(gPlayerRoot_11B2158+0x20);
    return *reinterpret_cast<const int*>(gPlayerRoot_11B2158+i*0x5940+0x273C4);
}

// 0xA2AC18..0xA2AC38 and twin 0xA2B0FC..0xA2B11D.
// When effective Full is false, player 0 anchors at 0.3 and every nonzero
// keyboard owner anchors at 0.7.  The caller substitutes 0.5 when Full is
// effective before reaching this owner branch.
float LocalOwnerAnchorFactor_A2AC1D(){
    return KeyboardOwner()==0 ? 0.30000001192092896f : 0.69999998807907104f;
}
float LocalOwnerAnchorFactor_A2B102(){
    return LocalOwnerAnchorFactor_A2AC1D();
}

struct UiDeviceRecord120 {
    std::uint8_t _00[0x08];
    std::int32_t logicalPlayer; // +08
    std::int32_t fallbackDevice;// +0C
    std::int32_t resolvedDevice;// +10
};
static_assert(__builtin_offsetof(UiDeviceRecord120,logicalPlayer)==0x08,"logical player");
static_assert(__builtin_offsetof(UiDeviceRecord120,fallbackDevice)==0x0C,"fallback device");
static_assert(__builtin_offsetof(UiDeviceRecord120,resolvedDevice)==0x10,"resolved device");

// 0xA2D4B0..0xA2D52D -- exact local-player/device resolution.
// Start with record+0x0C. In status 1, records not belonging to the currently
// selected logical player are disabled (-1). For the selected player, if the
// preferred physical device maps to a different local slot than +0x614, the
// preferred device overrides the fallback; if it maps to the keyboard owner,
// the original +0x0C value is retained.
void ResolveUiDeviceForKeyboardOwner_A2D4B0(UiDeviceRecord120* r){
    r->resolvedDevice=r->fallbackDevice;
    Session120* s=Session();
    if(QueryGameStatus_C42D90(s)!=1)
        return;
    if(r->logicalPlayer!=CurrentSelectedPlayer()){
        r->resolvedDevice=-1;
        return;
    }
    const int preferred=*reinterpret_cast<const int*>(gInput_1249C40+0x5D8);
    if(FindLocalSlotForDevice_C42C20(s,preferred)==KeyboardOwner())
        return;
    r->resolvedDevice=preferred;
}

// 0xA2E2A6..0xA2E2F7 -- owner-sensitive early dispatch predicate embedded in
// the UI state-machine. Returning true means native calls 0x9E4AC0 and exits.
bool ShouldTakeOwnerSensitiveEarlyUiPath_A2E2A6(int itemKind,
                                                bool parentRecordActive,
                                                int recordPlayer,
                                                int sessionMode)
{
    if(itemKind>0 && (itemKind<=4 || itemKind==7) && parentRecordActive)
        return true;
    if(itemKind==8 && recordPlayer!=KeyboardOwner())
        return true;
    if(itemKind==9 && sessionMode!=0)
        return true;
    return false;
}

// 0xA2F005..0xA2F022 and twin 0xA2F090..0xA2F0AB.
// Type 8 entries belonging to the non-keyboard local player keep only the
// upper 16 bits of the native ARGB/packed color (low 16 bits cleared). Type 9
// gets the same suppression outside Story (session mode != 0).
std::uint32_t ApplyLocalOwnerEntryColorMask_A2F00D(std::uint32_t color,
                                                   int itemKind,
                                                   int recordPlayer,
                                                   int sessionMode)
{
    if((itemKind==8 && recordPlayer!=KeyboardOwner()) ||
       (itemKind==9 && sessionMode!=0))
        color&=0xFFFF0000u;
    return color;
}
std::uint32_t ApplyLocalOwnerEntryColorMask_A2F098(std::uint32_t color,
                                                   int itemKind,
                                                   int recordPlayer,
                                                   int sessionMode)
{
    return ApplyLocalOwnerEntryColorMask_A2F00D(color,itemKind,recordPlayer,sessionMode);
}

} // namespace re5::split120
