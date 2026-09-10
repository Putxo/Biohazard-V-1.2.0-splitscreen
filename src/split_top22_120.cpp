#include "re5/split_localcoop_120.hpp"
#include <cstddef>
#include <cstdint>

namespace re5::split120 {

namespace {

template <class T>
static inline T& f(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(p) + off);
}

extern int QueryGameStatus_C42D90();
extern SplitRenderState120* GetSplitState_123457C();
extern int UiIndex_A17800(void* self);
extern std::uint16_t ScaleUiWord_76A4A0(int v);
extern std::uint16_t ReadWord_112BDF2();
extern std::uint16_t ReadWord_112BE50();
extern std::uint16_t ReadWord_112BE54(int index);
extern const void* UiTable_112C080(int index);
extern std::uint32_t UiArray_112C144(int index);
extern void DrawMain_9E5E90(void* self, int a0, int x, int y,
                            const void* table, int id, int h);
extern void DrawNamed_9E5330(void* self, const char* name, int x, int y,
                             const float* scale, float one);
extern void Format_E63B3E(char* dst, const char* fmt, std::uint32_t value);
extern void List_9E5020(void* self, char* text, int x, int y);
extern const char* Str_FA80D4();
extern const char* Str_FA80CC();
extern const char* Str_10BAC14();
extern const char* Fmt_FA6C20();
extern const char* Fmt_FA6C14();

static inline int truncf(float v) { return static_cast<int>(v); }

} // namespace

// 0x00A24850..0x00A24C70
// Structured reconstruction from the original 0x421-byte 1.2.0 body.
void SplitTop22_A24850(void* self)
{
    int baseIndex = UiIndex_A17800(self);
    int rowAdvance = baseIndex;

    std::uint16_t topWord;
    if (QueryGameStatus_C42D90() == 1) {
        int scaled = truncf(GetSplitState_123457C()->splitScale * 144.0f);
        topWord = ScaleUiWord_76A4A0(scaled);
    } else {
        topWord = ReadWord_112BDF2();
    }

    DrawMain_9E5E90(self, baseIndex, 0x280, static_cast<std::int16_t>(topWord),
                    UiTable_112C080(0), 0x530, 0x60);

    int xCursor = static_cast<std::int16_t>(topWord);
    if (QueryGameStatus_C42D90() == 1)
        xCursor = truncf(GetSplitState_123457C()->splitScale * 54.0f +
                         static_cast<float>(xCursor) + 30.0f);
    else
        xCursor += 0x34;

    for (int i = 0; i < 1; ++i) {
        int localBaseIndex = baseIndex;
        if (i != f<std::int32_t>(self, 0x48))
            localBaseIndex = (baseIndex * 0x56) / 100;

        const void* table = UiTable_112C080(i == f<std::int32_t>(self, 0x48) ? 0 : 1);

        if (i == 0) {
            float scale = QueryGameStatus_C42D90() == 1
                            ? GetSplitState_123457C()->splitScale : 1.0f;
            std::uint16_t id = ReadWord_112BE50();
            int x = QueryGameStatus_C42D90() != 1
                      ? 0x17C
                      : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * 260.0f);
            DrawMain_9E5E90(self, localBaseIndex, x, xCursor, table, id, 0x60);

            x = QueryGameStatus_C42D90() != 1
                  ? 0x33E
                  : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * -190.0f);
            DrawNamed_9E5330(self, Str_FA80D4(), x, xCursor, &scale, 1.0f);

            int ratio = (f<std::int32_t>(self, 0x190) * 0x137) /
                        f<std::int32_t>(self, 0x194);
            int pointX = ratio + 0x2A1;
            if (QueryGameStatus_C42D90() == 1)
                pointX = truncf(static_cast<float>(pointX - 0x280) *
                                GetSplitState_123457C()->splitScale2) + 0x280;
            DrawNamed_9E5330(self, Str_FA80CC(), pointX, xCursor + 0x11, &scale, 1.0f);

            x = QueryGameStatus_C42D90() != 1
                  ? 0x33C
                  : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * -188.0f);
            DrawNamed_9E5330(self, Str_10BAC14(), x, xCursor + 0x4E, &scale, 1.0f);
        }

        if (i == f<std::int32_t>(self, 0x48)) {
            int current = f<std::int32_t>(self, 0x190 + i * 4);
            int limit = f<std::int32_t>(self, 0x194 + i * 4);
            char text[64]{};

            if (current > 0) {
                Format_E63B3E(text, Fmt_FA6C20(), UiArray_112C144(i));
                int x = 0x280;
                if (QueryGameStatus_C42D90() == 1)
                    x -= truncf(GetSplitState_123457C()->splitScale2 * -0.0f);
                List_9E5020(self, text, x, xCursor);
            }

            if (current < limit) {
                Format_E63B3E(text, Fmt_FA6C14(), UiArray_112C144(i));
                int x = QueryGameStatus_C42D90() != 1
                          ? 0x3FC
                          : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * -380.0f);
                List_9E5020(self, text, x, xCursor);
            }
        }

        xCursor += localBaseIndex;
        rowAdvance = localBaseIndex;
    }

    xCursor += 0x82;
    if (f<std::int32_t>(self, 0x20) == 0)
        xCursor += rowAdvance;

    int selected = f<std::int32_t>(self, 0x48);
    std::uint16_t footerId = ReadWord_112BE54(selected);
    DrawMain_9E5E90(self, rowAdvance, 0x280, xCursor,
                    UiTable_112C080(0), footerId, 0x60);

    SplitDrawPrimary_A23A00(self, 2);
    DrawSecondarySplitMenu_A23B40(self);
}

} // namespace re5::split120
