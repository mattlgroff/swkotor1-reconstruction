// CSWSCreature combat round resolution (nwscreatureaicombat.o): damage,
// on-hit effects / visuals, melee and ranged attack resolution, parry and
// damage-shield helpers in 0x005B4AA0-0x005BBBF0.
//
// Names come from the debug map (matching/mac_names.csv) where it has them;
// placeholder member names keep their offset in the name.

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

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

class Vector;
Vector operator-(const Vector &a, const Vector &b);         // 0x00423BC0

class Vector {
public:
    float x, y, z;
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                          // 0x00503E40
    ~CGameEffect();                                         // 0x00503590
    int GetInteger(int nIndex);                             // 0x00503690
    void SetInteger(int nIndex, int nValue);                // 0x005036A0
    void SetCreator(unsigned long oid);                     // 0x00503A00
    void SetNumIntegers(int nNum);                          // 0x00503630
    void SetObjectID(int nIndex, unsigned long oid);        // 0x005036F0
    void UpdateLinked();                                    // 0x00504240
    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }
    void SetSubType(unsigned short nSubType)
    {
        if (nSubType == 0x08)
            m_nSubType = (m_nSubType & 0xffef) | 0x08;
        else if (nSubType == 0x10)
            m_nSubType = (m_nSubType & 0xfff7) | 0x10;
        else if (nSubType == 0x18)
            m_nSubType |= 0x18;
    }

    unsigned long m_nIDLow;             // 0x0
    unsigned long m_nIDHigh;            // 0x4
    unsigned short m_nType;             // 0x8
    unsigned short m_nSubType;          // 0xa
    float m_fDuration;                  // 0xc
    char pad_0x10[0x8c - 0x10];
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                                     // 0x004D69F0
    ~CSWCCMessageData();                                    // 0x004D6A40
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0
    void SetFloat(int nIndex, float fValue);                // 0x004D7720
    void SetObjectID(int nIndex, unsigned long oid);        // 0x004D7780
    unsigned char m_nType;              // 0x0
    char pad_0x1[0x34 - 0x1];
};

class CSWItemProperty {
public:
    unsigned short m_nPropertyName;     // 0x0
    unsigned short m_nSubType;          // 0x2
    char pad_0x4[0x9 - 0x4];
    unsigned char m_nParam1Value;       // 0x9
    unsigned char GetParam1Value() { return m_nParam1Value; }
};

extern int EFFECT_TYPE_VISUALEFFECT;   // 0x007466F4 (0x1e)
extern int EFFECT_TYPE_SETSTATE;       // 0x0074669C (8)
extern int EFFECT_TYPE_MOVEMENT_SPEED_DECREASE;  // 0x00746688 (3, placeholder)
extern int EFFECT_TYPE_POISON;         // 0x00746708 (0x23, placeholder)
extern int EFFECT_TYPE_ABILITY_DECREASE;  // 0x00746710 (0x25, placeholder)
extern int EFFECT_TYPE_746788;         // 0x00746788 (0x43)
extern int EFFECT_TYPE_DEATH;          // 0x007466C8
extern int EFFECT_TYPE_DAMAGE;         // 0x00746714
extern int EFFECT_TYPE_DAMAGE_SHIELD;  // 0x00746770
extern int EFFECT_TYPE_FORCE_JUMP;     // 0x00746810

extern int g_nDamageIndex;             // 0x008327E0
extern const float g_fInvLog10Of2;     // 0x00745FF8

static inline void FloatToInt(int *pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

extern "C" int __cdecl rand();
extern "C" double __cdecl log10(double x);
#pragma intrinsic(log10)
extern unsigned short DURATION_TYPE_INSTANT;      // 0x007473B8 (0)
extern unsigned short DURATION_TYPE_TEMPORARY;    // 0x007473C0 (1)
extern unsigned short DURATION_TYPE_PERMANENT;    // 0x007473C4

float Magnitude(const Vector &v);                           // 0x004AB100 (cdecl)
Vector NormalizeVector(const Vector &v);                    // 0x004AB180 (cdecl)
extern unsigned char IMMUNITY_TYPE_SNEAK_ATTACK;   // 0x0074696C (0x1e, placeholder)
extern unsigned char IMMUNITY_TYPE_CRITICAL_HIT;   // 0x00746970 (0x1f, placeholder)

class CSWSCreatureListEntry {
public:
    unsigned long m_oid;                // 0x0
    unsigned char m_nFlags;             // 0x4
};

class CExoString {
public:
    CExoString();                                           // 0x005B3190
    CExoString(const char *s);                              // 0x005E5A90
    ~CExoString();                                          // 0x005E5C20
    CExoString &operator=(const char *s);                   // 0x005E5140
    char *m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);  // 0x00413660
    int GetFLOATEntry(int nRow, const CExoString &sColumn, float *pValue);  // 0x00413430
};

class C2DArrays {
public:
    char pad_0x0[0x64];
    C2DA *m_pAmmunitionTypes;           // 0x64
    char pad_0x68[0x7c - 0x68];
    C2DA *m_pDifficultyOptions;         // 0x7c
    C2DA *GetDifficultyOptions() { return m_pDifficultyOptions; }
    char pad_0x80[0xb0 - 0x80];
    C2DA *m_pIPRPDamage;                // 0xb0
    char pad_0xb4[0xd0 - 0xb4];
    C2DA *m_pDamageHitVisual;           // 0xd0
    C2DA *GetDamageHitVisual() { return m_pDamageHitVisual; }
};

class CSWRules {
public:
    unsigned short RollDice(unsigned char nDice, unsigned char nSides);  // 0x00550B60

    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;             // 0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;              // 0x007A3A28
extern int EFFECT_TYPE_PROPERTY55;      // 0x007467E8
extern int EFFECT_TYPE_PROPERTY56;      // 0x007467EC

class CSWSCombatAttackData {
public:
    int GetDamage(unsigned short nDamageFlags);                        // 0x004D1F80
    int GetTotalDamage(int bOffHand);
    int GetBaseDamage();                                    // 0x004D2060                       // 0x004D2180
    void SetBaseDamage(int nDamage);                        // 0x004D2030
    void SetDamage(int nFlags, int nDamage);     // 0x004D2070
    void ResolveAttackToMeleeSubAttack(int nAttack, int nImpactTime);  // 0x004D5A50
    void ResolveAttackToRangedSubAttack(int nShot, int nTime, int nShots, int bParried,
                                        int nHand);         // 0x004D5B60

