// Options panels that persist settings to swkotor.ini and re-initialise the
// renderer when the vertex-buffer or display options change (swguioptions.o).
#include "l/sw_gui_options_in_game.h"

extern void AurDisableVertexBufferObjects(); // 0x0044EF60
extern void AurDisableVertexBufferObjectsWriteOnly(); // 0x0044EF70
extern void AurEnableVertexBufferObjectsWriteOnly(); // 0x0044EF80
extern int ReInitAurora(int nWidth, int nHeight, unsigned char nDepth, int bFullScreen, int bReload); // 0x00403800
extern int g_nScreenDepth; // 0x0078D1E0
extern int g_nScreenRefreshRate; // 0x0078D1DC
extern CSWGuiManager* g_pGuiManager; // 0x007A39F4

// Win32 DEVMODEA (0x9C bytes); only the fields used here are named.
struct DEVMODEA {
    char dmDeviceName[0x20];
    unsigned short dmSpecVersion;
    unsigned short dmDriverVersion;
    unsigned short dmSize;                          // 0x24
    unsigned short dmDriverExtra;
    char pad_0x28[0x78 - 0x28];
    unsigned long dmDisplayFrequency;               // 0x78
    char pad_0x7c[0x9c - 0x7c];
};
extern "C" __declspec(dllimport) int __stdcall EnumDisplaySettingsA(const char* lpszDeviceName,
                                                                   unsigned long iModeNum, DEVMODEA* lpDevMode);
extern "C" void* __cdecl memset(void* pDest, int nValue, unsigned int nCount);
#pragma intrinsic(memset)

class CExoSound {
public:
    void ReInitialize(unsigned long nQuality, unsigned long bHardware); // 0x005D63F0
    unsigned long GetProperty0x100(); // 0x005D6450 (current sound quality)
};
extern CExoSound* g_pExoSound; // 0x007A39EC

class CRenderWindow {
public:
    virtual void v00();
    virtual int IsFullScreen(); // 0x04
};
extern CRenderWindow* g_pRenderWindow; // 0x007A39F8

// FUNCTION: SWKOTOR 0x006DFD10
void CSWGuiOptionsMouse::OnRevMouseButtons(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CExoString sValue;
        CExoIni cIni;
        int bReverse = g_pAppManager->GetClientExoApp()->GetOptions()->m_bReverseMouseButtons;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bReverseMouseButtons = !bReverse;
        sValue.Format("%d", g_pAppManager->GetClientExoApp()->GetOptions()->m_bReverseMouseButtons);
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Reverse Mouse Buttons");
    }
}

// FUNCTION: SWKOTOR 0x006DEAE0
void CSWGuiOptionsGraphics::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
            if (m_bReinitPending) {
                CExoIni cIni;
                CExoString sValue;
                if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Disable Vertex Buffer Objects")) {
                    if (sValue.AsINT() == 1) {
                        AurDisableVertexBufferObjects();
                    }
                }
                ReInitAurora(g_nScreenWidth, g_nScreenHeight, g_nScreenDepth, g_pRenderWindow->IsFullScreen(), 1);
                if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Disable Write-Only VBO")) {
                    if (sValue.AsINT() == 1) {
                        AurDisableVertexBufferObjectsWriteOnly();
                    } else {
                        AurEnableVertexBufferObjectsWriteOnly();
                    }
                }
                m_bReinitPending = 0;
            }
            break;
        case 0x2d:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E01C0
void CSWGuiOptionsGraphicsAdvanced::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            if (m_bDiscard) {
                m_bDiscard = 0;
            } else {
                SetNewOptions();
                if (m_bChanged) {
                    CExoIni cIni;
                    CExoString sValue;
                    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Disable Vertex Buffer Objects")) {
                        if (sValue.AsINT() == 1) {
                            AurDisableVertexBufferObjects();
                        }
                    }
                    g_pAppManager->GetClientExoApp()->GetOptions()->SetAntiAliasing((unsigned char)m_nAntiAlias);
                    ReInitAurora(g_nScreenWidth, g_nScreenHeight, g_nScreenDepth, g_pRenderWindow->IsFullScreen(), 1);
                    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Disable Write-Only VBO")) {
                        if (sValue.AsINT() == 1) {
                            AurDisableVertexBufferObjectsWriteOnly();
                        } else {
                            AurEnableVertexBufferObjectsWriteOnly();
                        }
                    }
                    m_bChanged = 0;
                }
            }
            m_bChanged = 0;
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
            m_pParent->SetUpOptions();
            break;
        case 0x2d:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E0CF0
void CSWGuiOptionsResolution::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
            if (m_bReinitPending) {
                CExoIni cIni;
                CExoString sValue;
                if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Disable Vertex Buffer Objects")) {
                    if (sValue.AsINT() == 1) {
                        AurDisableVertexBufferObjects();
                    }
                }
                ReInitAurora(g_nScreenWidth, g_nScreenHeight, g_nScreenDepth, g_pRenderWindow->IsFullScreen(), 1);
                if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Disable Write-Only VBO")) {
                    if (sValue.AsINT() == 1) {
                        AurDisableVertexBufferObjectsWriteOnly();
                    } else {
                        AurEnableVertexBufferObjectsWriteOnly();
                    }
                }
                m_bReinitPending = 0;
            }
            break;
        case 0x27:
            m_pManager->PlayGuiSound(0);
            OnResolutionChosen(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E0F80
void CSWGuiOptionsSoundAdvanced::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x2d:
            m_pManager->PlayGuiSound(0);
            break;
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            if (m_bDiscard) {
                m_bDiscard = 0;
            } else {
                SetNewOptions();
                if (m_bChanged) {
                    g_pExoSound->ReInitialize(m_nEAX, g_pAppManager->GetClientExoApp()->GetOptions()->m_bForceSoftware == 0);
                    CExoIni cIni;
                    CExoString sValue;
                    sValue.Format("%d", g_pExoSound->GetProperty0x100());
                    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "EAX");
                    g_pAppManager->GetClientExoApp()->GetOptions()->SetSoundQuality((unsigned char)g_pExoSound->GetProperty0x100());
                    sValue.Format("%d", g_pAppManager->GetClientExoApp()->GetOptions()->m_bForceSoftware);
                    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Force Software");
                }
            }
            m_bChanged = 0;
            m_pManager->PopModalPanel();
            m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
            break;
        case 0x39:
            m_control_AE8.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_control_AE8.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006DF690
void CSWGuiOptionsResolution::OnResolutionChosen(CSWGuiControl* pControl)
{
    if (pControl && !pControl->m_n4c) {
        return;
    }
    int nMode = m_control_1A4.GetSelectedControl()->m_n58;
    DEVMODEA dm;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettingsA(0, nMode, &dm)) {
        g_nScreenRefreshRate = dm.dmDisplayFrequency;
    }
    if (g_pAppManager->GetClientExoApp()->SetVideoMode(nMode)) {
        CExoString sValue;
        CExoIni cIni;
        sValue.Format("%d", g_nScreenWidth);
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Width");
        sValue.Format("%d", g_nScreenHeight);
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Height");
        sValue.Format("%d", g_nScreenRefreshRate);
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "RefreshRate");
    }
    m_pManager->PopModalPanel();
    m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    m_bReinitPending = 1;
    g_pGuiManager->ReloadToolTipPanel();
}
