#include <cstdint>
namespace re5::split120 {
struct ViewRect120 {int x0; int y0; int x1; int y1;};
static inline int truncf(float v){return static_cast<int>(v);}
static inline int halfTowardZero(int v){return v/2;}
// 0x0076D340..0x0076D418 -- VERIFIED geometry block from the central display
// state updater. `frontendWideLayout` is 11B2018+20; `sessionField4Is3` is
// session+4==3. This block is the only split-dependent rectangle computation
// in the surrounding branch.
ViewRect120 ComputeDisplayViewportRect_76D340(bool splitActive,
                                             bool frontendWideLayout,
                                             bool sessionField4Is3,
                                             int physicalW,int physicalH)
{
    if(splitActive){
        const int h169=truncf(float(physicalH)*1.7777777910232544f);
        if(!frontendWideLayout && !sessionField4Is3)
            return {0,0,physicalW,physicalH};
        const int nominal=truncf(float(h169)*1.3235293626785278f);
        const int left=halfTowardZero(physicalW-nominal);
        return {left,0,nominal+left,physicalH};
    }
    const float ratio=(float(physicalW)/float(physicalH))*0.5625f;
    const float centerBias=(ratio-1.0f)*0.5f;
    const int y0=truncf((ratio*0.12222222238779068f-centerBias)*float(physicalH));
    const int y1=truncf((ratio*0.8777777552604675f-centerBias)*float(physicalH));
    return {0,y0,physicalW,y1};
}
int SplitDisplayResourceRoute_76D435(bool splitActive){return splitActive?2:0;}
bool UseStatus7InactiveResourcePath_76D621(int status,bool splitActive){return status==7&&!splitActive;}
}
