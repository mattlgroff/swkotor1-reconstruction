// CSWSCreature special (feat) attacks (nwscreatureaicombat.o): the ranged
// and melee feat handlers that ResolveRangedAttack / ResolveMeleeAttack call
// when the attack carries a feat id. Separate translation unit: here the
// range-state test 0x005B5510 stays an out-of-line call, while combat_ai.cpp
// inlines it.
//
// Names descriptive (the debug map has none for 0x005BA540 / 0x005BA8E0).

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    void AddOutOfLine(T t);
};

class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                          // 0x00503E40
    ~CGameEffect();                                         // 0x00503590
    void SetInteger(int nIndex, int nValue);                // 0x005036A0
    void SetCreator(unsigned long oid);                     // 0x00503A00
    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }
    void SetEffectType(unsigned short nType) { m_nType = nType; }
    void SetDuration(float fDuration) { m_fDuration = fDuration; }

    unsigned long m_nIDLow;             // 0x0
    unsigned long m_nIDHigh;            // 0x4
    unsigned short m_nType;             // 0x8
    unsigned short m_nSubType;          // 0xa
    float m_fDuration;                  // 0xc
    char pad_0x10[0x8c - 0x10];
};

extern unsigned short EFFECT_TYPE_SETSTATE;        // 0x0074669C (8)
extern unsigned short EFFECT_TYPE_AC_DECREASE;     // 0x00746740
extern int AC_TYPE_DODGE;               // 0x007468B0 (0, placeholder name)
extern int AC_VS_DAMAGE_TYPE_ALL;       // 0x0074688C (0x4007)
extern unsigned short DURATION_TYPE_TEMPORARY;  // 0x007473C0 (1)

class CSWRules {
public:
    char pad_0x0[0xaa];
    unsigned char m_nDifficultyRacialType;  // 0xaa
};

extern CSWRules *g_pRules;              // 0x007A3A28

class CSWSCombatAttackData {
public:
    char pad_0x0[0x5c];
    unsigned char m_nAttackResult;      // 0x5c
    char pad_0x5d[0x64 - 0x5d];
    unsigned short m_nAttackType;       // 0x64
    char pad_0x66[0x88 - 0x66];
    CExoArrayList<CGameEffect *> m_alstOnHitEffects;        // 0x88
    char pad_0x94[0x12d - 0x94];
    unsigned char m_nSaveRoll;          // 0x12d
    char pad_0x12e[0x13c - 0x12e];
    int m_b13C;                         // 0x13c (placeholder name)
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData *GetAttack(int nAttack);           // 0x004D2B50

    char pad_0x0[0x96c];
    unsigned char m_nCurrentAttack;     // 0x96c
};

class CServerAIMaster;

class CServerExoApp {
public:
    CServerAIMaster *GetServerAIMaster();                   // 0x004AED80
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;     // 0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;      // 0x007A39FC

class CSWSCreatureStats {
public:
    unsigned char GetLevel(int bFlag);                      // 0x005A5FD0
    unsigned char GetFeatRemainingUses(unsigned short nFeat);  // 0x005A6680
    int GetEffectLinkImmunity(CGameEffect *pEffect);        // 0x005A6A90

    char pad_0x0[0xea];
    char m_nStunningDCBonusMelee;       // 0xea (placeholder name)
    char pad_0xeb[0xf0 - 0xeb];
    char m_nStunningDCBonusRanged;      // 0xf0 (placeholder name)
};

class CSWSCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30
    unsigned long m_idSelf;             // +0x4
};

class CSWSObject : public CGameObject {
public:
    void ApplyEffect(CGameEffect *pEffect, int a2, int a3);  // 0x004D14F0
};

class CSWSCreature : public CSWSObject {
public:
    int GetIsRangeState0(int nValue);                       // 0x005B5510
    unsigned char SavingThrowRoll(unsigned char nSaveType, unsigned short nDC,
                                  unsigned char nSaveSubType, unsigned long oidVersus,
                                  int bSendFeedback, unsigned short nFeat,
                                  int bFromCombat);         // 0x005B92B0
    void ResolveRangedSpecialAttack(CSWSObject *pTarget, int nAttack);
    void ResolveMeleeSpecialAttack(int nAttack, int a2, CSWSObject *pTarget, int nImpactTime);

    char pad_0x8[0x9c8 - 0x8];
    CSWSCombatRound *m_pcCombatRound;   // 0x9c8
    char pad_0x9cc[0xa74 - 0x9cc];
    CSWSCreatureStats *m_pStats;        // 0xa74
    CSWSCreatureStats *GetStats() { return m_pStats; }
};

