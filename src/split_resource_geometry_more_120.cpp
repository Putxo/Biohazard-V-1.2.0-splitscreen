#include "re5/split_localcoop_120.hpp"
#include <cstdint>

namespace re5::split120 {

struct ResourceUiNode120 {
    std::uint8_t _00[0x38];
    std::int32_t mode;
    std::uint8_t _3C[0x14];
    float x;
    float y;
    float z;
    std::uint8_t _5C[0x14];
    float scaleX;
    float scaleY;
    std::uint8_t _78[0x08];
    std::uint8_t enabledByte;
};

extern SplitRenderState120* gSplitRenderState_123457C;
extern void RefreshResourceNode_CD8160(ResourceUiNode120* node);

// 0x009EF000..0x009EF092 -- verified split-aware row application.
void ApplyResourceRow_9EF000(ResourceUiNode120* node,
                             float& rowX,
                             float rowY,
                             float& rowScale,
                             int yOffset,
                             bool subtractOffset)
{
    if (QueryGameStatus_C42D90() == 1) {
        const int ix = static_cast<int>(rowX);
        rowX = static_cast<float>(
            SplitCoordTransform_76A420(gSplitRenderState_123457C, ix));
        rowScale = gSplitRenderState_123457C->splitScale;
        yOffset = 0;
    }

    if (!node) return;
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
void ApplyFourResourceNodes_9EF450(ResourceUiNode120* const nodes[4],
                                   const float positions[4][3][2],
                                   int rowSelector,
                                   float uniformScale)
{
    for (int i = 0; i < 4; ++i) {
        ResourceUiNode120* node = nodes[i];
        if (!node) continue;

        node->scaleX = uniformScale;
        node->scaleY = uniformScale;
        node->x = positions[i][rowSelector][0];

        if (QueryGameStatus_C42D90() == 1) {
            const int ix = static_cast<int>(node->x);
            node->x = static_cast<float>(
                SplitCoordTransform_76A420(gSplitRenderState_123457C, ix));
        }

        node->y = positions[i][rowSelector][1];
        RefreshResourceNode_CD8160(node);
    }
}

} // namespace re5::split120
