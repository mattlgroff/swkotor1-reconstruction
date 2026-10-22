// CSWSCreature weapon-discharge 2DA getters and combat resolvers
// (b005b_f2_combat).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    CExoString(const CExoString &s);
    CExoString(int n);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    CExoString operator+(const CExoString &s) const;
    char operator[](int nIndex) const;
    CExoString SubString(int nStart, int nCount) const;
    int AsINT() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
    int GetINTEntry(const CExoString &sRow, const CExoString &sColumn, int *pValue);
    int GetINTEntry(const CExoString &sRow, int nColumn, int *pValue);
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);
    int GetCExoStringEntry(const CExoString &sRow, const CExoString &sColumn, CExoString *pValue);
};

class C2DArrays {
public:
    char pad_0x00[0xc];
    C2DA *m_pRaceTable;                 // 0x0c
    char pad_0x10[0x68];
    C2DA *m_pCombatAnimations;          // 0x78
    char pad_0x7c[0x68];
    C2DA *m_pWeaponDischargeTable;      // 0xe4
    C2DA *m_pDroidDischargeTable;       // 0xe8

    C2DA *GetRaceTable() { return m_pRaceTable; }
    C2DA *GetCombatAnimations() { return m_pCombatAnimations; }
    C2DA *GetWeaponDischargeTable() { return m_pWeaponDischargeTable; }
};

class CSWRules {
public:
    char pad_0x00[0xb8];
    C2DArrays *m_p2DArrays;             // 0xb8

    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;              // 0x007A3A28

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int s);
    void AddOutOfLine(T t);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
};

struct Vector {
    float x, y, z;
};

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual CGameObject *AsDoor();        // +0x18
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual CGameObject *AsSWSCreature();    // +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual CGameObject *AsPlaceable();   // +0x48
    virtual void v4c(); virtual void v50(); virtual void v54(); virtual void v58();
    virtual void v5c(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7c(); virtual void v80(); virtual void v84(); virtual void v88();
    virtual void v8c(); virtual void v90(); virtual void v94(); virtual void v98();
    virtual short GetCurrentHitPoints(int bExcludeTemporary);   // +0x9c
    unsigned long GetID() { return m_idSelf; }
    unsigned long m_idSelf;             // +0x4
};

class CSWArea {
public:
    virtual ~CSWArea();
    char pad_0x4[0x118];
};

class CSWSArea : public CSWArea, public CGameObject {
public:
    char pad_0x124[0x6c];
    CExoArrayList<unsigned long> m_aCreatures;   // +0x190
};

class CGameEffect {
public:
    CGameEffect(int bCreateNewID);
    void SetCreator(unsigned long oidCreator);
    void SetInteger(int nIndex, int nValue);
    void SetFloat(int nIndex, float fValue);
    void SetLinked(CGameEffect *pChild1, CGameEffect *pChild2);
    void SetSubType(unsigned short nSubType) { m_nSubType = (m_nSubType & 0xfff8) | nSubType; }
    unsigned long m_nIDLow;     // 0x0
    unsigned long m_nIDHigh;    // 0x4
    unsigned short m_nType;     // 0x8
    unsigned short m_nSubType;  // 0xa
    char pad_0xc[0x80];
};

extern unsigned short EFFECT_TYPE_VISUAL_EFFECT;   // 0x007466F4
extern unsigned short SUBTYPE_MAGICAL;             // 0x007473B8
extern unsigned short EFFECT_TYPE_LINK;            // 0x0074671C
extern unsigned short EFFECT_TYPE_DEATH;           // 0x007466C8
extern unsigned char IMMUNITY_TYPE_04;             // 0x0074690C
extern unsigned char IMMUNITY_TYPE_06;             // 0x00746914
extern unsigned char IMMUNITY_TYPE_12;             // 0x0074692C
extern unsigned char IMMUNITY_TYPE_13;             // 0x00746930
extern unsigned char IMMUNITY_TYPE_16;             // 0x00746934
extern unsigned long g_nEventProjectileMiss;       // 0x007441C8

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
};

class CSWSCreature;

