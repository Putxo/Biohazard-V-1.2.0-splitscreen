#include <cstdint>
namespace re5::split120 {
struct Session120;
extern std::uint8_t* gRoot_12340A4;
extern bool IsMercsLocalSplitRow_C42EC0(const Session120*);
extern bool IsMercsRow0_C42E80(const Session120*);
static inline Session120* Session(){return *reinterpret_cast<Session120**>(gRoot_12340A4+0x1042C);} static inline std::uint8_t* SB(Session120*s){return reinterpret_cast<std::uint8_t*>(s);}
// 71AACE..71AAF3 VERIFIED: row0 and local row2 branch to 71ABC0, bypassing generic network/session checks.
bool UseMercsImmediateSessionGate_A71AAE6(){Session120*s=Session();return IsMercsRow0_C42E80(s)||IsMercsLocalSplitRow_C42EC0(s);}
// 722889..7228E7 VERIFIED: local split selects 71CAB0 instead of 71C890.
enum class SessionBuildPath120:std::uint8_t{Generic71C890,Local71CAB0};
SessionBuildPath120 SelectMercsSessionBuildPath_A722895(){return IsMercsLocalSplitRow_C42EC0(Session())?SessionBuildPath120::Local71CAB0:SessionBuildPath120::Generic71C890;}
// 792986..7929D9 VERIFIED: current-player bit always set; local split also sets XOR-1 partner bit.
std::uint32_t ExpandLocalContextMask_A7929AB(std::uint32_t existing,int player){std::uint32_t m=existing|(1u<<unsigned(player));if(IsMercsLocalSplitRow_C42EC0(Session()))m|=(1u<<unsigned(player^1));return m;}
// 7979F0..797A68 VERIFIED: row0=1, local row2=3, row1=1<<session+5DC.
std::uint32_t SelectMercsDeviceScanMask_A797A20(){Session120*s=Session();if(IsMercsRow0_C42E80(s))return 1u;if(IsMercsLocalSplitRow_C42EC0(s))return 3u;return 1u<<unsigned(*reinterpret_cast<int*>(SB(s)+0x5DC));}
// A00023..A00057 VERIFIED: local row2 exits through A000F7 before generic self+1EC=0x1A is installed.
bool SuppressGenericFrontendState1A_A0003E(){return IsMercsLocalSplitRow_C42EC0(Session());}
// A1F01D..A1F03A VERIFIED: C42EC0 return is immediately overwritten by XOR AL,AL and never tested; predicate is pure.
bool C42EC0ResultIsIgnored_A1F032(){(void)IsMercsLocalSplitRow_C42EC0(Session());return false;}
} // namespace re5::split120
