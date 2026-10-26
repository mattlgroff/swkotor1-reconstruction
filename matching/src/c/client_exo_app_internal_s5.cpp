// CClientExoAppInternal members, worker C s5.

#include <string.h>
#include "c/client_exo_app.h"

class CResRef {
public:
    char m_resRef[16];

    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const char* s);               // 0x00406290
};

class CExoFile {
public:
    CExoFile(const CExoString& sFile, unsigned short nType, const CExoString& sMode);  // 0x005E68D0
    ~CExoFile();                                    // 0x005E69E0
    int FileOpened();                               // 0x005E6A10

    void* m_pcExoFileInternal;                      // 0x00
};

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                     const CExoString& sKey);       // 0x005E67C0

    void* m_pcExoIniInternal;                       // 0x00
};

struct DEVMODEA {
    char dmDeviceName[32];                          // 0x00
    unsigned short dmSpecVersion;                   // 0x20
    unsigned short dmDriverVersion;                 // 0x22
    unsigned short dmSize;                          // 0x24
    unsigned short dmDriverExtra;                   // 0x26
    unsigned long dmFields;                         // 0x28
    char pad_0x2c[0x68 - 0x2c];
    unsigned long dmBitsPerPel;                     // 0x68
    unsigned long dmPelsWidth;                      // 0x6c
    unsigned long dmPelsHeight;                     // 0x70
    unsigned long dmDisplayFlags;                   // 0x74
    unsigned long dmDisplayFrequency;               // 0x78
    char pad_0x7c[0x9c - 0x7c];
};

extern "C" __declspec(dllimport) int __stdcall EnumDisplaySettingsA(const char* sDevice, unsigned long nMode,
                                                                    void* pDevMode);   // IAT 0x0073D3E4

// Display-mode table helpers of the renderer (descriptive names).
unsigned long FindVideoMode(unsigned long nWidth, unsigned long nHeight, unsigned char nDepth,
                            unsigned long nRefresh);        // 0x0070D220
unsigned long FindNearestVideoMode(unsigned long nWidth, unsigned long nHeight, unsigned char nDepth,
                                   unsigned long nRefresh); // 0x0070D0A0

extern int g_bMovieVideoMode;                       // 0x007A3C54 (descriptive)
extern unsigned long g_nScreenWidth;                // 0x0078D1D4
extern unsigned long g_nScreenHeight;               // 0x0078D1D8
extern unsigned char g_nScreenDepth;                // 0x0078D1E0
extern unsigned long g_nScreenRefreshRate;          // 0x0078D1DC

struct RECT {
    long left;
    long top;
    long right;
    long bottom;
};

extern "C" __declspec(dllimport) void* __stdcall GetDesktopWindow();                   // IAT 0x0073D3BC
extern "C" __declspec(dllimport) int __stdcall GetWindowRect(void* hWnd, RECT* pRect);  // IAT 0x0073D3C0

class CRender {
public:
    virtual void v00();
    virtual int GetFullScreen();                    // 0x04 (descriptive)
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void ReleaseDevice();                   // 0x18 (descriptive)
};

extern CRender* g_pRender;                          // 0x007A39F8
extern int g_nActivateState;                        // 0x007A3A2C
extern int g_bWindowed;                             // 0x007A3A30

int ReInitAurora(unsigned long nWidth, unsigned long nHeight, unsigned long nDepth, int bFullScreen,
                 int b);                            // 0x00403800

class CSWGuiMainMenu : public CSWGuiPanel {
public:
    void ResetLayout(CSWGuiManager* pManager);      // 0x0067ACE0 (descriptive)
};

class CSWSCreature {
public:
    void SetActivity(int nActivity, int bOn, int bUpdate);   // 0x004F00A0
};

class CSWSClient {
public:
    void* m_vtable;                                 // 0x00
    unsigned long m_nPlayerId;                      // 0x04

    unsigned long GetPlayerId() { return m_nPlayerId; }
};

class CSWSDialog {
public:
    unsigned long GetDialogModel(int nIndex, int* pbLocal);   // 0x005A0240
    int SendDialogModels(CGameObject* pObject, unsigned long nPlayerId);   // 0x005A0080

    char pad_0x0[0x48];
    int m_nDialogModels;                            // 0x48
    char pad_0x4c[0x80 - 0x4c];
    unsigned long m_oidOwner;                       // 0x80
};

class CSWSObject : public CGameObject {
};

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

extern Vector g_vAbilityColorAvailable;             // 0x007A23D8
extern Vector g_vAbilityColorLocked;                // 0x007A23E4
extern Vector g_vFloatyTextColorWhite;              // 0x007A23A8 (descriptive)
extern Vector g_vKeyMapChangedColor;                // 0x007A23FC
extern Vector g_vFloatyTextColor23F0;               // 0x007A23F0 (descriptive)

