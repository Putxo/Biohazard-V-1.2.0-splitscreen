#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct ResourceUiNode120 {
    std::uint8_t _00[0x38];
    std::int32_t mode;           // +0x38
    std::uint8_t _3C[0x14];
    float x;                     // +0x50
    float y;                     // +0x54
    float z;                     // +0x58
    std::uint8_t _5C[0x14];
    float scaleX;                // +0x70
    float scaleY;                // +0x74
    std::uint8_t _78[0x08];
    std::uint8_t enabledByte;    // +0x80
};

extern SplitRenderState120* gSplitRenderState_123457C;
extern int QueryGameStatus_C42D90();
extern int SplitXFromParam_76A420(const SplitRenderState120* split, int value);
extern void RefreshResourceNode_CD8160(ResourceUiNode120* node);

// 0x009EF000..0x009EF092 -- verified split-aware row application used by
// the 18-entry resource loop in 0x009EDE30. The caller selects one of the
// three-float variants before entering this block; this helper models the
// exact status-1 transform and node writes once that variant is selected.
void ApplyResourceRow_9EF000(ResourceUiNode120* node,
                             float& rowX,
                             float rowY,
                             float& rowScale,
                             int yOffset,
                             bool subtractOffset)
{
    if (QueryGameStatus_C42D90() == 1) {
        const int ix = static_cast<int>(rowX); // CVTTSS2SI
        rowX = static_cast<float>(
            SplitXFromParam_76A420(gSplitRenderState_123457C, ix));
        rowScale = gSplitRenderState_123457C->splitScale;
        yOffset = 0;
    }

    if (!node)
        return;

    node->x = rowX;
    node->y = subtractOffset
        ? rowY - static_cast<float>(yOffset)
        : rowY + static_cast<float>(yOffset);
    node->z = 0.0f;
    node->enabledByte = 0;
    node->mode = 0x0D;
    node->scaleX = rowScale;
    node->scaleY = rowScale;
}

// 0x009EF450..0x009EF4C4 -- VERIFIED four-node application loop.
// Native code iterates exactly four local table entries. For each live node it
// copies the selected position pair, applies split X in status 1, applies the
// same splitScale to +0x70/+0x74, then calls 0xCD8160.
void ApplyFourResourceNodes_9EF450(ResourceUiNode120* const nodes[4],
                                   const float positions[4][3][2],
                                   int rowSelector,
                                   float uniformScale)
{
    for (int i = 0; i < 4; ++i) {
        ResourceUiNode120* node = nodes[i];
        if (!node)
            continue;

        node->scaleX = uniformScale;
        node->scaleY = uniformScale;
        node->x = positions[i][rowSelector][0];

        if (QueryGameStatus_C42D90() == 1) {
            const int ix = static_cast<int>(node->x); // CVTTSS2SI
            node->x = static_cast<float>(
                SplitXFromParam_76A420(gSplitRenderState_123457C, ix));
        }

        node->y = positions[i][rowSelector][1];
        RefreshResourceNode_CD8160(node);
    }
}

} // namespace re5::split120
