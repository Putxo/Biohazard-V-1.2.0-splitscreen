#include <cstdint>
namespace re5::split120 {
struct SplitOverlayState120{std::uint8_t _0[0x3064];std::uint8_t splitActive;std::uint8_t _3065[0x3070-0x3065];float splitScale;float splitParam;float splitScale2;float splitOffset;};
struct OverlayMetrics120{float primaryScale;float itemHeight;float logicalWidth;float uniformScale;int horizontalBias;};
extern bool IsFullSplitEffective(const void*);
extern int PanelTransform_76C2A0(void*,int);
static inline int truncf(float v){return static_cast<int>(v);}
OverlayMetrics120 PrepareLocalOverlaySplitMetrics_764BB1(const SplitOverlayState120* s,int status,int profileMode,bool frontendWideLayout,int physicalW,int physicalH)
{
    OverlayMetrics120 o{0.8f,36.0f,1280.0f,1.0f,0};
    if(status==1){
        o.primaryScale=s->splitScale*0.8f;
        if(!IsFullSplitEffective(s))o.logicalWidth=s->splitScale2*1280.0f;
        o.itemHeight=s->splitScale*36.0f;o.uniformScale=s->splitScale;
        if(profileMode==9||profileMode==10){o.itemHeight*=0.699999988079071f;o.primaryScale*=0.699999988079071f;}
    }
    if(s->splitActive && frontendWideLayout){
        const float v=((float(physicalH)/float(physicalW))*2.3529410362243652f-1.0f)*o.logicalWidth*0.5f;
        o.horizontalBias=truncf(v);
    }
    if(status==1&&!IsFullSplitEffective(s))o.horizontalBias=truncf(float(o.horizontalBias)+s->splitOffset);
    return o;
}
void TransformOverlayPairY_764F7B(void* split,int status,int& y0,int& y1){if(status==1){y0=PanelTransform_76C2A0(split,y0);y1=PanelTransform_76C2A0(split,y1);}}
void TransformOverlayNodeXY_7653EA(void* split,int status,float& x,float& y){if(status==1){x=float(SplitCoordTransform_76A420(split,truncf(x)));y=float(PanelTransform_76C2A0(split,truncf(y)));}}
void TransformOverlayFullNode_765859(void* split,int status,float& x,float& y){if(status==1){x=float(FullCoordTransform_76A460(split,truncf(x)));y=float(PanelTransform_76C2A0(split,truncf(y*0.5f)));}}
float TransformMeasuredOverlay_765C52(void* split,const SplitOverlayState120* s,int status,int profileMode,float& x,float& y){if(status!=1)return 36.0f;x=float(FullCoordTransform_76A460(split,truncf(x)));y=float(PanelTransform_76C2A0(split,truncf(y*0.5f)));float measure=s->splitScale*36.0f;if(profileMode==9||profileMode==10)measure*=0.699999988079071f;return measure;}
void ApplyOverlayNodeSplit_765E2D(void* split,const SplitOverlayState120* s,int status,float& x,float& y,float& sx,float& sy,float& sz){if(status!=1)return;x=float(FullCoordTransform_76A460(split,truncf(x)));y=float(PanelTransform_76C2A0(split,truncf(y*0.5f)));sx=sy=sz=s->splitScale;}
}