    unsigned char m_nAttackId;          // 0x0
    char pad_0x1[0xc - 0x1];
    unsigned long m_oidTarget;          // 0xc
    char pad_0x10[0x3a - 0x10];
    unsigned char m_nWeaponAttackType;  // 0x3a
    char pad_0x3b[0x40 - 0x3b];
    int m_bRangedAttack;                // 0x40
    int m_n44;                          // 0x44
    int m_bKillingBlow;                 // 0x48
    int m_bCoupDeGrace;                 // 0x4c
    char pad_0x50[0x5c - 0x50];
    unsigned char m_nAttackResult;      // 0x5c
    char pad_0x5d[0x64 - 0x5d];
    unsigned short m_nAttackType;       // 0x64
    char pad_0x66[0x88 - 0x66];
    CExoArrayList<CGameEffect *> m_alstOnHitEffects;        // 0x88
    char pad_0x94[0xa0 - 0x94];
    CExoArrayList<CSWCCMessageData *> m_alstFeedback;       // 0xa0
    char pad_0xac[0xb8 - 0xac];
    int m_bB8;                          // 0xb8
    int m_nBC;                          // 0xbc
    char pad_0xc0;
    unsigned char m_nToHitRoll;         // 0xc1
    char m_nToHitMod;                   // 0xc2
    char pad_0xc3[0xd8 - 0xc3];
    int m_nD8;                          // 0xd8
    int m_nDC;                          // 0xdc
    char pad_0xe0[0xec - 0xe0];
    int m_nEC;                          // 0xec
    char pad_0xf0[0xf4 - 0xf0];
    char m_nF4;                         // 0xf4
    char pad_0xf5[0x12c - 0xf5];
    unsigned char m_nSaveType;          // 0x12c (saving throw feedback)
    unsigned char m_nSaveRoll;          // 0x12d
    unsigned char m_nSaveSubType;       // 0x12e
    unsigned char m_nSaveDC;            // 0x12f
    unsigned char m_nSaveBonus;         // 0x130
    unsigned char m_nSaveBase;          // 0x131
    unsigned char m_nSaveTotal;         // 0x132
    unsigned char m_nSaveDCByte;        // 0x133
    unsigned char m_nSaveResult;        // 0x134
    char pad_0x135[0x138 - 0x135];
    int m_n138;                         // 0x138
    char pad_0x13c[0x140 - 0x13c];
    unsigned char m_nDeflectRoll;       // 0x140
    unsigned char m_nDeflectDC;         // 0x141
    unsigned char m_nDeflectFeat;       // 0x142
    unsigned char m_nDeflectFeatBonus;  // 0x143
    unsigned char m_nDeflectEffectBonus;  // 0x144
    unsigned char m_nDeflectResult;     // 0x145
    unsigned char m_nDeflectBAB;        // 0x146
    unsigned char m_nDeflectAbility;    // 0x147
    unsigned char m_nDeflectTotal;      // 0x148
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData *GetAttack(int nAttack);           // 0x004D2B50
    void SetOffHandAttack(int bOffHand);                    // 0x004D29F0
    int GetOffHandAttack();                                 // 0x004D2A00
    unsigned char GetTotalAttacks();                        // 0x004D2EB0
    unsigned char GetWeaponAttackType();                    // 0x004D3DA0
    CSWSCombatAttackData *GetCurrentAttack() { return GetAttack(m_nCurrentAttack); }
    unsigned char GetNextAttackID() { return ++m_nAttackID; }
    unsigned char GetCurrentAttackIndex() { return m_nCurrentAttack; }

    char pad_0x0[0x96c];
    unsigned char m_nCurrentAttack;     // 0x96c
    char pad_0x96d[0x974 - 0x96d];
    unsigned char m_nAttackID;          // 0x974
    char pad_0x975[0x97c - 0x975];
    int m_bWeaponIneffectiveSent;       // 0x97c (placeholder name)
    char pad_0x980[0x990 - 0x980];
    unsigned char m_nOnHandAttacks;     // 0x990
};

class CSWGuiInGame {
public:
    char pad_0x0[0xb4];
    int m_bB4;                          // 0xb4
};

class CClientOptions {
public:
    char pad_0x0[0x4];
    unsigned char m_nDifficulty;        // 0x4
};

class CClientExoApp {
public:
    CClientOptions *GetClientOptions();                     // 0x005ED700
    CSWGuiInGame *GetInGameGui();                           // 0x005ED690
    int GetClientRangedAnimation(unsigned long oidSelf, int nAttackType, int a3,
                                 int a4);                   // 0x005EDCC0
    int GetClientMeleeAnimation(unsigned long oidSelf, unsigned long oidTarget,
                                unsigned long nAppearance, int nAttackType, int a5,
                                int a6);            // 0x005EDCE0
};

class CResRef {
public:
    CResRef(const char *s);                                 // 0x00406D80
    char m_resRef[16];
};

class CSWPartyTable {
public:
    int GetIsNPCObjectMember(unsigned long oid);            // 0x00563740
};

class CSWSCreature;
class CGameObject;
class CSWSClient;

class CServerExoApp {
public:
    CGameObject *GetGameObject(unsigned long oid);          // 0x004AE750
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);  // 0x004AE770
    CSWSClient *GetClientObjectByObjectID(unsigned long oid);    // 0x004AE910
    CSWPartyTable *GetPartyTable();                         // 0x004AEE70
    // Name descriptive: the line-walk test the AI uses before a force jump.
    int TestDirectLine(unsigned long oidSelf, CResRef &cResRef, const Vector &vStart,
                       const Vector &vEnd, float *pRadius, float fRange, int a7,
                       int a8);                             // 0x004AEE80
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp *m_pClientExoApp;     // 0x4
    CServerExoApp *m_pServerExoApp;     // 0x8
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;      // 0x007A39FC
extern int g_bDebugNoDamage;            // 0x00832878 (placeholder name)

class CSWSCreature;
class CSWSObject;

class CSWSCombatActionData {
public:
    char pad_0x0[0x4];
    float m_fRadius;                    // 0x4
    char pad_0x8[0x14 - 0x8];
    float m_fRange;                     // 0x14
    char pad_0x18[0x30 - 0x18];
    unsigned long m_oidTarget;          // 0x30
};

class CSWCCreature {
public:
    int GetForceShield();                                   // 0x00616890

    char pad_0x0[0x30];
    Vector m_vOrientation;              // 0x30
};

class CSWBaseItem {
public:
    char pad_0x0[0x8e];
    unsigned char m_nAmmunitionType;    // 0x8e
};

class CSWSItem {
public:
    CSWBaseItem *GetBaseItem();                             // 0x005B4790
};

class CSWInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);           // 0x005A4C20
};

class CSWSCreatureStats {
public:
    int HasFeat(unsigned short nFeat);                      // 0x005A6630
    int GetBaseAttackBonus();                               // 0x005A60D0
    unsigned char GetLevel(int bFlag);                      // 0x005A5FD0
    int GetEffectImmunity(unsigned char nType, CSWSCreature *pVersus);  // 0x005A6960
    int GetDamageRoll(CSWSObject *pTarget, int bOffHand, int bSneak, int n44, int a5,
                      int a6);                              // 0x005A9050

