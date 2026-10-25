// console command registration / completion and the AurRes
// open / close helpers, 0x0044c040-0x0044c97f.
#include "g/console.h"

extern "C" {
char* __cdecl strcpy(char* d, const char* s);
unsigned int __cdecl strlen(const char* s);
int __cdecl strncmp(const char* a, const char* b, unsigned int n);
int __cdecl sprintf(char* buf, const char* fmt, ...);
FILE* __cdecl fopen(const char* path, const char* mode);
int __cdecl fclose(FILE* f);
}
#pragma intrinsic(strcpy, strlen)

// Loader callbacks for in-memory resources.
extern unsigned char* (__cdecl* g_pfnAurResGet)(const char* szName,
                                                const char* szExt,
                                                int* pnSize); // 0x007b8fc4
extern void (__cdecl* g_pfnAurResFree)(void* pHandle, int nArg);    // 0x007b8fc8

void __cdecl operator delete[](void* p);

extern "C" void* __cdecl AurResResourceOperatorNew(unsigned int n)
{
    return operator new(n);
}

class AurResStackAllocateHelper {
public:
    void Allocate(int n);
};

#pragma auto_inline(off)
void AurResStackAllocateHelper::Allocate(int n)
{
    ((CExoArrayList<AurRes*>*)this)->Allocate(n);
}
#pragma auto_inline(on)

