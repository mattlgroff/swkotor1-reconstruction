// WM_KEYDOWN handling (0x00402240): event capture, the cheat-console toggle
// key (EnableCheats in swkotor.ini) and keys for the command line or the
// GUI focus.
#include "d/exo_key_table.h"

class CExoIni {
public:
    CExoIni();                                      // 0x005e6750
    ~CExoIni();                                     // 0x005e67e0
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                     const CExoString& sEntry);     // 0x005e67c0
    void* m_pIniInternal;
};

class CWorldTimer {
public:
    unsigned __int64 GetWorldTime();                // 0x004adc70
};

class CExoFileWriter {
public:
    int Write(const CExoString& sText);             // 0x005e6a70 (CExoFile::Write)
};

class CClientExoApp {
public:
    CExoFileWriter* GetCaptureFile();               // 0x005ed460
    int GetCaptureEvents();                         // 0x005ed470
    unsigned long GetClientLanguage();              // 0x005ed4c0
    CWorldTimer* GetJournalClock();                 // 0x005ed4f0
    void TurnOffKeyboardModal();                    // 0x005eda20
};

class CAppManager {
public:
    void* m_pLargeTempBuffer;                       // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

class CExoInput {
public:
    void ClearEvents();                             // 0x005df4e0
    void KeyboardModeOn();                          // 0x005df520
    void KeyboardModeOff();                         // 0x005df530
    int GetKeyboardMode();                          // 0x005ed700 (folded getter)
};

class CSWGuiManager {
public:
    int SetFocusControl(int nKey);                  // 0x0040b2a0
};

extern CAppManager* g_pAppManager;                  // 0x007a39fc
extern CExoInput* g_pExoInput;                      // 0x007a39e4
extern CSWGuiManager* g_pGuiManager;                // 0x007a39f4
extern int g_bShowFrameInfo;                        // 0x007a3a40
extern int g_bFrameInfoTextInput;                   // 0x007a3cb4
extern int g_bSwapMouseButtons;                     // 0x008338f0

void __cdecl HandleFrameInfoKey(unsigned short nKey); // 0x00401870

inline CClientExoApp* GetClientExoApp() { return g_pAppManager->m_pClientExoApp; }

// FUNCTION: SWKOTOR 0x00402240
int __cdecl HandleKeyDown(unsigned short nKey)
{
    if (GetClientExoApp()->GetCaptureEvents() && GetClientExoApp()->GetCaptureFile()) {
        CExoString sEvent;
        sEvent.Format("%d %c %d\n",
                      (unsigned long)(GetClientExoApp()->GetJournalClock()->GetWorldTime() / 1000),
                      'C', nKey);
        GetClientExoApp()->GetCaptureFile()->Write(sEvent);
    }
    unsigned short nConsoleKey;
    switch (GetClientExoApp()->GetClientLanguage()) {
    case 2:
        nConsoleKey = 0xb0;
        break;
    case 5:
        nConsoleKey = 3;
        break;
    default:
        nConsoleKey = 0x60;
        break;
    }
    if (nKey == nConsoleKey) {
        if (nConsoleKey == 3) {
            g_bSwapMouseButtons = 0;
        }
        CExoIni cIni;
        CExoString sValue;
        cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Game Options", "EnableCheats");
        if (sValue.AsINT() == 1) {
            if (g_bShowFrameInfo) {
                if (g_bFrameInfoTextInput) {
                    g_pExoInput->KeyboardModeOff();
                }
                g_bShowFrameInfo = 0;
            } else {
                g_bFrameInfoTextInput = !g_pExoInput->GetKeyboardMode();
                g_pExoInput->KeyboardModeOn();
                g_pExoInput->ClearEvents();
                GetClientExoApp()->TurnOffKeyboardModal();
                g_bShowFrameInfo = 1;
            }
        }
        return 1;
    }
    if (g_pExoInput->GetKeyboardMode()) {
        if (g_bShowFrameInfo) {
            HandleFrameInfoKey(nKey);
            return 1;
        }
        if (g_pGuiManager && g_pGuiManager->SetFocusControl(nKey)) {
            return 1;
        }
    }
    return 0;
}
