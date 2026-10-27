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
    char* CStr() const;                             // 0x005E5670
    void Format(const char* format, ...);           // 0x005E5680
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
    unsigned long m_bEnabled : 1;                   // 0x30 bit 0 (descriptive)
    unsigned long m_nDisabledReason : 4;            // 0x30 bits 1-4 (descriptive)
    unsigned long m_nFlagsUnused : 27;
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

class CSWCCreature;

class CSWParty {
public:
    CSWCCreature* GetPartyMember(int n);            // 0x006346C0
};

class CClientExoApp {
public:
    CSWParty* GetSWParty();                         // 0x005ED8B0
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

struct CSWItemProperty {
    char pad_0x0[0x2];
    unsigned short m_nSubType;                      // 0x02
    char pad_0x4[0xc - 0x4];
    unsigned char m_bUsable;                        // 0x0c (descriptive)
};

class CSWSItem {
public:
    int FindProperty(CSWItemProperty** ppProperty, unsigned short nType, unsigned short nSubType);   // 0x005539C0
    CSWBaseItemData* GetBaseItem();                 // 0x005B4790
    CResRef GetIcon();                              // 0x005556B0
    unsigned long GetBaseItemId() { return m_nBaseItem; }
    unsigned long GetStackSize() { return m_nStackSize; }

    char pad_0x0[0x14];
    unsigned long m_nBaseItem;                      // 0x14
    char pad_0x18[0x108 - 0x18];
    int m_bIdentifiedUsable;                        // 0x108 (descriptive)
    char pad_0x10c[0x280 - 0x10c];
    CExoLocString m_sLocName;                       // 0x280
    char pad_0x281[0xb];
    unsigned long m_nStackSize;                     // 0x28c
};

class CSWSArea {
public:
    char pad_0x0[0x2b0];
    int m_nRestrictMode;                            // 0x2b0
};

struct Vector {
    float x, y, z;
};

class CSWInventory {
public:
    int GetItemInInventory(CSWSItem* pItem);        // 0x005A4950
};

class CSWSCreatureStats {
public:
    int GetCanUseSkill(unsigned char nSkill);       // 0x005AF880

    char pad_0x0[0x124];
    short m_nCurrentForce;                          // 0x124 (descriptive)
    short m_nForceBonus;                            // 0x126 (descriptive)
};

class CSWSCreature {
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
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual short GetCurrentForcePoints(int bIncludeBonus);   // 0x9c (descriptive)

    CSWSArea* GetArea();                            // 0x004CB120
    unsigned long GetForceItemMask();               // 0x004EFA80
    int CanUseItem(CSWSItem* pItem, int bArg);      // 0x0051B440 (descriptive)

    char pad_0x4[0x9e0 - 0x4];
    int m_bCanUseHealingItems;                      // 0x9e0 (descriptive)
    char pad_0x9e4[0xa2c - 0x9e4];
    CSWInventory* m_pInventory;                     // 0xa2c
    char pad_0xa30[0xa74 - 0xa30];
    CSWSCreatureStats* m_pStats;                    // 0xa74
    char pad_0xa78[0xa9c - 0xa78];
    int m_bUseCurrentForcePoints;                   // 0xa9c (descriptive)
};

extern const unsigned short RACIAL_TYPE_DROID;      // 0x0074BF10
extern const unsigned short RACIAL_TYPE_HUMAN;      // 0x0074BF14
extern const unsigned short g_nForceTargetHuman;    // 0x0074BF34 (descriptive)
extern const unsigned short g_nForceTargetDroid;    // 0x0074BF38 (descriptive)
extern const unsigned short g_nForceTargetOther;    // 0x0074BF3C (descriptive)

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

class CSWSpell {
public:
    char pad_0x0[0x194];
    unsigned char m_nUsableFromItem;                // 0x194 (descriptive)
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00

    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34
    char pad_0x38[0x8c - 0x38];
    CSWSpellArray* m_pSpellArray;                   // 0x8c

    CSWBaseItemArray* GetBaseItemArray() { return m_pBaseItemArray; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual CSWCItem* AsSWCItem();                  // 0x34

    unsigned long m_idSelf;                         // 0x04
};

class CSWCObject : public CGameObject {
public:
    char pad_0x8[0x24 - 0x8];
    Vector m_vPosition;                             // 0x24
};

class CSWCCreatureStats {
public:
    short GetMaxHitPoints();                        // 0x00647A80
    short GetCurrentHitPoints() { return m_nCurrentHitPoints; }

