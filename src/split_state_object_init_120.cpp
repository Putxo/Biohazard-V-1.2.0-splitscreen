#include <cstdint>
namespace re5::split120 {
struct SplitStateInitView120 {
    std::uint8_t _0[0x2f54];
    std::int32_t field2f54;
    std::uint8_t _2f58[0x3000-0x2f58];
    void* resource3000;
    void* resource3004;
    void* resource3008;
    std::uint8_t _300c[0x3064-0x300c];
    std::uint8_t splitActive;
    std::uint8_t _3065[3];
    std::int32_t logicalWidth;
    std::int32_t logicalHeight;
    std::uint8_t _3070[0x307c-0x3070];
    float splitOffset;
    std::uint8_t _3080[4];
    std::uint8_t fullScreenSplitMode;
};
// 0x0076DDC0..0x0076E05C constructor -- split/local fields only.
// The long constructor also initializes unrelated display/resources; those
// members are intentionally outside this decompilation scope.
void InitSplitStateFields_76DDC0(SplitStateInitView120* self){
    self->field2f54=1;
    self->resource3000=nullptr;
    self->resource3004=nullptr;
    self->resource3008=nullptr;
    self->logicalWidth=0x320;
    self->logicalHeight=0x168;
    self->fullScreenSplitMode=0;
    self->splitActive=0;
    self->splitOffset=0.0f;
}
}