    char pad_0x0[0xdc];
    unsigned short m_nRace;             // 0xdc
    unsigned short GetRace() { return m_nRace; }
    char pad_0xde[0x156 - 0xde];
    unsigned short m_nDamageShieldEffectStart;  // 0x156 (placeholder name)
    char pad_0x158[0x16c - 0x158];
    unsigned char m_nDexterityModifier; // 0x16c (placeholder name)
    unsigned char m_nDexterityBonus;    // 0x16d (placeholder name)
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation); // +0x7c
    virtual void v80(); virtual void v84(); virtual void v88();
    virtual void v8c(); virtual void v90(); virtual void v94(); virtual void v98();
    virtual short GetCurrentHitPoints(int bExcludeTemporary);   // +0x9c
    unsigned long m_idSelf;             // +0x4
};

class CSWSObject : public CGameObject {
public:
    char pad_0x8[0x90 - 0x8];
    Vector m_vPosition;                 // 0x90
    Vector m_vOrientation;              // 0x9c
    char pad_0xa8[0xd4 - 0xa8];
    unsigned long m_nAppearanceType;    // 0xd4
    char pad_0xd8[0xf8 - 0xd8];
    int m_bF8;                          // 0xf8
    char pad_0xfc[0x124 - 0xfc];
    CExoArrayList<CGameEffect *> m_appliedEffects;  // 0x124
};

class CSWSCreature : public CSWSObject {
public:
    int GetDeadTemp();
    void ApplyEffect(CGameEffect *pEffect, int a2, int a3);    // 0x004D14F0
    void ResolveDamageShields(CSWSCreature *pAttacker);
    int ResolveMoveToForceJump(CSWSObject *pTarget, unsigned short *pFeat);
    CSWCCreature *GetClientCreature();                      // 0x004EFB20
    int IsLightsaberEquipped();                             // 0x005B5690                                      // 0x004EF890
    int GetIsWeaponEffective(unsigned long oidTarget, int bOffHand);  // 0x004EDB20
    int GetDamageFlags();                        // 0x004ED5E0
    void BroadcastVoiceChat(unsigned char nType);           // 0x004F2520
    void BroadcastSavingThrowData(CSWCCMessageData *pData); // 0x004EC760
    void BroadcastAttackSummaryToParty(CSWCCMessageData *pData);  // 0x004EC9E0
    void SendAttackSummaryFeedback(unsigned long oidTarget);
    CSWSCreatureListEntry *GetFromList920(unsigned long oid);  // 0x00517A20
    // Name descriptive (sets the attack's sneak-attack flag +0x44).
    void ResolveSneakAttack(CSWSCreature *pTarget);
    int GetWeaponImpactTime(int nAnimation, int nAttack);   // 0x005B4F30
    int GetWeaponDischargeShots(int nAnimation);            // 0x005B5230
    int GetWeaponHitShots(int nAnimation);                  // 0x005B52E0
    int GetWeaponDischargeTime(int nAnimation, int nShot);  // 0x005B5390
    int GetWeaponDischargeHand(int nAnimation, int nShot);  // 0x005B5420
    void ResolveRangedAnimations(CSWSObject *pTarget, unsigned short nLength, int bNoReaction);
    void ResolveRangedHit(CSWSObject *pTarget);             // 0x005B6DB0
    void ResolveRangedMiss(CSWSObject *pTarget);            // 0x005B6E90
    void SetRangeState(int nValue);                         // 0x005B7400
    void ResolveMeleeAnimations(int nAttack, int a2, CSWSObject *pTarget, int nLength,
                                int bForceReaction);        // 0x005B7470
    int ResolveDefensiveEffects(CSWSObject *pTarget, int bHit);
    int IsProjectileParryable(CSWSObject *pTarget, int a2, int a3, int &bDeflect,
                              int nHand);                   // 0x005B78E0
    void ResolvePostMeleeDamage(CSWSCreature *pTarget);     // 0x005B8B00
    void ResolvePostRangedDamage(CSWSObject *pTarget);      // 0x005B8C20
    void ResolveOnHitVisuals(CSWSObject *pTarget);          // 0x005B9040
    // The callers push the whole effect-type dword (the debug map has
    // unsigned short parameters).
    CGameEffect *CreateLinkedEffect(int nType1, int nValue1, int nType2,
                                    int nValue2);           // 0x005B5C40
    CGameEffect *CreateLinkedEffect(CGameEffect *pOther, int nType, int nValue);  // 0x005B5DA0
    void ApplyOnHitConfusion(CSWSObject *pTarget, int nRounds);
    void ApplyOnHitFear(CSWSObject *pTarget, int nRounds);
    void ApplyOnHitHold(CSWSObject *pTarget, int nRounds);
    void ApplyOnHitSleep(CSWSObject *pTarget, int nRounds);
    void ApplyOnHitSlow(CSWSObject *pTarget, int nRounds);
    void ApplyOnHitStun(CSWSObject *pTarget, int nRounds);
    void ApplyOnHitAbilityDamage(CSWSObject *pTarget, CSWItemProperty *pProperty);
    void ApplyOnHitPoison(CSWSObject *pTarget, CSWItemProperty *pProperty);
    void ApplyOnHitSlayRacialGroup(CSWSObject *pTarget, CSWItemProperty *pProperty);
    CSWSCombatAttackData *GetCurrentAttack()
    {
        return m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    }
    void ResolveRangedSpecialAttack(CSWSObject *pTarget, int nShot);   // 0x005BA540
    void ResolveMeleeSpecialAttack(int nAttack, int a2, CSWSObject *pTarget,
                                   int nImpactTime);        // 0x005BA8E0
    void ResolveAttackRoll(CSWSObject *pTarget, int bOverride, unsigned char nOverrideResult);
    void ResolveOnHitEffect(CSWSObject *pTarget, int bOffHand, int bSneak);  // 0x005BAFA0
    void ResolveDamage(CSWSObject *pTarget, int bFixed, int nAmount);
    void ResolveRangedAttack(CSWSObject *pTarget, int nAttacks, int bNoReaction, int bFixed,
                             int nAnimation, int a6, int nAmount);
    void ResolveMeleeAttack(CSWSObject *pTarget, int a2, int nLength, int bFixed, int a5,
                            int a6, int nAmount);

    CSWSCreatureStats *GetStats() { return m_pStats; }
    int GetIsRangeState0(int nValue)
    {
        int bResult = 0;
        if (nValue == -1) {
            if (m_nRangeState == 0)
                return 1;
        } else if (nValue > 0 && nValue <= 3) {
            bResult = 1;
        }
        return bResult;
    }

    int GetIsRangeState1(int nValue)
    {
        int bResult = 0;
        if (nValue == -1) {
            if (m_nRangeState == 1)
                return 1;
        } else if (nValue >= 4 && nValue <= 6) {
            bResult = 1;
        }
        return bResult;
    }
    int GetIsRangeState2(int nValue)
    {
        int bResult = 0;
        if (nValue == -1) {
            if (m_nRangeState == 2)
                return 1;
        } else if (nValue >= 8 && nValue <= 10) {
            bResult = 1;
        }
        return bResult;
    }

