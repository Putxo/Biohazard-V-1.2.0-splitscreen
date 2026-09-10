#include <cstdint>
namespace re5::split120 {
struct MercsJoinResetView120 {
    std::uint8_t _0000[0x6AC];
    std::int32_t uiState6AC;
    std::int32_t selectedJoinDevice6B0;
    std::uint8_t _6B4;
    std::uint8_t flag6B5;
};
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gLocalJoinOwner_11B1DD8;
extern void SessionSetDevice_C42A50(void* session, int slot, int device);
extern void RefreshLocalJoinOwner_7F1280(void* owner);
// 0x00A44274..0x00A44306 -- VERIFIED Mercenaries/Reunion local-join cleanup.
// Rebinds slot0 to the preferred input, clears slot1, and invalidates the
// persistent local J1/J2 bindings +654/+658 after the local-join UI exits.
void ResetMercsLocalJoinBindings_A44274(MercsJoinResetView120* self) {
    self->flag6B5 = 0;
    *reinterpret_cast<std::int32_t*>(gInput_1249C40 + 0x614) = 0;
    auto* session = *reinterpret_cast<std::uint8_t**>(gRoot_12340A4 + 0x1042C);
    const int preferred = *reinterpret_cast<const std::int32_t*>(gInput_1249C40 + 0x5D8);
    SessionSetDevice_C42A50(session, 0, preferred);
    SessionSetDevice_C42A50(session, 1, -1);
    *reinterpret_cast<std::int32_t*>(session + 0x624) = 0;
    *reinterpret_cast<std::int32_t*>(session + 0x654) = -1;
    *reinterpret_cast<std::int32_t*>(session + 0x658) = -1;
    RefreshLocalJoinOwner_7F1280(gLocalJoinOwner_11B1DD8);
    self->selectedJoinDevice6B0 = -1;
    self->uiState6AC = 0;
}
}
