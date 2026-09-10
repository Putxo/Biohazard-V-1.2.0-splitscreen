#include <cstdint>
namespace re5::split120 {
struct TrialSplit120 { std::uint8_t _000[0x188]; std::int32_t state188; };
extern void TrialCommonInit_A17010(void*,const void*); extern void TrialUiInit_9E3780(void*); extern void* Allocate120(std::uint32_t); extern void TrialBaseCtor_A16EC0(void*);
// 0x00A59570 -- VERIFIED mapped trial_split/uIdTrial05 method. Writes +0x188, not +0x184.
void InitTrialSplit_A59570(TrialSplit120* self){TrialCommonInit_A17010(self,reinterpret_cast<const void*>(0x00FAB3DC));TrialUiInit_9E3780(self);self->state188=0;}
// 0x00A59590 -- VERIFIED factory: size 0x188; final native vtable 0x00FAB368.
void* CreateTrialSplit_A59590(){void* p=Allocate120(0x188);if(!p)return nullptr;TrialBaseCtor_A16EC0(p);*reinterpret_cast<std::uint32_t*>(p)=0x00FAB368;return p;}
// 0x00A59010 -- VERIFIED descriptor getter.
const void* TrialSplitDescriptor_A59010(){return reinterpret_cast<const void*>(0x011F5EF0);}
const char* TrialSplitResourceName(){return "id\\jpn\\show_E3\\trial_split";}
// The +4 growth is shared by neighboring uIdTrial types; it is not a unique J2 activation member. Story J2 activation is 723460 -> 720CE0 -> state13 -> 716720(active=1).
bool TrialSplitIsDirectJ2Activator(){return false;}
} // namespace re5::split120
