#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint8_t* gInputRoutes_1249C78;
extern void* gUiInput_11B20C4;
extern void* gUiRouteNotify_11B1FA4;
extern bool __thiscall UiInputSystemReady_79A8B0(void* input);
extern std::uint8_t __thiscall InputOwnerModeGate_799AF0(void* input,int player);
extern void __thiscall NotifyUiRouteState_7ADC00(void* notify,int active);

struct UiOwnerRouteManager120 {
    std::uint8_t bytes[0x330];
};

static inline std::uint8_t* B(void* p){return static_cast<std::uint8_t*>(p);}
static inline const std::uint8_t* B(const void* p){return static_cast<const std::uint8_t*>(p);}
static inline std::int32_t I32(const void* p,std::uint32_t off){return *reinterpret_cast<const std::int32_t*>(B(p)+off);}
static inline std::int32_t& I32(void* p,std::uint32_t off){return *reinterpret_cast<std::int32_t*>(B(p)+off);}
static inline std::uint8_t& U8(void* p,std::uint32_t off){return *(B(p)+off);}
static inline std::uint8_t* Record(void* self,int index){return B(self)+index*0x70;}
static inline const std::uint8_t* Record(const void* self,int index){return B(self)+index*0x70;}

// 0x7B4210 / 0x7B42A0. These are the first two owner-gated mask queries in
// the direct-799B50 family. mode 0/1/2 selects table +40/+38/+44.
static bool QueryOwnerMask(void* self,int mode,std::uint32_t field)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return false;
    if(static_cast<unsigned>(mode)>2u)return false;
    const int route=I32(self,0x31C);
    const std::uint32_t tableOff=(mode==0)?0x40u:(mode==1)?0x38u:0x44u;
    const auto mask=*reinterpret_cast<const std::uint32_t*>(gInputRoutes_1249C78+route*0x70+tableOff);
    return (static_cast<std::uint32_t>(I32(self,field))&mask)!=0;
}
bool __thiscall UiOwnerMaskA_7B4210(void* self,int mode){return QueryOwnerMask(self,mode,0x2FC);}
bool __thiscall UiOwnerMaskB_7B42A0(void* self,int mode){return QueryOwnerMask(self,mode,0x300);}

// 0x7B44D0..0x7B452A -- owner-gated pointer replacement. Native releases an
// existing element through vtable[0](1), clears it, then stores the new value.
void __thiscall ReplaceOwnedUiObject_7B44D0(void* self,void* value,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return;
    const int count=I32(self,0x30C);
    if(static_cast<unsigned>(index)>=static_cast<unsigned>(count))return;
    auto** list=*reinterpret_cast<void***>(B(self)+0x318);
    if(void* old=list[index]){
        auto** vt=*reinterpret_cast<void***>(old);
        using ReleaseFn=void(__thiscall*)(void*,int);
        reinterpret_cast<ReleaseFn>(vt[0])(old,1);
        list[index]=nullptr;
    }
    list[index]=value;
}

// 0x7B4660 / 0x7B4710 / 0x7B47C0. Root bit 0x1000, 799B50 and 79A8B0 all
// gate the query. selector 0/1/2 chooses self +2FC/+300/+304 respectively.
static bool QueryOwnedInputBit(void* self,int selector,std::uint32_t tableOff)
{
    const auto* root=*reinterpret_cast<std::uint8_t* const*>(0x012340A4);
    if((*reinterpret_cast<const std::uint32_t*>(root+0x10434)&0x1000u)!=0)return false;
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return false;
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return false;
    if(static_cast<unsigned>(selector)>2u)return false;
    const std::uint32_t selfOff=(selector==0)?0x2FCu:(selector==1)?0x300u:0x304u;
    const int route=I32(self,0x31C);
    const auto mask=*reinterpret_cast<const std::uint32_t*>(gInputRoutes_1249C78+route*0x70+tableOff);
    return (static_cast<std::uint32_t>(I32(self,selfOff))&mask)!=0;
}
bool __thiscall UiOwnedInput38_7B4660(void* self,int selector){return QueryOwnedInputBit(self,selector,0x38);}
bool __thiscall UiFallbackInput_7B4710(void* self,int selector){return QueryOwnedInputBit(self,selector,0x40);}
bool __thiscall UiOwnedInput44_7B47C0(void* self,int selector){return QueryOwnedInputBit(self,selector,0x44);}

