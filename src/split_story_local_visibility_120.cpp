#include <cstdint>

namespace re5::split120 {

extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gSplitState_123457C;
extern void* gObjectList_12340B0;
extern std::uint32_t gTrackedObjectId_124A614;
extern void* FindNamedResource_6D05F0(const void*,const void*);
extern void* ResolveNamedResource_45EA40(void*,const void*);
extern void FinalizeNamedResource_45E9F0(void*);
extern void SetTrackedObjectEnabled_477300(void*,bool);
extern void RefreshSplitState_76CC60(void*);

// 0x0071AC10..0x0071AD06 -- VERIFIED split/local-visibility controller.
// The routine folds root+0x10438 bit17, bit 0x10000000 and the caller's
// suppression byte into a single disabled flag. A change updates the cached UI
// object +0x800 visibility bit, mirrors visibility to matching tracked objects,
// caches the state at self+0x448, then always refreshes the split render state.
void UpdateStoryLocalVisibility_71AC10(void* selfRaw,int callerSuppress) {
    auto* self=static_cast<std::uint8_t*>(selfRaw);
    const std::uint32_t flags=*reinterpret_cast<const std::uint32_t*>(gRoot_12340A4+0x10438);
    const bool disabled=((flags&0x00020000u)!=0)||((flags&0x10000000u)!=0)||(callerSuppress!=0);
    if((self[0x448]!=0)!=disabled){
        void* ui=*reinterpret_cast<void**>(self+0x28);
        if(ui){
            static const std::uintptr_t kNameA=0x010AEF48u;
            static const std::uintptr_t kNameB=0x011AF714u;
            static const std::uintptr_t kNameC=0x010AEF68u;
            void* r=FindNamedResource_6D05F0(reinterpret_cast<const void*>(kNameA),reinterpret_cast<const void*>(kNameB));
            if(r){void* obj=ResolveNamedResource_45EA40(r,reinterpret_cast<const void*>(kNameC));if(obj){FinalizeNamedResource_45E9F0(obj);auto& bits=*reinterpret_cast<std::uint32_t*>(static_cast<std::uint8_t*>(obj)+4);if(disabled)bits&=~0x800u;else bits|=0x800u;}}
            self[0x449]=disabled?0:1;
        }
        auto* head=static_cast<std::uint8_t*>(gObjectList_12340B0);
        if(head){
            for(auto* node=*reinterpret_cast<std::uint8_t**>(head+0xAC);node;node=*reinterpret_cast<std::uint8_t**>(node+0x10)){
                if(*reinterpret_cast<const std::uint32_t*>(node+0x0C)==gTrackedObjectId_124A614)
                    SetTrackedObjectEnabled_477300(node,!disabled);
            }
        }
        self[0x448]=disabled?1:0;
    }
    RefreshSplitState_76CC60(gSplitState_123457C);
}

} // namespace re5::split120
