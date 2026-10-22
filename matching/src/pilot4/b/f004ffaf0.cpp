// Pilot 4 arm B: 0x004FFAF0, creature reacts to an ability increase/decrease
// effect (recomputes the stat modifier and dependent values).
// NONMATCH (31 diff insns, same size): the original loads m_nType with
// `mov ax` and pushes ebx/ebp/edi before the effect-type compare (8 bytes
// longer, second global loaded into edx first), and schedules `sub edi, ebp`
// after the GetMaxForcePoints call. Everything else lines up.

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);              // 0x00503E40
    int GetInteger(int nIndex);                 // 0x00503690
    void SetInteger(int nIndex, int nValue);    // 0x005036A0
    void SetCreator(unsigned long oid);         // 0x00503A00
    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }

    unsigned __int64 m_nID;                     // 0x00
    unsigned short m_nType;                     // 0x08
    unsigned short m_nSubType;                  // 0x0a
    float m_fDuration;                          // 0x0c
    char pad_0x10[0x8];
    unsigned long m_oidCreator;                 // 0x18
    char pad_0x1c[0x8c - 0x1c];
};
#pragma pack(pop)

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);  // 0x004B08D0
};

class CServerExoApp {
public:
    CServerAIMaster *GetServerAIMaster();       // 0x004AED80
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;             // 0x8
};
extern CAppManager *g_pAppManager;              // 0x007A39FC
inline CServerExoApp *GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

extern const unsigned long EFFECT_TYPE_ABILITY_INCREASE;       // 0x0074670C
extern const unsigned long EFFECT_TYPE_ABILITY_DECREASE;       // 0x00746710
extern const unsigned short EFFECT_TYPE_DEATH;  // 0x007466C8
extern const unsigned short DURATION_TYPE_INSTANT;  // 0x007473B8
extern const unsigned long EVENT_APPLY_EFFECT;  // 0x007441C8

inline int IsAbilityEffect(unsigned short nType)
{
    return nType == EFFECT_TYPE_ABILITY_INCREASE || nType == EFFECT_TYPE_ABILITY_DECREASE;
}

class CSWSCreatureStats {
public:
    char CalcStatModifier(unsigned char nStat); // 0x005A5460
    unsigned char GetSTRStat();                 // 0x005A6190
    unsigned char GetDEXStat();                 // 0x005A6550
    unsigned char GetCONStat();                 // 0x005A6250
    unsigned char GetINTStat();                 // 0x005A6310
    unsigned char GetWISStat();                 // 0x005A63D0
    unsigned char GetCHAStat();                 // 0x005A6490
    void AdjustSpellUsesPerDay();               // 0x005A6D80

    char pad_0x0[0xea];
    char m_nStrengthModifier;                   // 0xea
    char pad_0xeb;
    char m_nDexterityModifier;                  // 0xec
    char pad_0xed;
    char m_nConstitutionModifier;               // 0xee
    char pad_0xef;
    char m_nIntelligenceModifier;               // 0xf0
    char pad_0xf1;
    char m_nWisdomModifier;                     // 0xf2
    char pad_0xf3;
    char m_nCharismaModifier;                   // 0xf4
    char pad_0xf5[0x124 - 0xf5];
    short m_nForcePoints;                       // 0x124
    void SetForcePoints(short n) { m_nForcePoints = n; }
};

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();                      // 0x94
    virtual short GetMaxHitPoints(int bIncludeTemp);      // 0x98
    virtual short GetCurrentHitPoints(int bIncludeTemp);  // 0x9c

    int GetIsPossessed();                       // 0x0063E7F0 (placeholder name)
    int GetDeadTemp();                          // 0x004EF890
    short GetMaxForcePoints();                  // 0x004FD490 (placeholder name)
    void SetCurrentHitPoints(int nHP);          // 0x004CC280
    void OnAbilityEffectChanged(CGameEffect *pEffect);

    unsigned long m_idSelf;                     // 0x04
    char pad_0x8[0x344 - 0x8];
    int m_bUpdateCombatInformation;             // 0x344
    char pad_0x348[0xa74 - 0x348];
    CSWSCreatureStats *m_pStats;                // 0xa74
};

// FUNCTION: SWKOTOR 0x004FFAF0
void CSWSCreature::OnAbilityEffectChanged(CGameEffect *pEffect)
{
    {
        if (pEffect && (pEffect->m_nType == EFFECT_TYPE_ABILITY_INCREASE ||
                        pEffect->m_nType == EFFECT_TYPE_ABILITY_DECREASE)) {
            int nAbility = pEffect->GetInteger(0);
            if (nAbility == 0) {
                CSWSCreatureStats *pStats = m_pStats;
                pStats->m_nStrengthModifier = pStats->CalcStatModifier(pStats->GetSTRStat());
                m_bUpdateCombatInformation = 1;
            }
            else if (nAbility == 1) {
                CSWSCreatureStats *pStats = m_pStats;
                pStats->m_nDexterityModifier = pStats->CalcStatModifier(pStats->GetDEXStat());
                m_bUpdateCombatInformation = 1;
            }
            else if (nAbility == 2) {
                GetIsPossessed();
                int bWasDead = GetDead();
                int bWasDeadTemp = GetDeadTemp();
                short nOldMax = GetMaxHitPoints(1);
                CSWSCreatureStats *pStats = m_pStats;
                pStats->m_nConstitutionModifier = pStats->CalcStatModifier(pStats->GetCONStat());
                SetCurrentHitPoints(GetCurrentHitPoints(1) - nOldMax + GetMaxHitPoints(1));
                if ((!bWasDead && GetDead()) || (!bWasDeadTemp && GetDeadTemp())) {
                    CGameEffect *pDeath = new CGameEffect(1);
                    pDeath->m_nType = EFFECT_TYPE_DEATH;
                    pDeath->SetDurationType(DURATION_TYPE_INSTANT);
                    pDeath->SetCreator(pEffect->m_oidCreator);
                    pDeath->SetInteger(1, 1);
                    GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                        0, 0, pDeath->m_oidCreator, m_idSelf, EVENT_APPLY_EFFECT, pDeath);
                }
            }
            else if (nAbility == 3) {
                CSWSCreatureStats *pStats = m_pStats;
                pStats->m_nIntelligenceModifier = pStats->CalcStatModifier(pStats->GetINTStat());
                m_pStats->AdjustSpellUsesPerDay();
            }
            else if (nAbility == 4) {
                int nOldMax = GetMaxForcePoints();
                CSWSCreatureStats *pStats = m_pStats;
                pStats->m_nWisdomModifier = pStats->CalcStatModifier(pStats->GetWISStat());
                CSWSCreatureStats *pStats2 = m_pStats;
                int nDiff = pStats2->m_nForcePoints - nOldMax;
                pStats2->m_nForcePoints = nDiff + GetMaxForcePoints();
                m_pStats->AdjustSpellUsesPerDay();
            }
            else if (nAbility == 5) {
                int nOldMax = GetMaxForcePoints();
                CSWSCreatureStats *pStats = m_pStats;
                pStats->m_nCharismaModifier = pStats->CalcStatModifier(pStats->GetCHAStat());
                CSWSCreatureStats *pStats2 = m_pStats;
                int nDiff = pStats2->m_nForcePoints - nOldMax;
                pStats2->m_nForcePoints = nDiff + GetMaxForcePoints();
                m_pStats->AdjustSpellUsesPerDay();
            }
        }
    }
}
