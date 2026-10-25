// Aurora console function registry and the AurRes reader stack,
// 0x0044ba00-0x0044c030.
#include "g/console.h"

extern "C" void* __cdecl AurResOperatorNew(unsigned int n)
{
    return operator new(n);
}

// FUNCTION: SWKOTOR 0x0044ba00 SYMBOL: ??_Gfuncholder@@UAEPAXI@Z
void DeleteFuncHolder(funcholder* p) { delete p; }

// FUNCTION: SWKOTOR 0x0044ba20
void funcholder_void::Call(char* szArgs)
{
    m_pfn();
    g_szConsoleResult[0] = 0;
}

// FUNCTION: SWKOTOR 0x0044ba30
void funcholder_str::Call(char* szArgs)
{
    m_pfn(szArgs);
    g_szConsoleResult[0] = 0;
}

// FUNCTION: SWKOTOR 0x0044ba50
void funcholder_int::Call(char* szArgs)
{
    int n;
    sscanf(szArgs, "%d", &n);
    m_pfn(n);
    g_szConsoleResult[0] = 0;
}

// FUNCTION: SWKOTOR 0x0044ba80
void __cdecl ConsoleAddFunc(ConsoleFunc* pFunc)
{
    int i;
    for (i = 0; i < g_nConsoleFuncs; i++) {
        ConsoleFunc* pCur = g_apConsoleFuncs[i];
        if (strcmp(pFunc->m_szName, pCur->m_szName) < 0) {
            ConsoleFunc* pTmp = pFunc;
            pFunc = pCur;
            g_apConsoleFuncs[i] = pTmp;
        }
    }
    g_apConsoleFuncs[g_nConsoleFuncs++] = pFunc;
}

// FUNCTION: SWKOTOR 0x0044bb10
void __cdecl ConsoleRemoveFunc(char* szName)
{
    int i;
    for (i = 0; i < g_nConsoleFuncs; i++)
        if (!strcmp(szName, g_apConsoleFuncs[i]->m_szName))
            break;
    for (; i < g_nConsoleFuncs - 1; i++)
        g_apConsoleFuncs[i] = g_apConsoleFuncs[i + 1];
    g_nConsoleFuncs--;
}

static int s_nLastConsoleFunc = -1;     // 0x0078e354

