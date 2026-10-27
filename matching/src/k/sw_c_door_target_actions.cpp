// CSWCDoor target-menu actions (nwcdoor.o): open and security entries.
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

class CSWSDoor {
public:
    char pad_0x0[0x2d8];
    int m_bKeyRequired;                             // 0x2d8
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual CSWSDoor* AsSWSDoor();                  // 0x18

    unsigned long m_idSelf;                         // 0x04
};

class CSWCObject : public CGameObject {
public:
    CGameObject* GetServerObject();                 // 0x0063D4B0
    unsigned short GetCurrentAnimation();           // 0x0063C060

    char pad_0x8[0x20 - 0x8];
    CSWCArea* m_pArea;                              // 0x20
    char pad_0x24[0xfc - 0x24];
};

class CSWCCreature : public CSWCObject {
public:
    char pad_0xfc[0x2f8 - 0xfc];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

class CSWCDoor : public CSWCObject {
public:
    void ActionMenuLock(int nId, CSWCCreature* pCreature);  // 0x00683E50
    void ActionMenuOpen(int nId, CSWCCreature* pCreature);  // 0x00683E90
    void GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);

    char pad_0xfc[0x104 - 0xfc];
    int m_n104;                                     // 0x104
    int m_n108;                                     // 0x108
    char pad_0x10c[0x138 - 0x10c];
    int m_n138;                                     // 0x138
};

typedef void (CSWCObject::*CSWCActionCallback)(int, CSWCCreature*);

// FUNCTION: SWKOTOR 0x00684410
void CSWCDoor::GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions)
{
    CSWSDoor* pServerDoor = GetServerObject() ? GetServerObject()->AsSWSDoor() : 0;
    int bRestricted = m_pArea->GetServerArea()->m_nRestrictMode;
    int nIndex = lstActions.num;
    CExoString sUnused;
    switch (nMenu) {
    case 0:
        if (GetCurrentAnimation() == 10050 || GetCurrentAnimation() == 10051) {
            break;
        }
        if (m_n138 || m_n104 || bRestricted || !m_n108) {
            break;
        }
        lstActions.SetSize(nIndex + 1);
        lstActions.element[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x170);
        lstActions.element[nIndex].m_nActionId = 0x3f5;
        lstActions.element[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCDoor::ActionMenuOpen;
        lstActions.element[nIndex].m_oidTarget = m_idSelf;
        lstActions.element[nIndex].m_resIcon = "i_attack";
        break;
    case 1:
        if (m_n108 && pServerDoor && !pServerDoor->m_bKeyRequired &&
            pSource->m_pStats->GetCanUseSkill(6)) {
            lstActions.SetSize(nIndex + 1);
            lstActions.element[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x149);
            lstActions.element[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCDoor::ActionMenuLock;
            lstActions.element[nIndex].m_oidTarget = m_idSelf;
            lstActions.element[nIndex].m_nActionId = 0x3f3;
            lstActions.element[nIndex].m_resIcon = g_pRules->m_lstSkills[6].m_sIcon;
        }
        break;
    }
}