    char pad_0x130[0x340 - 0x130];
    CSWSCombatActionData *m_pForceJumpData;  // 0x340 (placeholder name)
    char pad_0x344[0x550 - 0x344];
    int m_n550;                         // 0x550
    char pad_0x554[0x558 - 0x554];
    int m_nRangeState;                  // 0x558
    char pad_0x55c[0x8d8 - 0x55c];
    int m_bDeflectBack;                 // 0x8d8
    int m_bForceShield;                 // 0x8dc
    char pad_0x8e0[0x8ed - 0x8e0];
    unsigned char m_bStunned;           // 0x8ed
    char pad_0x8ee[0x9c8 - 0x8ee];
    CSWSCombatRound *m_pcCombatRound;   // 0x9c8
    char pad_0x9cc[0x9d4 - 0x9cc];
    int m_b9D4;                         // 0x9d4
    char pad_0x9d8[0xa04 - 0x9d8];
    int m_bA04;                         // 0xa04
    char pad_0xa08[0xa2c - 0xa08];
    CSWInventory *m_pInventory;         // 0xa2c
    char pad_0xa30[0xa74 - 0xa30];
    CSWSCreatureStats *m_pStats;        // 0xa74
    char pad_0xa78[0xa88 - 0xa78];
    int m_bA88;                         // 0xa88
};


// The ApplyOnHit* handlers are the targets of ResolveOnHitEffect's
// member-function table (names descriptive). 0x005B9670, 0x005B9770,
// 0x005B9870, 0x005B9D40, 0x005BA0D0 and 0x005BA170 have no row in
// functions.csv (they follow the previous function's padding).

// FUNCTION: SWKOTOR 0x005B9670
void CSWSCreature::ApplyOnHitConfusion(CSWSObject *pTarget, int nRounds)
{
    CGameEffect *pLink = CreateLinkedEffect(EFFECT_TYPE_SETSTATE, 1, EFFECT_TYPE_VISUALEFFECT, 0xd0);
    if (!pLink)
        return;
    CGameEffect *pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_VISUALEFFECT, 0xcf);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x11);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = nRounds * 6.0f;
    pLink->SetSubType(0x08);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
}

// FUNCTION: SWKOTOR 0x005B8E50
int CSWSCreature::ResolveDefensiveEffects(CSWSObject *pTarget, int bHit)
{
    int bResult = 0;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    int nDeflect;
    if (!IsProjectileParryable(pTarget, bHit, bResult, nDeflect, -1))
        return bResult;
    if (!pTarget->AsSWSCreature())
        return bResult;
    CSWSCreature *pCreature = pTarget->AsSWSCreature();
    if (pCreature->m_bForceShield) {
        if (pCreature->m_bDeflectBack) {
            pAttack->m_nAttackResult = 9;
            pAttack->m_nDeflectResult = 9;
            return 1;
        }
        pAttack->m_nAttackResult = 8;
        pAttack->m_nDeflectResult = 8;
        return 1;
    }
    int nTotal = g_pRules->RollDice(1, 20);
    pAttack->m_nDeflectRoll = (unsigned char)nTotal;
    if (pCreature->m_pStats->HasFeat(0x18)) {
        nTotal += 6;
        pAttack->m_nDeflectFeat = 0x18;
        pAttack->m_nDeflectFeatBonus = 6;
    } else if (pCreature->m_pStats->HasFeat(1)) {
        nTotal += 3;
        pAttack->m_nDeflectFeat = 1;
        pAttack->m_nDeflectFeatBonus = 3;
    }
    nTotal += pCreature->m_pStats->GetBaseAttackBonus();
    pAttack->m_nDeflectBAB = (unsigned char)pCreature->m_pStats->GetBaseAttackBonus();
    CSWSCreatureStats *pStats = pCreature->m_pStats;
    nTotal += (char)(pStats->m_nDexterityBonus + pStats->m_nDexterityModifier);
    pAttack->m_nDeflectAbility = pStats->m_nDexterityBonus + pStats->m_nDexterityModifier;
    CExoArrayList<CGameEffect *> *pEffects = &pCreature->m_appliedEffects;
    if (pEffects) {
        int nBonus = 0;
        for (int i = 0; i < pEffects->num; i++) {
            CGameEffect *pEffect = pEffects->element[i];
            if (pEffect->m_nType == EFFECT_TYPE_PROPERTY55 || pEffect->m_nType == EFFECT_TYPE_PROPERTY56)
                nBonus += pEffect->GetInteger(1);
        }
        nTotal += nBonus;
        pAttack->m_nDeflectEffectBonus = (unsigned char)nBonus;
    }
    int nNeed = pAttack->m_nToHitMod + pAttack->m_nToHitRoll;
    pAttack->m_nDeflectTotal = (unsigned char)nTotal;
    pAttack->m_nDeflectDC = (unsigned char)nNeed;
    if (nTotal < nNeed)
        return 0;
    nNeed += 6;
    if (nTotal < nNeed) {
        pAttack->m_nAttackResult = 8;
        pAttack->m_nDeflectResult = 8;
    } else if (bHit) {
        pAttack->m_nAttackResult = 9;
        pAttack->m_nDeflectResult = 9;
    } else if (nTotal < nNeed) {
        pAttack->m_nAttackResult = 8;
        pAttack->m_nDeflectResult = 8;
    } else {
        pAttack->m_nAttackResult = 9;
        pAttack->m_nDeflectResult = 9;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005BB440
void CSWSCreature::ResolveDamage(CSWSObject *pTarget, int bFixed, int nAmount)
{
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    int n44 = pAttack->m_n44;
    int bSneak = (pAttack->m_nAttackResult == 2) ? 1 : 0;
    int bOffHand = m_pcCombatRound->GetOffHandAttack();
    int nDamage = 0;
    int bInvulnerable = (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bB4 && m_bA88) ? 1 : 0;
    if (!pTarget->m_bF8 && !bInvulnerable && !g_bDebugNoDamage && !bFixed) {
        nDamage = GetStats()->GetDamageRoll(pTarget, bOffHand, bSneak, n44, 0, 1);
        if (pTarget->AsSWSCreature())
            pTarget->AsSWSCreature()->GetCurrentHitPoints(0);
        pAttack->SetBaseDamage(nDamage);
        ResolveOnHitEffect(pTarget, bOffHand, bSneak);
        ResolveOnHitVisuals(pTarget);
        if (bSneak)
            BroadcastVoiceChat(0x11);
    } else {
        if (bFixed)
            nDamage = nAmount;
        pAttack->SetBaseDamage(nDamage);
        pAttack->SetDamage(GetDamageFlags(), nDamage);
    }
}

// FUNCTION: SWKOTOR 0x005BB890
void CSWSCreature::ResolveMeleeAttack(CSWSObject *pTarget, int a2, int nLength, int bFixed,
                                      int a5, int a6, int nAmount)
{
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    int nAttacks = m_pcCombatRound->GetTotalAttacks();
    int i;
    for (i = 0; i < nAttacks; i++) {
        pAttack->m_oidTarget = pTarget->m_idSelf;
        pAttack->m_nWeaponAttackType = m_pcCombatRound->GetWeaponAttackType();
        m_pcCombatRound->SetOffHandAttack(i >= m_pcCombatRound->m_nOnHandAttacks);
        CSWSCreature *pCreature = pTarget->AsSWSCreature();
        if (pCreature && (pCreature->m_bStunned || pCreature->GetDeadTemp()) &&
            pCreature->GetStats()->GetLevel(0) <= 4 && !pCreature->m_bA88)
            m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack)->m_bCoupDeGrace = 1;
        ResolveAttackRoll(pTarget, bFixed, (unsigned char)a6);
        CClientExoApp *pClient = g_pAppManager->m_pClientExoApp;
        int nAnimation = pClient->GetClientMeleeAnimation(m_idSelf, pTarget->m_idSelf, m_nAppearanceType,
                                                          pAttack->m_nAttackType, bFixed, a5);
        int nImpact = GetWeaponImpactTime(nAnimation, i);
        if (pAttack->m_nAttackType)
            ResolveMeleeSpecialAttack(i, a2, pTarget, nImpact);
        if (GetIsRangeState0(pAttack->m_nAttackResult)) {
            ResolveDamage(pTarget, bFixed, nAmount);
            ResolvePostMeleeDamage(pTarget->AsSWSCreature());
        }
        pAttack->ResolveAttackToMeleeSubAttack(i, nImpact);
        SetRangeState(pAttack->m_nAttackResult);
    }
    m_n550 = pAttack->m_nAttackType;
    ResolveMeleeAnimations(i, a2, pTarget, nLength, bFixed);
    if (bFixed) {
        pAttack->m_bB8 = 1;
        pAttack->m_nBC = a5;
    }
}

// FUNCTION: SWKOTOR 0x005B9770
void CSWSCreature::ApplyOnHitFear(CSWSObject *pTarget, int nRounds)
{
    CGameEffect *pLink = CreateLinkedEffect(EFFECT_TYPE_SETSTATE, 2, EFFECT_TYPE_VISUALEFFECT, 0xda);
    if (!pLink)
        return;
    CGameEffect *pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_VISUALEFFECT, 0xcf);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x06);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = nRounds * 6.0f;
    pLink->SetSubType(0x08);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
}

