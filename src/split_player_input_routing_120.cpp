#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gRoot_12340A4;
extern std::uint8_t* gInput_1249C40;
extern std::uint32_t ReadInputMaskA_799820(std::uint8_t*,int);
extern std::uint32_t ReadInputMaskB_7998B0(std::uint8_t*,int);
extern std::uint32_t ReadInputMaskC_7997C0(std::uint8_t*,int);
extern std::uint32_t ReadOwnerMaskA_79B340(std::uint8_t*,int);
extern std::uint32_t ReadOwnerMaskB_79B530(std::uint8_t*,int);
extern std::uint32_t ReadOwnerMaskC_79B720(std::uint8_t*);
static inline bool InputSuppressed(){return *reinterpret_cast<std::uint8_t*>(gRoot_12340A4+0x10428)!=0;}
static inline int DeviceOrPreferred(int d){return d<0?*reinterpret_cast<int*>(gInput_1249C40+0x5D8):d;}
static inline bool IncludeOwnerChannel(int player){return player<0||player==*reinterpret_cast<int*>(gInput_1249C40+0x614);}
// 0x009E3420..0x009E3476 -- VERIFIED exact two-channel merge.
std::uint32_t ReadRoutedInputMaskA_9E3420(int device,int player){if(InputSuppressed())return 0;std::uint32_t m=ReadInputMaskA_799820(gInput_1249C40,DeviceOrPreferred(device));if(IncludeOwnerChannel(player))m|=ReadOwnerMaskA_79B340(gInput_1249C40,1);return m;}
// 0x009E3480..0x009E34D6 -- VERIFIED exact two-channel merge.
std::uint32_t ReadRoutedInputMaskB_9E3480(int device,int player){if(InputSuppressed())return 0;std::uint32_t m=ReadInputMaskB_7998B0(gInput_1249C40,DeviceOrPreferred(device));if(IncludeOwnerChannel(player))m|=ReadOwnerMaskB_79B530(gInput_1249C40,1);return m;}
// 0x009E34E0..0x009E3534 -- VERIFIED exact two-channel merge.
std::uint32_t ReadRoutedInputMaskC_9E34E0(int device,int player){if(InputSuppressed())return 0;std::uint32_t m=ReadInputMaskC_7997C0(gInput_1249C40,DeviceOrPreferred(device));if(IncludeOwnerChannel(player))m|=ReadOwnerMaskC_79B720(gInput_1249C40);return m;}
struct PlayerRouteOwner120{std::uint8_t _000[0x20];int profileIndex;};
// 0x007B82F0..0x007B8369 -- VERIFIED routing owner setter. Native also wraps two platform
// synchronization callbacks around these writes; they do not alter the local-player mapping.
void SetKeyboardPlayerAndPreferredBinding_7B82F0(PlayerRouteOwner120*self,int player){*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self)+0x273C4+self->profileIndex*0x5940)=player;*reinterpret_cast<int*>(gInput_1249C40+0x614)=player;auto*session=*reinterpret_cast<void**>(gRoot_12340A4+0x1042C);SessionSetDevice_C42A50(session,player,*reinterpret_cast<int*>(gInput_1249C40+0x5D8));}
} // namespace re5::split120
