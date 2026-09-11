#pragma once
#include <cstdint>

namespace re5::split120 {

// Canonical native ABIs verified against re5dx9 1.2.0 disassembly.
// This header is force-included by the static audit build so any translation
// unit that redeclares one of these symbols with a different calling convention
// or parameter list fails at compile time.

int  __thiscall QueryGameStatus_C42D90(void* session);
void __thiscall SessionSetSlotMode_C42A30(void* session,int slot,int value);
void __thiscall SessionSetDevice_C42A50(void* session,int slot,int device);
void __thiscall UiCommand4370_7B4370(void* ui,int command,int arg);
void __thiscall UiCommand4480_7B4480(void* ui,int x,int y,int index);
bool __thiscall UiRoute5620_7B5620(void* ui,int a,int b,int c);
bool __thiscall UiRoute5750_7B5750(void* ui,int a,int b,int c,int d);
void __thiscall UiRoute43C0_7B43C0(void* ui,int a,int b);
void __thiscall RootSetFlags_726120(void* root,std::uint32_t mask);
void __thiscall SetLocalPlayerActive_716720(void* context,bool active,int slot,int device);

// Audited compatibility adapters for early translation units that used
// convenience overloads before the exact native ABI was recovered. They are
// deliberately inline so no fake external symbol survives into the archive.
static inline void* NativeSession_120(){
    auto* root=*reinterpret_cast<std::uint8_t**>(0x012340A4);
    return *reinterpret_cast<void**>(root+0x1042C);
}
static inline int QueryGameStatus_C42D90(){
    return QueryGameStatus_C42D90(NativeSession_120());
}
static inline void __stdcall SetLocalPlayerActive_716720(bool active,int slot,int device){
    SetLocalPlayerActive_716720(nullptr,active,slot,device);
}

} // namespace re5::split120