// FUNCTION: SWKOTOR 0x005B9870
void CSWSCreature::ApplyOnHitHold(CSWSObject *pTarget, int nRounds)
{
    CGameEffect *pLink = CreateLinkedEffect(EFFECT_TYPE_SETSTATE, 5, EFFECT_TYPE_VISUALEFFECT, 0xe8);
    if (!pLink)
        return;
    CGameEffect *pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_VISUALEFFECT, 0x52);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_VISUALEFFECT, 0xcf);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x0f);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = nRounds * 6.0f;
    pLink->SetSubType(0x08);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
}

// FUNCTION: SWKOTOR 0x005B9980
void CSWSCreature::ApplyOnHitSleep(CSWSObject *pTarget, int nRounds)
{
    CGameEffect *pVisual = new CGameEffect(1);
    if (!pVisual)
        return;
    pVisual->m_nType = EFFECT_TYPE_VISUALEFFECT;
    pVisual->SetInteger(0, 0x5e);
    pVisual->SetCreator(m_idSelf);
    pVisual->SetDurationType(DURATION_TYPE_INSTANT);
    m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack)->m_alstOnHitEffects.Add(pVisual);
    CGameEffect *pLink = CreateLinkedEffect(EFFECT_TYPE_SETSTATE, 6, EFFECT_TYPE_VISUALEFFECT, 7);
    if (!pLink)
        return;
    CGameEffect *pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_VISUALEFFECT, 0xcf);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x1b);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = nRounds * 6.0f;
    pLink->SetSubType(0x08);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
}

// FUNCTION: SWKOTOR 0x005B9B80
void CSWSCreature::ApplyOnHitSlow(CSWSObject *pTarget, int nRounds)
{
    CGameEffect *pVisual = new CGameEffect(1);
    if (!pVisual)
        return;
    pVisual->m_nType = EFFECT_TYPE_VISUALEFFECT;
    pVisual->SetInteger(0, 0x5f);
    pVisual->SetCreator(m_idSelf);
    pVisual->SetDurationType(DURATION_TYPE_INSTANT);
    m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack)->m_alstOnHitEffects.Add(pVisual);
    CGameEffect *pLink = CreateLinkedEffect(EFFECT_TYPE_MOVEMENT_SPEED_DECREASE, 0, EFFECT_TYPE_VISUALEFFECT, 0xcf);
    if (!pLink)
        return;
    CGameEffect *pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x1c);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = nRounds * 6.0f;
    pLink->SetSubType(0x08);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
}

// FUNCTION: SWKOTOR 0x005B9D40
void CSWSCreature::ApplyOnHitStun(CSWSObject *pTarget, int nRounds)
{
    CGameEffect *pLink = CreateLinkedEffect(EFFECT_TYPE_SETSTATE, 4, EFFECT_TYPE_VISUALEFFECT, 0xd0);
    if (!pLink)
        return;
    CGameEffect *pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_VISUALEFFECT, 0xcf);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pEffect = pLink;
    pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x1d);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = nRounds * 6.0f;
    pLink->SetSubType(0x08);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
}

// NONMATCH (31): eax/ecx/edx roles rotated by one from the first SetCreator on.
// FUNCTION: SWKOTOR 0x005B9E40
void CSWSCreature::ApplyOnHitAbilityDamage(CSWSObject *pTarget, CSWItemProperty *pProperty)
{
    CGameEffect *pVisual = new CGameEffect(1);
    if (!pVisual)
        return;
    pVisual->m_nType = EFFECT_TYPE_VISUALEFFECT;
    pVisual->SetInteger(0, 0x5b);
    pVisual->SetCreator(m_idSelf);
    pVisual->SetDurationType(DURATION_TYPE_TEMPORARY);
    pVisual->m_fDuration = 30.0f;
    m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack)->m_alstOnHitEffects.Add(pVisual);
    CGameEffect *pEffect = new CGameEffect(1);
    if (!pEffect)
        return;
    pEffect->m_nType = EFFECT_TYPE_ABILITY_DECREASE;
    pEffect->SetInteger(0, pProperty->m_nParam1Value);
    pEffect->SetInteger(1, 1);
    CGameEffect *pLink = CreateLinkedEffect(pEffect, EFFECT_TYPE_746788, 0x1c);
    if (!pLink) {
        delete pEffect;
        return;
    }
    pLink->SetDurationType(DURATION_TYPE_TEMPORARY);
    pLink->m_fDuration = 30.0f;
    if (pProperty->m_nPropertyName == 0x30)
        pLink->SetSubType(0x10);
    else
        pLink->SetSubType(0x18);
    pLink->UpdateLinked();
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pLink);
    CSWCCMessageData *pData = new CSWCCMessageData;
    pData->SetObjectID(0, pTarget->m_idSelf);
    pData->SetInteger(0, 0x8e);
    pData->SetInteger(1, pProperty->m_nParam1Value);
    pData->SetInteger(2, 1);
    pData->SetFloat(0, 30.0f);
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstFeedback.AddOutOfLine(pData);
}

