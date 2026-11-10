// CSWCCreature leaves (nwccreature.o, worker C s3) with a translation-unit
// local creature layout; the shared creature base is not changed.

#include <string.h>
#include <stdlib.h>

struct Vector {
    float x, y, z;
};

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(int n);                              // 0x005E5BC0
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    int IsEmpty() const;                            // 0x005E58A0
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* CStr() const;                             // 0x005E5670
    char* m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);   // 0x00413430
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);   // 0x00413660
    int GetCExoStringEntry(const CExoString& sRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413DE0
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue);   // 0x00413190
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413270
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);   // 0x00413350
    int GetINTEntry(int nRow, int nColumn, int* pValue);   // 0x00413510
};

class C2DAs {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
    char pad_0x10[0x24 - 0x10];
    C2DA* m_pDoorTypes;                             // 0x24
    C2DA* m_pGenericDoors;                          // 0x28
    C2DA* m_pPlaceables;                            // 0x2c
    char pad_0x30[0x5c - 0x30];
    C2DA* m_pAppearanceSoundSet;                    // 0x5c
    C2DA* m_pWeaponSounds;                          // 0x60
    char pad_0x64[0x70 - 0x64];
    C2DA* m_pPlaceableSounds;                       // 0x70
    char pad_0x74[0x88 - 0x74];
    C2DA* m_pVisualEffects;                         // 0x88

    C2DA* GetVisualEffects() { return m_pVisualEffects; }
    C2DA* GetAppearanceTable() { return m_pAppearanceTable; }
    C2DA* GetDoorTypes() { return m_pDoorTypes; }
    C2DA* GetGenericDoors() { return m_pGenericDoors; }
    C2DA* GetPlaceables() { return m_pPlaceables; }
    C2DA* GetAppearanceSoundSet() { return m_pAppearanceSoundSet; }
    C2DA* GetWeaponSounds() { return m_pWeaponSounds; }
    C2DA* GetPlaceableSounds() { return m_pPlaceableSounds; }
};

class CSWBaseItem {
public:
    char pad_0x0[0x8c];
    unsigned char m_nWeaponSoundRow;                // 0x8c (descriptive)
    char pad_0x8d[0xbc - 0x8d];
    CExoString m_sArmorSound;                       // 0xbc (descriptive)
};

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

    C2DAs* Get2DArrays() { return m_p2DArrays; }
    CSWBaseItemArray* GetBaseItems() { return m_pBaseItems; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CCreatureAppearanceS3 {
public:
    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
};

extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644

class CSWCCreature;
class CSWCItem;

class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetVisible(int bVisible);          // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CSWCItem* GetItem();                    // 0x70
    virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0();
    virtual void SetRenderGroup(int nGroup);        // 0xc4 (descriptive)
};

// Attaches equipped-item models to the creature's hand nodes.
class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void AttachItem(unsigned long oidItem, const CExoString& sNode, int bRightHand,
                            float fScale);          // 0x7c (descriptive)

    char pad_0x4[0xc5 - 0x4];
    unsigned char m_nC5;                            // 0xc5 (3: two-handed stance, descriptive)
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

    void StartFlash();                              // 0x0063D540 (CSWCObject)
    void StopFlash();                               // 0x0063D550 (CSWCObject)

    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                         // 0x14 of the item
};

