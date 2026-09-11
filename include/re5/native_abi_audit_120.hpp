#pragma once
#include <cstdint>

namespace re5::split120 {

// Canonical native ABIs verified against re5dx9 1.2.0 disassembly.
// This header is force-included by the static audit build so translation units
// share one ABI for each native VA. Older descriptive names below are explicit
// inline adapters, never independent external/native symbols.

int  __thiscall QueryGameStatus_C42D90(void* session);
void __thiscall SessionSetSlotMode_C42A30(void* session,int slot,int value);
void __thiscall SessionSetDevice_C42A50(void* session,int slot,int device);
void __thiscall SetSessionLocalPair_C42A70(void* session,int slot,int value);
void __thiscall SetSessionPartnerPair_C42A90(void* session,int slot,int value);
void __thiscall SetSessionAuxPair_C42AB0(void* session,int slot,int value);
int  __thiscall CountActiveSessionSlots_C42B60(const void* session);
bool __thiscall IsMercsRow0_C42E80(const void* session);
bool __thiscall IsMercsRow1_C42EA0(const void* session);
bool __thiscall IsMercsLocalSplitRow_C42EC0(const void* session);
bool __thiscall IsDlcRow0_C432E0(const void* session);
bool __thiscall IsDlcRow1_C43300(const void* session);
bool __thiscall IsDlcLocalSplitRow_C43320(const void* session);
bool __thiscall IsNativeLocalCoopActive_C43BB0(const void* session);
bool __thiscall PreferredOwnerAllowsCurrentKeyboard_799B50(void* input);

int  __thiscall SplitCoordTransform_76A420(void* split,int x);
int  __thiscall FullCoordTransform_76A460(void* split,int x);
int  __thiscall SplitVerticalTransform_76A4A0(void* split,int y);

void __thiscall UiCommand4370_7B4370(void* ui,int command,int arg);
void __thiscall UiCommand4480_7B4480(void* ui,int x,int y,int index);
bool __thiscall UiRoute5620_7B5620(void* ui,int a,int b,int c);
bool __thiscall UiRoute5750_7B5750(void* ui,int a,int b,int c,int d);
void __thiscall UiRoute43C0_7B43C0(void* ui,int a,int b);
int  __thiscall UiGetCursor_7B52A0(void* ui,int index);
int  __thiscall UiGetSelection_7B5360(void* ui,int index);
bool __thiscall UiOwnedInput38_7B4660(void* ui,int selector);
int  __thiscall UiSelectionStep_7B5900(void* ui,int current,int expectedRoute,int count,int minimum,int index);
int  __thiscall UiSelectionStepBy_7B59B0(void* ui,int current,int expectedRoute,int maximum,int minimum,int step,int index);

void __thiscall RootSetFlags_726120(void* root,std::uint32_t mask);
void __thiscall SetLocalPlayerActive_716720(void* context,bool active,int slot,int device);

static inline void* NativeSession_120(){
    auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4);
    return *reinterpret_cast<void**>(root+0x1042C);
}

// C42D90 compatibility names used by older decomp units.
inline int QueryGameStatus_C42D90(){return QueryGameStatus_C42D90(NativeSession_120());}
inline int GetRuntimeStatus_C42D90(void* session){return QueryGameStatus_C42D90(session);}
inline int GetRuntimeStatus_C42D90(){return QueryGameStatus_C42D90(NativeSession_120());}
inline int SessionDisplayStatus_C42D90(void* session){return QueryGameStatus_C42D90(session);}
inline int SessionStatus_C42D90(void* session){return QueryGameStatus_C42D90(session);}
inline int Status_C42D90(){return QueryGameStatus_C42D90(NativeSession_120());}

// C42A30/C42A50 compatibility names.
inline void __thiscall SetSessionSlotMode_C42A30(void* session,int slot,int value){SessionSetSlotMode_C42A30(session,slot,value);}
inline void __thiscall SetSessionSlotDevice_C42A50(void* session,int slot,int device){SessionSetDevice_C42A50(session,slot,device);}
inline void __thiscall SessionSetSlotBinding_C42A50(void* session,int slot,int device){SessionSetDevice_C42A50(session,slot,device);}

// Relationship-setter compatibility names retained only as inline adapters.
inline void __thiscall SessionSetLocalPair_C42A70(void* session,int slot,int value){SetSessionLocalPair_C42A70(session,slot,value);}
inline void __thiscall SessionSetPartnerPair_C42A90(void* session,int slot,int value){SetSessionPartnerPair_C42A90(session,slot,value);}
inline void __thiscall SessionSetSlotAux_C42AB0(void* session,int slot,int value){SetSessionAuxPair_C42AB0(session,slot,value);}
inline void __thiscall SessionSetPair_C42A70(void* session,int slot,int value){SetSessionLocalPair_C42A70(session,slot,value);}
inline void __thiscall SessionSetPair_C42A90(void* session,int slot,int value){SetSessionPartnerPair_C42A90(session,slot,value);}
inline void __thiscall SessionSetPair_C42AB0(void* session,int slot,int value){SetSessionAuxPair_C42AB0(session,slot,value);}
inline void __thiscall SessionField_C42A70(void* session,int slot,int value){SetSessionLocalPair_C42A70(session,slot,value);}
inline void __thiscall SessionField_C42A90(void* session,int slot,int value){SetSessionPartnerPair_C42A90(session,slot,value);}
inline void __thiscall SessionField_C42AB0(void* session,int slot,int value){SetSessionAuxPair_C42AB0(session,slot,value);}

// C42B60/C43BB0/799B50 compatibility names.
inline int __thiscall CountActivePlayers_C42B60(const void* session){return CountActiveSessionSlots_C42B60(session);}
inline bool __thiscall SessionHasLocalCoop_C43BB0(const void* session){return IsNativeLocalCoopActive_C43BB0(session);}
inline bool __thiscall IsNativeLocalSplitRoute_C43BB0(const void* session){return IsNativeLocalCoopActive_C43BB0(session);}
inline bool __thiscall LocalSplitInputPrecheck_799B50(void* input){return PreferredOwnerAllowsCurrentKeyboard_799B50(input);}

// 7B4370/7B4480 compatibility names used by geometry/front-end units.
inline void InputBaseUpdate_7B4370(void* ui,int command,int arg){UiCommand4370_7B4370(ui,command,arg);}
inline void UiCommand_7B4370(void* ui,int command,int arg){UiCommand4370_7B4370(ui,command,arg);}
inline void InputSplitOffset_7B4480(void* ui,int x,int y,int index){UiCommand4480_7B4480(ui,x,y,index);}
inline void ApplyUiInputOffset_7B4480(void* ui,int x,int y,int index){UiCommand4480_7B4480(ui,x,y,index);}

// Audited convenience form used by older local-activation units.
inline void __stdcall SetLocalPlayerActive_716720(bool active,int slot,int device){
    SetLocalPlayerActive_716720(nullptr,active,slot,device);
}

} // namespace re5::split120
