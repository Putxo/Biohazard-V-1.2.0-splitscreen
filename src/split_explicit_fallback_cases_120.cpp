#include "re5/split_localcoop_120.hpp"

namespace re5::split120 {

// Exact explicit-event portion of 0x009E5870, before the generic
// 55-record table fallback. Argument ordering follows the native thiscall:
//   (arg1, x, y, arg4, eventId, arg6, arg7)
// and the dispatcher returns true when the event was consumed.

extern int DrawHeaderAdvance_9E3D40(void* self, int arg1, int eventId, int arg7);
extern int DrawShiftedByMeasuredExtent_9E3E70(void* self, int arg1, int x, int y,
                                             int arg4, int resourceId,
                                             int arg6, int arg7);
extern void DrawOpaque_9E3E10(void* self, int arg1, int x, int y,
                              int arg4, int resourceId, int arg6);
extern void DrawCenteredSplitAware_9E5E90(void* self, int arg1, int x, int y,
                                          int arg4, int resourceId, int arg6);
extern bool DrawSplitAwareDispatch_9E5DE0(void* self, int arg1, int x, int y,
                                         int arg4, int resourceId,
                                         int arg6, int arg7);

static bool DrawMeasuredSequence_112_118(void* self, int arg1, int x, int arg4,
                                        int eventId, int arg6, int arg7,
                                        bool include97C)
{
    int cursor = x + DrawHeaderAdvance_9E3D40(self, arg1, eventId, arg7);

    int extent = DrawShiftedByMeasuredExtent_9E3E70(
        self, arg1, cursor, 0x279, arg4, 0x97F, arg6, -1);
    cursor += -2 - extent;

    extent = DrawShiftedByMeasuredExtent_9E3E70(
        self, arg1, cursor, 0x279, arg4, 0x97E, arg6, -1);
    cursor += -2 - extent;

    extent = DrawShiftedByMeasuredExtent_9E3E70(
        self, arg1, cursor, 0x279, arg4, 0x97D, arg6, -1);

    if (include97C) {
        cursor += -2 - extent;
        DrawShiftedByMeasuredExtent_9E3E70(
            self, arg1, cursor, 0x279, arg4, 0x97C, arg6, -1);
    }
    return true;
}

// 0x009E58A6..0x009E5C6B — VERIFIED explicit switch cases.
bool DrawExplicitSplitFallbackCase_9E58A6(void* self,
                                           int arg1, int x, int y,
                                           int arg4, int eventId,
                                           int arg6, int arg7)
{
    (void)y; // explicit native cases replace Y with fixed constants below.

    switch (eventId) {
    case 0x112:
        return DrawMeasuredSequence_112_118(self, arg1, x, arg4,
                                           0x112, arg6, arg7, true);

    case 0x118:
        return DrawMeasuredSequence_112_118(self, arg1, x, arg4,
                                           0x118, arg6, arg7, false);

    case 0x115:
        DrawSplitAwareDispatch_9E5DE0(self, arg1, 0x388, 0x279,
                                      arg4, 0x980, arg6, -1);
        DrawOpaque_9E3E10(self, arg1, 0x45A, 0x279,
                          arg4, 0x967, arg6);
        return true;

    case 0x14D: {
        int cursor = x + DrawHeaderAdvance_9E3D40(self, arg1, 0x14D, arg7);

        int extent = DrawShiftedByMeasuredExtent_9E3E70(
            self, arg1, cursor, 0x279, arg4, 0x14E, arg6, -1);
        cursor += -2 - extent;

        // 0x9E5AC3 jumps into the shared 0x9E593E tail: 0x97E then 0x97D.
        extent = DrawShiftedByMeasuredExtent_9E3E70(
            self, arg1, cursor, 0x279, arg4, 0x97E, arg6, -1);
        cursor += -2 - extent;

        DrawShiftedByMeasuredExtent_9E3E70(
            self, arg1, cursor, 0x279, arg4, 0x97D, arg6, -1);
        return true;
    }

    case 0x2B8:
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x0CF, 0x279,
                                      arg4, 0x979, arg6);
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x1DC, 0x279,
                                      arg4, 0x978, arg6);
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x45A, 0x279,
                                      arg4, 0x113, arg6);
        return true;

    case 0x2B9:
    case 0x2C6:
    case 0x2C8:
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x45A, 0x279,
                                      arg4, 0x113, arg6);
        return true;

    case 0x2BE:
    case 0x2C5:
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x201, 0x279,
                                      arg4, 0x99A, arg6);
        [[fallthrough]];
    case 0x2BF:
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x30F, 0x279,
                                      arg4, 0x99B, arg6);
        DrawCenteredSplitAware_9E5E90(self, arg1, 0x45A, 0x279,
                                      arg4, 0x99C, arg6);
        return true;

    case 0x694:
        DrawSplitAwareDispatch_9E5DE0(self, arg1, 0x60, 0x258,
                                      arg4, 0x972, arg6, -1);
        DrawSplitAwareDispatch_9E5DE0(self, arg1, 0x60, 0x279,
                                      arg4, 0x967, arg6, -1);
        return true;

    default:
        return false;
    }
}

} // namespace re5::split120