class CSWCItem {
public:
    void SetEquippedBy(CSWCCreature* pCreature);    // 0x00646600
    void SetPowered(int a, int b, int c, int d);    // 0x00647130
    void SetSpecialAttackModel(const CExoString& sModel, const CExoString& sNode);   // 0x00646880
    CSWBaseItem* GetBaseItem();                     // 0x005B4790 (CSWItem)

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
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    class CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
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
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);      // 0x98
    virtual void SetAnimation(unsigned short nAnim, int a, int b);  // 0x9c
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8();
    virtual int HasVisualEffect(int nEffect);       // 0xcc

    CSWCAnimBase* GetAnimBase();                    // 0x0063C070
    unsigned short GetCurrentAnimation();           // 0x0063C060
    int AnimationLooping(unsigned short nAnim);     // 0x0063C300 (CSWCObject)
    void EquipToInventorySlot(unsigned long nSlot, CSWCItem* pItem);
    void SetOverrideLightsaberState(int bState, int a, int b);
    void BlurWeapon(int bBlur);
    void SetAttackAnimationData(int nBaseItem, int nAttackType, int bRanged);
    int SelectByMode(int nMode, int a, int b, int c);   // 0x0060D430
    void SetItemUnEquippedByMe(unsigned long oid);  // 0x0060D020
    void UpdateWeaponClass();                       // 0x00613710
    void UpdateVisibleWeapons(unsigned long nSlot, CSWCItem* pItem, int bFlag);   // 0x00613850
    unsigned short GetLoopingAnimation();           // 0x0063C100 (CSWCObject)

    int GetFreeLookVideoEffect();
    int GetForceShield();                           // 0x00616890
    int PlayHitSound(class CSWCCombatAttackData* pData, int bSuppress);
    unsigned long GetEquippedItemID(unsigned long nSlot);
    void SetEquippedItemID(unsigned long nSlot, unsigned long oidItem);
    void SetLastInstant(unsigned long nSlot, int bValue);
    int GetLastInstant(unsigned long nSlot);
    unsigned short GetAppearanceType() { return m_pAppearance->m_nAppearanceType; }

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0xc - 0x8];
    int m_n0C;                                      // 0x0c (CSWCObject; "stone" hit sound)
    int m_n10;                                      // 0x10 (CSWCObject; "wood" hit sound)
    int m_n14;                                      // 0x14 (CSWCObject; "stone" hit sound)
    char pad_0x18[0x24 - 0x18];
    Vector m_vPosition;                             // 0x24
    char pad_0x30[0x138 - 0x30];
    int m_n138;                                     // 0x138
    char pad_0x13c[0x21c - 0x13c];
    CCreatureAppearanceS3* m_pAppearance;           // 0x21c
    unsigned long m_aEquippedItems[15];             // 0x220 (by slot bit)
    char pad_0x25c[0x268 - 0x25c];
    unsigned long m_oidShownRightWeapon;            // 0x268 (descriptive)
    unsigned long m_oidShownLeftWeapon;             // 0x26c (descriptive)
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

// Picks the special-attack impact effect row for the weapon type and hands the
// effect model and hook node to the equipped weapons.
// FUNCTION: SWKOTOR 0x006142F0
void CSWCCreature::SetAttackAnimationData(int nBaseItem, int nAttackType, int bRanged)
{
    int nEffect;
    switch (nBaseItem) {
    case 17:
        nEffect = SelectByMode(nAttackType, 0xfbb, 0xfac, 0xfbb);
        break;
    case 83:
        nEffect = SelectByMode(nAttackType, 0xfbc, 0xfb3, 0xfbc);
        break;
    case 19:
        nEffect = SelectByMode(nAttackType, 0xfb9, 0xfae, 0xfb9);
        break;
    case 81:
        nEffect = SelectByMode(nAttackType, 0xfba, 0xfb2, 0xfba);
        break;
    case 91:
        nEffect = SelectByMode(nAttackType, 0xfbe, 0xfb5, 0xfbe);
        break;
    case 53:
        nEffect = SelectByMode(nAttackType, 0xfbf, 0xfb1, 0xfbf);
        break;
    case 31:
        nEffect = 0xfc6;
        break;
    case 20:
        nEffect = 0xfaf;
        break;
    case 77:
        nEffect = 0xfb4;
        break;
    case 92:
        nEffect = 0xfb6;
        break;
    case 26:
        nEffect = 0xfb0;
        break;
    case 18:
        nEffect = 0xfad;
        break;
    case 82:
        nEffect = 0xfbd;
        break;
    default:
        return;
    }
    if (!nEffect) {
        return;
    }
    CExoString sRow(nEffect);
    CExoString sModel;
    {
        CExoString sColumn("Imp_Impact_Node");
        g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(sRow, sColumn, &sModel);
    }
    if (!sModel.IsEmpty()) {
        CExoString sNode;
        if (bRanged) {
            sNode = "bullethook";
        } else {
            sNode = "root";
        }
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(GetEquippedItemID(0x10));
        if (pItem) {
            pItem->SetSpecialAttackModel(sModel, sNode);
        }
        if (nAttackType == 4 || nAttackType == 6) {
            pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(GetEquippedItemID(0x20));
            if (pItem) {
                pItem->SetSpecialAttackModel(sModel, sNode);
            }
        }
    }
}

class CAurObject;

