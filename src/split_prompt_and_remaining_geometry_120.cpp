#include <cstdint>
namespace re5::split120 {
struct SplitState { unsigned char pad[0x3064]; unsigned char active; unsigned char p1[0xb]; float scale; float param; float scale2; float offset; float vertical; unsigned char full; unsigned char transient; };
struct Display { unsigned char p[0x50]; int w; int h; };
extern SplitState* gSplit_123457C; extern Display* gDisplay_12345D4; extern unsigned char* gRoot_12340A4; extern unsigned char* gInput_1249C40;
extern int Status_C42D90(); extern int MapPlayer_76A1E0(SplitState*,int); extern int MapViewport_76A250(SplitState*,int); extern int Y_76A4A0(SplitState*,int); extern int XFull_76A460(SplitState*,int); extern bool EffectiveFull_763170(SplitState*);
extern void* DefaultActor_7E6290(); extern void DrawIndicator_7DDFB0(int,int,float,float,float,int); extern float FrameDelta();
void UpdateLocalInputIndicator_79BE50(unsigned char* self, bool physicalActivity, bool pairedIndicator)
{
    auto& state=self[0x5F4]; auto& timer=*reinterpret_cast<float*>(self+0x5F8);
    if(state==0){ timer=0.0f; state=1; }
    if(self[0x5F1] != gInput_1249C40[0x5F0]) timer=3.0f;
    if(physicalActivity) timer=3.0f;
    if(timer<=0.0f) return;
    int player=-1; if(void* a=DefaultActor_7E6290()) player=MapPlayer_76A1E0(gSplit_123457C, static_cast<unsigned char*>(a)[0x2D9C]);
    if(player<0) return; int viewport=MapViewport_76A250(gSplit_123457C,player);
    int resource=self[0x5F1]?0x954:0x955; float x=96.f,y=24.f,s=30.f;
    if(Status_C42D90()==1){
        player=MapPlayer_76A1E0(gSplit_123457C,*reinterpret_cast<int*>(self+0x614));
        viewport=MapViewport_76A250(gSplit_123457C,player);
        if(gSplit_123457C->active && !EffectiveFull_763170(gSplit_123457C)) x=gSplit_123457C->offset+80.f;
        y=float(Y_76A4A0(gSplit_123457C,int(gSplit_123457C->scale*117.f)));
        s=gSplit_123457C->scale*30.f;
    } else if(gSplit_123457C->active) x=96.f-gSplit_123457C->offset;
    if(pairedIndicator) resource=0x32;
    DrawIndicator_7DDFB0(resource,viewport,x,y,s,player);
    timer-=FrameDelta(); if(timer<0.f) timer=0.f;
}
float AdjustMeasuredSplitDelta_7B3A04(float base,float measured,bool active,int w,int h){float d=base-measured; if(active) return d*1.3235293626785278f;float ratio=(float(w)*0.5625f)/float(h); if(ratio<=1.f) return d;float inv=1.f/ratio; float scaled=measured*inv; return base-scaled+(ratio-1.f)*272.f;}
struct ResourceGeom { unsigned char p[0x14]; float x; unsigned char p18[4]; float width; float scale; };
void ApplyWideSplitResources_A00FD9(ResourceGeom* a,ResourceGeom* b,int w,int h){if(!gSplit_123457C->active) return;if(a){ a->x += -1280.f; a->width=1280.f; }if(b) b->scale=(float(w)/float(h))*0.5625f;}
struct A5FGeometry { int x; int width; int tail; };
A5FGeometry ComputeA5FGeometry_A5F79D(bool active,int w,int h){A5FGeometry o{}; o.width=1280; o.x=0;if(active){o.x=0x58;o.width=0xA00;o.tail=int(((float(h)/float(w))*2.3529410362243652f-1.f)*640.f);return o;}float ratio=(float(w)*0.5625f)/float(h); if(ratio>1.f)o.x=int((ratio-1.f)*272.f); return o;}
float SelectType2AspectScalar_BFB522(bool splitActive,float normalScalar){ return splitActive?1.3235293626785278f:normalScalar; }
void ApplySplitNodeUniformScale_9F995F(float* xyz){ if(!xyz)return; xyz[0]=xyz[1]=xyz[2]=gSplit_123457C->scale; }
int ComputeSplitRowY_A22D50(int fallback){ return Status_C42D90()==1?Y_76A4A0(gSplit_123457C,int(gSplit_123457C->scale*144.f)):fallback; }
int InterpolateSplitWidth_A22DCC(int a,int b){ return a+int(float(b-a)*gSplit_123457C->scale2); }
}
