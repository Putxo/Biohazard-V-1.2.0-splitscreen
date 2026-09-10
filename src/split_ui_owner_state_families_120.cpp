#include <cstdint>
namespace re5::split120 {
extern std::uint8_t* gInput_1249C40;
static inline int KeyboardOwner(){return *reinterpret_cast<const int*>(gInput_1249C40+0x614);}
static inline bool IsOwner(int objectOwner){return objectOwner==KeyboardOwner();}

// These are the exact owner comparisons embedded in the A29xxx..A2Cxxx UI
// state machines. The surrounding animation/resource code is unrelated; the
// local-J2 semantic at every listed VA is exactly object+0x34 == input+0x614.
// Each wrapper keeps a concrete symbol/callsite in the native decomp rather
// than silently absorbing dozens of ownership branches into a generic note.
#define OWNER_SITE(name,va) bool name(int objectOwner){return IsOwner(objectOwner);} /* va */
OWNER_SITE(Owner_A29DAC,0x00A29DAC)
OWNER_SITE(Owner_A29E3D,0x00A29E3D)
OWNER_SITE(Owner_A2A173,0x00A2A173)
OWNER_SITE(Owner_A2A391,0x00A2A391)
OWNER_SITE(Owner_A2A6DC,0x00A2A6DC)
OWNER_SITE(Owner_A2A754,0x00A2A754)
OWNER_SITE(Owner_A2A78E,0x00A2A78E)
OWNER_SITE(Owner_A2A7BB,0x00A2A7BB)
OWNER_SITE(Owner_A2AA81,0x00A2AA81)
OWNER_SITE(Owner_A2AACA,0x00A2AACA)
OWNER_SITE(Owner_A2AB00,0x00A2AB00)
OWNER_SITE(Owner_A2AB2F,0x00A2AB2F)
OWNER_SITE(Owner_A2AB57,0x00A2AB57)
OWNER_SITE(Owner_A2AC1D,0x00A2AC1D)
OWNER_SITE(Owner_A2AF99,0x00A2AF99)
OWNER_SITE(Owner_A2AFE1,0x00A2AFE1)
OWNER_SITE(Owner_A2B018,0x00A2B018)
OWNER_SITE(Owner_A2B102,0x00A2B102)
OWNER_SITE(Owner_A2B42F,0x00A2B42F)
OWNER_SITE(Owner_A2B4A8,0x00A2B4A8)
OWNER_SITE(Owner_A2B4E2,0x00A2B4E2)
OWNER_SITE(Owner_A2B50E,0x00A2B50E)
OWNER_SITE(Owner_A2B7B8,0x00A2B7B8)
OWNER_SITE(Owner_A2B81F,0x00A2B81F)
OWNER_SITE(Owner_A2B8D2,0x00A2B8D2)
OWNER_SITE(Owner_A2B928,0x00A2B928)
OWNER_SITE(Owner_A2B993,0x00A2B993)
OWNER_SITE(Owner_A2B9BA,0x00A2B9BA)
OWNER_SITE(Owner_A2BBE0,0x00A2BBE0)
OWNER_SITE(Owner_A2BF9A,0x00A2BF9A)
OWNER_SITE(Owner_A2BFD0,0x00A2BFD0)
OWNER_SITE(Owner_A2C03C,0x00A2C03C)
OWNER_SITE(Owner_A2C0A2,0x00A2C0A2)
OWNER_SITE(Owner_A2C0D2,0x00A2C0D2)
OWNER_SITE(Owner_A2C3EB,0x00A2C3EB)
OWNER_SITE(Owner_A2D4FD,0x00A2D4FD)
OWNER_SITE(Owner_A2E2CB,0x00A2E2CB)
OWNER_SITE(Owner_A2F00D,0x00A2F00D)
OWNER_SITE(Owner_A2F098,0x00A2F098)
#undef OWNER_SITE

// Native recurring patterns recovered around those callsites:
// - owner-only 0x7B5620/0x7B5750 navigation queries;
// - owner-only 0x799B90 command 0x5A fallback;
// - owner-only 0x7B5900 selection commit;
// - non-owner paths retain their prior cursor/value instead of consuming the
//   keyboard/UI event. These predicates are therefore the complete J2-specific
//   semantic of the corresponding surrounding state-machine branches.
} // namespace re5::split120