// Model-event callback (Mac name) that re-attaches the equipped weapon models
// to the hands. Needs GetLastInstant earlier in this translation unit: the
// original keeps the item in edx across that call.
// NONMATCH (38 insns, one register choice): the original keeps the left item
// in edi from its lookup on and compares the right id from its stack slot;
// ours caches the right id in edi and loads the left item later.
// FUNCTION: SWKOTOR 0x00610F40
void SwitchWeaponEvent(CAurObject* pObject, char* sEvent, void* pData)
{
    CSWCCreature* pCreature = (CSWCCreature*)pData;
    int bForce = 0;
    int bLoading = 0;
    int bPowered = 1;
    if (sEvent) {
        if (strcmp(sEvent, "AppearanceForceUpdate") == 0) {
            bForce = 1;
            if (pCreature && !pCreature->m_n138) {
                bPowered = 0;
            }
        }
        if (strcmp(sEvent, "Loading") == 0) {
            bLoading = 1;
            bPowered = 0;
        }
    }
    CSWCItem* pOldItem = 0;
    if (!pCreature) {
        return;
    }
    unsigned long aoidWeapon[2];
    aoidWeapon[0] = pCreature->GetEquippedItemID(0x10);
    CSWCItem* pRight = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(aoidWeapon[0]);
    aoidWeapon[1] = pCreature->GetEquippedItemID(0x20);
    CSWCItem* pLeft = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(aoidWeapon[1]);
    float fScale;
    if (!g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pCreature->GetAppearanceType(), "WEAPONSCALE",
                                                                      &fScale)) {
        fScale = 1.0f;
    }
    if (pCreature->m_oidShownRightWeapon != aoidWeapon[0] || bForce) {
        if (pRight) {
            CGob* pModel = pRight->m_cObject.FindPart(0xff, 1);
            pRight->SetEquippedBy(pCreature);
            pRight->m_oidPossessor = pCreature->m_idSelf;
            if (pModel) {
                if (pCreature == g_pAppManager->GetClientExoApp()->GetPlayerCreature()) {
                    pModel->SetRenderGroup(2);
                } else {
                    pModel->SetRenderGroup(4);
                }
                if (pCreature->HasVisualEffect(0x1f40)) {
                    CSWCItem* pShown =
                        g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->m_oidShownRightWeapon);
                    if (pShown) {
                        pShown->m_cObject.StopFlash();
                    }
                    pRight->m_cObject.StartFlash();
                }
                pCreature->GetAnimBase()->AttachItem(pRight->m_cObject.GetId(), "rhand", 1, fScale);
                if (pCreature->AnimationLooping(pCreature->GetCurrentAnimation())) {
                    pCreature->SetAnimation(pCreature->GetCurrentAnimation(), 0, 0);
                }
                pRight->SetPowered(bPowered, 0, !pCreature->GetLastInstant(0x10), bLoading | bForce);
            }
        } else {
            CGob* pPart = pCreature->FindPart(0xf7, 1);
            if (pPart) {
                pOldItem = pPart->GetItem();
                if (pOldItem) {
                    pOldItem->m_oidPossessor = 0x7f000000;
                }
            }
            if (pOldItem) {
                pOldItem->SetPowered(0, 0, !pCreature->GetLastInstant(0x10), bLoading | bForce);
            }
            pCreature->GetAnimBase()->AttachItem(0x7f000000, "", 1, 1.0f);
        }
        pCreature->m_oidShownRightWeapon = aoidWeapon[0];
    }
    if (pCreature->m_oidShownLeftWeapon != aoidWeapon[1] || bForce) {
        if (pLeft) {
            CGob* pModel = pLeft->m_cObject.FindPart(0xff, 1);
            pLeft->SetEquippedBy(pCreature);
            pLeft->m_oidPossessor = pCreature->m_idSelf;
            if (pModel) {
                if (pCreature == g_pAppManager->GetClientExoApp()->GetPlayerCreature()) {
                    pModel->SetRenderGroup(2);
                } else {
                    pModel->SetRenderGroup(4);
                }
                if (pCreature->HasVisualEffect(0x1f40)) {
                    CSWCItem* pShown =
                        g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->m_oidShownLeftWeapon);
                    if (pShown) {
                        pShown->m_cObject.StopFlash();
                    }
                    pLeft->m_cObject.StartFlash();
                }
                pCreature->GetAnimBase()->AttachItem(pLeft->m_cObject.GetId(), "lhand", 0, fScale);
                if (pCreature->AnimationLooping(pCreature->GetCurrentAnimation())) {
                    pCreature->SetAnimation(pCreature->GetCurrentAnimation(), 0, 0);
                }
                pLeft->SetPowered(bPowered, 0, !pCreature->GetLastInstant(0x20), 0);
            }
        } else {
            CGob* pPart = pCreature->FindPart(0xfb, 1);
            if (pPart) {
                pOldItem = pPart->GetItem();
                if (pOldItem) {
                    pOldItem->m_oidPossessor = 0x7f000000;
                }
            }
            if (pOldItem) {
                pOldItem->SetPowered(0, 0, !pCreature->GetLastInstant(0x10), 0);
            }
            pCreature->GetAnimBase()->AttachItem(0x7f000000, "", 0, 1.0f);
        }
        pCreature->m_oidShownLeftWeapon = aoidWeapon[1];
    }
}

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