// Shared exact preconditions of the native 0x7B5180..0x7B55C0 route readers.
// 799B50 is intentionally last, matching the executable call order.
static const std::uint8_t* OwnedReadableRecord(void* self,int index,bool requireRoot1054cClear)
{
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return nullptr;
    const auto* r=Record(self,index);
    if(r[0x20]==1)return nullptr;
    if(InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1)return nullptr;
    if(I32(r,0x60)!=I32(r,0x48))return nullptr;
    if(requireRoot1054cClear){
        const auto* root=*reinterpret_cast<std::uint8_t* const*>(0x012340A4);
        if(root[0x1054C]!=0)return nullptr;
    }
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return nullptr;
    return r;
}
static int ReadOwnedRouteField(void* self,int index,std::uint32_t field,bool rootGate=false){
    const auto* r=OwnedReadableRecord(self,index,rootGate);
    return r?I32(r,field):-1;
}

int __thiscall UiRouteValue24_7B5180(void* self,int index){return ReadOwnedRouteField(self,index,0x24);}
int __thiscall UiRouteValue28_7B51E0(void* self,int index){return ReadOwnedRouteField(self,index,0x28);}
int __thiscall UiRouteValue2C_7B5240(void* self,int index){return ReadOwnedRouteField(self,index,0x2C);}
int __thiscall UiGetCursor_7B52A0(void* self,int index){return ReadOwnedRouteField(self,index,0x30);}
int __thiscall UiRouteValue34_7B5300(void* self,int index){return ReadOwnedRouteField(self,index,0x34);}
int __thiscall UiGetSelection_7B5360(void* self,int index){return ReadOwnedRouteField(self,index,0x3C);}
int __thiscall UiRouteValue40_7B53C0(void* self,int index){return ReadOwnedRouteField(self,index,0x40);}
int __thiscall UiRouteValue44_7B5420(void* self,int index){return ReadOwnedRouteField(self,index,0x44);}
int __thiscall UiRouteValue58_7B5480(void* self,int index){return ReadOwnedRouteField(self,index,0x58,true);}
int __thiscall UiRouteValue5C_7B54F0(void* self,int index){return ReadOwnedRouteField(self,index,0x5C,true);}
int __thiscall UiRouteValue7C_7B5560(void* self,int index){return ReadOwnedRouteField(self,index,0x7C);}
int __thiscall UiRouteValue84_7B55C0(void* self,int index){return ReadOwnedRouteField(self,index,0x84);}

static int DispatchOwnedGetter(void* self,int kind,int index)
{
    switch(kind){
    case 0:return UiGetSelection_7B5360(self,index); // native 7B56DB/7B57F0
    case 1:return UiGetCursor_7B52A0(self,index);    // native 7B56E5/7B57FA
    case 2:return UiRouteValue34_7B5300(self,index);
    case 3:return UiRouteValue40_7B53C0(self,index);
    case 4:return UiRouteValue44_7B5420(self,index);
    case 7:return UiRouteValue84_7B55C0(self,index);
    default:return -1; // jump-table entries 5 and 6 go directly to failure
    }
}

// 0x7B5620..0x7B572A -- exact owner-routed equality query, RET 0x0C.
bool __thiscall UiRoute5620_7B5620(void* self,int expected,int kind,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return false;

    if(expected==-1){
        const int objectIndex=I32(Record(self,index),0x48);
        const int count=I32(self,0x30C);
        int objectValue=0;
        if(static_cast<unsigned>(objectIndex)<static_cast<unsigned>(count)){
            auto** objects=*reinterpret_cast<void***>(B(self)+0x318);
            if(void* object=objects[objectIndex])objectValue=I32(object,0x0C);
        }
        expected=objectValue-2;
    }

    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return false;
    const auto* r=Record(self,index);
    if(r[0x20]==1)return false;
    if(InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1)return false;
    if(I32(r,0x60)!=I32(r,0x48))return false;

    // Native has signed upper-bound checks only; valid callers keep these >=0.
    if(index<4 && expected<0x40)U8(self,0x1E0+index*0x40+expected)=1;
    if(static_cast<unsigned>(kind)>7u)return false;
    const int value=DispatchOwnedGetter(self,kind,index);
    return value!=-1 && value==expected;
}

// 0x7B5750..0x7B5846 -- exact inclusive-range route query, RET 0x10.
bool __thiscall UiRoute5750_7B5750(void* self,int minimum,int maximum,int kind,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return false;
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return false;
    const auto* r=Record(self,index);
    if(r[0x20]==1)return false;
    if(InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1)return false;
    if(I32(r,0x60)!=I32(r,0x48))return false;

    for(int value=minimum;value<=maximum;++value)
        if(index<4 && value<0x40)U8(self,0x1E0+index*0x40+value)=1;

    if(static_cast<unsigned>(kind)>7u)return false;
    const int value=DispatchOwnedGetter(self,kind,index);
    return value!=-1 && value>=minimum && value<=maximum;
}

