#include "re5/split_localcoop_120.hpp"
#include <cstddef>
#include <cstdint>

namespace re5::split120 {

extern void SplitDrawPrimary_A23A00(void* self, int itemIndex);
extern void DrawSecondarySplitMenu_A23B40(void* self);
extern int QueryGameStatus_C42D90();
extern SplitRenderState120* GetSplitState_123457C();
extern int UiIndex_A17800(void* self);
extern std::uint16_t ScaleUiWord_76A4A0(int v);
extern std::uint16_t ReadWord_112BDF2();
extern std::uint16_t ReadWord_112BE50();
extern std::uint16_t ReadWord_112BE54(int index);
extern const void* UiTableEntry_112C080(int index);
extern std::uint32_t UiArray_112C144(int index);
extern void DrawMain_9E5E90(void* self, int a0, int x, int y,
                            const void* descriptorRef, int id, int h);
extern void DrawNamed_9E5330(void* self, const char* name, int x, int y,
                             const float* scale, float one);
extern void Format_E63B3E(char* dst, const char* fmt, std::uint32_t value);
extern void List_9E5020(void* self, char* text, int x, int y);
extern const char* Str_FA80D4();
extern const char* Str_FA80CC();
extern const char* Str_10BAC14();
extern const char* Fmt_FA6C20();
extern const char* Fmt_FA6C14();
extern const void* DescriptorTableBase_112C080();

namespace {

template <class T>
static inline T& f(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(p) + off);
}

static inline int truncf(float v) { return static_cast<int>(v); }

} // namespace

// 0x00A24850..0x00A24C70
// Direct structured reconstruction. The body is fully disassembled, but the
// semantic names/types of 0x9E5330/0x9E5020 data arguments remain provisional.
void SplitTop22_A24850(void* self)
{
    const int baseIndex = UiIndex_A17800(self); // EDI at entry

    std::uint16_t topWord;
    if (QueryGameStatus_C42D90() == 1) {
        const int scaled = truncf(GetSplitState_123457C()->splitScale * 144.0f);
        topWord = ScaleUiWord_76A4A0(scaled);
    } else {
        topWord = ReadWord_112BDF2();
    }

    // Native first draw pushes literal address 0x112C080, not table[0].
    DrawMain_9E5E90(self, baseIndex, 0x280, static_cast<std::int16_t>(topWord),
                    DescriptorTableBase_112C080(), 0x530, 0x60);

    int yCursor = static_cast<std::int16_t>(topWord); // EBX
    if (QueryGameStatus_C42D90() == 1) {
        yCursor = truncf(GetSplitState_123457C()->splitScale * 54.0f +
                         static_cast<float>(yCursor) + 30.0f);
    } else {
        yCursor += 0x34;
    }

    // Native loop executes exactly once: EBP=0, then ++EBP and CMP EBP,1/JL.
    const int i = 0;
    int drawIndex = baseIndex;
    if (i != f<std::int32_t>(self, 0x48))
        drawIndex = (baseIndex * 0x56) / 100;

    // The selected table value is stored in a stack local; 0x9E5E90 receives
    // the address of that local, not the raw table entry value directly.
    const void* descriptor = UiTableEntry_112C080(
        i == f<std::int32_t>(self, 0x48) ? 0 : 1);

    if (i == 0) {
        const float scale = QueryGameStatus_C42D90() == 1
            ? GetSplitState_123457C()->splitScale : 1.0f;
        const std::uint16_t id = ReadWord_112BE50();

        int x = QueryGameStatus_C42D90() != 1
            ? 0x17C
            : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * 260.0f);
        DrawMain_9E5E90(self, drawIndex, x, yCursor, &descriptor, id, 0x60);

        x = QueryGameStatus_C42D90() != 1
            ? 0x33E
            : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * -190.0f);
        DrawNamed_9E5330(self, Str_FA80D4(), x, yCursor, &scale, 1.0f);

        const int ratio = (f<std::int32_t>(self, 0x190) * 0x137) /
                          f<std::int32_t>(self, 0x194);
        int pointX = ratio + 0x2A1;
        if (QueryGameStatus_C42D90() == 1) {
            pointX = truncf(static_cast<float>(pointX - 0x280) *
                            GetSplitState_123457C()->splitScale2) + 0x280;
        }
        DrawNamed_9E5330(self, Str_FA80CC(), pointX, yCursor + 0x11,
                         &scale, 1.0f);

        x = QueryGameStatus_C42D90() != 1
            ? 0x33C
            : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * -188.0f);
        DrawNamed_9E5330(self, Str_10BAC14(), x, yCursor + 0x4E,
                         &scale, 1.0f);
    }

    if (i == f<std::int32_t>(self, 0x48)) {
        const int current = f<std::int32_t>(self, 0x190);
        const int limit = f<std::int32_t>(self, 0x194);
        char text[64]{};

        if (current > 0) {
            Format_E63B3E(text, Fmt_FA6C20(), UiArray_112C144(i));
            int x = 0x280;
            if (QueryGameStatus_C42D90() == 1)
                x -= truncf(GetSplitState_123457C()->splitScale2 * -0.0f);
            List_9E5020(self, text, x, yCursor);
        }

        if (current < limit) {
            Format_E63B3E(text, Fmt_FA6C14(), UiArray_112C144(i));
            const int x = QueryGameStatus_C42D90() != 1
                ? 0x3FC
                : 0x280 - truncf(GetSplitState_123457C()->splitScale2 * -380.0f);
            List_9E5020(self, text, x, yCursor);
        }
    }

    // A24C10 adds entry-local [S+0x14] == baseIndex. It does NOT add the
    // potentially 86%-scaled drawIndex. After POP EBP, A24C31 and A24C40
    // reference that same baseIndex local again.
    yCursor += baseIndex;
    yCursor += 0x82;
    if (f<std::int32_t>(self, 0x20) == 0)
        yCursor += baseIndex;

    const int selected = f<std::int32_t>(self, 0x48);
    const std::uint16_t footerId = ReadWord_112BE54(selected);
    DrawMain_9E5E90(self, baseIndex, 0x280, yCursor,
                    DescriptorTableBase_112C080(), footerId, 0x60);

    SplitDrawPrimary_A23A00(self, 2);
    DrawSecondarySplitMenu_A23B40(self);
}

} // namespace re5::split120
