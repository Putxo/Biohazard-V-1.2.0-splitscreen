#include <cstdint>
namespace re5::split120 {
struct SplitStateView120{std::uint8_t _0[0x3064];std::uint8_t active;std::uint8_t _3065[0x3070-0x3065];float scale;float scaleX;float param;float offset;float vertical;std::uint8_t full;std::uint8_t transient;};
extern SplitStateView120* gSplit_123457C; extern int FullX_76A460(void*,int); extern int SplitY_76A4A0(void*,int); extern int PanelY_76C2A0(void*,int); extern bool EffectiveFull_763170(void*);
struct XYScale120{float x,y,s;};
XYScale120 SplitDrawTransform_9E1C08(float x,float y,float s,bool status1){if(status1){s*=gSplit_123457C->scale;x=float(FullX_76A460(gSplit_123457C,int(x)));y*=0.5f;}return{x,y,s};}
XYScale120 SplitCenteredTransform_9E2A21(float x,float s,bool status1){float y=360.0f;if(status1){s*=gSplit_123457C->scale;x=float(FullX_76A460(gSplit_123457C,int(x)));y=float(PanelY_76C2A0(gSplit_123457C,180));}return{x,y,s};}
struct PackedBounds120{int left,width;};
PackedBounds120 SplitPackedBounds_9E3990(bool enabled){if(!enabled)return{0,0};return gSplit_123457C->active?PackedBounds120{-0x500,0xF00}:PackedBounds120{0,0x500};}
XYScale120 SplitHudAnchor_9EAE23(int x,int y,float baseScale,bool status1){if(!status1)return{float(x),float(y),baseScale};float s=gSplit_123457C->scale;return{float(FullX_76A460(gSplit_123457C,x)),float(SplitY_76A4A0(gSplit_123457C,int(float(y)*s))),s};}
void ApplySplitNodeXYZScale_9EC52B(void*nodeRaw,bool status1){auto*n=static_cast<std::uint8_t*>(nodeRaw);float s=1.0f;if(status1){float x=*reinterpret_cast<float*>(n+0x50);x=float(FullX_76A460(gSplit_123457C,int(x)));*reinterpret_cast<float*>(n+0x50)=x;s=gSplit_123457C->scale;}*reinterpret_cast<float*>(n+0x70)=s;*reinterpret_cast<float*>(n+0x74)=s;*reinterpret_cast<float*>(n+0x78)=s;}
struct SplitSpriteMetric120{int x,width;float scale;};
SplitSpriteMetric120 SplitSpriteMetric_9ED443(int x,int width,float defaultScale,bool status1){if(!status1)return{x,width,defaultScale};return{x/2,int(float(width)*gSplit_123457C->param),gSplit_123457C->scale};}
struct SplitSize120{int size;float scale;}; SplitSize120 SplitSize32_9ED812(int normal,float normalScale,bool status1){if(!status1)return{normal,normalScale};float s=gSplit_123457C->scale;return{int(s*32.0f),s};}
void ComposeSplitNode_A162CA(void*dstRaw,const void*srcRaw,bool status1){auto*d=static_cast<std::uint8_t*>(dstRaw);auto*s=static_cast<const std::uint8_t*>(srcRaw);float k=status1?gSplit_123457C->scale:1.0f;*reinterpret_cast<float*>(d+0x50)+=*reinterpret_cast<const float*>(s+0x50)*k;*reinterpret_cast<float*>(d+0x54)+=*reinterpret_cast<const float*>(s+0x54)*k;if(status1){*reinterpret_cast<float*>(d+0x70)=k;*reinterpret_cast<float*>(d+0x74)=k;*reinterpret_cast<float*>(d+0x78)=k;}}
void ApplySplitVerticalScale_A1E384(void*nodeRaw){auto*n=static_cast<std::uint8_t*>(nodeRaw);float&y=*reinterpret_cast<float*>(n+0x54);y*=((gSplit_123457C->scale+1.0f)*0.5f);y+=float(SplitY_76A4A0(gSplit_123457C,0));}
float SplitResourceUniformScale_7F25B1(bool status1){float s=gSplit_123457C->active?2.0f:1.0f;if(status1)s*=2.0f;return s;}
}
