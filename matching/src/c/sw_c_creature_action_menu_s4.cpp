// CSWCCreature action-menu callbacks (worker c, s4). Both are real starts
// missing from functions.csv; the GUI action list stores them as pointers to
// members (see AddFeatAction / AddHostileItem in sw_c_creature_action_menu.cpp,
// which already names them).

struct CSWFeat;
struct CSWBaseItemData {
    char pad_0x0[0xac];
    unsigned char m_nItemClass;                     // 0xac
};

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWSItem {
public:
    CSWBaseItemData* GetBaseItem();                 // 0x005B4790
};

class CSWCMessage {
public:
    int SendPlayerToServerInput_Attack(unsigned long oid, unsigned short nFeat);   // 0x00677A90
};

class CClientExoApp {
public:
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
    int ShowTutorialWindow(int nWindow, unsigned long oidA, unsigned long oidB, int nValue);  // 0x005EDF40
};

class CServerExoApp {
public:
    CSWSItem* GetItemByGameObjectID(unsigned long oid);     // 0x004AE760
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CGameObject;

class CSWCCreature {
public:
    virtual void v00();

    void SetAttackTarget(int b);                    // 0x00610A10
    void PartyMenuUseItem(unsigned long oidItem, CSWCCreature* pTarget);   // 0x0060F590
    void ActionMenuFeat(unsigned long nFeat, CSWCCreature* pCreature);
    void ActionMenuUseItem(unsigned long oidItem, CSWCCreature* pCreature);

    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                         // 0x04
};

#define OBJECT_INVALID 0x7f000000

// FUNCTION: SWKOTOR 0x00617DD0
void CSWCCreature::ActionMenuFeat(unsigned long nFeat, CSWCCreature* pCreature)
{
    if (!g_pRules->GetFeat((unsigned short)nFeat)) {
        return;
    }
    if (g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0, GetId(), OBJECT_INVALID, (unsigned short)nFeat)) {
        return;
    }
    CSWCMessage* pMessage = g_pAppManager->GetClientExoApp()->GetSWCMessage();
    if (pMessage) {
        pMessage->SendPlayerToServerInput_Attack(m_idSelf, (unsigned short)nFeat);
    }
    if (pCreature) {
        pCreature->SetAttackTarget(1);
    }
}

// FUNCTION: SWKOTOR 0x00617E40
void CSWCCreature::ActionMenuUseItem(unsigned long oidItem, CSWCCreature* pCreature)
{
    CSWSItem* pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(oidItem);
    if (pItem && pItem->GetBaseItem()->m_nItemClass == 6) {
        if (g_pAppManager->GetClientExoApp()->ShowTutorialWindow(1, GetId(), pCreature->GetId(), oidItem)) {
            return;
        }
    }
    if (pCreature) {
        pCreature->SetAttackTarget(1);
    }
    PartyMenuUseItem(oidItem, pCreature);
}

// Target classification for the action menus (named GetTargetActionType by
// its caller in sw_c_creature_target_actions.cpp): 0 none, 1 door / usable
// placeable, 2 trigger, 3 other placeable or friendly, 4 hostile creature.

class CSWCObjectT;

class CSWCCreatureT {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
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

struct CSWSPlaceableT {
    char pad_0x0[0x324];
    int m_bUsable;                                  // 0x324
};

class CSWCPlaceableT {
public:
    CSWSPlaceableT* GetServerPlaceable();           // 0x00682E00
};

class CSWCObjectT {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual void* AsSWCDoor();                      // 0x14
    virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreatureT* AsSWCCreature();         // 0x2c
    virtual void v30(); virtual void v34(); virtual void v38();
    virtual void* AsSWCTrigger();                   // 0x3c
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceableT* AsSWCPlaceable();       // 0x4c
};

class CClientExoAppT {
public:
    int IsUnknown378Positive();                     // 0x005EE0F0
};

class CAppManagerT {
public:
    void* m_pUnknown00;
    CClientExoAppT* m_pClientExoApp;                // 0x04
};

extern CAppManagerT* g_pAppManagerT;                // 0x007A39FC (g_pAppManager)

class CSWCCreatureTarget {
public:
    int GetTargetActionType(CSWCObjectT* pTarget);
};

// FUNCTION: SWKOTOR 0x0060FCC0
int CSWCCreatureTarget::GetTargetActionType(CSWCObjectT* pTarget)
{
    int nType = 0;
    if (!pTarget) {
        return 0;
    }
    if (pTarget->AsSWCCreature()) {
        if (pTarget->AsSWCCreature()->IsHostile() || g_pAppManagerT->m_pClientExoApp->IsUnknown378Positive()) {
            return 4;
        }
        return 3;
    } else if (pTarget->AsSWCDoor()) {
        return 1;
    } else if (pTarget->AsSWCPlaceable()) {
        CSWCPlaceableT* pPlaceable = pTarget->AsSWCPlaceable();
        if (pPlaceable) {
            CSWSPlaceableT* pServer = pPlaceable->GetServerPlaceable();
            if (pServer && pServer->m_bUsable) {
                return 1;
            }
        }
        return 3;
    } else if (pTarget->AsSWCTrigger()) {
        return 2;
    }
    return nType;
}
