class UnknownObjectAt0050DFB0 {
public:
    ~UnknownObjectAt0050DFB0();
};

// FUNCTION: SWKOTOR 0x0050DFB0
void* __fastcall unknown_scalar_delete_0050dfb0(void* pThis, void*, unsigned int flags)
{
    ((UnknownObjectAt0050DFB0*)pThis)->~UnknownObjectAt0050DFB0();
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}
