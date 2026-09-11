#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4; extern std::uint8_t* gSplit_123457C; extern std::uint8_t* gDisplay_12345D4; extern std::uint8_t* gProfile_11B2158; extern void* gPlayerManager_11B27DC; extern void* gUiInput_11B20C4;
extern int ResolveSplitPlayer_76A1E0(void*,int); extern int ResolveSplitSubIndex_76A250(void*,int); extern int PanelTransform_76C2A0(void*,int);
extern void* FindPlayerActorByNo_7E6220(void*,int); extern int ResolvePlayerRuntime_7E6370(void*,void*);
extern void MeasureOverlay_7DC6B0(void*,int,int,void*,int,int,int,int); extern void* gUiRenderer_11B26F0;
// 0x7DDFB0 is a generic renderer. Its non-split internal ABI remains abstracted;
// the local-J2 call sites below preserve the native prompt/resource ids,
// coordinates, per-player token and native nominal dimensions.
extern void DrawOverlayNative_7DDFB0(void*,int,int,float,float,int,int,float,float,int,int,int,int); extern int RectBuild_422A80(void*,void*); extern bool UiHitRect_7B4570(void*,int,int,int); extern void ActorAction_B5C3A0(void*,int,float);
static inline std::uint8_t* Session(){return *reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);}
struct OverlayRecord120 {std::uint8_t zero; std::uint8_t kind; std::uint16_t hi; std::uint16_t lo; std::uint8_t _06[2]; std::int32_t count; std::uint8_t _0C[4]; std::uint8_t flag10;};
static_assert(__builtin_offsetof(OverlayRecord120,kind)==1,"kind offset");
static_assert(__builtin_offsetof(OverlayRecord120,hi)==2,"hi offset");
static_assert(__builtin_offsetof(OverlayRecord120,lo)==4,"lo offset");
static_assert(__builtin_offsetof(OverlayRecord120,count)==8,"count offset");
static_assert(__builtin_offsetof(OverlayRecord120,flag10)==0x10,"flag offset");
struct Metric120 {float a,b,w,h;};
struct OverlayGeometry120 {float x,y,nominal;int packedPlayer;};

static float NativeOverlayNominal(void* selfRaw,const OverlayRecord120*rec){
 if(rec->kind==5)return 35.0f; // 0x10BCD00
 const int mode=*reinterpret_cast<const int*>(static_cast<const std::uint8_t*>(selfRaw)+0x2F644);
 return (mode==0||mode==9||mode==10)?40.0f:36.0f; // 0x10BCB4C / 0xF52E14
}

// Split/local-player geometry from 0x7DEAF1..0x7DED37.
static OverlayGeometry120 ComputeLocalOverlayGeometry(void*selfRaw,OverlayRecord120*rec,int player,float measuredW,float measuredH,float nominal){
 auto*self=static_cast<std::uint8_t*>(selfRaw); const int mapped=ResolveSplitPlayer_76A1E0(gSplit_123457C,player); const int sub=ResolveSplitSubIndex_76A250(gSplit_123457C,mapped);
 const int status=QueryGameStatus_C42D90(Session()); float x=0.0f,y=0.0f;
 if(rec->kind==5){
   x=220.0f; y=(720.0f-measuredH)*0.5f;
   if(status==1)y=float(SplitVerticalTransform_76A4A0(gSplit_123457C,static_cast<int>(y)));
 } else {
   // Native calls both helpers without a pre-null actor guard.
   void*a=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,player); ResolvePlayerRuntime_7E6370(gPlayerManager_11B27DC,a);
   int w=*reinterpret_cast<int*>(gDisplay_12345D4+0x50),h=*reinterpret_cast<int*>(gDisplay_12345D4+0x54);
   if(status==1){w=800;h=360;} if(gSplit_123457C[0x3064]){w=1280;h=720;}
   const float q=(float(w)/float(h))*0.5625f;
   const float spread=(1.0f/q+1.0f-q)*85.0f;
   x=640.0f-measuredW*0.5f;
   y=540.0f-nominal*0.5f;
   if(!gSplit_123457C[0x3064]&&status==7)y-=spread; else y+=spread;
 }
 if(status==1){
   x=float(FullCoordTransform_76A460(gSplit_123457C,static_cast<int>(x)));
   y=float(PanelTransform_76C2A0(gSplit_123457C,static_cast<int>(y*0.5f)));
   nominal*=*reinterpret_cast<float*>(gSplit_123457C+0x3070);
   y-=nominal*0.5f;
   const int mode=*reinterpret_cast<int*>(self+0x2F644); if(sub!=0&&(mode==9||mode==10))nominal=24.0f;
 }
 return {x,y,nominal,(mapped<<4)|sub};
}

