#include <cstdint>
namespace re5::split120 {
struct ScreenProjectionInput120 { int x; int y; };
struct SplitProjectionState120 { std::uint8_t _0[0x3064]; std::uint8_t splitActive; };
static inline int truncf(float v){ return static_cast<int>(v); }

// 0x009F3200..0x009F32E2 -- isolated split-aware screen-space projection
// from the 0x9F2CF0 widget/update family. All surrounding widget state is
// unrelated to split/local-J2.
ScreenProjectionInput120 ProjectWidgetPointA_9F3200(
    const SplitProjectionState120* split,int physicalW,int physicalH,int x,int y,int projectionMode)
{
    const float w=float(physicalW), h=float(physicalH);
    if(split->splitActive){
        float ratio=(w/h)*0.5625f;
        float nx=(float(x)/w)*ratio - (ratio-1.0f)*0.5f;
        float ny=float(y)/h;
        return {truncf(nx*1280.0f),truncf(ny*720.0f)};
    }
    float aspect=1.7777777910232544f/(w/h);
    if(projectionMode!=0) aspect=1.0f;
    float nx=float(x)/w;
    float ny=(float(y)/h)*aspect - (aspect-1.0f)*0.5f;
    return {truncf(nx*1280.0f),truncf(ny*720.0f)};
}

// 0x009F3852..0x009F3934 -- twin projection in 0x9F35C0.
ScreenProjectionInput120 ProjectWidgetPointB_9F3852(
    const SplitProjectionState120* split,int physicalW,int physicalH,int x,int y)
{
    const float w=float(physicalW), h=float(physicalH);
    if(split->splitActive){
        float ratio=(w/h)*0.5625f;
        float nx=(float(x)/w)*ratio - (ratio-1.0f)*0.5f;
        float ny=float(y)/h;
        return {truncf(nx*1280.0f),truncf(ny*720.0f)};
    }
    float aspect=1.7777777910232544f/(w/h);
    float nx=float(x)/w;
    float ny=(float(y)/h)*aspect - (aspect-1.0f)*0.5f;
    return {truncf(nx*1280.0f),truncf(ny*720.0f)};
}
}