// FUNCTION: SWKOTOR 0x005BA0D0
void CSWSCreature::ApplyOnHitPoison(CSWSObject *pTarget, CSWItemProperty *pProperty)
{
    CGameEffect *pEffect = CreateLinkedEffect(EFFECT_TYPE_POISON, pProperty->GetParam1Value(), EFFECT_TYPE_746788, 0x17);
    if (!pEffect)
        return;
    pEffect->SetDurationType(DURATION_TYPE_PERMANENT);
    pEffect->SetSubType(0x08);
    m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack)->m_alstOnHitEffects.Add(pEffect);
}

// NONMATCH (22): eax/ecx/edx roles rotated by one (as AbilityDamage).
// FUNCTION: SWKOTOR 0x005BA170
void CSWSCreature::ApplyOnHitSlayRacialGroup(CSWSObject *pTarget, CSWItemProperty *pProperty)
{
    CSWSCreature *pCreature = pTarget->AsSWSCreature();
    if (!pCreature)
        return;
    if (pCreature->GetStats()->GetRace() != pProperty->m_nParam1Value)
        return;
    CGameEffect *pVisual = new CGameEffect(1);
    if (!pVisual)
        return;
    pVisual->m_nType = EFFECT_TYPE_VISUALEFFECT;
    pVisual->SetInteger(0, 0x32);
    pVisual->SetCreator(m_idSelf);
    pVisual->SetDurationType(DURATION_TYPE_INSTANT);
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pVisual);
    CGameEffect *pDeath = new CGameEffect(1);
    if (!pDeath)
        return;
    pDeath->m_nType = EFFECT_TYPE_DEATH;
    pDeath->SetCreator(m_idSelf);
    pDeath->SetDurationType(DURATION_TYPE_INSTANT);
    pDeath->SetInteger(0, 0);
    pDeath->SetInteger(1, 1);
    m_pcCombatRound->GetAttack(m_pcCombatRound->GetCurrentAttackIndex())->m_alstOnHitEffects.AddOutOfLine(pDeath);
}

// NONMATCH (21): bOffHand sete register and the else-branch vtable load
// that the original hoists above the je.
// FUNCTION: SWKOTOR 0x005B8C20
void CSWSCreature::ResolvePostRangedDamage(CSWSObject *pTarget)
{
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    int nDamage = pAttack->GetTotalDamage(1);
    int bOffHand = pAttack->m_nWeaponAttackType == 2;
    if (pTarget->AsSWSCreature()) {
        pTarget->AsSWSCreature();
        if (nDamage >= pTarget->AsSWSCreature()->GetCurrentHitPoints(0) || pAttack->m_bCoupDeGrace == 1)
            pAttack->m_bKillingBlow = 1;
        float fZ = m_vPosition.z - pTarget->m_vPosition.z;
        float fY = m_vPosition.y - pTarget->m_vPosition.y;
        float fX = m_vPosition.x - pTarget->m_vPosition.x;
        if (pAttack->m_bCoupDeGrace == 1 && fX * fX + fY * fY + fZ * fZ <= 100.0f) {
            CGameEffect *pEffect = new CGameEffect(1);
            pEffect->m_nType = EFFECT_TYPE_DEATH;
            pEffect->SetDurationType(DURATION_TYPE_INSTANT);
            pEffect->SetCreator(m_idSelf);
            pEffect->SetInteger(0, 0);
            pEffect->SetInteger(1, 1);
            pAttack->m_alstOnHitEffects.AddOutOfLine(pEffect);
        }
    } else {
        if (nDamage >= pTarget->GetCurrentHitPoints(0))
            pAttack->m_bKillingBlow = 1;
    }
    if (nDamage > 0)
        return;
    if (m_pcCombatRound->m_bWeaponIneffectiveSent)
        return;
    if (GetIsWeaponEffective(pTarget->m_idSelf, bOffHand))
        return;
    CSWCCMessageData *pData = new CSWCCMessageData;
    pData->m_nType = 3;
    pData->SetInteger(0, 0x75);
    pAttack->m_alstFeedback.AddOutOfLine(pData);
    m_pcCombatRound->m_bWeaponIneffectiveSent = 1;
}

// NONMATCH (24): the original loads g_pRules into ecx; register roles follow.
// FUNCTION: SWKOTOR 0x005B9040
void CSWSCreature::ResolveOnHitVisuals(CSWSObject *pTarget)
{
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    for (int nType = 0; nType < 14; nType++) {
        if (nType == 1 || nType == 2 || nType == 0)
            continue;
        if (pAttack->GetDamage(1 << nType) <= 0)
            continue;
        CExoString sColumn;
        int nVisual = 0;
        if (pAttack->m_bRangedAttack)
            sColumn = "RangedEffect";
        else
            sColumn = "VisualEffect";
        g_pRules->m_p2DArrays->m_pDamageHitVisual->GetINTEntry(nType, sColumn, &nVisual);
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->m_nType = EFFECT_TYPE_VISUALEFFECT;
        pEffect->SetDurationType(DURATION_TYPE_INSTANT);
        pEffect->SetCreator(m_idSelf);
        pEffect->SetInteger(0, nVisual);
        pAttack->m_alstOnHitEffects.Add(pEffect);
    }
    if (!GetIsRangeState2(pAttack->m_nAttackResult))
        return;
    CGameEffect *pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_VISUALEFFECT;
    pEffect->SetDurationType(DURATION_TYPE_INSTANT);
    pEffect->SetCreator(pTarget->m_idSelf);
    pEffect->SetInteger(0, 0xfb7);
    pAttack->m_alstOnHitEffects.Add(pEffect);
}