class CSWSCreatureStats {
public:
    int GetFeatImmunity(unsigned short nFeat);
    int GetEffectImmunity(unsigned char nType, CSWSCreature *pVersus);
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
    CGameObject *GetGameObject(unsigned long oid);
    CServerAIMaster *GetServerAIMaster();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;     // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;      // 0x007A39FC

class CSWBaseItem {
public:
    char pad_0x0[0x8e];
    unsigned char m_nWeaponWield;       // +0x8e
};

class CSWSItem {
public:
    CSWBaseItem *GetBaseItem();
    char pad_0x0[0x14];
    unsigned long m_idSelf;             // 0x14
    char pad_0x18[0xe0];
    int m_bF8;                          // 0xf8
};

class CSWInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCombatAttackData {
public:
    int GetBaseDamage();
    int GetTotalDamage(int bOffHand);
    char pad_0x0[0xc];
    unsigned long m_oidTarget;          // 0xc
    char pad_0x10[0x8];
    unsigned long m_n18;                // 0x18
    char pad_0x1c[0x2c];
    int m_bKillingBlow;                 // 0x48
    int m_bCoupDeGrace;                 // 0x4c
    char pad_0x50[0xc];
    unsigned char m_nProjectileType;    // 0x5c
    char pad_0x5d[0x7];
    unsigned short m_nAttackType;       // 0x64
    char pad_0x66[0x2];
    Vector m_vTarget;                   // 0x68
    unsigned long m_oidWeapon;          // 0x74
    char pad_0x78[0x10];
    CExoArrayList<CGameEffect *> m_alstOnHitEffects;  // 0x88
    char pad_0x94[0x99];
    unsigned char m_nAttackResult;      // 0x12d
    char pad_0x12e[0xa];
    int m_bImmune;                      // 0x138
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData *GetAttack(int nAttack);
    char pad_0x0[0x96c];
    unsigned char m_nCurrentAttack;     // 0x96c
};

struct CSWSDetectEntry {
    unsigned long m_oid;
    unsigned char m_nFlags;             // +0x4
};

class CSWSCreature : public CGameObject {
public:
    int IsCreature();
    int HasTargetBeenDetected(unsigned long oidTarget, int bCheckSeen);
    void ResolveSafeProjectile(int nAttack, unsigned long nDelay, int bUseAttack, int nHand);
    void ResolveProjectileMissImpact(Vector *pPosition, unsigned long nDelay);
    int GetWeaponImpactTime(int nWeapon, int nColumn);
    CExoString GetWeaponDischargeRow(int nWeapon);
    int GetWeaponDischargeShots(int nWeapon);
    int GetWeaponHitShots(int nWeapon);
    int GetWeaponDischargeTime(int nWeapon, int nColumn);
    int GetWeaponDischargeHand(int nWeapon, int nHand);
    int CalculateMaxElementalDamage(CSWSItem *pItem, int nUnused);
    int CalculateDamage(int nType, CSWSItem *pItem, int *pOut, int a4, int a5,
                        int a6, int a7, unsigned char a8, unsigned char a9, int a10);
    CSWSDetectEntry *GetDetectEntry(unsigned long oid);
    CGameEffect *CreateLinkedEffect(unsigned short nType1, int nValue1, unsigned short nType2, int nValue2);
    CGameEffect *CreateLinkedEffect(CGameEffect *pOther, unsigned short nType, int nValue);
    void SetAttackSummaryFeedbackEffectData(int nUnused, unsigned long oidTarget);
    void ResolvePostMeleeDamage(CGameObject *pTarget);
    void ApplyOnHitInstantDeath(int nUnused1, int nUnused2);
    unsigned long GetFirstAttacker();
    CSWSArea *GetArea();
    void BroadcastSafeProjectile(unsigned long oidSource, unsigned long oidTarget,
                                 Vector *pSource, Vector *pTarget, unsigned long nDelay,
                                 unsigned char nProjectileType, int nHand,
                                 unsigned char nWield, unsigned char bSpecial,
                                 unsigned long nExtra);

