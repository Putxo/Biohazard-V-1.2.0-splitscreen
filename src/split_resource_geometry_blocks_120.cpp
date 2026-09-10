#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct ResourceNode120 {
    std::uint8_t _00[0x38];
    std::int32_t mode;      // +0x38
    std::uint8_t _3C[0x14];
    float x;                // +0x50
    float y;                // +0x54
    float z;                // +0x58
    std::uint8_t _5C[0x14];
    float scaleX;           // +0x70
    float scaleY;           // +0x74
    std::uint8_t _78[0x08];
    std::uint8_t visible80; // +0x80
};

struct LocalTriplet120 {
    float x;
    float y;
    float scale;
};

extern SplitRenderState120* gSplitRenderState_123457C;
extern int SplitXFromParam_76A420(const SplitRenderState120* split, int value);
extern int SplitYWithVertical_76A4A0(const SplitRenderState120* split, int value);

// 0x009EEF19..0x009EEF95 -- VERIFIED isolated branch inside 0x9EDE30.
// Entered only when the selected variant index is > 1.
// Native code transforms X with 0x76A420, replaces scale with split+0x3070,
// subtracts an integer local Y offset, writes node position/state, then copies
// splitScale into +0x70/+0x74.
void ApplySelectedResourceSplitTransform_9EEF19(ResourceNode120* node,
                                                LocalTriplet120& triplet,
                                                int integerYOffset)
{
    SplitRenderState120* split = gSplitRenderState_123457C;
    triplet.x = static_cast<float>(
        SplitXFromParam_76A420(split, static_cast<int>(triplet.x)));
    triplet.scale = split->splitScale;
    triplet.y -= static_cast<float>(integerYOffset);

    if (!node)
        return;

    node->x = triplet.x;
    node->y = triplet.y;
    node->visible80 = 0;
    node->z = 0.0f;
    node->mode = 0x0D;
    node->scaleX = triplet.scale;
    node->scaleY = triplet.scale;
}

// 0x009EF16A..0x009EF216 -- VERIFIED isolated status-1 transform inside 0x9EDE30.
// In status 1, native code transforms X with 0x76A420, scales Y by splitScale,
// adds 0x76A4A0(0), and stores splitScale as uniform XY scale.
void ApplyStatus1ResourceSplitTransform_9EF16A(ResourceNode120* node,
                                               LocalTriplet120& triplet,
                                               bool statusIs1)
{
    if (statusIs1) {
        SplitRenderState120* split = gSplitRenderState_123457C;
        triplet.x = static_cast<float>(
            SplitXFromParam_76A420(split, static_cast<int>(triplet.x)));
        triplet.y = triplet.y * split->splitScale +
                    static_cast<float>(SplitYWithVertical_76A4A0(split, 0));
        triplet.scale = split->splitScale;
    }

    if (!node)
        return;

    node->x = triplet.x;
    node->y = triplet.y;
    node->visible80 = 0;
    node->z = 0.0f;
    node->mode = 0x0D;
    node->scaleX = triplet.scale;
    node->scaleY = triplet.scale;
}

} // namespace re5::split120