class CGuiInGame {
public:
    void ResetInterfaceForSize();                   // 0x0062F5F0
    void AddFloatyText(unsigned long oid, const CExoString& sText, const Vector& vColor, float fScale);   // 0x0062B080
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bUnknown);            // 0x00413CC0
    virtual ~C2DA();                                // vtable slot 0
    int Load2DArray();                              // 0x004143B0
    void Unload2DArray();                           // 0x004139E0
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413270
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);             // 0x00413430
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);                 // 0x00413660

    char pad_0x4[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x54 - 0x28];
};

class CTwoDimArrays {
public:
    char pad_0x0[0x128];
    C2DA* m_pTexPacks;                              // 0x128
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

void AurTextureResetAll();                          // 0x00421D50
void AurTextureSetDynamicMemoryRatio(float fRatio); // 0x0041E9B0
int AurTextureSetMem(int nMem);                     // 0x00421D90

class CAuroraInterface {
public:
    static int SetTexturePack(unsigned char nType, const CExoString& sPack);   // 0x0070D800
};

// FUNCTION: SWKOTOR 0x005F0AD0
void CClientExoAppInternal::LoadItemDropSoundList()
{
    if (m_pItemDropSounds) {
        return;
    }
    C2DA* pTable = new C2DA("inventorysnds", 0);
    if (!pTable) {
        return;
    }
    if (!pTable->Load2DArray()) {
        return;
    }
    CExoString sSound;
    m_pItemDropSounds = new CResRef[pTable->m_nNumRows];
    for (unsigned char i = 0; i < pTable->m_nNumRows; i++) {
        if (pTable->GetCExoStringEntry(i, "inventorysound", &sSound)) {
            m_pItemDropSounds[i] = sSound.CStr();
        }
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005F5910
void CClientExoAppInternal::setUpEventPlayBack(const CExoString& sFile)
{
    m_bCaptureEventPlayBack = 1;
    if (m_bCaptureEvents == 1) {
        m_bCaptureEventPlayBack = 0;
        return;
    }
    if (m_pCaptureFile) {
        delete m_pCaptureFile;
        m_pCaptureFile = 0;
        while (!m_lstPlayBackEvents.IsEmpty()) {
            m_lstPlayBackEvents.RemoveTail();
        }
    }
    CExoString sPath;
    sPath.Format("HD0:%s", sFile.CStr());
    m_pCaptureFile = new CExoFile(sPath, 10, "rb");
    if (m_pCaptureFile) {
        if (m_pCaptureFile->FileOpened()) {
            LoadPlayBackEvents();
            g_pAppManager->GetClientExoApp()->GetWorldTimer()->SetFixedSnapshotRate(1, m_nUnknown94);
        } else {
            delete m_pCaptureFile;
            m_pCaptureFile = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x005F5AB0
void CClientExoAppInternal::ReadAndSetVideoMode(int bForMovie, int bFullScreen)
{
    unsigned long nWidth = 0;
    unsigned long nHeight = 0;
    unsigned long nRefresh = 0;
    unsigned char nDepth = 0;
    CExoString sUnused;
    CExoIni cIni;
    int bRead = ReadVideoModeSettings(&nWidth, &nHeight, &nDepth, &nRefresh);
    m_pClientOptions->SetFullScreenEnabled(bFullScreen);
    if (bForMovie) {
        nWidth = 640;
        nHeight = 480;
        nDepth = 32;
        g_bMovieVideoMode = 1;
    }
    unsigned long nMode = FindVideoMode(nWidth, nHeight, nDepth, nRefresh);
    if ((!bRead && !bForMovie) || nMode == 0xFFFFFFFF) {
        unsigned long nNearest = FindNearestVideoMode(800, 600, 32, 60);
        DEVMODEA dm;
        memset(&dm, 0, sizeof(dm));
        dm.dmSize = sizeof(dm);
        if (!EnumDisplaySettingsA(0, nNearest, &dm)) {
            nMode = 0xFFFFFFFF;
        } else if (dm.dmBitsPerPel != 32) {
            nMode = 0xFFFFFFFF;
        } else {
            nMode = nNearest;
        }
    }
    if (bFullScreen) {
        SetVideoMode(nMode, 2);
    } else {
        SetVideoMode(nMode, 1);
    }
    if (bForMovie) {
        ReadVideoModeSettings(&nWidth, &nHeight, &nDepth, &nRefresh);
        g_nScreenWidth = nWidth;
        g_nScreenHeight = nHeight;
        g_nScreenDepth = nDepth;
    }
}

// NONMATCH (79 diff insns): the original keeps `this` in ebx and the
// full-screen flag in a stack slot, reloads the screen size only in the
// small-desktop branch and folds three of the five ReInitAurora calls into
// one call with the flag in eax; ours caches the size and merges all five.
// FUNCTION: SWKOTOR 0x005F1830
int CClientExoAppInternal::SetVideoMode(unsigned long nMode, unsigned char nFlags)
{
    if (nMode == m_nVideoMode) {
        return 1;
    }
    DEVMODEA dm;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);
    if (!EnumDisplaySettingsA(0, nMode, &dm)) {
        return 0;
    }
    g_nScreenDepth = (unsigned char)dm.dmBitsPerPel;
    g_nScreenWidth = dm.dmPelsWidth;
    g_nScreenHeight = dm.dmPelsHeight;
    int bFullScreen;
    if (nFlags == 1) {
        bFullScreen = 0;
    } else if (nFlags == 2) {
        bFullScreen = 1;
    } else {
        bFullScreen = g_pRender->GetFullScreen();
    }
    unsigned long nWidth = g_nScreenWidth;
    unsigned long nHeight = g_nScreenHeight;
    if (g_nActivateState == 1) {
        g_pRender->ReleaseDevice();
    }
    g_nScreenWidth = nWidth;
    g_nScreenHeight = nHeight;
    RECT rcDesktop;
    GetWindowRect(GetDesktopWindow(), &rcDesktop);
    nWidth = g_nScreenWidth;
    nHeight = g_nScreenHeight;
    if (!g_bWindowed) {
        ReInitAurora(nWidth, nHeight, g_nScreenDepth, 1, 1);
    } else if (bFullScreen) {
        ReInitAurora(nWidth, nHeight, g_nScreenDepth, bFullScreen, 1);
    } else if (rcDesktop.bottom <= 479 || rcDesktop.right <= 639) {
        ReInitAurora(g_nScreenWidth, g_nScreenHeight, g_nScreenDepth, 1, 1);
    } else if (rcDesktop.bottom > (long)nHeight && rcDesktop.right > (long)nWidth) {
        ReInitAurora(nWidth, nHeight, g_nScreenDepth, bFullScreen, 1);
    } else {
        ReInitAurora(nWidth, nHeight, g_nScreenDepth, 1, 1);
    }
    m_nVideoMode = nMode;
    if (m_pGuiManager) {
        m_pGuiManager->SetSize(g_nScreenWidth, g_nScreenHeight);
        if (m_pMainMenu && m_pGuiManager->PanelExists(m_pMainMenu)) {
            ((CSWGuiMainMenu*)m_pMainMenu)->ResetLayout(m_pGuiManager);
        }
        if (m_pLoadScreen) {
            delete m_pLoadScreen;
            m_pLoadScreen = new CSWGuiLoadScreen(m_pGuiManager);
        }
        if (m_pGuiInGame) {
            m_pGuiInGame->ResetInterfaceForSize();
        }
    }
    return 1;
}


// FUNCTION: SWKOTOR 0x005F14A0
int CClientExoAppInternal::SetTexturePack(unsigned char nPack, int bUnused)
{
    CExoString sTexture;
    CExoString sUnused1;
    CExoString sUnused2;
    CExoString sGui;
    int bTemporary = 0;
    float fRatio = 0.0f;
    int nMem = 0;
    if (nPack == m_nTexturePack) {
        return 1;
    }
    m_nTexturePack = nPack;
    C2DA* pTable;
    if (g_pRules && g_pRules->m_p2DArrays) {
        pTable = g_pRules->m_p2DArrays->m_pTexPacks;
    } else {
        bTemporary = 1;
        pTable = new C2DA("texpacks", 1);
        if (!pTable) {
            return 0;
        }
        if (!pTable->Load2DArray()) {
            delete pTable;
            return 0;
        }
    }
    if (!pTable) {
        return 0;
    }
    int nRows = pTable->m_nNumRows;
    if (nRows == 0 || nPack >= nRows) {
        pTable->Unload2DArray();
        delete pTable;
        return 0;
    }
    pTable->GetCExoStringEntry(nPack, "Texture", &sTexture);
    pTable->GetCExoStringEntry(nPack, "Gui", &sGui);
    pTable->GetFLOATEntry(nPack, "DynMemRatio", &fRatio);
    pTable->GetINTEntry(nPack, "Mem", &nMem);
    AurTextureResetAll();
    CAuroraInterface::SetTexturePack(1, CExoString("TEXTUREPACKS:") + sTexture);
    CAuroraInterface::SetTexturePack(2, CExoString("TEXTUREPACKS:") + sGui);
    AurTextureSetDynamicMemoryRatio(fRatio);
    AurTextureSetMem(nMem);
    if (bTemporary) {
        pTable->Unload2DArray();
        delete pTable;
    }
    m_nTexturePack = nPack;
    return 1;
}

// NONMATCH (4 real diff insns): the two AsSWSObject vtable loads use
// eax then edx in the original, edx then eax here; separate ifs, an else
// form and locals for the player id/client do not swap them.
// FUNCTION: SWKOTOR 0x005F3070
int CClientExoAppInternal::GetCanSendDialog(unsigned long oidSpeaker, unsigned long oidListener, CSWSDialog* pDialog)
{
    CGameObject* pObject = g_pAppManager->m_pServerExoApp->GetGameObject(oidListener);
    if (pObject && pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->SetActivity(1, 0, 1);
    }
    pObject = g_pAppManager->m_pServerExoApp->GetGameObject(oidSpeaker);
    if (pObject && pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->SetActivity(1, 0, 1);
    }
    if (g_pAppManager->m_pServerExoApp && g_pAppManager->m_pServerExoApp->GetPauseState(2)) {
        return 0;
    }
    if (m_bAutoPaused) {
        return 0;
    }
    if (m_bDialogSent) {
        return 1;
    }
    if (!m_bCanSendDialog || !m_nCanSendDialogTime) {
        return 0;
    }
    unsigned long nNow = (unsigned long)((__int64)g_pAppManager->m_pClientExoApp->m_pcInternal->m_pPauseTimer->GetSnapshotTime() / 1000);
    if (nNow - m_nCanSendDialogTime <= 1000) {
        return 0;
    }
    m_nCanSendDialogTime = nNow - 2000;
    if (oidSpeaker != 0x7F000000) {
        pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
            g_pAppManager->m_pClientExoApp->ServerToClientObjectId(oidSpeaker));
        if (!pObject || !pObject->AsSWCObject()) {
            return 0;
        }
    }
    if (oidListener != 0x7F000000) {
        pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
            g_pAppManager->m_pClientExoApp->ServerToClientObjectId(oidListener));
        if (!pObject || !pObject->AsSWCObject()) {
            return 0;
        }
    }
    if (!m_bDialogModelsSent) {
        for (int i = 0; i < pDialog->m_nDialogModels; i++) {
            int bLocal;
            unsigned long oidModel = pDialog->GetDialogModel(i, &bLocal);
            if (!bLocal) {
                pObject = g_pAppManager->m_pClientExoApp->GetGameObject(oidModel);
                if (!pObject || !pObject->AsSWCObject()) {
                    return 0;
                }
            }
        }
        CSWSClient* pClient = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(
            g_pAppManager->m_pServerExoApp->GetPlayerCreatureID());
        pObject = g_pAppManager->GetServerExoApp()->GetGameObject(pDialog->m_oidOwner);
        if (!pObject || !pObject->AsSWSObject()) {
            return 0;
        }
        CSWSObject* pOwner = pObject->AsSWSObject();
        pDialog->SendDialogModels(pOwner, pClient->m_nPlayerId);
        if (!m_bDialogModelsSent) {
            return 0;
        }
    }
    m_bDialogSent = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x006027C0
void CClientExoAppInternal::AddFloatyText(unsigned long oid, unsigned char nType, int nValue)
{
    CClientUnknown700* pOptions = g_pAppManager->m_pClientExoApp->GetUnknown700();
    if (pOptions && !(pOptions->m_nFeedbackFlags & 0x10)) {
        return;
    }
    Vector vColor(0.0f, 0.0f, 0.0f);
    CExoString sText;
    float fScale;
    switch (nType) {
    case 1:
        sText = nValue;
        vColor = g_vAbilityColorLocked;
        fScale = 1.5f;
        break;
    case 0:
        sText = nValue;
        vColor = g_vAbilityColorAvailable;
        fScale = 1.5f;
        break;
    case 2:
        sText = GetGUIString(0x55d);
        vColor = g_vFloatyTextColorWhite;
        fScale = 1.5f;
        break;
    case 3:
        sText.Format("%s %d", GetGUIString(0x9697).CStr(), nValue);
        vColor = g_vKeyMapChangedColor;
        fScale = 3.0f;
        break;
    case 4:
        sText.Format("%s %d", GetGUIString(0x7d9a).CStr(), nValue);
        vColor = g_vFloatyTextColor23F0;
        fScale = 3.0f;
        break;
    case 5:
        sText = GetGUIString(0x56f);
        vColor = g_vFloatyTextColorWhite;
        fScale = 1.5f;
        break;
    case 6:
        sText = GetGUIString(0xa5a3);
        vColor = g_vFloatyTextColorWhite;
        fScale = 1.5f;
        break;
    default:
        return;
    }
    m_pGuiInGame->AddFloatyText(oid, sText, vColor, fScale);
}
