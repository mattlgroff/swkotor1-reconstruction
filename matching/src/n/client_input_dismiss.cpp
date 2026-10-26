// CClientExoAppInternal input-class leaves (clientinput.o).
class CGuiInGame {
public:
    int HideSWInGameGui(int bHide); // 0x0062CBA0
};
class CClientExoAppInternal {
public:
    char pad_0x0[0x40];
    CGuiInGame* m_pGuiInGame;
    int SetInputClass(int nClass,int bForce); // 0x006200E0
    int DismissInGameGUI();
};

// FUNCTION: SWKOTOR 0x00620320
int CClientExoAppInternal::DismissInGameGUI()
{
    if (m_pGuiInGame->HideSWInGameGui(0)) {
        SetInputClass(0,1);
        return 1;
    }
    return 0;
}
