#include <cstdint>
namespace re5::split120 {
struct SplitState120{std::uint8_t _0[0x3070];float scale;};
extern SplitState120* gSplit_123457C;
extern int PanelTransform_76C2A0(void*,int);
static inline int truncf(float v){return static_cast<int>(v);}
static inline int halfTowardZero(int v){return v/2;}

// 0x00A2DCE1..0x00A2DD29 -- exact status-1 geometry branch.
struct DrawGeometryA2DC120 { int x; int y; int width; };
DrawGeometryA2DC120 ApplyA2DCSplitGeometry_A2DCE1(int x,int yBase){
    const int tx=FullCoordTransform_76A460(gSplit_123457C,x);
    const int py=PanelTransform_76C2A0(gSplit_123457C,halfTowardZero(yBase));
    const int width=truncf(gSplit_123457C->scale*30.0f);
    return {tx,py+width,width};
}

// 0x00A2DF55..0x00A2DF7D -- status 1 overrides object+0x30 with splitScale*36.
int A2DFStatus1RowWidth_A2DF5F(){return truncf(gSplit_123457C->scale*36.0f);}

// 0x00A2EF59..0x00A2EF83 -- status 1 computes splitScale*200 then applies
// native vertical offset helper 0x76A4A0.
int A2EFStatus1Y_A2EF63(){return SplitVerticalTransform_76A4A0(gSplit_123457C,truncf(gSplit_123457C->scale*200.0f));}

// 0x00A30339..0x00A30377 -- exact row stride logic from A302D0.
// Status 1: trunc(splitScale*36 + 1). Other statuses: field04==0 ? 36 : 30.
int ComputeA302D0RowStride_A30339(bool status1,bool field04NonZero){
    if(status1)return truncf(gSplit_123457C->scale*36.0f+1.0f);
    return field04NonZero?30:36;
}
}
