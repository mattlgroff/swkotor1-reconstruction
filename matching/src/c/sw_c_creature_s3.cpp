// CSWCCreature leaves (nwccreature.o, worker C s3) with a translation-unit
// local creature layout; the shared creature base is not changed.

class CExoString {
public:
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);   // 0x00413660
};

class C2DAs {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
};

class CSWBaseItem;

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItems;                 // 0x34
    char pad_0x38[0xb8 - 0x38];
    C2DAs* m_p2DArrays;                             // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CCreatureAppearanceS3 {
public:
    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
};

extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644

class CSWCCreature;

class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetVisible(int bVisible);          // 0x4c
};

// The client object part of an item (CSWCItem + 0x10).
class CSWCItemObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void UnEquipModel();                    // 0x80 (descriptive)
    virtual void ShowEquipped(int b);               // 0x84 (descriptive)
    virtual void v88(); virtual void v8C(); virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);   // 0x98

    unsigned long m_idSelf;                         // 0x14 of the item
};

class CSWCItem {
public:
    void SetEquippedBy(CSWCCreature* pCreature);    // 0x00646600
    void SetPowered(int a, int b, int c, int d);    // 0x00647130

    char pad_0x0[0xc];
    int m_nBaseItem;                                // 0x0c
    CSWCItemObject m_cObject;                       // 0x10
    char pad_0x18[0x164 - 0x18];
    unsigned long m_oidPossessor;                   // 0x164 (descriptive)
    char pad_0x168[0x198 - 0x168];
    int m_bPowered;                                 // 0x198
};

class CClientExoApp {
public:
    CSWCItem* GetItemByGameObjectID(unsigned long oid);   // 0x005ED5A0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCCreature {
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
    virtual void SetAnimation(unsigned short nAnim, int a, int b);  // 0x9c

    void EquipToInventorySlot(unsigned long nSlot, CSWCItem* pItem);
    void SetOverrideLightsaberState(int bState, int a, int b);
    void BlurWeapon(int bBlur);
    void SetItemUnEquippedByMe(unsigned long oid);  // 0x0060D020
    void UpdateWeaponClass();                       // 0x00613710
    void UpdateVisibleWeapons(unsigned long nSlot, CSWCItem* pItem, int bFlag);   // 0x00613850
    unsigned short GetLoopingAnimation();           // 0x0063C100 (CSWCObject)

    int GetFreeLookVideoEffect();
    unsigned long GetEquippedItemID(unsigned long nSlot);
    void SetEquippedItemID(unsigned long nSlot, unsigned long oidItem);
    void SetLastInstant(unsigned long nSlot, int bValue);
    int GetLastInstant(unsigned long nSlot);
    unsigned short GetAppearanceType() { return m_pAppearance->m_nAppearanceType; }

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x21c - 0x8];
    CCreatureAppearanceS3* m_pAppearance;           // 0x21c
    unsigned long m_aEquippedItems[15];             // 0x220 (by slot bit)
    char pad_0x25c[0x14];
    int m_aLastInstant[15];                         // 0x270 (by slot bit)
    char pad_0x2ac[0x2cc - 0x2ac];
    int m_bBlurWeapon;                              // 0x2cc
    char pad_0x2d0[0x3d8 - 0x2d0];
    int m_bOverrideLightsaber;                      // 0x3d8
};

// NONMATCH (17 insns): the original keeps the appearance row in si and loads
// the table before the argument pushes; ours interleaves them.
// FUNCTION: SWKOTOR 0x00610490
int CSWCCreature::GetFreeLookVideoEffect()
{
    int nEffect = -1;
    if (m_pAppearance != 0) {
        if (!g_pRules->m_p2DArrays->m_pAppearanceTable->GetINTEntry(
                GetAppearanceType(), "FreeLookEffect", &nEffect)) {
            return -1;
        }
        return nEffect;
    }
    return nEffect;
}

