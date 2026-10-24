#pragma once
#include <cstdint>
#include <windows.h>
using NewHandler=int (__cdecl*)(std::uint32_t);
// Every original input is a live binding, not a per-call copied value.
struct LiveAllocatorGlobals {
    HANDLE volatile* heap;
    std::uint32_t volatile *mode,*threshold,*new_mode;
    NewHandler volatile* handler;
    std::uint32_t volatile *header_base,*header_count;
};
struct LiveAllocatorDependencies {
    void* (__stdcall *heap_alloc)(HANDLE,DWORD,SIZE_T);
    BOOL (__stdcall *heap_free)(HANDLE,DWORD,void*);
    void (__cdecl *lock)(int);
    void (__cdecl *unlock)(int);
    void* (__cdecl *sbh_alloc)(std::uint32_t);
    void (__cdecl *sbh_free)(void*,void*);
};
extern LiveAllocatorGlobals live_globals;
extern LiveAllocatorDependencies live_dependencies;
extern "C" {
void* __cdecl live_heap_alloc(std::uint32_t);
void* __cdecl live_nh_malloc(std::uint32_t,int);
void* __cdecl live_operator_new(std::uint32_t);
void* __cdecl live_malloc(std::uint32_t);
int __cdecl live_callnewh(std::uint32_t);
void* __cdecl live_sbh_find(void*);
void __cdecl live_free(void*);
}
