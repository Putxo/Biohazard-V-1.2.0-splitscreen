#include <cstdint>
namespace re5::split120 {
struct Split{unsigned char p[0x3070];float scale;float param;float scale2;float offset;float vertical;unsigned char full;unsigned char transient;};
extern Split* gSplit_123457C; extern int Status_C42D90(); extern int Y_76A4A0(Split*,int);
int MenuRowYStatus1_120(int fallback){return Status_C42D90()==1?Y_76A4A0(gSplit_123457C,int(gSplit_123457C->scale*144.f)):fallback;}
int MenuCenterScaled2_120(float logicalOffset,int fallback){return Status_C42D90()==1?0x280-int(gSplit_123457C->scale2*logicalOffset):fallback;}
int MenuAffineCenterScaled2_120(int logicalX){return 0x280+int(float(logicalX-0x280)*gSplit_123457C->scale2);}
float MenuScaleStatus1_120(float normal,float base){return Status_C42D90()==1?gSplit_123457C->scale*base:normal;}
float MenuUniformScaleStatus1_120(float normal){return Status_C42D90()==1?gSplit_123457C->scale:normal;}
struct MenuSplitSite120{std::uint32_t va;std::uint16_t field;std::uint8_t kind;};
enum:std::uint8_t{ROW_Y=1,CENTER_SCALE2=2,SCALE_VALUE=3,AFFINE_CENTER=4};
static const MenuSplitSite120 kCoreSites[]={
 {0xA23E8C,0x3070,ROW_Y},{0xA2435F,0x3070,ROW_Y},
 {0xA24458,0x3078,CENTER_SCALE2},{0xA244B9,0x3078,CENTER_SCALE2},{0xA2453E,0x3078,CENTER_SCALE2},{0xA245AA,0x3078,CENTER_SCALE2},
 {0xA24ECF,0x3070,ROW_Y},{0xA24FB2,0x3078,CENTER_SCALE2},{0xA2500C,0x3070,SCALE_VALUE},{0xA25056,0x3078,CENTER_SCALE2},{0xA250C6,0x3070,SCALE_VALUE},{0xA25114,0x3078,CENTER_SCALE2},{0xA251AA,0x3078,CENTER_SCALE2},{0xA2521A,0x3078,CENTER_SCALE2},
 {0xA2554F,0x3070,ROW_Y},{0xA25625,0x3078,CENTER_SCALE2},{0xA256CE,0x3078,CENTER_SCALE2},{0xA2573B,0x3078,CENTER_SCALE2},{0xA2578D,0x3078,CENTER_SCALE2},{0xA257D5,0x3070,SCALE_VALUE},
 {0xA2729F,0x3070,ROW_Y},{0xA27367,0x3078,CENTER_SCALE2},{0xA275D5,0x3078,CENTER_SCALE2},{0xA276E0,0x3078,CENTER_SCALE2},{0xA27744,0x3078,CENTER_SCALE2},{0xA2789E,0x3078,AFFINE_CENTER},{0xA27906,0x3070,ROW_Y},{0xA2794E,0x3078,CENTER_SCALE2},{0xA279A4,0x3078,CENTER_SCALE2}
};
const MenuSplitSite120* MenuSplitCoreSites(std::uint32_t& n){n=sizeof(kCoreSites)/sizeof(kCoreSites[0]);return kCoreSites;}
}
