#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4; extern std::uint8_t* gSplit_123457C; extern std::uint8_t* gDisplay_12345D4; extern std::uint8_t* gUiMode_11B2018;
// 0x007DC143..0x007DC1B4 -- exact split-only delta correction isolated from its generic parent.
float ApplySplitMeasuredXDelta_7DC143(float elementX,float baseX){
 auto*session=*reinterpret_cast<void**>(gRoot_12340A4+0x1042C); float delta=elementX-baseX;
 if(QueryGameStatus_C42D90(session)==1)delta=float(SplitCoordTransform_76A420(gSplit_123457C,static_cast<int>(elementX)))-baseX;
 if(gSplit_123457C[0x3064]&&gUiMode_11B2018[0x20]){
   const float w=float(*reinterpret_cast<int*>(gDisplay_12345D4+0x50)); const float h=float(*reinterpret_cast<int*>(gDisplay_12345D4+0x54));
   delta=elementX*((h/w)*2.3529410362243652f)-baseX;
 }
 return delta;
}
}
