#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// -----------------------------------------------------------------------------
// RE5DX9 1.2.0 split-aware UI/menu draw family.
// Addresses are original 1.2.0 VAs.
// -----------------------------------------------------------------------------

struct UiPanel120 {
    unsigned char _00[0x24];
    int gridX;      // +0x24
    int gridY;      // +0x28
    int originY;    // +0x2C
    int originX;    // +0x30
};

extern int  GetRuntimeStatus_C42D90();
extern int  CoordScale2Offset_A239xx(int value);
extern int  PanelTransform_A239xx(int value);
extern void DrawUiPrimitive_1p2(UiPanel120* self,
                                int width,
                                int x,
                                int y,
                                void* local,
                                int id,
                                int height,
                                int unkMinusOne);

// 0x00A23B40
// PARTIAL, instruction-derived from the preserved Stage24 assembly translation.
// The arithmetic and split predicates below are recovered. The semantic names of
// the final primitive's constants are still unknown, so the function stays Partial.
void DrawSecondarySplitMenu_A23B40(UiPanel120* self)
{
    int baseWidth;
    if (GetRuntimeStatus_C42D90() == 1) {
        // Original uses split scale (+0x3070) * 30.0f with cvttss2si.
        auto* split = reinterpret_cast<SplitRenderState120*>(
            *reinterpret_cast<void**>(0x0123457C));
        baseWidth = static_cast<int>(split->splitScale * 30.0f);
    } else {
        baseWidth = 0x1E;
    }

    // Exact integer idioms reconstructed from the original instruction sequence.
    int x = self->gridX / 10;
    int y = (self->gridY * 7) / 8;

    if (GetRuntimeStatus_C42D90() == 1) {
        auto* split = reinterpret_cast<SplitRenderState120*>(
            *reinterpret_cast<void**>(0x0123457C));

        if (IsFullSplitEffective(split)) {
            x = CoordScale2Offset_A239xx(x);
        } else {
            x = static_cast<int>(static_cast<float>(x) + split->splitOffset);
        }

        y = PanelTransform_A239xx(y + baseWidth / 2);
    } else {
        y -= baseWidth / 2;
    }

    y -= self->originX;
    x -= self->originY;

    int local = -1;
    DrawUiPrimitive_1p2(self, baseWidth, x, y, &local,
                        0x951, 0x60, -1);
}

// 0x00A23A00 (inside parent 0x00A239C0 family)
// PARTIAL STRUCTURAL reconstruction.
// Direct xrefs prove this operation branches on +0x3084/+0x3085 and belongs to
// the same split-aware menu geometry family as A23B40. The complete arithmetic
// body is not reproduced here until its exact Stage23 instruction listing is
// recovered, so no guessed constants are introduced.
void DrawPrimarySplitMenu_A23A00(UiPanel120* self)
{
    (void)self;
    // Intentionally unresolved: exact instruction body pending direct listing.
}

// 0x00A24850..0x00A24C70
// The 1.2.0 function is 0x421 bytes and calls both A23A00 and A23B40.
// Existing evidence proves the dependency set and split fields but does not yet
// provide a trustworthy C reconstruction of the entire body. Keep this as a
// named boundary instead of pasting relocated machine code as fake decompilation.
void BuildSplitMenuTop22_A24850(UiPanel120* self)
{
    (void)self;
    // Pending exact instruction-to-C pass.
}

} // namespace re5::split120
