class UnknownObjectAt004fde20 {
public:
    ~UnknownObjectAt004fde20();
};

// FUNCTION: SWKOTOR 0x004FDE20
void* __fastcall unknown_scalar_delete_004fde20(void* pThis, void*, unsigned int flags)
{
    ((UnknownObjectAt004fde20*)pThis)->~UnknownObjectAt004fde20();
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}
