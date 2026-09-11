#include <cstdint>
namespace re5::split120 {
struct SplitCoordinateState120 {
    std::uint8_t _0000[0x3074];
    float splitParam;   // +3074
    float splitScale2;  // +3078
    float splitOffset;  // +307C
    float splitVertical;// +3080
};
struct GameRoot120;
extern GameRoot120* gRoot_12340A4;
extern int QueryGameStatus_C42D90(void* session);
extern void* RootSession_1042C(GameRoot120*);
// These two CRT helpers preserve the exact native float->double->x87->int
// rounding path used at 0x76A420/460. The reconstruction intentionally keeps
// them explicit instead of replacing them with a guessed C library rounding mode.
extern double NativeRoundHelper_E64AD0(double);
extern int NativeX87ToInt_E63D80(double);
static inline int NativeScaledRound(float v){return NativeX87ToInt_E63D80(NativeRoundHelper_E64AD0(static_cast<double>(v)));}
static inline SplitCoordinateState120* S(void* p){return static_cast<SplitCoordinateState120*>(p);}

// 0x0076A420..0x0076A45B -- VERIFIED native ABI: ECX=split, stack x, RET 4.
int __thiscall SplitCoordTransform_76A420(void* selfRaw,int x){
    auto*self=S(selfRaw);
    return static_cast<int>(static_cast<float>(NativeScaledRound(float(x)*self->splitParam))+self->splitOffset);
}
// 0x0076A460..0x0076A49B -- VERIFIED native ABI: ECX=split, stack x, RET 4.
int __thiscall FullCoordTransform_76A460(void* selfRaw,int x){
    auto*self=S(selfRaw);
    return static_cast<int>(static_cast<float>(NativeScaledRound(float(x)*self->splitScale2))+self->splitOffset);
}
// 0x0076A4A0..0x0076A4D3 -- VERIFIED. Only status 1 gets vertical split offset.
int __thiscall SplitVerticalTransform_76A4A0(void* selfRaw,int y){
    auto*self=S(selfRaw);
    if(QueryGameStatus_C42D90(RootSession_1042C(gRoot_12340A4))!=1)return y;
    return static_cast<int>(float(y)+self->splitVertical);
}
}
