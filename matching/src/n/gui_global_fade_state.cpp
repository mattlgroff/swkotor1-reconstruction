// CGuiInGame fade state helpers. The playable reconstruction gives the
// object offsets; callee identities are checked against matching/symbols.csv.
class CSWGuiPanel {
};

class CSWGuiFade : public CSWGuiPanel {
public:
    void ResetFade();                                // 0x006247A0
    int IsFadeDone();                                // 0x006244B0
};

class CSWGuiLetterbox {
public:
    int IsFadeDone();                                // 0x006A6DA0
};

class CSWGuiManager {
public:
    int RemovePanel(CSWGuiPanel* pPanel);            // 0x0040C830
};

class CGuiInGame {
public:
    char pad_0x0[0x38];
    CSWGuiManager* m_pGuiManager;                     // +0x38
    char pad_0x3c[0x2c];
    CSWGuiLetterbox* m_pDialogFade;                   // +0x68
    CSWGuiFade* m_pGlobalFade;                        // +0x6c

    int IsFadingInDialog();
    void StopGlobalFade();
    int IsGlobalFading();
};

// FUNCTION: SWKOTOR 0x0062ABB0
int CGuiInGame::IsFadingInDialog()
{
    if (m_pDialogFade && m_pDialogFade->IsFadeDone() == 0)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0062AC40
void CGuiInGame::StopGlobalFade()
{
    if (!m_pGlobalFade)
        return;
    m_pGlobalFade->ResetFade();
    m_pGuiManager->RemovePanel(m_pGlobalFade);
}

// FUNCTION: SWKOTOR 0x0062AC60
int CGuiInGame::IsGlobalFading()
{
    if (m_pGlobalFade && m_pGlobalFade->IsFadeDone() == 0)
        return 1;
    return 0;
}