    char pad_0x08[0x88];
    Vector m_vPosition;                 // 0x90
    char pad_0x9c[0x468];
    unsigned long m_oidAttackTarget;    // 0x504
    char pad_0x508[0x4c0];
    CSWSCombatRound *m_pcCombatRound;   // 0x9c8
    char pad_0x9cc[0x60];
    CSWInventory *m_pInventory;         // 0xa2c
    char pad_0xa30[0x30];
    unsigned short m_nAppearanceType;   // 0xa60
    char pad_0xa62[0x12];
    CSWSCreatureStats *m_pStats;        // 0xa74
    CSWSCreatureStats *GetStats() { return m_pStats; }
    char pad_0xa78[0x8];
    CExoArrayList<unsigned long> *m_pAttackers;  // 0xa80
    int m_bA84;                         // 0xa84
    int m_bA88;                         // 0xa88
};

// FUNCTION: SWKOTOR 0x005B47C0
int CSWSCreature::IsCreature()
{
    int bCreature = 0;
    CExoString sModelType;
    CExoString sColumn("MODELTYPE");
    g_pRules->Get2DArrays()->GetRaceTable()->GetCExoStringEntry(m_nAppearanceType, sColumn, &sModelType);
    char c = sModelType[0];
    if (c == 'S' || c == 'L' || c == 's' || c == 'l')
        bCreature = 1;
    return bCreature;
}

// NONMATCH (9): the original keeps the constant 1 in ebp (mov ebp,1 before the
// GetGameObject call) and returns it with mov eax,ebp from one shared exit;
// ours folds bDetected to an immediate. Control flow and tests are identical.
// FUNCTION: SWKOTOR 0x005B48F0
int CSWSCreature::HasTargetBeenDetected(unsigned long oidTarget, int bCheckSeen)
{
    int bDetected = 1;
    CGameObject *pObject = g_pAppManager->m_pServerExoApp->GetGameObject(oidTarget);
    if (pObject && (pObject->AsDoor() || pObject->AsPlaceable()))
        return bDetected;
    if (m_bA88)
        return bDetected;
    CSWSDetectEntry *pEntry = GetDetectEntry(oidTarget);
    if (pEntry) {
        if (!bCheckSeen || (pEntry->m_nFlags & 0xc) != 4) {
            if (!(pEntry->m_nFlags & 0x10) && (pEntry->m_nFlags & 1))
                return bDetected;
        }
    }
    return 0;
}

// NONMATCH (10): register allocation only: the original keeps this in ebx and
// pWeapon in edi (ours swapped). Arg-slot reuse for the byte locals and the
// switch (movzx/cmp eax) already match.
// FUNCTION: SWKOTOR 0x005B49C0
void CSWSCreature::ResolveSafeProjectile(int nAttack, unsigned long nDelay, int bUseAttack, int nHand)
{
    unsigned long nSlot = 0;
    if (nHand == 0)
        nSlot = 0x10;
    else if (nHand == 1)
        nSlot = 0x20;
    CSWSItem *pWeapon = m_pInventory->GetItemInSlot(nSlot);
    if (!pWeapon)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(nAttack);
    pAttack->m_oidWeapon = pWeapon->m_idSelf;
    unsigned char nProjectileType;
    if (bUseAttack)
        nProjectileType = pAttack->m_nProjectileType;
    else
        nProjectileType = 4;
    pAttack->GetBaseDamage();
    unsigned char nWield = pWeapon->GetBaseItem()->m_nWeaponWield;
    unsigned char bSpecial = 0;
    switch (pAttack->m_nAttackType) {
    case 0x12:
    case 0x1d:
    case 0x52:
        bSpecial = 1;
        break;
    }
    BroadcastSafeProjectile(GetID(), pAttack->m_oidTarget, &m_vPosition, &pAttack->m_vTarget,
                            nDelay, nProjectileType, nHand, nWield, bSpecial, pAttack->m_n18);
}

// FUNCTION: SWKOTOR 0x005B4E30
void CSWSCreature::ResolveProjectileMissImpact(Vector *pPosition, unsigned long nDelay)
{
    CGameEffect *pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_VISUAL_EFFECT;
    pEffect->SetSubType(SUBTYPE_MAGICAL);
    pEffect->SetCreator(m_idSelf);
    pEffect->SetInteger(0, 0xfc0);
    pEffect->SetFloat(0, pPosition->x);
    pEffect->SetFloat(1, pPosition->y);
    pEffect->SetFloat(2, pPosition->z);
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
        0, nDelay, GetID(), GetArea()->GetID(), g_nEventProjectileMiss, pEffect);
}

