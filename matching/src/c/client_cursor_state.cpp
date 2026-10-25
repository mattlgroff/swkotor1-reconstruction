class CExoInput {
public:
    int SetMouseCursor(int nCursor, int nOption, int bAlternate);
    void HideMouse();
    void ShowMouse();
};

extern CExoInput* g_pExoInput;

class CSWGuiManager {
public:
    void SetSoftwareMouseCursor(int nCursor, int nOption, int bAlternate); // 0x0040A270
    void HideSoftwareMouse();                                           // 0x0040A260
};

class CClientExoAppInternal {
public:
    char pad_0x0[0x274];
    void* m_pGuiManager;
    char pad_0x278[0x160];
    unsigned long m_nInputState;

    void SetMouseCursor(int nCursor, int nOption, int bAlternate);
    void SetInputState(int nMask);
    void ClearInputState(int nMask);
};

// FUNCTION: SWKOTOR 0x0061F980
void CClientExoAppInternal::SetMouseCursor(int nCursor, int nOption, int bAlternate)
{
    if (g_pExoInput != 0) {
        nCursor = g_pExoInput->SetMouseCursor(nCursor, nOption, bAlternate);
    }
    if (m_pGuiManager != 0) {
        ((CSWGuiManager*)m_pGuiManager)->SetSoftwareMouseCursor(nCursor, nOption, bAlternate);
    }
}

// FUNCTION: SWKOTOR 0x0061F9C0
void CClientExoAppInternal::SetInputState(int nMask)
{
    m_nInputState |= nMask;
    if (m_nInputState == 0) {
        return;
    }
    if (g_pExoInput != 0) {
        g_pExoInput->HideMouse();
    }
    if (m_pGuiManager != 0) {
        ((CSWGuiManager*)m_pGuiManager)->HideSoftwareMouse();
    }
}
