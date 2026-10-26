// CClientExoAppInternal::HandleMouseOver (clientinput.o). SetMouseCursor
// (0x0061F980, matched in c/client_cursor_state.cpp) is defined here again
// without an annotation so cl can inline it the way the original TU did.
struct Vector { float x,y,z; };
class CExoInput {
public:
    int SetMouseCursor(int nCursor,int nOption,int bAlternate); // 0x005DF5C0
    int GetLastSetMouseDownState(); // 0x005DF730
};
extern CExoInput* g_pExoInput; // 0x007A39E4
class CSWGuiControl;
class CSWGuiManager {
public:
    char pad_0x0[8]; CSWGuiControl* m_pMouseOverControl;
    void SetSoftwareMouseCursor(int nCursor,int nOption,int bAlternate); // 0x0040A270
};
extern CSWGuiManager* g_pGuiManager; // 0x007A39F4
extern int g_nMouseDownState; // 0x008338F4
class CClientExoApp { public: unsigned long GetPlayerCreatureId(); /* 0x005ED550 */ };
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager; // 0x007A39FC
class CClientOptions { public: char pad_0x0[0x6d]; unsigned char m_nCameraMode; };
class CSWGuiMainInterface { public: char pad_0x0[0x64]; unsigned long m_oidTarget; };
class CGuiInGame {
public:
    char pad_0x0[0x30]; int m_bInputActive; char pad_0x34[0x5c]; CSWGuiMainInterface* m_pMainInterface;
    char pad_0x94[0x20]; int m_nGlobalDialogState;
};
struct CDefaultAction { char pad_0x0[8]; int m_nAction; };
int CompareObjectIds(const void* a,const void* b); // 0x0061FA90
class CObjectIdList {
public:
    int Find(unsigned long oid,int (*pfnCompare)(const void*,const void*)); // 0x005F4FB0
};
class CClientExoAppInternal {
public:
    void* m_p0; CClientOptions* m_pClientOptions;
    char pad_0x8[0x38]; CGuiInGame* m_pGuiInGame;
    char pad_0x44[0x230]; CSWGuiManager* m_pGuiManager;
    char pad_0x278[0x30]; CObjectIdList m_lstSelectable; char pad_0x2a9[0xb]; unsigned long m_oidMouseTarget;
    char pad_0x2b8[0x1ec]; unsigned long m_oidHilite; Vector m_vMouseOverPosition;
    char pad_0x4b4[0x14]; CDefaultAction* m_pDefaultActions; int m_nDefaultActions;
    void SetMouseCursor(int nCursor,int nOption,int bAlternate);
    void GetDefaultActions(); // 0x00620620
    int GetDefaultActionCursor(int nAction,int bMultiple); // 0x0061FAA0
    void HandleMouseOver(unsigned long oid,const Vector& vPosition,int bDefault);
};

void CClientExoAppInternal::SetMouseCursor(int nCursor,int nOption,int bAlternate)
{
    if (g_pExoInput != 0) nCursor = g_pExoInput->SetMouseCursor(nCursor,nOption,bAlternate);
    if (m_pGuiManager != 0) m_pGuiManager->SetSoftwareMouseCursor(nCursor,nOption,bAlternate);
}

// FUNCTION: SWKOTOR 0x006222F0
void CClientExoAppInternal::HandleMouseOver(unsigned long oid,const Vector& vPosition,int bDefault)
{
    m_vMouseOverPosition = vPosition;
    m_oidHilite = oid;
    if (m_pGuiInGame && (m_pGuiInGame->m_bInputActive || m_pGuiInGame->m_nGlobalDialogState == 1 || m_pClientOptions->m_nCameraMode == 5)) {
        SetMouseCursor(1,0,0);
        return;
    }
    CSWGuiControl* pOver = g_pGuiManager->m_pMouseOverControl;
    if (!(bDefault || (oid != 0x7f000000 && m_oidMouseTarget != 0x7f000000 &&
                       oid != g_pAppManager->GetClientExoApp()->GetPlayerCreatureId() && !pOver &&
                       m_oidMouseTarget == m_oidHilite))) {
        if (m_oidHilite != 0x7f000000 && m_lstSelectable.Find(m_oidHilite,CompareObjectIds) != -1 && !pOver) {
            SetMouseCursor(0x2d,0,0);
            return;
        }
        SetMouseCursor(1,0,g_pExoInput->GetLastSetMouseDownState());
    } else {
        int nCursor = 1;
        CDefaultAction* pFirst = 0;
        GetDefaultActions();
        int bMultiple = 0;
        if (m_nDefaultActions > 0) {
            pFirst = m_pDefaultActions;
            bMultiple = m_nDefaultActions > 1;
        }
        int nDown = g_nMouseDownState;
        if (m_oidHilite != 0x7f000000 && m_oidHilite != m_pGuiInGame->m_pMainInterface->m_oidTarget) {
            SetMouseCursor(0x2d,0,0);
            return;
        }
        if (pFirst) nCursor = GetDefaultActionCursor(pFirst->m_nAction,bMultiple);
        SetMouseCursor(nCursor,0,nDown);
    }
}
