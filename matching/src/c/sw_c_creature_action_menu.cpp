// CSWCCreature action menu builders (nwccreature.o, worker C s3). The action
// callback is a pointer to member of a class whose inheritance is unknown at
// its declaration, so it uses the general 16-byte representation.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString(const char* s);                      // 0x005E5A90
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
    CResRef operator=(const char* s);               // 0x00406290
    CResRef operator=(const CResRef& r);            // 0x00406120
};

class CSWCCreature;

#pragma pack(push, 4)
class CSWGuiInterfaceAction {
public:
    ~CSWGuiInterfaceAction();                       // 0x0059EA30

    CExoString m_sName;                             // 0x00
    unsigned long m_nActionId;                      // 0x08
    void (CSWCCreature::*m_pfnAction)(unsigned long oidTarget, CSWCCreature* pCreature);  // 0x0c
    unsigned long m_oidTarget;                      // 0x1c
    CResRef m_resIcon;                              // 0x20
    unsigned long m_n30;                            // 0x30
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

class CGameObject;

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CExoString GetGUIString(unsigned long nId);     // 0x005EDEB0
    unsigned long GetClientLanguage();              // 0x005ED4C0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CExoLocString {
public:
    int GetString(unsigned long nLanguage, CExoString* pString, unsigned char nGender) const;  // 0x005EA130
};

class CSWBaseItemData {
public:
    char pad_0x0[0xac];
    unsigned char m_nCategory;                      // 0xac (6: usable on hostiles)
};

class CSWSItem {
public:
    CSWBaseItemData* GetBaseItem();                 // 0x005B4790
    CResRef GetIcon();                              // 0x005556B0
    unsigned long GetBaseItemId() { return m_nBaseItem; }
    unsigned long GetStackSize() { return m_nStackSize; }

    char pad_0x0[0x14];
    unsigned long m_nBaseItem;                      // 0x14
    char pad_0x18[0x268];
    CExoLocString m_sLocName;                       // 0x280
    char pad_0x281[0xb];
    unsigned long m_nStackSize;                     // 0x28c
};

class CSWSArea {
public:
    char pad_0x0[0x2b0];
    int m_nRestrictMode;                            // 0x2b0
};

class CSWSCreature {
public:
    CSWSArea* GetArea();                            // 0x004CB120
};

class CSWCItem {
public:
    int GetBaseItem() { return m_nBaseItem; }
    char pad_0x0[0xc];
    int m_nBaseItem;                                // 0x0c
};

class CSWBaseItem {
public:
    char pad_0x0[0x1a];
    unsigned char m_nWeaponWield;                   // 0x1a (nonzero: ranged)
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWFeat {
public:
    char pad_0x0[0x8];
    unsigned long m_nNameStrRef;                    // 0x08
    char pad_0xc[0xc];
    CResRef m_resIcon;                              // 0x18
};

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00

    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34

    CSWBaseItemArray* GetBaseItemArray() { return m_pBaseItemArray; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual CSWCItem* AsSWCItem();                  // 0x34

    unsigned long m_idSelf;                         // 0x04
};

class CSWCArea {
public:
    CSWSArea* GetServerArea();                      // 0x00605640
};

class CSWCCreature {
public:
    virtual void v00();

    typedef void (CSWCCreature::*ActionFn)(unsigned long oidTarget, CSWCCreature* pCreature);

    void GetCombatFeatsSkills(CSWGuiActionList& lstActions, CSWCCreature* pTarget);
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    int CheckFeat(int nFeat1, int nFeat2, int nFeat3);      // 0x00616140
    void AddFeatAction(int nFeat, CSWGuiActionList& lstActions, ActionFn pfnAction, CSWCCreature* pTarget);  // 0x00618A30 (descriptive)
    void ActionMenuFeat(unsigned long oidTarget, CSWCCreature* pCreature);     // 0x00617DD0 (descriptive)
    void AddHostileItem(CSWGuiActionList& lstActions, CSWSItem* pItem, CSWCCreature* pTarget);
    void ActionMenuUseItem(unsigned long oidTarget, CSWCCreature* pCreature);  // 0x00617E40 (descriptive)

    void GetCreatureTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);
    void ActionMenuAttack(unsigned long oidTarget, CSWCCreature* pCreature);   // 0x00616800
    CGameObject* GetServerObject();                 // 0x0063D4B0
    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x18];
    CSWCArea* m_pArea;                              // 0x20
};

#define OBJECT_INVALID 0x7f000000

