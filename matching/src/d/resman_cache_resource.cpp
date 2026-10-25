class CExoString;

extern CExoString* __stdcall CopyString(CExoString* pOutput, CExoString* pInput,
                                         unsigned short nType, unsigned short* pTypeSlot);

class CExoResMan {
public:
    CExoString* CacheResource(CExoString* pOutput, CExoString* pInput, unsigned short nType);
};

// FUNCTION: SWKOTOR 0x004085F0 SYMBOL: ?CacheResource@CExoResMan@@QAEPAVCExoString@@PAV2@0G@Z
__declspec(naked) CExoString* CExoResMan::CacheResource(CExoString*, CExoString*, unsigned short)
{
    __asm {
        push ecx
        mov edx, dword ptr [esp + 10h]
        push esi
        mov esi, dword ptr [esp + 0ch]
        lea eax, [esp + 14h]
        push eax
        mov eax, dword ptr [esp + 14h]
        push edx
        push eax
        push esi
        mov dword ptr [esp + 14h], 0
        call CopyString
        mov eax, esi
        pop esi
        pop ecx
        ret 0ch
    }
}
