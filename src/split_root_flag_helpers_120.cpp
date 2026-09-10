#include <cstdint>

namespace re5::split120 {

struct GameRoot120 {
    std::uint8_t _0000[0x1C];
    std::uint8_t syncMode;          // +0x1C
    std::uint8_t _001D[3];
    std::int32_t activeProfile;     // +0x20
    std::uint8_t _0024[0x10410];
    std::uint32_t gateFlags10434;   // +0x10434
    std::uint32_t stateFlags10438;  // +0x10438
};

extern std::uint8_t gRootSyncEnabled_1249C2E;
extern void RootSyncBegin_F4A0F8(void* rootPlus4);
extern void RootSyncEnd_F4A164(void* rootPlus4);

static inline bool RootNeedsSync(const GameRoot120* root)
{
    return root->syncMode != 0 || gRootSyncEnabled_1249C2E != 0;
}

static inline void BeginRootWrite(GameRoot120* root)
{
    if (RootNeedsSync(root))
        RootSyncBegin_F4A0F8(reinterpret_cast<std::uint8_t*>(root) + 4);
}

static inline void EndRootWrite(GameRoot120* root)
{
    if (RootNeedsSync(root))
        RootSyncEnd_F4A164(reinterpret_cast<std::uint8_t*>(root) + 4);
}

// 0x007159E0..0x00715A22, RET 4 -- VERIFIED.
// Synchronized AND-NOT into root+0x10434.
void ClearRootGateFlags_7159E0(GameRoot120* root, std::uint32_t mask)
{
    BeginRootWrite(root);
    root->gateFlags10434 &= ~mask;
    EndRootWrite(root);
}

// 0x00715A30..0x00715A72, RET 4 -- VERIFIED.
// Synchronized AND-NOT into root+0x10438.
void ClearRootStateFlags_715A30(GameRoot120* root, std::uint32_t mask)
{
    BeginRootWrite(root);
    root->stateFlags10438 &= ~mask;
    EndRootWrite(root);
}

// 0x007260D0..0x00726110, RET 4 -- VERIFIED.
// Synchronized OR into root+0x10434. PollLocalJoin_723460 requires bits 0/1
// of this word to be clear before the native J2 join path can run.
void OrRootGateFlags_7260D0(GameRoot120* root, std::uint32_t mask)
{
    BeginRootWrite(root);
    root->gateFlags10434 |= mask;
    EndRootWrite(root);
}

// 0x00726120..0x00726160, RET 4 -- VERIFIED.
// Synchronized OR into root+0x10438. EnableLocalJoinRootState_71A4C0 calls
// this exact helper with 0x10; the same word feeds the local-join gates.
void OrRootStateFlags_726120(GameRoot120* root, std::uint32_t mask)
{
    BeginRootWrite(root);
    root->stateFlags10438 |= mask;
    EndRootWrite(root);
}

// 0x00726170..0x007261BD, RET 8 -- VERIFIED.
// Indexed synchronized OR into root+0x1043C[index].
void OrRootIndexedFlags_726170(GameRoot120* root, std::uint32_t mask,
                              std::uint8_t index)
{
    BeginRootWrite(root);
    auto* indexed = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(root) + 0x1043C);
    indexed[index] |= mask;
    EndRootWrite(root);
}

// 0x007261C0..0x0072620A, RET 8 -- VERIFIED.
// Indexed synchronized AND-NOT into root+0x1043C[index].
void ClearRootIndexedFlags_7261C0(GameRoot120* root, std::uint32_t mask,
                                 std::uint8_t index)
{
    BeginRootWrite(root);
    auto* indexed = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(root) + 0x1043C);
    indexed[index] &= ~mask;
    EndRootWrite(root);
}

} // namespace re5::split120
