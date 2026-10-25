// CGuiInGame mode and target forwards. Mac-name hints are confirmed by the
// original field offsets and direct-call/tail-jump targets.
class CSWGuiMainInterface {
public:
    void SetTargetId(int nTarget, int bTarget);       // 0x006855F0
};

class CSWGuiTutorialBox {
public:
    int ShowTutorial(unsigned char nReason);         // 0x006AA900
};

class CGuiInGame {
public:
    char pad_0x0[0x90];
    CSWGuiMainInterface* m_pMainInterface;            // +0x90
    char pad_0x94[0x0c];
    CSWGuiTutorialBox* m_pTutorialBox;                // +0xA0
    char pad_0xA4[0x68];
    int m_nLevelUpMode;                              // +0x10C

    void SetLevelUpMode(int nMode);
    void SetMainInterfaceTarget(unsigned long nTarget, int bTarget);
    int SetTutorialReason(unsigned char nReason);
};

// FUNCTION: SWKOTOR 0x00628650
void CGuiInGame::SetLevelUpMode(int nMode)
{
    if (m_nLevelUpMode != nMode)
        m_nLevelUpMode = nMode;
}

// FUNCTION: SWKOTOR 0x0062B000
void CGuiInGame::SetMainInterfaceTarget(unsigned long nTarget, int bTarget)
{
    if (m_pMainInterface)
        m_pMainInterface->SetTargetId((int)nTarget, bTarget);
}

// FUNCTION: SWKOTOR 0x0062B0D0
int CGuiInGame::SetTutorialReason(unsigned char nReason)
{
    if (!m_pTutorialBox)
        return 0;
    return m_pTutorialBox->ShowTutorial(nReason);
}