void DrawLocalPlayerOverlay_7DEA10(void*selfRaw,void*recordRaw,int player){
 if(*reinterpret_cast<std::uint32_t*>(gRoot_12340A4+0x1043C)&0x8000u)return; auto*rec=static_cast<OverlayRecord120*>(recordRaw);
 if(rec->flag10){const int idx=*reinterpret_cast<int*>(gProfile_11B2158+0x20);if(*reinterpret_cast<std::uint16_t*>(gProfile_11B2158+idx*0x8C74+0x6C)==1)return;}
 int mapped=ResolveSplitPlayer_76A1E0(gSplit_123457C,player); if(static_cast<unsigned>(mapped)>=8u||mapped==-1)return; int sub=ResolveSplitSubIndex_76A250(gSplit_123457C,mapped);
 const int resource=(int(rec->hi)<<16)|rec->lo; Metric120 m{}; const float nominal=NativeOverlayNominal(selfRaw,rec);
 MeasureOverlay_7DC6B0(gUiRenderer_11B26F0,rec->kind,resource,&m,0,int(nominal),int(nominal),-1);
 auto g=ComputeLocalOverlayGeometry(selfRaw,rec,player,m.w,m.h,nominal);
 DrawOverlayNative_7DDFB0(gUiRenderer_11B26F0,rec->kind,resource,g.x,g.y,0,g.packedPlayer,g.nominal,g.nominal,0xE0,-1,0,0);
 if(rec->kind!=5)return;

 // 0x7DED9E..: primary marker is 0x2AB until the last entry, then 0x2AC.
 const int marker=(rec->lo>=rec->count-1)?0x2AC:0x2AB;
 const int fallback=(rec->lo>0)?0x2AF:-1;
 Metric120 mm{}; float mx=640.0f,my=600.0f;
 if(QueryGameStatus_C42D90(Session())==1){mx=float(FullCoordTransform_76A460(gSplit_123457C,640));my=float(PanelTransform_76C2A0(gSplit_123457C,300))+g.nominal;}
 MeasureOverlay_7DC6B0(gUiRenderer_11B26F0,0,marker,&mm,0,int(g.nominal),int(g.nominal),-1);
 // Native marker call positions the marker relative to its measured extent;
 // keep the generic renderer details abstracted while preserving its native
 // base coordinates and nominal size.
 DrawOverlayNative_7DDFB0(gUiRenderer_11B26F0,0,marker,mx,my,0,(mapped<<4)|sub,g.nominal,g.nominal,0xE0,-1,0,0);

 int primaryA[2]{180,30}, primaryB[2]{550,609};
 int alternateA[2]{450,609}, alternateB[2]{180,30};
 int*pa=(rec->lo==0)?primaryA:alternateA; int*pb=(rec->lo==0)?primaryB:alternateB;
 int built=RectBuild_422A80(pa,pb); UiHitRect_7B4570(gUiInput_11B20C4,0xCE,0,built);
 // Native dereferences the returned actor directly on the 0x2AB/0x2AC path.
 void*a=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,player); *reinterpret_cast<std::uint32_t*>(static_cast<std::uint8_t*>(a)+0x2D84)&=~0x10u;

 // 0x7DEF7A..0x7DF04A: if lo>0 a second 0x2AF marker is drawn and gets
 // its own hit rectangle and actor action.
 if(fallback==0x2AF){
   Metric120 fm{}; MeasureOverlay_7DC6B0(gUiRenderer_11B26F0,0,fallback,&fm,0,int(g.nominal),int(g.nominal),-1);
   DrawOverlayNative_7DDFB0(gUiRenderer_11B26F0,0,fallback,mx,my,0,(mapped<<4)|sub,g.nominal,g.nominal,0xE0,-1,0,0);
   int fa[2]{650,609},fb[2]{180,30}; int fBuilt=RectBuild_422A80(fa,fb); UiHitRect_7B4570(gUiInput_11B20C4,0xCE,1,fBuilt);
   void*faActor=FindPlayerActorByNo_7E6220(gPlayerManager_11B27DC,player); ActorAction_B5C3A0(faActor,0x10,0.0f);
 }
}
}