    void AppendForceActions(CSWGuiActionList* pActions, int nType, int nForcePoints, int nTargetType,
                            int nItemMask, float fDistanceSquared);   // 0x0064AF10

    char pad_0x0[0x24];
    unsigned short m_nRace;                         // 0x24
    char pad_0x26[0x4c - 0x26];
    short m_nCurrentHitPoints;                      // 0x4c
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
    void GetHostileForcePowers(CSWCObject* pTarget, CSWGuiActionList& lstActions);
    void GetEquipmentForcePowers(CSWGuiActionList& lstActions, unsigned long nType, int nTargetType,
                                 unsigned long nItemMask);  // 0x00618C20
    void ForceMenuHostileAction(unsigned long oidTarget, CSWCCreature* pCreature);  // 0x00616200
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    int CheckFeat(int nFeat1, int nFeat2, int nFeat3);      // 0x00616140
    void AddFeatAction(int nFeat, CSWGuiActionList& lstActions, ActionFn pfnAction, CSWCCreature* pTarget);  // 0x00618A30 (descriptive)
    void ActionMenuFeat(unsigned long oidTarget, CSWCCreature* pCreature);     // 0x00617DD0 (descriptive)
    void AddHostileItem(CSWGuiActionList& lstActions, CSWSItem* pItem, CSWCCreature* pTarget);
    void ActionMenuUseItem(unsigned long oidTarget, CSWCCreature* pCreature);  // 0x00617E40 (descriptive)
    void CreateUsableItemEntry(CSWGuiActionList& lstActions, CSWSItem* pItem, int bEquipped, unsigned char nFlags);
    int CanUseItemInMenu(CSWSItem* pItem, int bEquipped, unsigned char nFlags);   // 0x00616520 (descriptive)
    void PartyMenuUseItem(unsigned long oidTarget, CSWCCreature* pCreature);    // 0x0060F590
    CSWSCreature* AsServerCreature();               // 0x0060FB20
    CSWSCreature* GetServerCreature()
    {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }

