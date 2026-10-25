class CResHelperMiniGame {
public:
    virtual ~CResHelperMiniGame();
};

// FUNCTION: SWKOTOR 0x00506E00
void* __fastcall mini_game_helper_scalar_delete_00506e00(void* pThis, void*, unsigned int flags)
{
    ((CResHelperMiniGame*)pThis)->CResHelperMiniGame::~CResHelperMiniGame();
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}