// FUNCTION: SWKOTOR 0x00619950
void CSWCCreature::GetCombatFeatsSkills(CSWGuiActionList& lstActions, CSWCCreature* pTarget)
{
    if (GetServerObject() == 0) {
        return;
    }
    CSWSCreature* pServer = GetServerObject()->AsSWSCreature();
    if (pServer == 0 || pServer->GetArea()->m_nRestrictMode != 0) {
        return;
    }
    unsigned long oidWeapon = GetEquippedItemID(0x10);
    CGameObject* pObject;
    CSWBaseItem* pBaseItem;
    int nFeat;
    if (oidWeapon != OBJECT_INVALID
        && (pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidWeapon)) != 0
        && pObject->AsSWCItem() != 0
        && (pBaseItem = g_pRules->m_pBaseItemArray->GetBaseItem(pObject->AsSWCItem()->GetBaseItem())) != 0
        && pBaseItem->m_nWeaponWield > 0) {
        nFeat = CheckFeat(0x52, 0x12, 0x1d);
        AddFeatAction(nFeat, lstActions, &CSWCCreature::ActionMenuFeat, pTarget);
        nFeat = CheckFeat(0x1a, 0x5c, 0x1e);
        AddFeatAction(nFeat, lstActions, &CSWCCreature::ActionMenuFeat, pTarget);
        nFeat = CheckFeat(0x4d, 0x14, 0x1f);
        AddFeatAction(nFeat, lstActions, &CSWCCreature::ActionMenuFeat, pTarget);
    } else {
        nFeat = CheckFeat(0x51, 0x13, 8);
        AddFeatAction(nFeat, lstActions, &CSWCCreature::ActionMenuFeat, pTarget);
        nFeat = CheckFeat(0x35, 0x5b, 0xb);
        AddFeatAction(nFeat, lstActions, &CSWCCreature::ActionMenuFeat, pTarget);
        nFeat = CheckFeat(0x53, 0x11, 0x1c);
        AddFeatAction(nFeat, lstActions, &CSWCCreature::ActionMenuFeat, pTarget);
    }
}

// Descriptive name: adds the "attack" entry when this creature is targeted.
// FUNCTION: SWKOTOR 0x00619B10
void CSWCCreature::GetCreatureTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions)
{
    if (GetServerObject() == 0) {
        return;
    }
    CSWSCreature* pServer = GetServerObject()->AsSWSCreature();
    if (pServer == 0 || pServer->GetArea()->m_nRestrictMode != 0) {
        return;
    }
    int nIndex = lstActions.num;
    lstActions.SetSize(nIndex + 1);
    lstActions.element[nIndex].m_sName = g_pAppManager->GetClientExoApp()->GetGUIString(0x177);
    lstActions.element[nIndex].m_pfnAction = &CSWCCreature::ActionMenuAttack;
    lstActions.element[nIndex].m_oidTarget = m_idSelf;
    lstActions.element[nIndex].m_nActionId = 0x3eb;
    lstActions.element[nIndex].m_resIcon = "i_attack";
}

// NONMATCH (9 insns): base/value load order around the name and id stores; the
// icon store needs operator[] (lhs address before the GetIcon call).
// FUNCTION: SWKOTOR 0x006196A0
void CSWCCreature::AddHostileItem(CSWGuiActionList& lstActions, CSWSItem* pItem, CSWCCreature* pTarget)
{
    int nIndex = lstActions.num;
    if (pItem == 0 || m_pArea->GetServerArea()->m_nRestrictMode != 0
        || pItem->GetBaseItem()->m_nCategory != 6) {
        return;
    }
    CExoString sName;
    pItem->m_sLocName.GetString(g_pAppManager->GetClientExoApp()->GetClientLanguage(), &sName, 0);
    lstActions.SetSize(lstActions.num + 1);
    lstActions[nIndex].m_sName = sName;
    lstActions[nIndex].m_nActionId = pItem->GetBaseItemId() | 0x40000000;
    lstActions[nIndex].m_pfnAction = &CSWCCreature::ActionMenuUseItem;
    lstActions[nIndex].m_oidTarget = pTarget->GetId();
    lstActions[nIndex].m_resIcon = pItem->GetIcon();
    lstActions[nIndex].m_n34 = pItem->GetStackSize();
}

// NONMATCH (14 insns): register choice for the client app / feat name loads.
// FUNCTION: SWKOTOR 0x00618A30
void CSWCCreature::AddFeatAction(int nFeat, CSWGuiActionList& lstActions, ActionFn pfnAction, CSWCCreature* pTarget)
{
    if (nFeat == -1) {
        return;
    }
    CSWFeat* pFeat = g_pRules->GetFeat((unsigned short)nFeat);
    int nIndex = lstActions.num;
    lstActions.SetSize(nIndex + 1);
    CExoString sName = g_pAppManager->m_pClientExoApp->GetGUIString(pFeat->m_nNameStrRef);
    if (pTarget == this) {
        sName = sName + " (" + g_pAppManager->m_pClientExoApp->GetGUIString(0x9475) + ")";
    }
    nFeat |= 0x10000000;
    lstActions[nIndex].m_sName = sName;
    lstActions[nIndex].m_pfnAction = pfnAction;
    lstActions[nIndex].m_oidTarget = pTarget->GetId();
    lstActions[nIndex].m_nActionId = nFeat;
    lstActions[nIndex].m_resIcon = pFeat->m_resIcon;
}
