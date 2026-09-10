#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4; extern std::uint8_t* gSplit_123457C; extern std::uint8_t* gDisplay_12345D4; extern std::uint8_t* gProfile_11B2158; extern void* gPlayerManager_11B27DC; extern void* gUiInput_11B20C4;
extern int QueryGameStatus_C42D90(void*); extern int ResolveSplitPlayer_76A1E0(void*,int); extern int ResolveSplitSubIndex_76A250(void*,int); extern int FullCoordTransform_76A460(void*,int); extern int PanelTransform_76C2A0(void*,int);
extern void* FindPlayerActorByNo_7E6220(void*,int); extern int ResolvePlayerRuntime_7E6370(void*,void*);
extern void MeasureOverlay_7DC6B0(void*,int,int,void*,int,int,int,int); extern void* gUiRenderer_11B26F0;
extern void DrawOverlayNative_7DDFB0(void*,int,int,float,float,int,int,float,float,int,int,int,int); extern int RectBuild_422A80(void*,void*); extern bool UiHitRect_7B4570(void*,int,int,int); extern void ActorAction_B5C3A0(void*,int,float);
static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
struct OverlayRecord120 {std::uint8_t zero; std::uint8_t kind; std::uint16_t hi; std::uint16_t lo; std::uint8_t _06[2]; std::int32_t count; std::uint8_t _0C[4]; std::uint8_t flag10;};
struct Metric120 {float a,b,w,h;};
struct OverlayGeometry120 {float x,y,scale;int packedPlayer;};
static OverlayGeometry120 ComputeLocalOverlayGeometry(void*selfRaw,OverlayRecord120*rec,int player,float measuredW,float measuredH){
 auto*self=static_cast<std::uint8_t*>(selfRaw); const int mapped=ResolveSplitPlayer_76A1E0(gSplit_123457C,player); const int sub=ResolveSplitSubIndex_76A250(gSplit_123457C,mapped);
 float x=0.0f,y=0.0f,scale=1.0f;
 if(rec->kind==5){x=640.0f; y=96.0f-measuredH*0.5f; if(QueryGameStatus_C42D90(Session())==1)y=float(PanelTransform_76C2A0(gSplit_123457C,static_cast<int>(y)));}
 else {if(gPlayerManager_11B27DC){void*a=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,player);ResolvePlayerRuntime_7E6370(gPlayerManager_11B27DC,a);}int w=*reinterpret_cast<int*>(gDisplay_12345D4+0x50),h=*reinterpret_cast<int*>(gDisplay_12345D4+0x54); if(QueryGameStatus_C42D90(Session())==1){w=800;h=360;} if(gSplit_123457C[0x3064]){w=1280;h=720;}const float q=(float(w)/float(h))*0.5625f; const float spread=(1.0f/q+1.0f-q)*320.0f; x=640.0f-measuredW*0.5f;y=30.0f-measuredH*0.5f + spread; if(!gSplit_123457C[0x3064]&&QueryGameStatus_C42D90(Session())==7)y-=spread;}
 if(QueryGameStatus_C42D90(Session())==1){x=float(FullCoordTransform_76A460(gSplit_123457C,static_cast<int>(x))); y=float(PanelTransform_76C2A0(gSplit_123457C,static_cast<int>(y*0.5f))); scale=*reinterpret_cast<float*>(gSplit_123457C+0x3070); measuredW*=scale; y-=measuredW*0.5f; int mode=*reinterpret_cast<int*>(self+0x2F644); if(sub!=0&&(mode==9||mode==10))scale=24.0f;}
 return {x,y,scale,(mapped<<4)|sub};
}
void DrawLocalPlayerOverlay_7DEA10(void*selfRaw,void*recordRaw,int player){
 if(*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x1043C)&0x8000u)return; auto*rec=static_cast<OverlayRecord120*>(recordRaw);
 if(rec->flag10){const int idx=*reinterpret_cast<int*>(gProfile_11B2158+0x20);if(*reinterpret_cast<std::uint16_t*>(gProfile_11B2158+idx*0x8C74+0x6C)==1)return;}
 int mapped=ResolveSplitPlayer_76A1E0(gSplit_123457C,player); if(static_cast<unsigned>(mapped)>=8u||mapped==-1)return; int sub=ResolveSplitSubIndex_76A250(gSplit_123457C,mapped);
 const int resource=(int(rec->hi)<<16)|rec->lo; Metric120 m{}; const float nominal=(rec->kind==5)?360.0f:((*reinterpret_cast<int*>(static_cast<std::uint8_t*>(selfRaw)+0x2F644)==0||*reinterpret_cast<int*>(static_cast<std::uint8_t*>(selfRaw)+0x2F644)==9||*reinterpret_cast<int*>(static_cast<std::uint8_t*>(selfRaw)+0x2F644)==10)?24.0f:30.0f);
 MeasureOverlay_7DC6B0(gUiRenderer_11B26F0,rec->kind,resource,&m,0,int(nominal),int(nominal),-1);
 auto g=ComputeLocalOverlayGeometry(selfRaw,rec,player,m.w,m.h); DrawOverlayNative_7DDFB0(gUiRenderer_11B26F0,rec->kind,resource,g.x,g.y,0,g.packedPlayer,g.scale,g.scale,0xE0,-1,0,0);
 if(rec->kind!=5)return;
 const int marker=(rec->lo>=rec->count-1)?0x2AC:0x2AB; const int fallback=(rec->lo>0)?0x2AF:-1;
 Metric120 mm{}; float mx=640.0f,my=0.0f; if(QueryGameStatus_C42D90(Session())==1){mx=float(FullCoordTransform_76A460(gSplit_123457C,640));my=float(PanelTransform_76C2A0(gSplit_123457C,300))+nominal;}
 MeasureOverlay_7DC6B0(gUiRenderer_11B26F0,0,marker,&mm,0,int(nominal),int(nominal),-1); DrawOverlayNative_7DDFB0(gUiRenderer_11B26F0,0,marker,mx,my-mm.w*0.5f,0,(mapped<<4)|sub,1.0f,1.0f,0xE0,-1,0,0);
 int r0[4]{180,30,550,609}, r1[4]{450,609,180,30}, r2[4]{650,609,180,30}, rz[4]{}; int*out[4]{};int*rect=(marker==0x2AB?(rec->lo==0?r0:r1):(marker==0x2AC?(rec->lo==0?r0:r1):(fallback==0x2AF?r2:rz))); (void)out;
 int built=RectBuild_422A80(rect,rect+2); UiHitRect_7B4570(gUiInput_11B20C4,0xCE,marker==0x2AF?1:0,built);
 if(void*a=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,player)){if(marker==0x2AB||marker==0x2AC)*reinterpret_cast<std::uint32_t*>(static_cast<std::uint8_t*>(a)+0x2D84)&=~0x10u; if(fallback==0x2AF)ActorAction_B5C3A0(a,0x10,0.0f);}
}
}
