// CSWCPlaceable target-menu actions (nwcplaceable.o): use and security entries.
// The action record layout follows matching/src/c/sw_c_creature_action_menu.cpp;
// the callback is a pointer to member of a class still incomplete at the
// record's declaration (general 16-byte representation).

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
    CResRef operator=(const char* s);               // 0x00406290
    CResRef operator=(const CExoString& s);         // 0x004061F0
};

struct Vector {
    float x, y, z;
};

class CSWCCreature;
class CSWCObject;

#pragma pack(push, 4)
class CSWGuiInterfaceAction {
public:
    CExoString m_sName;                             // 0x00
    unsigned long m_nActionId;                      // 0x08
    void (CSWCObject::*m_pfnAction)(int nId, CSWCCreature* pCreature);  // 0x0c
    unsigned long m_oidTarget;                      // 0x1c
    CResRef m_resIcon;                              // 0x20
    unsigned long m_nFlags;                         // 0x30
    unsigned long m_n34;                            // 0x34
};
#pragma pack(pop)

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    T& operator[](int i) { return element[i]; }
    void SetSize(int n);                            // <CSWGuiInterfaceAction>: 0x004FDDD0
};

typedef CExoArrayList<CSWGuiInterfaceAction> CSWGuiActionList;

class CSWSArea {
public:
    char pad_0x0[0x2b0];
    int m_nRestrictMode;                            // 0x2b0
};

class CSWCArea {
public:
    CSWSArea* GetServerArea();                      // 0x00605640
};

class CSWSkill {
public:
    int m_nNameStrRef;                              // 0x00
    char pad_0x4[0xc];
    CExoString m_sIcon;                             // 0x10
    char pad_0x18[0x8];
};

class CSWRules {
public:
    char pad_0x0[0xb4];
    CSWSkill* m_lstSkills;                          // 0xb4
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CClientExoApp {
public:
    CExoString GetGUIString(unsigned long nId);     // 0x005EDEB0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCCreatureStats {
public:
    int GetCanUseSkill(unsigned short nSkill);      // 0x006477E0
};

class CSWSPlaceable {
public:
    char pad_0x0[0x218];
    int m_n218;                                     // 0x218
    char pad_0x21c[0x324 - 0x21c];
    int m_bUseable;                                 // 0x324
};

class CSWSCreature {
public:
    int GetDeadTemp();                              // 0x004EF890
};

class CSWSClient {
public:
    char pad_0x0[0x38];
    unsigned long m_oidPlayer;                      // 0x38
};

class CServerExoApp {
public:
    CSWSClient* GetClientObjectByObjectID(unsigned long oid);  // 0x004AE910
    unsigned long GetPlayerCreatureID();            // 0x004AEA40
};

class CSWCCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44();
    virtual CSWSPlaceable* AsSWSPlaceable();        // 0x48

    unsigned long m_idSelf;                         // 0x04
};

class CClientExoAppMenu : public CClientExoApp {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);  // 0x005EDA50
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
};

class CAppManagerMenu : public CAppManager {
public:
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

class CSWCObject : public CGameObject {
public:
    CGameObject* GetServerObject();                 // 0x0063D4B0

    char pad_0x8[0x20 - 0x8];
    CSWCArea* m_pArea;                              // 0x20
    char pad_0x24[0xfc - 0x24];
};

class CSWCCreature : public CSWCObject {
public:
    CSWSCreature* AsServerCreature();               // 0x0060FB20

    char pad_0xfc[0x2f8 - 0xfc];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    char pad_0x2fc[0x3a4 - 0x2fc];
    int m_n3a4;                                     // 0x3a4
};

class CSWCPlaceable : public CSWCObject {
public:
    void ActionMenuOpen(int nId, CSWCCreature* pCreature);  // 0x006826A0
    void ActionMenuLock(int nId, CSWCCreature* pCreature);  // 0x00682DD0
    CSWSPlaceable* GetServerPlaceable();            // 0x00682E00
    void GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);

    char pad_0xfc[0x110 - 0xfc];
    int m_n110;                                     // 0x110
    char pad_0x114[0x118 - 0x114];
    int m_n118;                                     // 0x118
};

typedef void (CSWCObject::*CSWCActionCallback)(int, CSWCCreature*);

// FUNCTION: SWKOTOR 0x006837D0
void CSWCPlaceable::GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions)
{
    CSWSPlaceable* pServer = GetServerObject() ? GetServerObject()->AsSWSPlaceable() : 0;
    int bRestricted = m_pArea->GetServerArea()->m_nRestrictMode;
    int nIndex = lstActions.num;
    CExoString sUnused;
    switch (nMenu) {
    case 0:
        if (pServer && pServer->m_bUseable) {
            if (m_n110 || bRestricted || !m_n118)
                break;
            lstActions.SetSize(nIndex + 1);
            lstActions.element[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x170);
            lstActions.element[nIndex].m_nActionId = 0x3f5;
            lstActions.element[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCPlaceable::ActionMenuOpen;
            unsigned long oidSelf = m_idSelf;
            lstActions.element[nIndex].m_oidTarget = oidSelf;
            lstActions.element[nIndex].m_resIcon = "i_attack";
            unsigned long oidPlayer = ((CClientExoAppMenu*)g_pAppManager->m_pClientExoApp)->ServerToClientObjectId(
                ((CAppManagerMenu*)g_pAppManager)->m_pServerExoApp->GetClientObjectByObjectID(
                    ((CAppManagerMenu*)g_pAppManager)->m_pServerExoApp->GetPlayerCreatureID())->m_oidPlayer);
            CGameObject* pObject = ((CClientExoAppMenu*)g_pAppManager->m_pClientExoApp)->GetGameObject(oidPlayer);
            if (!pObject || !pObject->AsSWCCreature())
                break;
            CSWCCreature* pPlayer = pObject->AsSWCCreature();
            if (pSource->m_idSelf == oidPlayer || !pPlayer->m_n3a4)
                break;
            if (!pPlayer->AsServerCreature() || !pPlayer->AsServerCreature()->GetDeadTemp())
                break;
            if (!GetServerPlaceable() || GetServerPlaceable()->m_n218)
                break;
            lstActions.element[nIndex].m_nFlags &= ~1;
            lstActions.element[nIndex].m_nFlags = (lstActions.element[nIndex].m_nFlags & ~0x12) | 0xc;
        } else {
            if (m_n110 || bRestricted || !m_n118)
                break;
            lstActions.SetSize(nIndex + 1);
            lstActions.element[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x170);
            lstActions.element[nIndex].m_nActionId = 0x3f5;
            lstActions.element[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCPlaceable::ActionMenuOpen;
            unsigned long oidSelf = m_idSelf;
            lstActions.element[nIndex].m_oidTarget = oidSelf;
            lstActions.element[nIndex].m_resIcon = "i_attack";
        }
        break;
    case 1:
        if (pServer && pServer->m_bUseable && m_n118 && pSource->m_pStats->GetCanUseSkill(6)) {
            lstActions.SetSize(nIndex + 1);
            lstActions.element[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x149);
            lstActions.element[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCPlaceable::ActionMenuLock;
            unsigned long oidSelf = m_idSelf;
            lstActions.element[nIndex].m_oidTarget = oidSelf;
            lstActions.element[nIndex].m_nActionId = 0x3f3;
            lstActions.element[nIndex].m_resIcon = g_pRules->m_lstSkills[6].m_sIcon;
        }
        break;
    }
}