// NONMATCH (16): the inventory pointer stays in edi and the ShieldHit string
// is built before the 2DA pointer load.
// FUNCTION: SWKOTOR 0x005B78E0
int CSWSCreature::IsProjectileParryable(CSWSObject *pTarget, int bCheckShield, int bRanged,
                                        int &bForceShield, int nHand)
{
    int bResult = 0;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    if (pTarget->AsSWSCreature() && pAttack->m_bRangedAttack) {
        CSWSCreature *pCreature = pTarget->AsSWSCreature();
        if ((pCreature->m_pStats->HasFeat(0x37) || pCreature->m_pStats->HasFeat(1) ||
             pCreature->m_pStats->HasFeat(0x18)) &&
            !pCreature->m_bStunned && !pCreature->GetDeadTemp()) {
            if (pCreature->GetClientCreature() && pCreature->IsLightsaberEquipped() && !pCreature->m_bA04) {
                Vector vFacing = pCreature->GetClientCreature()->m_vOrientation;
                if (vFacing.x * m_vOrientation.x + vFacing.z * m_vOrientation.z + vFacing.y * m_vOrientation.y <= 0.0f)
                    bResult = 1;
            }
        } else if (bRanged && bCheckShield && pCreature->GetClientCreature() &&
                   pCreature->GetClientCreature()->GetForceShield()) {
            unsigned long nSlot = 0;
            if (nHand == 0)
                nSlot = 0x10;
            else if (nHand == 1)
                nSlot = 0x20;
            CSWSItem *pItem = m_pInventory->GetItemInSlot(nSlot);
            if (pItem) {
                unsigned char nAmmunitionType = pItem->GetBaseItem()->m_nAmmunitionType;
                int nShieldHit = 0;
                g_pRules->m_p2DArrays->m_pAmmunitionTypes->GetINTEntry(nAmmunitionType, CExoString("ShieldHit"),
                                                                      &nShieldHit);
                if (nShieldHit)
                    bResult = 1;
            }
            bForceShield = pCreature->GetClientCreature()->GetForceShield();
        }
    }
    return bResult;
}

// NONMATCH (82): register allocation (index in ecx / effect in edi, the
// shield type reloaded at the latch).
// FUNCTION: SWKOTOR 0x005B7D10
void CSWSCreature::ResolveDamageShields(CSWSCreature *pAttacker)
{
    CExoArrayList<CGameEffect *> *pEffects = &m_appliedEffects;
    int i = m_pStats->m_nDamageShieldEffectStart;
    if (!pAttacker)
        return;
    for (; i < pEffects->num; i++) {
        if (pEffects->element[i]->m_nType > EFFECT_TYPE_DAMAGE_SHIELD)
            break;
        CGameEffect *pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_SHIELD) {
            int nBase = pEffect->GetInteger(0);
            int nDamage = pEffect->GetInteger(1);
            int nDamageType = pEffect->GetInteger(2);
            if (nDamage > 5) {
                int nNumDice;
                int nDie;
                g_pRules->m_p2DArrays->m_pIPRPDamage->GetINTEntry(nDamage, CExoString("NumDice"), &nNumDice);
                g_pRules->m_p2DArrays->m_pIPRPDamage->GetINTEntry(nDamage, CExoString("Die"), &nDie);
                nDamage = g_pRules->RollDice((unsigned char)nNumDice, (unsigned char)nDie);
            }
            nDamage += nBase;
            FloatToInt(&g_nDamageIndex, log10((double)nDamageType) * g_fInvLog10Of2);
            int nIndex = g_nDamageIndex;
            CGameEffect *pShield = new CGameEffect(1);
            pShield->m_nType = EFFECT_TYPE_DAMAGE;
            pShield->SetDurationType(DURATION_TYPE_INSTANT);
            pShield->SetCreator(m_idSelf);
            pShield->SetNumIntegers(0x15);
            for (int j = 0; j < 15; j++) {
                if (j == nIndex)
                    pShield->SetInteger(nIndex, nDamage);
                else
                    pShield->SetInteger(j, -1);
            }
            pShield->SetInteger(0x10, 1000);
            pAttacker->ApplyEffect(pShield, 0, 0);
            CGameEffect *pVisual = new CGameEffect(1);
            pVisual->m_nType = EFFECT_TYPE_VISUALEFFECT;
            pVisual->SetDurationType(DURATION_TYPE_INSTANT);
            pVisual->SetCreator(m_idSelf);
            pVisual->SetInteger(0, 0);
            pAttacker->ApplyEffect(pVisual, 0, 0);
        }
    }
}

// Name descriptive.
// NONMATCH (37): the original keeps 0 in edi, overlaps vDiff with the
// CResRef and spills fRange into the dead pTarget slot.
// FUNCTION: SWKOTOR 0x005B7B30
int CSWSCreature::ResolveMoveToForceJump(CSWSObject *pTarget, unsigned short *pFeat)
{
    int bResult = 0;
    if (pTarget && pTarget->AsSWSCreature() && *pFeat == 0 && m_pStats->HasFeat(0x65) &&
        IsLightsaberEquipped()) {
        {
            Vector vDiff = m_vPosition - pTarget->m_vPosition;
            if (!(vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z >= 100.0f))
                return 0;
        }
        m_pForceJumpData->m_oidTarget = pTarget->m_idSelf;
        float fRadius = m_pForceJumpData->m_fRadius;
        CResRef cResRef("");
        if (g_pAppManager->m_pServerExoApp->TestDirectLine(m_idSelf, cResRef, m_vPosition, pTarget->m_vPosition,
                                                           &fRadius, m_pForceJumpData->m_fRange, 0, 0) != 1)
            return 0;
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->m_nType = EFFECT_TYPE_FORCE_JUMP;
        pEffect->SetObjectID(0, pTarget->m_idSelf);
        pEffect->SetInteger(0, 0);
        ApplyEffect(pEffect, 0, 0);
        if (m_pStats->HasFeat(0x67)) {
            *pFeat = 0x67;
            return 1;
        }
        *pFeat = m_pStats->HasFeat(0x66) ? 0x66 : 0x65;
        return 1;
    }
    return bResult;
}

