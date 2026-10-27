// CSWCTrigger target-menu actions (nwctrigger.o): the mine disable /
// recover entries and their callbacks. The action record layout follows
// matching/src/c/sw_c_creature_action_menu.cpp; the callback is a pointer
// to member of a class still incomplete at the record's declaration
// (general 16-byte representation).

class CExoString {
public:
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
    CResRef operator=(const char* s);               // 0x00406290
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

class CSWCMessage {
public:
    void SendPlayerToServerInput_UseSkill(int nSkill, int nSubSkill, unsigned long oidTarget,
                                          const Vector& vTarget);  // 0x00677B10
};

class CClientExoApp {
public:
    CExoString GetGUIString(unsigned long nId);     // 0x005EDEB0
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
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

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();

    unsigned long m_idSelf;                         // 0x04
};

class CSWCObject : public CGameObject {
public:
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134();
    virtual int IsHostile();                        // 0x138
};

class CSWCCreature : public CSWCObject {
public:
    void PerformSkill(int nSkill, int nSubSkill, unsigned long oidTarget);   // 0x0060EE00
    char pad_0x8[0x2f8 - 0x8];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

class CSWCTrigger : public CSWCObject {
public:
    void ActionDisableMine(int nId, CSWCCreature* pCreature);     // 0x00691900 (descriptive)
    void ActionRecoverMine(int nId, CSWCCreature* pCreature);     // 0x00691950 (descriptive)
    void GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);
};

typedef void (CSWCObject::*CSWCActionCallback)(int, CSWCCreature*);

// Not in the function table.
// FUNCTION: SWKOTOR 0x00691900
void CSWCTrigger::ActionDisableMine(int nId, CSWCCreature* pCreature)
{
    CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
    if (pMessage) {
        Vector vTarget;
        vTarget.x = 0.0f;
        vTarget.y = 0.0f;
        vTarget.z = 0.0f;
        pMessage->SendPlayerToServerInput_UseSkill(1, 0, m_idSelf, vTarget);
    }
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x00691950
void CSWCTrigger::ActionRecoverMine(int nId, CSWCCreature* pCreature)
{
    pCreature->PerformSkill(1, 0x65, m_idSelf);
}

// FUNCTION: SWKOTOR 0x00691F00
void CSWCTrigger::GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions)
{
    int nIndex = lstActions.num;
    switch (nMenu) {
    case 0:
        if (pSource->m_pStats->GetCanUseSkill(1) && IsHostile()) {
            lstActions.SetSize(nIndex + 1);
            lstActions[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x172);
            lstActions[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCTrigger::ActionDisableMine;
            lstActions[nIndex].m_oidTarget = m_idSelf;
            lstActions[nIndex].m_nActionId = 0x3f4;
            lstActions[nIndex].m_resIcon = "i_disablemine";
        }
        break;
    case 1:
        if (pSource->m_pStats->GetCanUseSkill(1)) {
            lstActions.SetSize(nIndex + 1);
            lstActions[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x5fb);
            lstActions[nIndex].m_pfnAction = (CSWCActionCallback)&CSWCTrigger::ActionRecoverMine;
            lstActions[nIndex].m_oidTarget = m_idSelf;
            lstActions[nIndex].m_nActionId = 0x402;
            lstActions[nIndex].m_resIcon = "i_recovermine";
        }
        break;
    }
}
