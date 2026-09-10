#include <cstdint>
namespace re5::split120 {
struct MercsPlayerUi120 { std::uint8_t _000[0x188]; std::int32_t slot188; std::int32_t device18C; std::uint8_t isVersus190; std::uint8_t flag191; };
extern void BaseCtor_A16EC0(void*);
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern bool IsMercsLocalSplit_C42EC0(void*);
extern bool SessionPredicate_C42870(void*);
// 0x00A42110..0x00A421C5 -- VERIFIED constructor.
// In the native Mercs/Reunion local route, slot 0 is rebound to session+654
// and slot 1 to session+658. Otherwise the preferred device is retained.
MercsPlayerUi120* InitMercsPlayerDeviceOwner_A42110(MercsPlayerUi120* self) {
    BaseCtor_A16EC0(self);
    *reinterpret_cast<std::uint32_t*>(self)=0x00FA9D04;
    auto* session=*reinterpret_cast<std::uint8_t**>(gRoot_12340A4+0x1042C);
    self->slot188=*reinterpret_cast<std::int32_t*>(session+0x5DC);
    self->device18C=*reinterpret_cast<std::int32_t*>(gInput_1249C40+0x5D8);
    if(IsMercsLocalSplit_C42EC0(session)) {
        if(self->slot188==0) self->device18C=*reinterpret_cast<std::int32_t*>(session+0x654);
        else if(self->slot188==1) self->device18C=*reinterpret_cast<std::int32_t*>(session+0x658);
    }
    self->isVersus190=(*reinterpret_cast<std::int32_t*>(session+0x58)==1)?1:0;
    self->flag191=SessionPredicate_C42870(session)?1:0;
    return self;
}
}