class CExoSound {
public:
    int PlayOneShotSoundAt(const CResRef& cResRef, Vector vPosition, float fVolume, unsigned long nPriority,
                           unsigned long a, unsigned long b, unsigned long c, float fPitch);   // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639

extern int g_nAppearanceCol_SoundAppType;           // 0x007A2128
extern int g_nAppearanceSoundsCol_Weapon;           // 0x007A2140
extern int g_nAppearanceSoundsCol_ArmorType;        // 0x007A2144
extern int g_nWeaponSoundPitchOffsetColumn;         // 0x007A21A4
extern int g_nDoorTypesCol_SoundAppType;            // 0x007A224C
extern int g_nGenericDoorsCol_SoundAppType;         // 0x007A2250
extern int g_nPlaceablesCol_SoundAppType;           // 0x007A2254
extern int g_nPlaceableSoundsCol_ArmorType;         // 0x007A2260

class CSWCCombatAttackData {
public:
    char pad_0x0[0xc];
    unsigned char m_nAttackResult;                  // 0x0c
    char pad_0xd[0x14 - 0xd];
    unsigned long m_oidTarget;                      // 0x14
    char pad_0x18[0x20 - 0x18];
    int m_bHandled;                                 // 0x20 (descriptive)
    char pad_0x24[0x28 - 0x24];
    unsigned char m_nWeaponAttackType;              // 0x28 (descriptive)
};

class CSWCDoor {
public:
    char pad_0x0[0x124];
    unsigned char m_nAppearanceType;                // 0x124 (doortypes.2da row, 0 = generic)
    unsigned char m_nGenericType;                   // 0x125 (genericdoors.2da row)

