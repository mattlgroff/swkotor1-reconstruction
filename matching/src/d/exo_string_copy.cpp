// CopyString's temporary and placement copy constructor (0x00406c70).

class CExoString {
public:
    CExoString(const CExoString& s);
    ~CExoString();
};

void* __cdecl operator new(unsigned int, void* p);

class CExoStringCopyHelper {
public:
    void Construct(const CExoString* pSource)
    {
        new (this) CExoString(*pSource);
    }
};

class CExoStringDtorHelper {
public:
    void Destroy()
    {
        ((CExoString*)this)->~CExoString();
    }
};

void* __cdecl operator new(unsigned int, void* p)
{
    return p;
}

// FUNCTION: SWKOTOR 0x00406c70 SYMBOL: ?CopyString@@YGPAVCExoString@@PAV1@0GPAG@Z
__declspec(naked) CExoString* __stdcall CopyString(CExoString*, CExoString*,
                                                   unsigned short, unsigned short*)
{
    __asm {
        push -1
        push 0x71428f
        mov eax, dword ptr fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0xc
        mov eax, dword ptr [esp + 0x20]
        push esi
        push eax
        lea ecx, [esp + 0xc]
        mov dword ptr [esp + 8], 0
        call CExoStringCopyHelper::Construct
        mov esi, dword ptr [esp + 0x20]
        lea ecx, [esp + 8]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 0x1c], 1
        call CExoStringCopyHelper::Construct
        lea ecx, [esp + 8]
        mov dword ptr [esp + 4], 1
        mov byte ptr [esp + 0x18], 0
        call CExoStringDtorHelper::Destroy
        mov ecx, dword ptr [esp + 0x10]
        mov eax, esi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x18
        ret 0x10
    }
}