// FUNCTION: SWKOTOR 0x005B4F30
int CSWSCreature::GetWeaponImpactTime(int nWeapon, int nColumn)
{
    CExoString sRow(nWeapon);
    int nValue = 0;
    g_pRules->Get2DArrays()->GetCombatAnimations()->GetINTEntry(sRow, nColumn + 1, &nValue);
    return nValue;
}

// FUNCTION: SWKOTOR 0x005B5230
int CSWSCreature::GetWeaponDischargeShots(int nWeapon)
{
    CExoString sRow = GetWeaponDischargeRow(nWeapon);
    int nShots = 0;
    g_pRules->Get2DArrays()->GetWeaponDischargeTable()->GetINTEntry(sRow, "Shots", &nShots);
    return nShots;
}

// FUNCTION: SWKOTOR 0x005B52E0
int CSWSCreature::GetWeaponHitShots(int nWeapon)
{
    CExoString sRow = GetWeaponDischargeRow(nWeapon);
    int nHits = 0;
    g_pRules->Get2DArrays()->GetWeaponDischargeTable()->GetINTEntry(sRow, "Hits", &nHits);
    return nHits;
}

// FUNCTION: SWKOTOR 0x005B5390
int CSWSCreature::GetWeaponDischargeTime(int nWeapon, int nColumn)
{
    CExoString sRow = GetWeaponDischargeRow(nWeapon);
    int nValue = 0;
    g_pRules->Get2DArrays()->GetWeaponDischargeTable()->GetINTEntry(sRow, nColumn + 4, &nValue);
    return nValue;
}

// FUNCTION: SWKOTOR 0x005B5420
int CSWSCreature::GetWeaponDischargeHand(int nWeapon, int nHand)
{
    CExoString sRow = GetWeaponDischargeRow(nWeapon);
    CExoString sMask;
    g_pRules->Get2DArrays()->GetWeaponDischargeTable()->GetCExoStringEntry(sRow, "SwitchMask", &sMask);
    return sMask.SubString(nHand, 1).AsINT();
}

