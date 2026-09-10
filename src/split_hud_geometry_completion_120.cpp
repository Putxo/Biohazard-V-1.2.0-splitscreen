#include <cstdint>
namespace re5::split120 {
struct Split { unsigned char p[0x3064]; unsigned char active; unsigned char q[0xb]; float scale; float param; float scale2; float offset; float vertical; unsigned char full; unsigned char transient; };
extern Split* gSplit_123457C; extern int Status_C42D90(); extern int X_76A420(Split*,int); extern int XFull_76A460(Split*,int); extern int Y_76A4A0(Split*,int); extern int PanelY_76C2A0(Split*,int); extern bool IsFullSplitEffective(Split*);
struct Node{unsigned char p[0x50];float x,y,z;unsigned char q[0x14];float sx,sy,sz;};
void PromptGeometry_A0FD72(int baseX,int& x,int& halfGap,float& scale){if(Status_C42D90()!=1){x=baseX;return;} scale=gSplit_123457C->scale; float h=scale*360.f; x=XFull_76A460(gSplit_123457C,baseX); halfGap=int((360.f-h)*0.5f);}
float PromptScale_A0FE14(int mode){float s=gSplit_123457C->scale*40.f; if(mode==9||mode==10)s*=0.699999988079071f; return s;}
void ScaleHudPoint_A33778(int& x,int& y,float& s){if(Status_C42D90()!=1)return; s=gSplit_123457C->scale; x=XFull_76A460(gSplit_123457C,int(s*40.f)); y=Y_76A4A0(gSplit_123457C,int(float(y)*s));}
void TransformHudPoint_A3383F(int& x,int& y){if(Status_C42D90()!=1)return; x=XFull_76A460(gSplit_123457C,x); y=Y_76A4A0(gSplit_123457C,int(float(y)*gSplit_123457C->scale));}
// Same native +3074 transform occurs at A3427F, A34337 and A343D2.
void ScaleHudNodeParam_A34260(Node* n,int x){if(!n)return; n->x=float(X_76A420(gSplit_123457C,x)); n->sx*=gSplit_123457C->param; n->sy*=gSplit_123457C->param;}
// 0xA3440E..A344B8. Note native IDIV uses integer W/H before conversion to
// float. Keeping that unusual order is required for instruction-faithful output.
int ComputeHudHorizontalSplitBias_A3440E(int status,int physicalW,int physicalH){
    int bias=0;
    if(status!=0){
        bias=160;
        if(IsFullSplitEffective(gSplit_123457C))
            bias=int((1.0f-gSplit_123457C->scale)*1280.0f+160.0f);
        if(gSplit_123457C->active){
            const int integerAspect=physicalH!=0?physicalW/physicalH:0;
            bias=int(float(bias)+float(integerAspect)*0.5625f*1280.0f);
        }
    } else if(gSplit_123457C->active){
        const int integerAspect=physicalH!=0?physicalW/physicalH:0;
        bias=int(float(integerAspect)*0.5625f*1280.0f);
    }
    return bias;
}
// 0xA34591..A345B7: resource pair is updated for every nonzero status; at
// status 0 it is updated only while splitActive is true.
bool HudSplitResourcesEnabled_A345AB(int status){return status!=0||gSplit_123457C->active!=0;}
void CenteredHudNode_A5C501(Node* n){if(!n)return; n->x=float(X_76A420(gSplit_123457C,0x190)); n->y=float(PanelY_76C2A0(gSplit_123457C,180)); n->sx*=gSplit_123457C->scale; n->sy*=gSplit_123457C->scale; n->sz*=gSplit_123457C->scale;}
void ScaleDescriptorIfSplit_A5C643(float& v,bool enabled){if(enabled)v*=gSplit_123457C->scale;}
void ScaleHudPair_A704B9(int& a,int& b){if(Status_C42D90()!=1)return;a=int(float(a)*gSplit_123457C->scale);b=int(float(b)*gSplit_123457C->scale);}
int ScaleHud32_A7063F(int fallback){return Status_C42D90()==1?int(gSplit_123457C->scale*32.f):fallback;}
float LocalHudDisplacement_A708C3(float base){return base-((gSplit_123457C->scale*2.f)-1.f)*20.f;}
void TransformTwoPlayerHudNode_A7352D(Node* n){if(!n||Status_C42D90()!=1)return;n->x*=gSplit_123457C->scale;n->x+=gSplit_123457C->offset;n->y*=gSplit_123457C->scale;n->y=float(Y_76A4A0(gSplit_123457C,int(n->y)));n->sx=n->sy=n->sz=gSplit_123457C->scale;}
void SetWidgetNodeScale_9F9959(Node* n){if(n)n->sx=n->sy=n->sz=gSplit_123457C->scale;}
}
