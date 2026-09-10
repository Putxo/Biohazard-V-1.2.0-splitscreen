#include <cstdint>

namespace re5::split120 {

struct PauseOption05RoutingView120 {
    std::uint8_t _000[0x34];
    std::int32_t playerIndex; // +0x34
};

extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern bool UiRoute5750_7B5750(void*, int, int, int, int);
extern int UiRoute5360_7B5360(void*, int);
extern bool UiRoute5620_7B5620(void*, int, int, int);
extern int UiRoute59B0_7B59B0(void*, int, int, int, int, int, int);

static inline bool IsOwningLocalPlayer(const PauseOption05RoutingView120* self)
{
    return self->playerIndex == *reinterpret_cast<std::int32_t*>(gInput_1249C40 + 0x614);
}

// 0x00A2B7B5..0x00A2B7E7 -- VERIFIED.
bool Option05OwnerRoute6RequiresFreeSlot_A2B7B5(const PauseOption05RoutingView120* self)
{
    if (!IsOwningLocalPlayer(self)) return false;
    if (!UiRoute5750_7B5750(gUiInput_11B20C4, 0, 6, 0, 0)) return false;
    return UiRoute5360_7B5360(gUiInput_11B20C4, 1) == -1;
}

// 0x00A2B81C..0x00A2B84E -- VERIFIED duplicate ownership gate.
bool Option05OwnerRoute6Cancel_A2B81C(const PauseOption05RoutingView120* self)
{
    if (!IsOwningLocalPlayer(self)) return false;
    if (!UiRoute5750_7B5750(gUiInput_11B20C4, 0, 6, 0, 0)) return false;
    return UiRoute5360_7B5360(gUiInput_11B20C4, 1) == -1;
}

// 0x00A2B8C9..0x00A2B8ED -- VERIFIED.
bool Option05OwnerCancelRoute7_A2B8C9(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 7, 0, 0);
}

// 0x00A2B923..0x00A2B94E -- VERIFIED.
int Option05OwnerSelectionRoute26_A2B923(const PauseOption05RoutingView120* self, int fallbackMetric)
{
    if (!IsOwningLocalPlayer(self)) return fallbackMetric;
    return UiRoute59B0_7B59B0(gUiInput_11B20C4, fallbackMetric, 0, 0x26, 0, 1, 2);
}

// 0x00A2B98A..0x00A2B9B0 -- VERIFIED.
bool Option05OwnerRoute1Variant1_A2B98A(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 1, 0, 1);
}

// 0x00A2B9B2..0x00A2B9D7 -- VERIFIED.
bool Option05OwnerRoute1Variant0_A2B9B2(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 0, 0, 1);
}

// 0x00A2BBD8..0x00A2BC10 -- VERIFIED.
bool Option05OwnerRoute2_A2BBD8(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 0, 0, 2);
}

// 0x00A2BF97..0x00A2BFB5 -- VERIFIED.
bool Option05OwnerCancelRoute8_A2BF97(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 8, 0, 0);
}

// 0x00A2BFCD..0x00A2BFEB -- VERIFIED.
bool Option05OwnerCancelRoute7Late_A2BFCD(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 7, 0, 0);
}

// 0x00A2C033..0x00A2C060 -- VERIFIED.
int Option05OwnerSelectionRoute4_A2C033(const PauseOption05RoutingView120* self, int fallbackMetric)
{
    if (!IsOwningLocalPlayer(self)) return fallbackMetric;
    return UiRoute59B0_7B59B0(gUiInput_11B20C4, fallbackMetric, 0, 4, 0, 1, 2);
}

// 0x00A2C099..0x00A2C0BD -- VERIFIED.
bool Option05OwnerLateRoute1Variant1_A2C099(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 1, 0, 1);
}

// 0x00A2C0C9..0x00A2C0F1 -- VERIFIED.
bool Option05OwnerLateRoute1Variant0_A2C0C9(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, 0, 0, 1);
}

// 0x00A2C3D7..0x00A2C406 -- VERIFIED.
bool Option05OwnerWildcardCancel_A2C3D7(const PauseOption05RoutingView120* self)
{
    return IsOwningLocalPlayer(self) && UiRoute5620_7B5620(gUiInput_11B20C4, -1, 0, 0);
}

// All direct InputManager+0x614 ownership comparisons in 0xA2B630 are
// represented above. Remaining A2B630 code is generic option/list animation.
constexpr unsigned Option05SplitOwnershipSites_A2B630() { return 13u; }

} // namespace re5::split120