    unsigned char GetGenericType() { return m_nGenericType; }
};

class CSWCPlaceable {
public:
    char pad_0x0[0x10c];
    unsigned char m_nAppearance;                    // 0x10c (placeables.2da row)
};

class CSWCObject;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
    virtual void v10();
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCObject : public CGameObject {
public:
    unsigned short GetCurrentAnimation();           // 0x0063C060
    int AnimationParry(unsigned short nAnim);       // 0x0063C4F0
};

// Plays the weapon-on-material impact sound for an attack on the data's
// target; the material comes from the target type and its 2DA rows. Each
// branch keeps its own 2DA lookups: the original cross-jumps their tails.
// NONMATCH (85 insns): statements and EH states line up; the fall-through
// copies of the weaponsounds/placeable-sound lookups pick ecx where the
// original picks eax (so the cross-jump starts later), and the item lookup
// loads the app after GetEquippedItemID (the inline getter swaps ebx/edi).
// FUNCTION: SWKOTOR 0x00617470
int CSWCCreature::PlayHitSound(CSWCCombatAttackData* pData, int bSuppress)
{
    if (!pData) {
        return 0;
    }
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(pData->m_oidTarget);
    if (!pObject || !pObject->AsSWCObject()) {
        return 0;
    }
    CSWCObject* pTarget = pObject->AsSWCObject();
    int nRow = 0;
    int nSoundType = 0;
    CExoString sSound;
    float fPitch = 0.0f;
    if (!bSuppress && !pData->m_bHandled) {
        g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(GetAppearanceType(),
            g_nAppearanceCol_SoundAppType, &nRow);
        g_pRules->Get2DArrays()->GetAppearanceSoundSet()->GetINTEntry(nRow,
            g_nAppearanceSoundsCol_Weapon, &nSoundType);
        if (nSoundType == 0) {
            unsigned long nSlot = 0;
            if (pData->m_nWeaponAttackType == 1 || pData->m_nWeaponAttackType == 6) {
                nSlot = 0x10;
            } else if (GetAnimBase()->m_nC5 == 3) {
                nSlot = 0x10;
            } else if (pData->m_nWeaponAttackType == 2) {
                nSlot = 0x20;
            }
            CSWCItem* pItem = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(GetEquippedItemID(nSlot));
            if (pItem) {
                nSoundType = g_pRules->GetBaseItems()->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponSoundRow;
            }
        }
        if ((pData->m_nAttackResult == 8 || pData->m_nAttackResult == 4) &&
            pTarget->AnimationParry(pTarget->GetCurrentAnimation())) {
            CExoString sMaterial = CExoString("Parry") + CExoString(rand() % 2);
            g_pRules->Get2DArrays()->GetWeaponSounds()->GetCExoStringEntry(nSoundType, sMaterial, &sSound);
            g_pRules->Get2DArrays()->GetWeaponSounds()->GetFLOATEntry(nSoundType,
                g_nWeaponSoundPitchOffsetColumn, &fPitch);
        } else {
            CExoString sMaterial;
            if (pTarget->m_nObjectType == OBJECT_TYPE_CREATURE) {
                CSWCCreature* pCreature = pTarget->AsSWCCreature();
                if (pCreature->GetForceShield()) {
                    sMaterial = "forcefield";
                } else if (pCreature->m_n0C || pCreature->m_n14) {
                    sMaterial = "stone";
                } else if (pCreature->m_n10) {
                    sMaterial = "wood";
                } else {
                    g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(pCreature->GetAppearanceType(),
                        g_nAppearanceCol_SoundAppType, &nRow);
                    g_pRules->Get2DArrays()->GetAppearanceSoundSet()->GetCExoStringEntry(nRow,
                        g_nAppearanceSoundsCol_ArmorType, &sMaterial);
                    if (sMaterial.IsEmpty()) {
                        CSWCItem* pArmor = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
                            pCreature->GetEquippedItemID(2));
                        if (pArmor) {
                            sMaterial = pArmor->GetBaseItem()->m_sArmorSound;
                        }
                    }
                    if (sMaterial.IsEmpty()) {
                        sMaterial = "leather";
                    }
                }
            } else if (pTarget->m_nObjectType == OBJECT_TYPE_DOOR) {
                CSWCDoor* pDoor = pTarget->AsSWCDoor();
                int nType = pDoor->m_nAppearanceType;
                if (nType == 0) {
                    g_pRules->Get2DArrays()->GetGenericDoors()->GetINTEntry(pDoor->GetGenericType(),
                        g_nGenericDoorsCol_SoundAppType, &nRow);
                } else {
                    g_pRules->Get2DArrays()->GetDoorTypes()->GetINTEntry(nType, g_nDoorTypesCol_SoundAppType, &nRow);
                }
                g_pRules->Get2DArrays()->GetPlaceableSounds()->GetCExoStringEntry(nRow,
                    g_nPlaceableSoundsCol_ArmorType, &sMaterial);
            } else if (pTarget->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                CSWCPlaceable* pPlaceable = pTarget->AsSWCPlaceable();
                g_pRules->Get2DArrays()->GetPlaceables()->GetINTEntry(pPlaceable->m_nAppearance,
                    g_nPlaceablesCol_SoundAppType, &nRow);
                g_pRules->Get2DArrays()->GetPlaceableSounds()->GetCExoStringEntry(nRow,
                    g_nPlaceableSoundsCol_ArmorType, &sMaterial);
            }
            sMaterial = sMaterial + CExoString(rand() % 2);
            g_pRules->Get2DArrays()->GetWeaponSounds()->GetCExoStringEntry(nSoundType, sMaterial, &sSound);
            g_pRules->Get2DArrays()->GetWeaponSounds()->GetFLOATEntry(nSoundType,
                g_nWeaponSoundPitchOffsetColumn, &fPitch);
        }
        sSound.CStr();
        g_pExoSound->PlayOneShotSoundAt(sSound.CStr(), m_vPosition, 1.5f, 15, 0, 0, 0, fPitch);
    }
    return 1;
}
