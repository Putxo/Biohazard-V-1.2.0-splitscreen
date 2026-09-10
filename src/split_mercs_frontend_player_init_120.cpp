#include <cstdint>
namespace re5::split120 {
struct MercsFrontendPlayerView120 {
    std::uint8_t _000[0x48]; std::int32_t cursor48;
    std::uint8_t _04c[0x188-0x4c]; std::int32_t slot188;
    std::uint8_t _18c[0x588-0x18c]; std::int32_t route588;
    std::uint8_t _58c[0x5A8-0x58c]; std::int32_t stage5A8;
    std::uint8_t _5ac[0x630-0x5ac]; std::int32_t option630;
    std::uint8_t _634[0x694-0x634]; std::uint32_t enabledMask694;
    std::uint8_t _698[0x6A0-0x698]; std::int32_t optionCount6A0;
    std::uint8_t _6A4[0x6AC-0x6A4]; std::int32_t localUiActive6AC;
    std::int32_t secondDevice6B0;
};
extern std::uint8_t* gRoot_12340A4;
extern bool IsMercsRow0_C42E80(void*);
extern bool IsMercsRow1_C42EA0(void*);
extern void RefreshMercsPlayer_A432C0(void*,int);
extern void ApplyMercsPlayerOption_A43360(void*,int);
// 0x00A466C0..0x00A46A30 -- VERIFIED split/local-relevant reconstruction.
// The middle native block copies presentation records for two slots; this
// function preserves the local routing decisions surrounding that data copy.
void InitMercsFrontendLocalPlayerState_A466C0(MercsFrontendPlayerView120* self) {
    auto* session=*reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);
    self->stage5A8=*reinterpret_cast<std::int32_t*>(session+0x5D4);
    self->route588=IsMercsRow0_C42E80(session)?0:1;
    if(*reinterpret_cast<std::int32_t*>(session+0x58)==2 &&
       *reinterpret_cast<std::uint8_t*>(session+0x79E)!=0) self->route588=2;
    self->slot188=*reinterpret_cast<std::int32_t*>(session+0x5DC);
    self->cursor48=self->slot188;
    self->secondDevice6B0=*reinterpret_cast<std::int32_t*>(session+0x658);
    RefreshMercsPlayer_A432C0(self,0);
    RefreshMercsPlayer_A432C0(self,1);
    if(*reinterpret_cast<std::int32_t*>(session+0x58)==5 &&
       IsMercsRow1_C42EA0(session) && self->slot188==1 && self->option630<0) {
        int choice=0;
        for(int i=0;i<self->optionCount6A0;++i) {
            if((self->enabledMask694 & (1u<<i))!=0) { choice=i; break; }
        }
        self->option630=choice;
        *reinterpret_cast<std::int32_t*>(session+0x604)=choice;
        ApplyMercsPlayerOption_A43360(self,1);
    }
    if(self->secondDevice6B0<0) self->localUiActive6AC=0;
    else { self->localUiActive6AC=1; ApplyMercsPlayerOption_A43360(self,1); }
}
}