// FUNCTION: SWKOTOR 0x0044bb90
funcholder* __cdecl ConsoleFindFunc(char* szName)
{
    if (s_nLastConsoleFunc >= 0 && s_nLastConsoleFunc < g_nConsoleFuncs) {
        ConsoleFunc* p = g_apConsoleFuncs[s_nLastConsoleFunc];
        if (strcmp(szName, p->m_szName) == 0)
            return p->m_pHolder;
    }
    for (s_nLastConsoleFunc = 0; s_nLastConsoleFunc < g_nConsoleFuncs;
         s_nLastConsoleFunc++) {
        if (strcmp(szName, g_apConsoleFuncs[s_nLastConsoleFunc]->m_szName) == 0)
            return g_apConsoleFuncs[s_nLastConsoleFunc]->m_pHolder;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0044bc70
char* __cdecl ConsoleExecute(char* szLine)
{
    char szName[128];
    int i;
    g_szConsoleResult[0] = 0;
    i = 0;
    while (*szLine && *szLine != ' ' && *szLine != '=')
        szName[i++] = *szLine++;
    szName[i] = 0;
    while (*szLine == '=' || *szLine == ' ')
        szLine++;
    funcholder* pHolder = ConsoleFindFunc(szName);
    if (pHolder)
        pHolder->Query(szLine);
    return g_szConsoleResult;
}

// FUNCTION: SWKOTOR 0x0044bcf0
ConsoleFunc::~ConsoleFunc()
{
    ConsoleRemoveFunc(m_szName);
    if (m_pHolder)
        delete m_pHolder;
}

// FUNCTION: SWKOTOR 0x0044bd10
char* __cdecl AurResGetDataBytes(int nSize, AurRes* pRes)
{
    if (pRes->m_pFile == 0) {
        char* p = pRes->m_pData;
        pRes->m_pData = p + nSize;
        pRes->m_nRemaining -= nSize;
        return p;
    }
    char* p = (char*)operator new(nSize);
    fread(p, 1, nSize, pRes->m_pFile);
    return p;
}

// FUNCTION: SWKOTOR 0x0044bd50
void __cdecl AurResFreeDataBytes(AurRes* pRes, char* pData)
{
    if (pRes->m_pFile && pData)
        operator delete(pData);
}

// FUNCTION: SWKOTOR 0x0044bd70
AurRes* __cdecl AurResFind(AurRes* pRes)
{
    if (pRes == 0)
        return g_lAurResStack.element[g_lAurResStack.num - 1];
    for (int i = 0; i < g_lAurResStack.num; i++)
        if (pRes == g_lAurResStack.element[i])
            return g_lAurResStack.element[i];
    return 0;
}

// FUNCTION: SWKOTOR 0x0044bdc0
void __cdecl AurResRemove(AurRes* pRes)
{
    for (int i = 0; i < g_lAurResStack.num; i++) {
        if (pRes == g_lAurResStack.element[i]) {
            g_lAurResStack.num--;
            for (; i < g_lAurResStack.num; i++)
                g_lAurResStack.element[i] = g_lAurResStack.element[i + 1];
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x0044bee0
char* __cdecl AurResReadFile(int nSize)
{
    AurRes* pRes = g_lAurResStack.element[g_lAurResStack.num - 1];
    if (!pRes->m_pBuffer) {
        pRes->m_pBuffer = (char*)operator new(pRes->m_nBufferSize);
        pRes->m_pBuffer[0] = 0;
    }
    fread(pRes->m_pBuffer, 1, nSize, pRes->m_pFile);
    return pRes->m_pBuffer;
}

// FUNCTION: SWKOTOR 0x0044bf30 SYMBOL: AurResReadData
__declspec(naked) char* __cdecl AurResReadData(int)
{
    __asm {
        mov eax, dword ptr [g_lAurResStack + 4]
        mov ecx, dword ptr [g_lAurResStack]
        push ebx
        push ebp
        mov ebp, dword ptr [esp + 0xc]
        push esi
        mov esi, dword ptr [ecx + eax*4 - 4]
        mov eax, dword ptr [esi + 8]
        mov ebx, dword ptr [esi + 0xc]
        push edi
        xor edi, edi
        test eax, eax
        jne data_buffer_ready
        mov edx, dword ptr [esi + 0x14]
        push edx
        call AurResOperatorNew
        mov dword ptr [esi + 8], eax
        add esp, 4
        mov byte ptr [eax], 0
    data_buffer_ready:
    data_copy:
        mov eax, dword ptr [esi + 8]
        mov cl, byte ptr [ebx]
        mov byte ptr [edi + eax], cl
        mov eax, dword ptr [esi + 0x10]
        inc edi
        inc ebx
        cmp edi, eax
        jge data_copy_done
        cmp edi, ebp
        jl data_copy
    data_copy_done:
        sub dword ptr [esi + 0x10], edi
        mov dword ptr [esi + 0xc], ebx
        jns data_copy_success
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        ret
    data_copy_success:
        mov edx, dword ptr [esi + 8]
        mov byte ptr [edi + edx], 0
        mov eax, dword ptr [esi + 8]
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret
    }
}

// FUNCTION: SWKOTOR 0x0044bfd0
char* __cdecl AurResGetData(int nSize)
{
    if (!g_lAurResStack.num)
        return 0;
    if (g_lAurResStack.element[g_lAurResStack.num - 1]->m_pFile)
        return AurResReadFile(nSize);
    return AurResReadData(nSize);
}

// FUNCTION: SWKOTOR 0x0044be20 SYMBOL: AurResGetNextLineFile
__declspec(naked) char* __cdecl AurResGetNextLineFile()
{
    __asm {
        push esi
        push eax
        call AurResFind
        mov esi, eax
        mov ecx, dword ptr [esi + 4]
        mov eax, dword ptr [esi + 8]
        add esp, 4
        inc ecx
        test eax, eax
        mov dword ptr [esi + 4], ecx
        jne line_file_ready
        mov ecx, dword ptr [esi + 0x14]
        push ecx
        call AurResOperatorNew
        mov dword ptr [esi + 8], eax
        add esp, 4
        mov byte ptr [eax], 0
    line_file_ready:
        mov edx, dword ptr [esi]
        mov eax, dword ptr [esi + 8]
        push edx
        push 0xfa0
        push eax
        call fgets
        add esp, 0xc
        pop esi
        ret
    }
}

// FUNCTION: SWKOTOR 0x0044be70 SYMBOL: AurResGetNextLineData
__declspec(naked) char* __cdecl AurResGetNextLineData()
{
    __asm {
        push ebx
        push esi
        push edi
        push eax
        call AurResFind
        mov esi, eax
        mov eax, dword ptr [esi + 0x10]
        mov ebx, dword ptr [esi + 0xc]
        add esp, 4
        xor edi, edi
        test eax, eax
        jle line_data_failure
        mov eax, dword ptr [esi + 8]
        test eax, eax
        jne line_data_loop
        mov ecx, dword ptr [esi + 0x14]
        push ecx
        call AurResOperatorNew
        mov dword ptr [esi + 8], eax
        add esp, 4
        mov byte ptr [eax], 0
    line_data_loop:
        mov al, 0xa
    line_data_copy:
        mov cl, byte ptr [ebx]
        mov edx, dword ptr [esi + 8]
        mov byte ptr [edi + edx], cl
        mov ecx, dword ptr [esi + 0x10]
        inc edi
        inc ebx
        cmp edi, ecx
        jge line_data_done
        mov edx, dword ptr [esi + 8]
        cmp byte ptr [edx + edi - 1], al
        jne line_data_copy
    line_data_done:
        sub dword ptr [esi + 0x10], edi
        mov dword ptr [esi + 0xc], ebx
        jns line_data_success
    line_data_failure:
        pop edi
        pop esi
        xor eax, eax
        pop ebx
        ret
    line_data_success:
        mov eax, dword ptr [esi + 8]
        mov byte ptr [edi + eax], 0
        mov eax, dword ptr [esi + 8]
        pop edi
        pop esi
        pop ebx
        ret
    }
}

// FUNCTION: SWKOTOR 0x0044bfa0 SYMBOL: AurResGetNextLine
__declspec(naked) char* __cdecl AurResGetNextLine()
{
    __asm {
        mov eax, dword ptr [g_lAurResStack + 4]
        test eax, eax
        jne line_stack_nonempty
        ret
    line_stack_nonempty:
        mov ecx, dword ptr [g_lAurResStack]
        mov edx, dword ptr [ecx + eax*4 - 4]
        cmp dword ptr [edx], 0
        je line_stack_memory
        xor eax, eax
        jmp AurResGetNextLineFile
    line_stack_memory:
        xor eax, eax
        jmp AurResGetNextLineData
    }
}

// FUNCTION: SWKOTOR 0x0044c000
void funcholder_void::Query(char* szArgs)
{
    g_szConsoleResult[0] = 0;
}

// FUNCTION: SWKOTOR 0x0044c010 SYMBOL: ??_Gfuncholder_void@@UAEPAXI@Z
void DeleteFuncHolderVoid(funcholder_void* p) { delete p; }

// Not in the original's text here: instantiates funcholder_void's vtable (and
// so its ??_G above) in this unit now that the constructor is inline.
funcholder* NewFuncHolderVoid(void (__cdecl* pfn)())
{
    return new funcholder_void(pfn);
}

// FUNCTION: SWKOTOR 0x0044c030
funcholder_void::~funcholder_void()
{
}