// FUNCTION: SWKOTOR 0x0060CA80
unsigned long CSWCCreature::GetEquippedItemID(unsigned long nSlot)
{
    switch (nSlot) {
    case 0x1:
        return m_aEquippedItems[0];
    case 0x2:
        return m_aEquippedItems[1];
    case 0x4:
        return m_aEquippedItems[2];
    case 0x8:
        return m_aEquippedItems[3];
    case 0x10:
        return m_aEquippedItems[4];
    case 0x20:
        return m_aEquippedItems[5];
    case 0x40:
        return m_aEquippedItems[6];
    case 0x80:
        return m_aEquippedItems[7];
    case 0x100:
        return m_aEquippedItems[8];
    case 0x200:
        return m_aEquippedItems[9];
    case 0x400:
        return m_aEquippedItems[10];
    case 0x4000:
        return m_aEquippedItems[11];
    case 0x8000:
        return m_aEquippedItems[12];
    case 0x10000:
        return m_aEquippedItems[13];
    case 0x20000:
        return m_aEquippedItems[14];
    }
    return OBJECT_INVALID_ID;
}

// FUNCTION: SWKOTOR 0x0060CBD0
void CSWCCreature::SetEquippedItemID(unsigned long nSlot, unsigned long oidItem)
{
    switch (nSlot) {
    case 0x1:
        m_aEquippedItems[0] = oidItem;
        break;
    case 0x2:
        m_aEquippedItems[1] = oidItem;
        break;
    case 0x4:
        m_aEquippedItems[2] = oidItem;
        break;
    case 0x8:
        m_aEquippedItems[3] = oidItem;
        break;
    case 0x10:
        m_aEquippedItems[4] = oidItem;
        break;
    case 0x20:
        m_aEquippedItems[5] = oidItem;
        break;
    case 0x40:
        m_aEquippedItems[6] = oidItem;
        break;
    case 0x80:
        m_aEquippedItems[7] = oidItem;
        break;
    case 0x100:
        m_aEquippedItems[8] = oidItem;
        break;
    case 0x200:
        m_aEquippedItems[9] = oidItem;
        break;
    case 0x400:
        m_aEquippedItems[10] = oidItem;
        break;
    case 0x4000:
        m_aEquippedItems[11] = oidItem;
        break;
    case 0x8000:
        m_aEquippedItems[12] = oidItem;
        break;
    case 0x10000:
        m_aEquippedItems[13] = oidItem;
        break;
    case 0x20000:
        m_aEquippedItems[14] = oidItem;
        break;
    }
}

// FUNCTION: SWKOTOR 0x0060CD50
void CSWCCreature::SetLastInstant(unsigned long nSlot, int bValue)
{
    switch (nSlot) {
    case 0x1:
        m_aLastInstant[0] = bValue;
        break;
    case 0x2:
        m_aLastInstant[1] = bValue;
        break;
    case 0x4:
        m_aLastInstant[2] = bValue;
        break;
    case 0x8:
        m_aLastInstant[3] = bValue;
        break;
    case 0x10:
        m_aLastInstant[4] = bValue;
        break;
    case 0x20:
        m_aLastInstant[5] = bValue;
        break;
    case 0x40:
        m_aLastInstant[6] = bValue;
        break;
    case 0x80:
        m_aLastInstant[7] = bValue;
        break;
    case 0x100:
        m_aLastInstant[8] = bValue;
        break;
    case 0x200:
        m_aLastInstant[9] = bValue;
        break;
    case 0x400:
        m_aLastInstant[10] = bValue;
        break;
    case 0x4000:
        m_aLastInstant[11] = bValue;
        break;
    case 0x8000:
        m_aLastInstant[12] = bValue;
        break;
    case 0x10000:
        m_aLastInstant[13] = bValue;
        break;
    case 0x20000:
        m_aLastInstant[14] = bValue;
        break;
    }
}

// FUNCTION: SWKOTOR 0x0060CED0
int CSWCCreature::GetLastInstant(unsigned long nSlot)
{
    switch (nSlot) {
    case 0x1:
        return m_aLastInstant[0];
    case 0x2:
        return m_aLastInstant[1];
    case 0x4:
        return m_aLastInstant[2];
    case 0x8:
        return m_aLastInstant[3];
    case 0x10:
        return m_aLastInstant[4];
    case 0x20:
        return m_aLastInstant[5];
    case 0x40:
        return m_aLastInstant[6];
    case 0x80:
        return m_aLastInstant[7];
    case 0x100:
        return m_aLastInstant[8];
    case 0x200:
        return m_aLastInstant[9];
    case 0x400:
        return m_aLastInstant[10];
    case 0x4000:
        return m_aLastInstant[11];
    case 0x8000:
        return m_aLastInstant[12];
    case 0x10000:
        return m_aLastInstant[13];
    case 0x20000:
        return m_aLastInstant[14];
    }
    return 0;
}

