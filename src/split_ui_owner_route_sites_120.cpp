#include <cstdint>

namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
extern void* gUiInput_11B20C4;
extern bool __thiscall UiRoute5620_7B5620(void*,int,int,int);
extern bool __thiscall UiRoute5750_7B5750(void*,int,int,int,int);
extern int  __thiscall UiSelectionStep_7B5900(void*,int,int,int,int,int);
extern int  __thiscall UiSelectionStepBy_7B59B0(void*,int,int,int,int,int,int);
extern int  __thiscall UiGetCursor_7B52A0(void*,int);
extern int  __thiscall UiGetSelection_7B5360(void*,int);

static inline bool Owner(int objectOwner){
    return objectOwner==*reinterpret_cast<const int*>(gInput_1249C40+0x614);
}

// Exact direct owner-routed queries following the +0x614 comparisons.
bool OwnerRoute_A29DAC(int owner){return Owner(owner)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,7,0,0);}
bool OwnerRoute_A29E3D(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,8,0,0);}
bool OwnerRoute_A2A173(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,-1,0,0);}
bool OwnerRoute_A2A391(int owner){return Owner(owner)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,1,0,0);}
bool OwnerRoute_A2A6DC(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,4,0,0);}
bool OwnerRoute_A2AA81(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,0);}
bool OwnerRoute_A2AF99(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,2,0,0);}
bool OwnerRoute_A2B42F(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,4,0,0);}
bool OwnerRoute_A2B8D2(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,7,0,0);}
bool OwnerRoute_A2BBE0(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,2);}
bool OwnerRoute_A2BF9A(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,8,0,0);}
bool OwnerRoute_A2BFD0(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,7,0,0);}
bool OwnerRoute_A2C3EB(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,-1,0,0);}

// Owner-driven +/-1 selection families. Physical-mask alternatives are tested
// by the parent before these helpers; these functions model only the +614 path.
int ApplyOwnerPlusOne_A2A754(int owner,int value,int maxExclusive){
    if(Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1)){
        ++value; if(value>=maxExclusive)value=maxExclusive-1;
    }
    return value;
}
int ApplyOwnerMinusOne_A2A78E(int owner,int value){
    if(Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1)){
        --value; if(value<0)value=0;
    }
    return value;
}
int ApplyOwnerPlusOne_A2B4A8(int owner,int value,int maxExclusive){return ApplyOwnerPlusOne_A2A754(owner,value,maxExclusive);}
int ApplyOwnerMinusOne_A2B4E2(int owner,int value){return ApplyOwnerMinusOne_A2A78E(owner,value);}

// Same pattern with step 5 at A2AFE1/A2B018.
int ApplyOwnerPlusFive_A2AFE1(int owner,int value,int maximum){
    if(Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1)){
        value+=5; if(value>maximum)value=maximum;
    }
    return value;
}
int ApplyOwnerMinusFive_A2B018(int owner,int value){
    if(Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1)){
        value-=5; if(value<0)value=0;
    }
    return value;
}

// Variant with route subselector 4 used by A2AACA/A2AB00.
int ApplyOwnerPlusOneSub4_A2AACA(int owner,int value,int maximum){
    if(Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,4,1)){
        ++value; if(value>maximum)value=maximum;
    }
    return value;
}
int ApplyOwnerMinusOneSub4_A2AB00(int owner,int value){
    if(Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,4,1)){
        --value; if(value<0)value=0;
    }
    return value;
}

// A2A7BB/A2B50E: 7B5900(current, expectedRoute, count, min, index=0).
int OwnerSelectionStep_A2A7BB(int owner,int current,int expectedRoute,int count){
    if(!Owner(owner))return current;
    return UiSelectionStep_7B5900(gUiInput_11B20C4,current,expectedRoute,count,0,0);
}
int OwnerSelectionStep_A2B50E(int owner,int current,int expectedRoute,int count){
    return OwnerSelectionStep_A2A7BB(owner,current,expectedRoute,count);
}

// A2AB2F: same helper with count = maximum+1.
int OwnerSelectionStep_A2AB2F(int owner,int current,int expectedRoute,int maximum){
    if(!Owner(owner))return current;
    return UiSelectionStep_7B5900(gUiInput_11B20C4,current,expectedRoute,maximum+1,0,0);
}

// A2AB57: owner must see UI record 1 in state/cursor value 2 before entering
// the following split anchor/geometry branch.
bool OwnerRecord1Is2_A2AB57(int owner){
    return Owner(owner)&&UiGetCursor_7B52A0(gUiInput_11B20C4,1)==2;
}

// A2B7B8/A2B81F: owner query (0,6,0,0), followed by selection[1] == -1.
bool OwnerRoute6SelectionInvalid_A2B7B8(int owner){
    return Owner(owner)&&UiRoute5750_7B5750(gUiInput_11B20C4,0,6,0,0)&&
           UiGetSelection_7B5360(gUiInput_11B20C4,1)==-1;
}
bool OwnerRoute6SelectionInvalid_A2B81F(int owner){return OwnerRoute6SelectionInvalid_A2B7B8(owner);}

// A2B928: 7B59B0(current, expectedRoute=0, max=0x26, min=0, step=1, index=2).
int OwnerSelectionStepBy_A2B928(int owner,int current){
    if(!Owner(owner))return current;
    return UiSelectionStepBy_7B59B0(gUiInput_11B20C4,current,0,0x26,0,1,2);
}
// A2C03C is the same six-argument helper with max=4.
int OwnerSelectionStepBy_A2C03C(int owner,int current){
    if(!Owner(owner))return current;
    return UiSelectionStepBy_7B59B0(gUiInput_11B20C4,current,0,4,0,1,2);
}

// Boolean owner contributions in the compound A2B99x/A2C0xx state logic.
bool OwnerRouteFlag_A2B993(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1);}
bool OwnerRouteFlag_A2B9BA(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1);}
bool OwnerRouteFlag_A2C0A2(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,1,0,1);}
bool OwnerRouteFlag_A2C0D2(int owner){return Owner(owner)&&UiRoute5620_7B5620(gUiInput_11B20C4,0,0,1);}

} // namespace re5::split120
