#include <cstdint>

namespace re5::split120 {

struct Session120;
static inline Session120* SessionFromRoot_12340A4(){auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4);return *reinterpret_cast<Session120**>(root+0x1042C);}
static inline std::uint8_t* Bytes(Session120* session){return reinterpret_cast<std::uint8_t*>(session);}

struct MercsFrontend120 {
    std::uint8_t _0000[0x188];
    std::int32_t selectedPlayer;
    std::uint8_t _018c[0x588 - 0x18c];
    std::int32_t field588;
    std::uint8_t _058c[0x5A8 - 0x58c];
    std::int32_t field5A8;
    std::uint8_t _05ac[0x600 - 0x5ac];
    std::int32_t perPlayer600[4];
    std::int32_t perPlayer610[4];
    std::uint8_t _0620[0x6B0 - 0x620];
    std::int32_t localJoinDeviceOverride;
};

extern bool IsMercsLocalSplitRow_C42EC0(const Session120*);
extern bool DeviceOverrideAllowed_7F1390(void*, int device, int zero);
extern void SelectInputDevice_7B66E0(void*, int device);
extern void* gPlayerState_11B2158;
extern void* gDeviceMgr_11B1DD8;

// 0xA43360..0xA43457 -- VERIFIED split-relevant reconstruction.
int RefreshMercsPlayerBinding_A43360(MercsFrontend120* self, int player)
{
    auto* input = reinterpret_cast<std::uint8_t*>(0x01249C40);
    int device = *reinterpret_cast<std::int32_t*>(input + 0x5D8);

    if (player != self->selectedPlayer) {
        auto* session = SessionFromRoot_12340A4();
        if (!IsMercsLocalSplitRow_C42EC0(session)) return 6;

        if (self->localJoinDeviceOverride >= 0 &&
            DeviceOverrideAllowed_7F1390(gDeviceMgr_11B1DD8,self->localJoinDeviceOverride,0)) {
            device = self->localJoinDeviceOverride;
        }
    }

    SelectInputDevice_7B66E0(gPlayerState_11B2158, device);
    return device;
}

// 0xA44370..0xA443D4 -- VERIFIED join completion block.
void MarkMercsLocalJoinComplete_A44370(MercsFrontend120* self)
{
    auto* session = SessionFromRoot_12340A4();
    *reinterpret_cast<std::int32_t*>(Bytes(session) + 0x624) = 1;
    (void)RefreshMercsPlayerBinding_A43360(self, 1);
}

// 0xA44274..0xA44300 -- VERIFIED local-join teardown/reset path.
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