// FUNCTION: SWKOTOR 0x005B5640
int CSWSCreature::CalculateMaxElementalDamage(CSWSItem *pItem, int nUnused)
{
    if (pItem && !pItem->m_bF8) {
        int nOut = 0;
        return CalculateDamage(2, pItem, &nOut, 1, 1, 0, 0, 0xff, 0xff, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005B5C40 SYMBOL: ?CreateLinkedEffect@CSWSCreature@@QAEPAVCGameEffect@@GHGH@Z
CGameEffect *CSWSCreature::CreateLinkedEffect(unsigned short nType1, int nValue1, unsigned short nType2, int nValue2)
{
    CGameEffect *pEffect1 = new CGameEffect(1);
    if (!pEffect1)
        return 0;
    CGameEffect *pEffect2 = new CGameEffect(1);
    if (!pEffect2)
        return 0;
    CGameEffect *pLink = new CGameEffect(1);
    if (!pLink)
        return 0;
    pEffect1->m_nType = nType1;
    pEffect1->SetInteger(0, nValue1);
    pEffect2->m_nType = nType2;
    pEffect2->SetInteger(0, nValue2);
    pLink->m_nType = EFFECT_TYPE_LINK;
    pLink->SetCreator(m_idSelf);
    pLink->SetLinked(pEffect1, pEffect2);
    return pLink;
}

// FUNCTION: SWKOTOR 0x005B5DA0 SYMBOL: ?CreateLinkedEffect@CSWSCreature@@QAEPAVCGameEffect@@PAV2@GH@Z
CGameEffect *CSWSCreature::CreateLinkedEffect(CGameEffect *pOther, unsigned short nType, int nValue)
{
    CGameEffect *pEffect = new CGameEffect(1);
    if (!pEffect)
        return 0;
    CGameEffect *pLink = new CGameEffect(1);
    if (!pLink)
        return 0;
    pEffect->m_nType = nType;
    pEffect->SetInteger(0, nValue);
    pLink->m_nType = EFFECT_TYPE_LINK;
    pLink->SetCreator(m_idSelf);
    pLink->SetLinked(pOther, pEffect);
    return pLink;
}

// FUNCTION: SWKOTOR 0x005B60F0
void CSWSCreature::SetAttackSummaryFeedbackEffectData(int nUnused, unsigned long oidTarget)
{
    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    switch (pAttack->m_nAttackResult) {
    case 6:
        if (pTarget->GetStats()->GetEffectImmunity(IMMUNITY_TYPE_13, this))
            pAttack->m_bImmune = 1;
        break;
    case 5:
        if (pTarget->GetStats()->GetEffectImmunity(IMMUNITY_TYPE_06, this) ||
            pTarget->GetStats()->GetFeatImmunity(0x64))
            pAttack->m_bImmune = 1;
        break;
    case 4:
        if (pTarget->GetStats()->GetEffectImmunity(IMMUNITY_TYPE_12, this) ||
            pTarget->GetStats()->GetFeatImmunity(0x63) ||
            pTarget->GetStats()->GetFeatImmunity(0x64))
            pAttack->m_bImmune = 1;
        break;
    case 1:
        if (pTarget->GetStats()->GetEffectImmunity(IMMUNITY_TYPE_16, this))
            pAttack->m_bImmune = 1;
        break;
    case 2:
        if (pTarget->GetStats()->GetEffectImmunity(IMMUNITY_TYPE_04, this) ||
            pTarget->GetStats()->GetFeatImmunity(0x62) ||
            pTarget->GetStats()->GetFeatImmunity(0x63) ||
            pTarget->GetStats()->GetFeatImmunity(0x64))
            pAttack->m_bImmune = 1;
        break;
    }
}

// NONMATCH (8): scheduling only: the original moves the damage into ebx
// right after GetTotalDamage and compares against movsx ecx (ours: eax).
// FUNCTION: SWKOTOR 0x005B8B00
void CSWSCreature::ResolvePostMeleeDamage(CGameObject *pTarget)
{
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    int nDamage = pAttack->GetTotalDamage(1);
    if (nDamage >= pTarget->GetCurrentHitPoints(0))
        pAttack->m_bKillingBlow = 1;
    if (pTarget->AsSWSCreature()) {
        pTarget->AsSWSCreature();
        if (pAttack->m_bCoupDeGrace) {
            CGameEffect *pEffect = new CGameEffect(1);
            pEffect->m_nType = EFFECT_TYPE_DEATH;
            pEffect->SetSubType(SUBTYPE_MAGICAL);
            pEffect->SetCreator(m_idSelf);
            pEffect->SetInteger(0, 0);
            pEffect->SetInteger(1, 1);
            pAttack->m_alstOnHitEffects.AddOutOfLine(pEffect);
        }
    }
}

// FUNCTION: SWKOTOR 0x005BA300
void CSWSCreature::ApplyOnHitInstantDeath(int nUnused1, int nUnused2)
{
    CGameEffect *pEffect = new CGameEffect(1);
    if (!pEffect)
        return;
    pEffect->m_nType = EFFECT_TYPE_DEATH;
    pEffect->SetSubType(SUBTYPE_MAGICAL);
    pEffect->SetCreator(m_idSelf);
    pEffect->SetInteger(0, 0);
    pEffect->SetInteger(1, 1);
    m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack)->m_alstOnHitEffects.Add(pEffect);
}

// FUNCTION: SWKOTOR 0x005BA400
unsigned long CSWSCreature::GetFirstAttacker()
{
    CSWSArea *pArea = GetArea();
    if (!pArea)
        return 0x7f000000;
    if (m_pAttackers)
        delete m_pAttackers;
    m_pAttackers = new CExoArrayList<unsigned long>;
    for (int i = 0; i < pArea->m_aCreatures.num; i++) {
        unsigned long oid = pArea->m_aCreatures.element[i];
        CSWSCreature *pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oid);
        if (pCreature && pCreature->m_oidAttackTarget == m_idSelf)
            m_pAttackers->Add(oid);
    }
    if (m_pAttackers->num > 0) {
        m_bA84 = 1;
        return m_pAttackers->element[0];
    }
    return 0x7f000000;
}