inline void CExoArrayList<ConsoleFunc*>::Allocate(int n)
{
    ConsoleFunc** pOld = element;
    array_size = n;
    element = new ConsoleFunc*[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

// FUNCTION: SWKOTOR 0x0044c040
char* __cdecl ConsoleComplete(char* szLine, CExoArrayList<ConsoleFunc*>* pList)
{
    pList->num = 0;
    g_szConsoleResult[0] = 0;
    if (!szLine)
        return g_szConsoleResult;
    {
        strcpy(g_szConsoleResult, szLine);
        char* pOut = g_szConsoleResult;
        char* pWord = szLine;
        int n = strlen(szLine);
        int i;
        for (i = 0; i < n; i++) {
            if (szLine[i] == ' ') {
                pWord = szLine + i + 1;
                pOut = &g_szConsoleResult[i + 1];
            }
        }
        for (i = 0; i < g_nConsoleFuncs; i++) {
            if (!strncmp(pWord, g_apConsoleFuncs[i]->m_szName, strlen(pWord)))
                pList->Add(g_apConsoleFuncs[i]);
        }
        if (pList->num) {
            if (pList->num == 1) {
                sprintf(pOut, "%s ", pList->element[0]->m_szName);
                return g_szConsoleResult;
            }
            char c;
            i = 0;
            do {
                c = pList->element[0]->m_szName[i];
                for (int k = 0; c && k < pList->num; k++)
                    if (c != pList->element[k]->m_szName[i])
                        c = 0;
                pOut[i] = c;
                i++;
            } while (c);
        }
    }
    return g_szConsoleResult;
}

// FUNCTION: SWKOTOR 0x0044c680 SYMBOL: AurResGetResource
__declspec(naked) AurRes* __fastcall AurResGetResource(const char*, const char*, int*)
{
    __asm {
        push ecx
        push ebx
        push ebp
        mov ebp, dword ptr [esp + 0x10]
        push edi
        lea eax, [esp + 0xc]
        push eax
        push ecx
        push edx
        call dword ptr [g_pfnAurResGet]
        mov edi, eax
        xor ebx, ebx
        add esp, 0xc
        cmp edi, ebx
        jne resource_data_ready
        pop edi
        pop ebp
        xor eax, eax
        pop ebx
        pop ecx
        ret
    resource_data_ready:
        push esi
        push 0x1c
        call AurResResourceOperatorNew
        add esp, 4
        cmp eax, ebx
        je resource_alloc_failed
        mov dword ptr [eax], ebx
        mov dword ptr [eax + 4], 0xffffffff
        mov dword ptr [eax + 0x14], 0x2000
        mov dword ptr [eax + 8], ebx
        mov dword ptr [eax + 0xc], ebx
        mov dword ptr [eax + 0x10], ebx
        mov dword ptr [eax + 0x18], ebx
        mov esi, eax
        jmp resource_initialized
    resource_alloc_failed:
        xor esi, esi
    resource_initialized:
        mov dword ptr [esi + 0x18], edi
        mov dword ptr [esi + 0xc], edi
        mov eax, dword ptr [esp + 0x10]
        cmp eax, ebx
        jl resource_negative_size
        mov dword ptr [esi + 0x10], eax
        jmp resource_size_ready
    resource_negative_size:
        or ecx, 0xffffffff
        sub ecx, eax
        mov dword ptr [esi + 0x10], ecx
    resource_size_ready:
        cmp ebp, ebx
        je resource_no_out_size
        mov edx, dword ptr [esp + 0x10]
        mov dword ptr [ebp], edx
    resource_no_out_size:
        mov eax, dword ptr [g_lAurResStack + 8]
        cmp dword ptr [g_lAurResStack + 4], eax
        jne resource_add_slot
        cmp eax, ebx
        je resource_initial_capacity
        add eax, eax
        jmp resource_capacity_ready
    resource_initial_capacity:
        mov eax, 8
    resource_capacity_ready:
        push eax
        mov ecx, offset g_lAurResStack
        call AurResStackAllocateHelper::Allocate
    resource_add_slot:
        mov eax, dword ptr [g_lAurResStack]
        mov ecx, dword ptr [g_lAurResStack + 4]
        mov dword ptr [eax + ecx*4], esi
        inc dword ptr [g_lAurResStack + 4]
        mov eax, esi
        pop esi
        pop edi
        pop ebp
        pop ebx
        pop ecx
        ret
    }
}

// FUNCTION: SWKOTOR 0x0044c740
void* __cdecl AurResGet(const char* szName, const char* szExt,
                        int* pnSize, char bMemoryOnly)
{
    char szPath[0x104];
    if (!szExt)
        szExt = "";
    if (g_pfnAurResGet) {
        AurRes* pRes = AurResGetResource(szExt, szName, pnSize);
        if (pRes)
            return pRes;
        if (bMemoryOnly)
            return 0;
    }
    const char* szSeparator = (szExt[0] && szExt[0] == '.') ? "" : ".";
    sprintf(szPath, "%s%s%s", szName, szSeparator, szExt);
    FILE* pFile = fopen(szPath, "rb");
    if (!pFile)
        return 0;
    AurRes* pRes = (AurRes*)operator new(0x1c);
    if (pRes) {
        pRes->m_pFile = 0;
        pRes->m_nLine = -1;
        pRes->m_nBufferSize = 0x7a120;
        pRes->m_pBuffer = 0;
        pRes->m_pData = 0;
        pRes->m_nRemaining = 0;
        pRes->m_pHandle = 0;
    }
    pRes->m_pFile = pFile;
    pRes->m_nLine = 0;
    g_lAurResStack.Add(pRes);
    if (pnSize)
        *pnSize = 1;
    return pRes;
}

// FUNCTION: SWKOTOR 0x0044c560 SYMBOL: ??0ConsoleFunc@@QAE@PADP6AXXZ@Z
ConsoleFunc::ConsoleFunc(char* szName, void (__cdecl* pfn)())
{
    strcpy(m_szName, szName);
    m_pHolder = new funcholder_void(pfn);
    ConsoleAddFunc(this);
}

// FUNCTION: SWKOTOR 0x0044c5c0 SYMBOL: ??0ConsoleFunc@@QAE@PADP6AX0@Z@Z
ConsoleFunc::ConsoleFunc(char* szName, void (__cdecl* pfn)(char*))
{
    strcpy(m_szName, szName);
    m_pHolder = new funcholder_str(pfn);
    ConsoleAddFunc(this);
}

// FUNCTION: SWKOTOR 0x0044c620 SYMBOL: ??0ConsoleFunc@@QAE@PADP6AXH@Z@Z
ConsoleFunc::ConsoleFunc(char* szName, void (__cdecl* pfn)(int))
{
    strcpy(m_szName, szName);
    m_pHolder = new funcholder_int(pfn);
    ConsoleAddFunc(this);
}

// FUNCTION: SWKOTOR 0x0044c870
AurRes* __cdecl AurResPushFile(FILE* pFile)
{
    AurRes* pRes = new AurRes(0x7a120);
    pRes->m_pFile = pFile;
    pRes->m_nLine = 0;
    g_lAurResStack.Add(pRes);
    return pRes;
}

// FUNCTION: SWKOTOR 0x0044c8f0
void __cdecl AurResClose(AurRes* pRes, int nArg)
{
    if (pRes) {
        if (pRes->m_pFile) {
            fclose(pRes->m_pFile);
            pRes->m_pFile = 0;
        } else {
            g_pfnAurResFree(pRes->m_pHandle, nArg);
            pRes->m_pHandle = 0;
        }
        if (g_lAurResStack.Contains(pRes))
            g_lAurResStack.Remove(pRes);
        if (pRes->m_pBuffer) {
            operator delete(pRes->m_pBuffer);
            pRes->m_pBuffer = 0;
        }
        operator delete(pRes);
    }
}