// NONMATCH (45): the original spills pTarget to its arg slot and keeps the
// rolled-attack index in ebp (one stack slot fewer).
// FUNCTION: SWKOTOR 0x005BB590
void CSWSCreature::ResolveRangedAttack(CSWSObject *pTarget, int nAttacks, int bNoReaction, int bFixed,
                                       int nAnimation, int a6, int nAmount)
{
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    pAttack->m_bRangedAttack = 1;
    pAttack->m_oidTarget = pTarget->m_idSelf;
    pAttack->m_nAttackId = m_pcCombatRound->GetNextAttackID();
    int nAnim;
    if (bFixed)
        nAnim = nAnimation;
    else
        nAnim = g_pAppManager->m_pClientExoApp->GetClientRangedAnimation(m_idSelf, pAttack->m_nAttackType, 0, 0);
    int nShots = GetWeaponDischargeShots(nAnim);
    int nShotsLeft = nShots;
    GetWeaponHitShots(nAnim);
    int nAttacksLeft = m_pcCombatRound->GetTotalAttacks();
    int nAttack = 0;
    int bAnyHit = 0;
    for (int nShot = 0; nShot < nShots; nShot++) {
        int bHit = 0;
        int bRolled = 0;
        pAttack->m_nAttackResult = 4;
        if (rand() % nShotsLeft < nAttacksLeft) {
            nAttacksLeft--;
            m_pcCombatRound->SetOffHandAttack(nAttack >= m_pcCombatRound->m_nOnHandAttacks);
            bRolled = 1;
            pAttack->m_nWeaponAttackType = (m_pcCombatRound->GetOffHandAttack() != 0) + 1;
            ResolveAttackRoll(pTarget, bFixed, (unsigned char)a6);
            if (pAttack->m_nAttackType)
                ResolveRangedSpecialAttack(pTarget, nAttack);
            if (GetIsRangeState0(pAttack->m_nAttackResult) || GetIsRangeState2(pAttack->m_nAttackResult)) {
                bHit = 1;
                bAnyHit = 1;
            }
            nAttack++;
        }
        if (GetIsRangeState1(pAttack->m_nAttackResult)) {
            int nHand = GetWeaponDischargeHand(nAnim, nShot);
            bHit = 0;
            int bParried = IsProjectileParryable(pTarget, 1, 1, bHit, nHand);
            if (bParried)
                GetCurrentAttack()->m_nAttackResult = bHit ? 10 : 8;
            bHit = bParried;
        }
        if (bHit) {
            ResolveRangedHit(pTarget);
            if (GetIsRangeState0(pAttack->m_nAttackResult) || pAttack->m_nAttackResult == 9) {
                ResolveDamage(pTarget, bFixed, nAmount);
                ResolvePostRangedDamage(pTarget);
            }
        } else {
            ResolveRangedMiss(pTarget);
        }
        pAttack->ResolveAttackToRangedSubAttack(nShot, GetWeaponDischargeTime(nAnim, nShot), bRolled, bHit,
                                                GetWeaponDischargeHand(nAnim, nShot));
        nShotsLeft--;
    }
    ResolveRangedAnimations(pTarget, (unsigned short)bNoReaction, bAnyHit);
    if (bFixed) {
        pAttack->m_bB8 = 1;
        pAttack->m_nBC = nAnimation;
    }
}

// NONMATCH (21): this and pTarget swap ebx/ebp (register tie).
// FUNCTION: SWKOTOR 0x005B5EA0
void CSWSCreature::SendAttackSummaryFeedback(unsigned long oidTarget)
{
    CSWSCombatRound *pRound = m_pcCombatRound;
    CSWCCMessageData *pData = new CSWCCMessageData;
    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pRound) {
        CSWSCombatAttackData *pAttack = pRound->GetAttack(pRound->m_nCurrentAttack);
        int nDamage = pAttack->GetBaseDamage();
        if (pTarget) {
            CSWPartyTable *pParty = g_pAppManager->m_pServerExoApp->GetPartyTable();
            if ((pParty && pParty->GetIsNPCObjectMember(oidTarget)) || pTarget->m_b9D4) {
                int nDifficulty = g_pAppManager->GetClientExoApp()->GetClientOptions()->m_nDifficulty;
                float fMultiplier;
                if (g_pRules->Get2DArrays()->GetDifficultyOptions()->GetFLOATEntry(nDifficulty, "MULTIPLIER",
                                                                                   &fMultiplier))
                    nDamage = (int)(nDamage * fMultiplier);
            }
        }
        pData->SetInteger(0, pAttack->m_nAttackResult);
        pData->SetInteger(1, pAttack->m_nAttackType);
        pData->SetInteger(2, pAttack->m_nToHitMod + pAttack->m_nToHitRoll);
        pData->SetInteger(3, pAttack->m_nF4);
        pData->SetInteger(4, nDamage);
        pData->SetInteger(5, pAttack->m_nEC);
        pData->SetInteger(6, pAttack->m_n44);
        pData->SetInteger(7, pAttack->m_bCoupDeGrace);
        pData->SetInteger(8, pAttack->m_nSaveRoll);
        pData->SetInteger(9, pAttack->m_n138);
        pData->SetInteger(10, pAttack->m_nD8);
        pData->SetInteger(11, pAttack->m_nDC);
        pData->SetObjectID(0, m_idSelf);
        pData->SetObjectID(1, oidTarget);
        BroadcastAttackSummaryToParty(pData);
        if (pTarget)
            pTarget->BroadcastAttackSummaryToParty(pData);
    }
    delete pData;
}

// NONMATCH (73): the original keeps the attack index in ebp and spills
// bRanged (into the dead pTarget slot), so the trailing +0x44 store stays one
// shared block; here the index is spilled and the block is duplicated.
// FUNCTION: SWKOTOR 0x005B8820
void CSWSCreature::ResolveSneakAttack(CSWSCreature *pTarget)
{
    if (!pTarget)
        return;
    CSWSCreatureStats *pStats = m_pStats;
    int nAttack = m_pcCombatRound->m_nCurrentAttack;
    int bRanged = m_pcCombatRound->GetAttack(nAttack)->m_bRangedAttack;
    if (!pStats->HasFeat(0x3c) && !pStats->HasFeat(0x3d) && !pStats->HasFeat(0x3e) &&
        !pStats->HasFeat(0x3f) && !pStats->HasFeat(0x40) && !pStats->HasFeat(0x41) &&
        !pStats->HasFeat(0x42) && !pStats->HasFeat(0x43) && !pStats->HasFeat(0x44) &&
        !pStats->HasFeat(0x45))
        return;
    float fDistance;
    if (bRanged) {
        Vector vDelta(m_vPosition.x - pTarget->m_vPosition.x, m_vPosition.y - pTarget->m_vPosition.y,
                      m_vPosition.z - pTarget->m_vPosition.z);
        fDistance = Magnitude(vDelta);
    }
    CSWSCreatureListEntry *pEntry;
    if (pTarget->m_bStunned || pTarget->GetDeadTemp() ||
        (((pEntry = pTarget->GetFromList920(m_idSelf)) == 0 || !(pEntry->m_nFlags & 1)) &&
         (bRanged == 1 ? fDistance <= 10.0f : bRanged == 0))) {
        if (pTarget->m_pStats->GetEffectImmunity(IMMUNITY_TYPE_SNEAK_ATTACK, this) ||
            pTarget->m_pStats->GetEffectImmunity(IMMUNITY_TYPE_CRITICAL_HIT, this)) {
            CSWCCMessageData *pData = new CSWCCMessageData;
            pData->SetObjectID(0, pTarget->m_idSelf);
            pData->SetInteger(0, 0x86);
            m_pcCombatRound->GetAttack(nAttack)->m_alstFeedback.AddOutOfLine(pData);
            return;
        }
    } else {
        Vector vDelta(m_vPosition.x - pTarget->m_vPosition.x, m_vPosition.y - pTarget->m_vPosition.y,
                      m_vPosition.z - pTarget->m_vPosition.z);
        Vector vDir = NormalizeVector(vDelta);
        Vector vFacing = NormalizeVector(pTarget->m_vOrientation);
        if (!(vDir.x * vFacing.x + vDir.y * vFacing.y + vDir.z * vFacing.z < 0.707))
            return;
        if (bRanged && !(fDistance <= 10.0f))
            return;
    }
    m_pcCombatRound->GetAttack(nAttack)->m_n44 = 1;
}
