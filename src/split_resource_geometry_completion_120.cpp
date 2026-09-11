#include <cstdint>

namespace re5::split120 {

struct SplitRenderState120 {
    std::uint8_t _0000[0x3064];
    std::uint8_t splitActive;       // +3064
    std::uint8_t _3065[0x0B];
    float splitScale;               // +3070
    std::uint8_t _3074[0x10];
    std::uint8_t fullSplit;         // +3084
    std::uint8_t transientSplit;    // +3085
};

struct ResourceOwner120 {
    std::uint8_t _0000[0xD8];
    float splitScaleCache;           // +D8
    std::uint8_t _00DC[4];
    std::int32_t gameStatusCache;    // +E0
    std::uint8_t fullEffectiveCache; // +E4
    std::uint8_t _00E5[0xC5];
    std::uint8_t sideSelector;       // +1AA
};

struct ResourceTriplet120 { float x, y, scale; };

extern SplitRenderState120* gSplitRenderState_123457C;
extern int QueryGameStatus_C42D90();

// 0x009EEE00..0x009EEE2F -- VERIFIED.
void CacheResourceSplitState_9EEE00(ResourceOwner120* self)
{
    const int status = QueryGameStatus_C42D90();
    self->gameStatusCache = status;

    const SplitRenderState120* split = gSplitRenderState_123457C;
    const bool effectiveFull =
        split->fullSplit != 0 && split->transientSplit == 0;
    self->fullEffectiveCache = effectiveFull ? 1u : 0u;
}

// 0x009EF37B..0x009EF3C9 -- VERIFIED.
float ApplyLateResourceRowSplit_9EF37B(ResourceTriplet120& row,
                                       int& integerYOffset,
                                       int selector)
{
    if (QueryGameStatus_C42D90() == 1) {
        if (selector != 0x0A) {
            SplitRenderState120* split = gSplitRenderState_123457C;
            row.x = static_cast<float>(
                SplitCoordTransform_76A420(split, static_cast<int>(row.x)));
            row.scale = split->splitScale;
        }
        integerYOffset = 0;
    }

    return row.y - static_cast<float>(integerYOffset);
}

// 0x009EF413..0x009EF444 -- VERIFIED.
void PrepareFourResourceNodesSplit_9EF413(ResourceOwner120* self,
                                          int& rowSelector,
                                          float& uniformScale)
{
    SplitRenderState120* split = gSplitRenderState_123457C;
    self->splitScaleCache = split->splitScale;
    rowSelector = (self->sideSelector != 0 ? 1 : 0) + 1;
    uniformScale = split->splitScale;
}

} // namespace re5::split120
