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

struct MercsFrontend120 {
    std::uint8_t _0000[0x188];
    std::int32_t selectedPlayer;          // +0x188
    std::uint8_t _018c[0x588 - 0x18c];
    std::int32_t field588;                // +0x588
    std::uint8_t _058c[0x5A8 - 0x58c];
    std::int32_t field5A8;                // +0x5A8
    std::uint8_t _05ac[0x600 - 0x5ac];
    std::int32_t perPlayer600[4];          // +0x600
    std::int32_t perPlayer610[4];          // +0x610
    std::uint8_t _0620[0x6B0 - 0x620];
    std::int32_t localJoinDeviceOverride; // +0x6B0
};

extern bool IsMercsLocalSplit_C42EC0(Session120*);
extern bool DeviceOverrideAllowed_7F1390(void*, int device, int zero);
extern void SelectInputDevice_7B66E0(void*, int device);
extern void* gPlayerState_11B2158;
extern void* gDeviceMgr_11B1DD8;

// 0xA43360..0xA43457 -- VERIFIED split-relevant reconstruction.
// Native receives a player index. Default input is input+0x5D8. If the player
// differs from self+0x188 and C42EC0 confirms Mercs/Reunion local split, a
// valid self+0x6B0 may override that device before the per-player loadout is
// resolved and written to session+0x610+4*player.
int RefreshMercsPlayerBinding_A43360(MercsFrontend120* self, int player)
{
    auto* input = reinterpret_cast<std::uint8_t*>(0x01249C40);
    int device = *reinterpret_cast<std::int32_t*>(input + 0x5D8);

    if (player != self->selectedPlayer) {
        auto* session = SessionFromRoot_12340A4();
        if (!IsMercsLocalSplit_C42EC0(session))
            return 6;

        if (self->localJoinDeviceOverride >= 0 &&
            DeviceOverrideAllowed_7F1390(gDeviceMgr_11B1DD8,
                                         self->localJoinDeviceOverride, 0)) {
            device = self->localJoinDeviceOverride;
        }
    }

    SelectInputDevice_7B66E0(gPlayerState_11B2158, device);

    // 0xA433C9..0xA43446 then reads session+0x600+4*player, resolves the
    // character/loadout through 0xCEF230 and optional 0xC43240, clamps the
    // result to <=6, and stores it at session+0x610+4*player. Those payload
    // semantics are not split-specific and remain represented by this boundary.
    return device;
}

// 0xA44370..0xA443D4 -- VERIFIED join completion block.
// After the state-0x32 slot-1 initialization, native marks session+0x624=1,
// refreshes player 1 through A43360(1), returns frontend state to 3 and marks
// self+0x6AC=1.
void MarkMercsLocalJoinComplete_A44370(MercsFrontend120* self)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x624) = 1;
    (void)RefreshMercsPlayerBinding_A43360(self, 1);
}

// 0xA44274..0xA44300 -- VERIFIED local-join teardown/reset path.
// When the temporary join phase expires/cancels, native restores slot0 to the
// preferred input device, clears slot1 to -1, clears +0x624 and invalidates
// both persisted local bindings +0x654/+0x658 before resetting the local join
// frontend substate.
void ResetMercsLocalJoinBindings_A44274()
{
    auto* session = SessionFromRoot_12340A4();
    auto* bytes = Bytes(session);
    auto* input = reinterpret_cast<std::uint8_t*>(0x01249C40);

    *reinterpret_cast<std::int32_t*>(bytes + 0x624) = 0;
    *reinterpret_cast<std::int32_t*>(bytes + 0x654) = -1;
    *reinterpret_cast<std::int32_t*>(bytes + 0x658) = -1;
    *reinterpret_cast<std::int32_t*>(input + 0x614) = 0;
}

} // namespace re5::split120