// Needs GetEquippedItemID/SetEquippedItemID/GetLastInstant earlier in this
// translation unit: the original relies on them leaving ecx intact.
// FUNCTION: SWKOTOR 0x00613BA0
void CSWCCreature::EquipToInventorySlot(unsigned long nSlot, CSWCItem* pItem)
{
    CGob* pModel;
    unsigned long oidItem;
    if (pItem) {
        pModel = pItem->m_cObject.FindPart(0xff, 1);
        oidItem = pItem->m_cObject.m_idSelf;
        pItem->SetEquippedBy(this);
    } else {
        pModel = 0;
        oidItem = 0x7F000000;
    }
    CSWCItem* pOldItem = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(GetEquippedItemID(nSlot));
    if (nSlot == 0x10 || nSlot == 0x20) {
        if (GetEquippedItemID(nSlot) != oidItem) {
            if (nSlot == 0x10) {
                if (GetEquippedItemID(0x20) == OBJECT_INVALID_ID || GetEquippedItemID(0x20) != oidItem) {
                    SetItemUnEquippedByMe(GetEquippedItemID(0x10));
                }
            } else {
                if (GetEquippedItemID(0x10) == OBJECT_INVALID_ID || GetEquippedItemID(0x10) != oidItem) {
                    SetItemUnEquippedByMe(GetEquippedItemID(nSlot));
                }
            }
        }
        SetEquippedItemID(nSlot, oidItem);
        UpdateWeaponClass();
        UpdateVisibleWeapons(nSlot, pItem, GetLastInstant(nSlot));
        if (GetLoopingAnimation() == 10001) {
            SetAnimation(10001, 0, 0);
        }
    } else if (nSlot == 2) {
        if (pModel) {
            pModel->SetVisible(0);
        }
        if (GetEquippedItemID(2) != oidItem) {
            SetItemUnEquippedByMe(GetEquippedItemID(2));
        }
        SetEquippedItemID(2, oidItem);
    } else if (nSlot == 1) {
        if (GetEquippedItemID(nSlot) != oidItem) {
            SetItemUnEquippedByMe(GetEquippedItemID(nSlot));
        }
        SetEquippedItemID(1, oidItem);
        if (pItem) {
            pItem->m_oidPossessor = m_idSelf;
        }
    } else {
        if (GetEquippedItemID(nSlot) != oidItem) {
            SetItemUnEquippedByMe(GetEquippedItemID(nSlot));
        }
        SetEquippedItemID(nSlot, oidItem);
    }
    if (pOldItem && pOldItem != pItem && pOldItem->m_cObject.FindPart(0xff, 1)) {
        pOldItem->m_cObject.UnEquipModel();
    }
    if (pItem) {
        pItem->m_cObject.ShowEquipped(0);
    }
}

// Moved here from sw_c_creature_misc.cpp: both rely on GetEquippedItemID
// (defined above) leaving edx intact.
// FUNCTION: SWKOTOR 0x0060FB80
void CSWCCreature::SetOverrideLightsaberState(int bState, int a, int b)
{
    if (bState) {
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(GetEquippedItemID(0x10));
        if (pItem) {
            pItem->SetPowered(a, 1, b, 0);
        }
        pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(GetEquippedItemID(0x20));
        if (pItem) {
            pItem->SetPowered(a, 1, b, 0);
        }
    }
    m_bOverrideLightsaber = bState;
}

// FUNCTION: SWKOTOR 0x006116A0
void CSWCCreature::BlurWeapon(int bBlur)
{
    if (m_bBlurWeapon == bBlur) {
        return;
    }
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(GetEquippedItemID(0x10));
    if (pItem) {
        g_pRules->m_pBaseItems->GetBaseItem(pItem->m_nBaseItem);
        if (pItem->m_bPowered == 1) {
            return;
        }
    }
    m_bBlurWeapon = bBlur;
}
