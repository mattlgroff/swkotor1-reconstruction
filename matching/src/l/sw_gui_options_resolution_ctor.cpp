// Resolution options panel constructor (layout "optresolution"): lists the
// 32-bit display modes that EnumDisplaySettingsA reports.
#include "l/sw_gui_options_in_game.h"
#include "l/sw_gui_in_game_abilities.h"

extern int g_nScreenRefreshRate; // 0x0078D1DC

// Win32 DEVMODEA (0x9C bytes); only the fields used here are named.
struct DEVMODEA {
    char dmDeviceName[0x20];
    unsigned short dmSpecVersion;
    unsigned short dmDriverVersion;
    unsigned short dmSize;                          // 0x24
    unsigned short dmDriverExtra;
    char pad_0x28[0x68 - 0x28];
    unsigned long dmBitsPerPel;                     // 0x68
    unsigned long dmPelsWidth;                      // 0x6c
    unsigned long dmPelsHeight;                     // 0x70
    unsigned long dmDisplayFlags;                   // 0x74
    unsigned long dmDisplayFrequency;               // 0x78
    char pad_0x7c[0x9c - 0x7c];
};
extern "C" __declspec(dllimport) int __stdcall EnumDisplaySettingsA(const char* lpszDeviceName,
                                                                   unsigned long iModeNum, DEVMODEA* lpDevMode);
extern "C" void* __cdecl memset(void* pDest, int nValue, unsigned int nCount);
#pragma intrinsic(memset)

// FUNCTION: SWKOTOR 0x006E0710
CSWGuiOptionsResolution::CSWGuiOptionsResolution(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optresolution");
    InitControl(&m_control_64, "LBL_RESOLUTION", 1);
    InitControl(&m_control_1A4, "LB_RESOLUTIONS", 1);
    InitControl(&m_control_484, "BTN_OK", 1);
    InitControl(&m_control_648, "BTN_CANCEL", 1);
    UnloadResource();
    CSWGuiButton* pOk = &m_control_484;
    pOk->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsResolution::OnResolutionChosen);
    pOk->m_bNavigable = 0;
    CSWGuiButton* pCancel = &m_control_648;
    pCancel->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pCancel->m_bNavigable = 0;
    m_control_1A4.SetListFlag0(1);
    CSWGuiPanel::SetActiveControl(&m_control_1A4, 0);

    CSWGuiControl* pSelected = 0;
    int nMode = 0;
    CExoArrayListL<CSWGuiControl*> lstButtons;
    CExoString sText;
    DEVMODEA dm;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);
    if (m_control_1A4.m_pProtoItem) {
        CSWGuiButton* pProto = (CSWGuiButton*)m_control_1A4.m_pProtoItem->IsSelectable();
        while (EnumDisplaySettingsA(0, nMode, &dm)) {
            if (dm.dmDisplayFrequency == 0 || dm.dmDisplayFrequency == 1) {
                if (dm.dmBitsPerPel == 32
                    && g_pAppManager->GetClientExoApp()->IsValidResolution(dm.dmPelsWidth, dm.dmPelsHeight)) {
                    CSWGuiButton* pButton = new CSWGuiButton;
                    pButton->CopyStyle(pProto->m_extent, pProto);
                    sText.Format("%d x %d", dm.dmPelsWidth, dm.dmPelsHeight);
                    pButton->m_cTextParams.SetText(sText);
                    pButton->m_n58 = nMode;
                    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsResolution::OnResolutionChosen);
                    lstButtons.AddAlloc(pButton);
                    if (dm.dmPelsWidth == (unsigned long)g_nScreenWidth && dm.dmPelsHeight == (unsigned long)g_nScreenHeight) {
                        pSelected = pButton;
                    }
                }
            } else if (dm.dmBitsPerPel == 32 && dm.dmDisplayFrequency >= 60
                       && g_pAppManager->GetClientExoApp()->IsValidResolution(dm.dmPelsWidth, dm.dmPelsHeight)
                       && (g_pAppManager->GetClientExoApp()->GetAllowHighMonitorFrequency()
                           || dm.dmDisplayFrequency <= 85.0f)) {
                CSWGuiButton* pButton = new CSWGuiButton;
                pButton->CopyStyle(pProto->m_extent, pProto);
                sText.Format("%d x %d @ %d Hz", dm.dmPelsWidth, dm.dmPelsHeight, dm.dmDisplayFrequency);
                pButton->m_cTextParams.SetText(sText);
                pButton->m_n58 = nMode;
                pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsResolution::OnResolutionChosen);
                lstButtons.AddAlloc(pButton);
                if (dm.dmDisplayFrequency == (unsigned long)g_nScreenRefreshRate
                    && dm.dmPelsWidth == (unsigned long)g_nScreenWidth && dm.dmPelsHeight == (unsigned long)g_nScreenHeight) {
                    pSelected = pButton;
                }
            }
            nMode++;
        }
        m_control_1A4.AddControls(lstButtons, 0, 1, 0);
        if (pSelected) {
            m_control_1A4.SetActiveControl(pSelected, 0);
        }
    }
    m_bReinitPending = 0;
}