    void GetCreatureTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);
    void ActionMenuAttack(unsigned long oidTarget, CSWCCreature* pCreature);   // 0x00616800
    CGameObject* GetServerObject();                 // 0x0063D4B0
    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x18];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    char pad_0x30[0x2f8 - 0x30];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
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

// NONMATCH (40 diff insns): register assignment only; the original keeps
// `this` in ebx and the target in edi, ours swaps them. Looping on nIndex
// itself gives 37 but loses the original's separate loop counter (ecx).
// FUNCTION: SWKOTOR 0x006191F0
void CSWCCreature::GetHostileForcePowers(CSWCObject* pTarget, CSWGuiActionList& lstActions)
{
    int nTargetType = 0;
    if (GetServerObject() == 0) {
        return;
    }
    CSWSCreature* pServer = GetServerObject()->AsSWSCreature();
    if (pServer == 0 || pServer->GetArea()->m_nRestrictMode != 0) {
        return;
    }
    if (pTarget->AsSWCCreature()) {
        if (pTarget->AsSWCCreature()->m_pStats->m_nRace == RACIAL_TYPE_DROID) {
            nTargetType = g_nForceTargetDroid;
        } else if (pTarget->AsSWCCreature()->m_pStats->m_nRace == RACIAL_TYPE_HUMAN) {
            nTargetType = g_nForceTargetHuman;
        }
    } else {
        nTargetType = g_nForceTargetOther;
    }
    unsigned long nItemMask = pServer->GetForceItemMask();
    int nForcePoints;
    if (pServer->m_bUseCurrentForcePoints) {
        nForcePoints = pServer->GetCurrentForcePoints(0);
    } else {
        nForcePoints = (short)(pServer->m_pStats->m_nForceBonus + pServer->m_pStats->m_nCurrentForce);
    }
    int nIndex = lstActions.num;
    if (m_pStats->m_nRace == RACIAL_TYPE_DROID) {
        GetEquipmentForcePowers(lstActions, 1, nTargetType, nItemMask);
    } else {
        float fX = pTarget->m_vPosition.x - m_vPosition.x;
        float fY = pTarget->m_vPosition.y - m_vPosition.y;
        float fZ = pTarget->m_vPosition.z - m_vPosition.z;
        m_pStats->AppendForceActions(&lstActions, 1, nForcePoints, nTargetType, nItemMask, fZ * fZ + fY * fY + fX * fX);
    }
    for (int i = nIndex; i < lstActions.num; i++) {
        lstActions.element[i].m_pfnAction = &CSWCCreature::ForceMenuHostileAction;
        lstActions.element[i].m_oidTarget = pTarget->m_idSelf;
    }
}

// Mac name CreateUsableItemEntry (r6 callers declare it AddUsableItem).
// NONMATCH (16 diff insns): same scratch-register choice as AddHostileItem
// for the SetSize argument and the name/id/oid stores (eax/ecx/edx rotate);
// element[] vs operator[] and GetId() vs m_idSelf do not change it.
// FUNCTION: SWKOTOR 0x006193A0
void CSWCCreature::CreateUsableItemEntry(CSWGuiActionList& lstActions, CSWSItem* pItem, int bEquipped, unsigned char nFlags)
{
    int nIndex = lstActions.num;
    if (!CanUseItemInMenu(pItem, bEquipped, nFlags)) {
        return;
    }
    CExoString sName;
    pItem->m_sLocName.GetString(g_pAppManager->GetClientExoApp()->GetClientLanguage(), &sName, 0);
    sName.Format("%s (%s)", sName.CStr(), g_pAppManager->GetClientExoApp()->GetGUIString(0x9475).CStr());
    lstActions.SetSize(lstActions.num + 1);
    lstActions.element[nIndex].m_sName = sName;
    lstActions.element[nIndex].m_nActionId = pItem->GetBaseItemId() | 0x40000000;
    lstActions[nIndex].m_pfnAction = &CSWCCreature::PartyMenuUseItem;
    lstActions[nIndex].m_oidTarget = GetId();
    lstActions[nIndex].m_resIcon = pItem->GetIcon();
    lstActions[nIndex].m_n34 = pItem->GetStackSize();
    int nCategory = pItem->GetBaseItem()->m_nCategory;
    if ((nCategory == 0x2d || nCategory == 0x1a) &&
        m_pStats->GetCurrentHitPoints() >= m_pStats->GetMaxHitPoints()) {
        CSWSCreature* pServer = GetServerCreature();
        if (!pServer || !pServer->m_bCanUseHealingItems) {
            lstActions[nIndex].m_bEnabled = 0;
            lstActions[nIndex].m_nDisabledReason = 5;
        }
    } else if (nCategory == 0x2f) {
        int bHurt = 0;
        if (m_pStats->GetCurrentHitPoints() < m_pStats->GetMaxHitPoints()) {
            bHurt = 1;
        }
        CSWCCreature* pMember = g_pAppManager->GetClientExoApp()->GetSWParty()->GetPartyMember(1);
        if (pMember && pMember->AsServerCreature() &&
            pMember->m_pStats->GetCurrentHitPoints() < pMember->m_pStats->GetMaxHitPoints()) {
            bHurt |= 1;
        }
        pMember = g_pAppManager->GetClientExoApp()->GetSWParty()->GetPartyMember(2);
        if (pMember && pMember->AsServerCreature() &&
            pMember->m_pStats->GetCurrentHitPoints() < pMember->m_pStats->GetMaxHitPoints()) {
            bHurt |= 1;
        }
        CSWSCreature* pServer = GetServerCreature();
        if (!bHurt || !pServer) {
            lstActions[nIndex].m_bEnabled = 0;
            lstActions[nIndex].m_nDisabledReason = 5;
        }
    } else if (bEquipped) {
        CSWItemProperty* pProperty;
        pItem->FindProperty(&pProperty, 10, 0);
        lstActions[nIndex].m_bEnabled = pProperty->m_bUsable;
    }
}

// Descriptive name: whether an item may be listed as usable in the radial
// menu. nFlags bit 0 allows medical items, bit 1 skill (0x1c) items, bit 2
// force-power items; bEquipped is unused.
// NONMATCH (40 diff insns): register allocation; the original keeps `this`
// in edi and the creature in ebp (four saved registers), we reuse ebx for
// `this` and then nFlags. The block order and code are otherwise the same.
// FUNCTION: SWKOTOR 0x00616520
int CSWCCreature::CanUseItemInMenu(CSWSItem* pItem, int bEquipped, unsigned char nFlags)
{
    if (!pItem) {
        return 0;
    }
    CSWSCreature* pServer = GetServerCreature();
    int nRestrictMode = m_pArea->GetServerArea()->m_nRestrictMode;
    if (pServer) {
        CSWItemProperty* pProperty;
        CSWBaseItemData* pBase;
        CSWSpell* pSpell;
        if (pItem->m_bIdentifiedUsable) {
            if ((nFlags & 2) && pItem->GetBaseItem()->m_nCategory == 0x1c) {
                if (!pItem->FindProperty(&pProperty, 0x2e, 0)) {
                    return pServer->CanUseItem(pItem, 1);
                }
                if (!pProperty->m_bUsable) {
                    return 0;
                }
                if (!pServer->m_pStats->GetCanUseSkill(1)) {
                    return 0;
                }
                return pServer->CanUseItem(pItem, 1);
            }
            if (!pItem->FindProperty(&pProperty, 0xa, 0)) {
                return 0;
            }
            pBase = pItem->GetBaseItem();
            if (pBase) {
                switch (pBase->m_nCategory) {
                case 0xc:
                case 0xd:
                case 0x14:
                    if (!pServer->m_pInventory->GetItemInInventory(pItem)) {
                        return 0;
                    }
                }
                if (!(nFlags & 4)) {
                    return 0;
                }
            }
            pSpell = g_pRules->m_pSpellArray->GetSpell(pProperty->m_nSubType);
            if (!pSpell || pSpell->m_nUsableFromItem != 1) {
                return 0;
            }
            return pServer->CanUseItem(pItem, 1);
        }
        if (nRestrictMode) {
            return 0;
        }
        if (nFlags & 1) {
            if (pItem->GetBaseItem()->m_nCategory == 0x2d || pItem->GetBaseItem()->m_nCategory == 0x2f ||
                pItem->GetBaseItem()->m_nCategory == 0x1a) {
                if (!pItem->FindProperty(&pProperty, 0xa, 0)) {
                    return pServer->CanUseItem(pItem, 1);
                }
                if (!pProperty->m_bUsable) {
                    return 0;
                }
                return pServer->CanUseItem(pItem, 1);
            }
        }
        unsigned char nForceFlag = (unsigned char)(nFlags & 4);
        if (nForceFlag && pItem->GetBaseItem()->m_nCategory == 0x19) {
            if (!pItem->FindProperty(&pProperty, 0xa, 0)) {
                return pServer->CanUseItem(pItem, 1);
            }
            if (!pProperty->m_bUsable) {
                return 0;
            }
            return pServer->CanUseItem(pItem, 1);
        }
        if ((nFlags & 2) && pItem->GetBaseItem()->m_nCategory == 0x1c) {
            if (!pItem->FindProperty(&pProperty, 0x2e, 0)) {
                return pServer->CanUseItem(pItem, 1);
            }
            if (!pProperty->m_bUsable) {
                return 0;
            }
            if (!pServer->m_pStats->GetCanUseSkill(1)) {
                return 0;
            }
            return pServer->CanUseItem(pItem, 1);
        }
        if (!pItem->FindProperty(&pProperty, 0xa, 0)) {
            return 0;
        }
        if (!pProperty->m_bUsable) {
            return 0;
        }
        pBase = pItem->GetBaseItem();
        if (pBase) {
            switch (pBase->m_nCategory) {
            case 0xc:
            case 0xd:
            case 0x14:
                if (!pServer->m_pInventory->GetItemInInventory(pItem)) {
                    return 0;
                }
            }
            if (!nForceFlag) {
                return 0;
            }
        }
        pSpell = g_pRules->m_pSpellArray->GetSpell(pProperty->m_nSubType);
        if (!pSpell || pSpell->m_nUsableFromItem != 1) {
            return 0;
        }
        return pServer->CanUseItem(pItem, 1);
    }
    return 0;
}
