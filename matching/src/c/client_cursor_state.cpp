class CExoInput {
public:
    int SetMouseCursor(int nCursor, int nOption, int bAlternate);
    void HideMouse();
    void ShowMouse();
    void RefreshMouseCursor(int b);                 // 0x005DF5B0
};

extern int g_bMouseButtonDown;                      // 0x008338F4

class CClientOptions {
public:
    unsigned char m_nFlags;                         // 0x00 (bit 1: software mouse off)
};

extern CExoInput* g_pExoInput;

class CSWGuiManager {
public:
    void SetSoftwareMouseCursor(int nCursor, int nOption, int bAlternate); // 0x0040A270
    void HideSoftwareMouse();                                           // 0x0040A260
    void* ShowSoftwareMouse();                                          // 0x0040B290
};

class CClientExoAppInternal {
public:
    void* m_pUnknown00;                             // 0x00
    CClientOptions* m_pClientOptions;               // 0x04
    char pad_0x8[0x26c];
    void* m_pGuiManager;                            // 0x274
    char pad_0x278[0x160];
    unsigned long m_nInputState;                    // 0x3d8
    float m_fMouseSpeedScale;                       // 0x3dc

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

// FUNCTION: SWKOTOR 0x0061FA00
void CClientExoAppInternal::ClearInputState(int nMask)
{
    m_nInputState &= ~nMask;
    if (m_nInputState != 0) {
        return;
    }
    if (nMask & 0x4a) {
        m_fMouseSpeedScale = 0.5f;
    }
    if (nMask & 0x14) {
        m_fMouseSpeedScale = -1.0f;
    }
    if (m_fMouseSpeedScale <= 0.0f) {
        g_pExoInput->ShowMouse();
        if (m_pGuiManager != 0 && !(m_pClientOptions->m_nFlags & 2)) {
            ((CSWGuiManager*)m_pGuiManager)->ShowSoftwareMouse();
        }
        g_pExoInput->RefreshMouseCursor(g_bMouseButtonDown);
    }
}