// NONMATCH (41): register rotation (edx/eax) in the feat-uses argument and
// the saving-throw pushes.
// FUNCTION: SWKOTOR 0x005BA540
void CSWSCreature::ResolveRangedSpecialAttack(CSWSObject *pTarget, int nAttack)
{
    if (!pTarget || nAttack != 0)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    if (GetStats()->GetFeatRemainingUses(pAttack->m_nAttackType) <= 0)
        pAttack->m_nAttackType = 0;
    switch (pAttack->m_nAttackType) {
    case 0x1a:
    case 0x1e:
    case 0x5c: {
        int nValue = 0;
        switch (pAttack->m_nAttackType) {
        case 0x1a:
            nValue = 1;
            break;
        case 0x1e:
            nValue = 4;
            break;
        case 0x5c:
            nValue = 2;
            break;
        }
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->SetEffectType(EFFECT_TYPE_AC_DECREASE);
        pEffect->SetDurationType(DURATION_TYPE_TEMPORARY);
        pEffect->SetDuration(3.0f);
        pEffect->SetCreator(m_idSelf);
        pEffect->SetInteger(0, AC_TYPE_DODGE);
        pEffect->SetInteger(1, nValue);
        pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
        pEffect->SetInteger(5, AC_VS_DAMAGE_TYPE_ALL);
        ApplyEffect(pEffect, 0, 0);
        break;
    }
    case 0x14:
    case 0x1f:
    case 0x4d: {
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->SetEffectType(EFFECT_TYPE_AC_DECREASE);
        pEffect->SetDurationType(DURATION_TYPE_TEMPORARY);
        pEffect->SetDuration(3.0f);
        pEffect->SetCreator(m_idSelf);
        pEffect->SetInteger(0, AC_TYPE_DODGE);
        pEffect->SetInteger(1, 5);
        pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
        pEffect->SetInteger(5, AC_VS_DAMAGE_TYPE_ALL);
        ApplyEffect(pEffect, 0, 0);
        if (!GetIsRangeState0(pAttack->m_nAttackResult))
            break;
        if (!pTarget->AsSWSCreature())
            break;
        CGameEffect *pStun = new CGameEffect(1);
        pStun->SetEffectType(EFFECT_TYPE_SETSTATE);
        pStun->SetInteger(0, 4);
        pStun->SetDurationType(DURATION_TYPE_TEMPORARY);
        pStun->SetDuration(6.0f);
        pStun->SetCreator(m_idSelf);
        char nBonus = GetStats()->m_nStunningDCBonusRanged;
        int nDC = nBonus + GetStats()->GetLevel(0);
        CSWSCreature *pCreature = pTarget->AsSWSCreature();
        if (!pCreature->GetStats()->GetEffectLinkImmunity(pStun) &&
            !pCreature->SavingThrowRoll(1, (unsigned short)nDC, 0, m_idSelf, 1, 0, 1)) {
            pAttack->m_alstOnHitEffects.AddOutOfLine(pStun);
            pAttack->m_nSaveRoll = 4;
        } else {
            delete pStun;
        }
        pAttack->m_b13C = 1;
        break;
    }
    }
}

// NONMATCH (80): the original evaluates the unused AI-master call with the
// app pointer in eax, shifting the register rotation of every effect block.
// FUNCTION: SWKOTOR 0x005BA8E0
void CSWSCreature::ResolveMeleeSpecialAttack(int nAttack, int a2, CSWSObject *pTarget, int nImpactTime)
{
    if (!pTarget || nAttack != 0)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    g_pAppManager->m_pServerExoApp->GetServerAIMaster();
    if (GetStats()->GetFeatRemainingUses(pAttack->m_nAttackType) <= 0)
        pAttack->m_nAttackType = 0;
    switch (pAttack->m_nAttackType) {
    case 0x08:
    case 0x13:
    case 0x51: {
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->SetEffectType(EFFECT_TYPE_AC_DECREASE);
        pEffect->SetDurationType(DURATION_TYPE_TEMPORARY);
        pEffect->SetDuration(3.0f);
        pEffect->SetCreator(m_idSelf);
        pEffect->SetInteger(0, AC_TYPE_DODGE);
        pEffect->SetInteger(1, 5);
        pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
        pEffect->SetInteger(5, AC_VS_DAMAGE_TYPE_ALL);
        ApplyEffect(pEffect, 0, 0);
        if (!GetIsRangeState0(pAttack->m_nAttackResult))
            break;
        if (!pTarget->AsSWSCreature())
            break;
        CGameEffect *pStun = new CGameEffect(1);
        pStun->SetEffectType(EFFECT_TYPE_SETSTATE);
        pStun->SetInteger(0, 4);
        pStun->SetDurationType(DURATION_TYPE_TEMPORARY);
        pStun->SetDuration(6.0f);
        pStun->SetCreator(m_idSelf);
        CSWSCreature *pCreature = pTarget->AsSWSCreature();
        char nBonus = GetStats()->m_nStunningDCBonusMelee;
        int nDC = nBonus + GetStats()->GetLevel(0);
        if (!pCreature->GetStats()->GetEffectLinkImmunity(pStun) &&
            !pCreature->SavingThrowRoll(1, (unsigned short)nDC, 0, m_idSelf, 1, 0, 1)) {
            pAttack->m_alstOnHitEffects.AddOutOfLine(pStun);
            pAttack->m_nSaveRoll = 4;
        } else {
            delete pStun;
        }
        pAttack->m_b13C = 1;
        break;
    }
    case 0x0b:
    case 0x35:
    case 0x5b: {
        int nValue = 0;
        switch (pAttack->m_nAttackType) {
        case 0x0b:
            nValue = 4;
            break;
        case 0x35:
            nValue = 1;
            break;
        case 0x5b:
            nValue = 2;
            break;
        }
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->SetEffectType(EFFECT_TYPE_AC_DECREASE);
        pEffect->SetDurationType(DURATION_TYPE_TEMPORARY);
        pEffect->SetDuration(3.0f);
        pEffect->SetCreator(m_idSelf);
        pEffect->SetInteger(0, AC_TYPE_DODGE);
        pEffect->SetInteger(1, nValue);
        pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
        pEffect->SetInteger(5, AC_VS_DAMAGE_TYPE_ALL);
        ApplyEffect(pEffect, 0, 0);
        break;
    }
    }
}