// 0x7B5870..0x7B58F1. Non-owner or any failed route condition returns the
// caller fallback. A valid +2C value in [lower,upper) is normalized by lower.
int __thiscall UiSelection_7B5870(void* self,int fallback,int upper,int lower,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return fallback;
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return fallback;
    const auto* r=Record(self,index);
    if(r[0x20]==1 || InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1 || I32(r,0x60)!=I32(r,0x48))return fallback;
    const int value=UiRouteValue2C_7B5240(self,index);
    if(value==-1 || value>=upper || value<lower)return fallback;
    return value-lower;
}

// 0x7B5900..0x7B59A9 -- one-step signed selection update.
int __thiscall UiSelectionStep_7B5900(void* self,int current,int expectedRoute,int count,int minimum,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return current;
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return current;
    const auto* r=Record(self,index);
    if(r[0x20]==1 || InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1 || I32(r,0x60)!=I32(r,0x48))return current;
    if(UiRouteValue58_7B5480(self,index)!=expectedRoute)return current;
    const int direction=UiRouteValue5C_7B54F0(self,index);
    if(direction==0)return current;
    int value=current;
    if(direction<0)--value;
    if(direction>0)++value;
    const int maximum=count-1;
    if(value>maximum)value=maximum;
    if(value<minimum)value=minimum;
    return value;
}

// 0x7B59B0..0x7B5A58 -- signed selection update by caller-provided step.
int __thiscall UiSelectionStepBy_7B59B0(void* self,int current,int expectedRoute,int maximum,int minimum,int step,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return current;
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return current;
    const auto* r=Record(self,index);
    if(r[0x20]==1 || InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1 || I32(r,0x60)!=I32(r,0x48))return current;
    if(UiRouteValue58_7B5480(self,index)!=expectedRoute)return current;
    const int direction=UiRouteValue5C_7B54F0(self,index);
    if(direction==0)return current;
    int value=current;
    if(direction<0)value+=step;
    if(direction>0)value-=step;
    if(value>maximum)value=maximum;
    if(value<minimum)value=minimum;
    return value;
}

// 0x7B5A60..0x7B5AF6 -- increment-and-wrap selector using +3C route value.
int __thiscall UiSelectionNext_7B5A60(void* self,int current,int expectedRoute,int count,int minimum,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return current;
    if(!UiInputSystemReady_79A8B0(gInput_1249C40))return current;
    const auto* r=Record(self,index);
    if(r[0x20]==1 || InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1 || I32(r,0x60)!=I32(r,0x48))return current;
    if(UiGetSelection_7B5360(self,index)!=expectedRoute)return current;
    int value=current+1;
    if(value>count-1)return minimum;
    if(value<minimum)value=minimum;
    return value;
}

// 0x7B5BB0..0x7B5C4F -- aggregate route-active notification. 0x7B5BF3 is a
// direct 799B50 call inside the four-route scan rather than an entry guard.
void __thiscall UpdateOwnedUiRouteActivity_7B5BB0(void* self)
{
    for(int index=0;index<4;++index){
        if(!UiInputSystemReady_79A8B0(gInput_1249C40))continue;
        const auto* r=Record(self,index);
        if(r[0x20]==1)continue;
        if(InputOwnerModeGate_799AF0(gInput_1249C40,-1)==1)continue;
        if(I32(r,0x60)!=I32(r,0x48))continue;
        if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))continue;
        const int value=I32(r,0x28);
        if(value==-1)continue;
        if(index<4 && value<0x40 && B(self)[0x1E0+index*0x40+value]!=0){
            NotifyUiRouteState_7ADC00(gUiRouteNotify_11B1FA4,1);
            return;
        }
    }
    NotifyUiRouteState_7ADC00(gUiRouteNotify_11B1FA4,0);
}

// 0x7B6330..0x7B637B -- owner-gated "route missing" predicate. The first
// stack argument is unused by the native body; the second is the route index.
bool __thiscall UiOwnedRouteMissing_7B6330(void* self,int /*unused*/,int index)
{
    if(!PreferredOwnerAllowsCurrentKeyboard_799B50(gInput_1249C40))return false;
    const int route=I32(self,0x31C);
    const auto mask=*reinterpret_cast<const std::uint32_t*>(gInputRoutes_1249C78+route*0x70+0x40);
    if((static_cast<std::uint32_t>(I32(self,0x2FC))&mask)==0)return false;
    return UiGetSelection_7B5360(self,index)==-1;
}

} // namespace re5::split120
