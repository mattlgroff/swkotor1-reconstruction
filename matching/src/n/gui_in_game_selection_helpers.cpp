// Small CGuiInGame dialog-selection and interface-state helpers. Offsets,
// stack cleanup, and forwarding targets are corroborated by the binary.
class CSWGuiPanel {
};

class CSWGuiMainInterface {
public:
    void ShowStatus(unsigned char nStatus);           // 0x00687FE0
};

class CGuiInGame {
public:
    char pad_0x0[0x0C];
    CSWGuiPanel* m_aScreenPanels[8];                  // +0x0C
    char pad_0x2c[0x64];
    CSWGuiMainInterface* m_pMainInterface;            // +0x90
    char pad_0x94[0x34];
    int m_nSelectedReply;                             // +0xC8
    int m_nReplyCount;                                // +0xCC
    int m_bReplySelectionValid;                       // +0xD0
    int m_bSendReply;                                 // +0xD4
    unsigned long m_nHideCount;                       // +0xD8

    void SetDialogSelection(int nIndex, int nCount, int bSend);
    CSWGuiPanel* GetGuiInGameScreenPanel(unsigned char nIndex);
    void FlashStatus(unsigned char nStatus);
    void AdjustHideCount(int nAdjust);
};

// FUNCTION: SWKOTOR 0x0062ADA0
void CGuiInGame::SetDialogSelection(int nIndex, int nCount, int bSend)
{
    m_nSelectedReply = nIndex;
    m_nReplyCount = nCount;
    m_bSendReply = bSend;
    m_bReplySelectionValid = 1;
}

// FUNCTION: SWKOTOR 0x0062B060
CSWGuiPanel* CGuiInGame::GetGuiInGameScreenPanel(unsigned char nIndex)
{
    if (nIndex >= 8)
        return 0;
    return m_aScreenPanels[nIndex];
}

// FUNCTION: SWKOTOR 0x0062B0B0
void CGuiInGame::FlashStatus(unsigned char nStatus)
{
    if (m_pMainInterface)
        m_pMainInterface->ShowStatus(nStatus);
}

// FUNCTION: SWKOTOR 0x0062B410
void CGuiInGame::AdjustHideCount(int nAdjust)
{
    if (nAdjust == 1) {
        ++m_nHideCount;
    } else if (m_nHideCount > 0) {
        --m_nHideCount;
    }
}
