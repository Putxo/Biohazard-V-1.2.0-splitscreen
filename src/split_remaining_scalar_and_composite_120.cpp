#include <cstdint>
namespace re5::split120 {
struct SplitState120{std::uint8_t _0[0x3064];std::uint8_t active;std::uint8_t _3065[0x3070-0x3065];float scale;};
extern SplitState120* gSplit_123457C;
extern int PanelTransform_76C2A0(void*,int);

// 0x00852B27..0x00852B79 -- exact scalar selection. Native starts at 1.0,
// selects 2.0 when splitActive, then multiplies by 2.0 again for status 1.
float ComputeSplitRuntimeScalar_852B27(int status){
    float value=gSplit_123457C->active?2.0f:1.0f;
    if(status==1)value*=2.0f;
    return value;
}

// 0x009EAE23..0x009EAE71 and repeated 0x009EAF00..0x009EAF2F.
// Both draw passes use the same status-1 transform: X through FullCoord,
// Y multiplied by splitScale then passed through 76A4A0; draw scale is exactly
// splitScale and width/measure becomes trunc(splitScale*40).
struct SplitHudPass120{int x;int y;int measure;float scale;};
SplitHudPass120 TransformSplitHudPass_9EAE23(int status,int x,int y){
    if(status!=1)return{x,y,40,1.0f};
    const float s=gSplit_123457C->scale;
    return{FullCoordTransform_76A460(gSplit_123457C,x),
           SplitVerticalTransform_76A4A0(gSplit_123457C,int(float(y)*s)),
           int(s*40.0f),s};
}
SplitHudPass120 TransformSplitHudPass_9EAF00(int status,int x,int y){return TransformSplitHudPass_9EAE23(status,x,y);}

// 0x00A162B5..0x00A163C4 -- split-specific composition from A16000.
// Initial split branch transforms Y, adds panel offset 24 and sets uniform scale;
// the later child composition multiplies child X/Y by the same splitScale.
void InitCompositeSplitNode_A162B5(float& y,float& sx,float& sy,float& sz){
    y=float(SplitVerticalTransform_76A4A0(gSplit_123457C,int(y)));
    y+=float(PanelTransform_76C2A0(gSplit_123457C,24));
    sx=sy=sz=gSplit_123457C->scale;
}
void AddCompositeSplitChild_A1637B(float childX,float childY,float& x,float& y,float& sx,float& sy,float& sz){
    const float s=gSplit_123457C->scale;
    x+=childX*s; y+=childY*s; sx=sy=sz=s;
}

// A703D0 repeats this exact status-1 scalar at A70645, A707A3 and A70A68.
int HudStatus1Size32_A70645(){return int(gSplit_123457C->scale*32.0f);}
} // namespace re5::split120
