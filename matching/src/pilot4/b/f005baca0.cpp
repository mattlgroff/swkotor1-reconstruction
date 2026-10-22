// Pilot 4 arm B: 0x005BACA0 CSWSCreature::ResolveAttackRoll.
// NONMATCH (39 diff insns, same size): register allocation differs. The
// original keeps nRoll and the attack total in the dead argument slots,
// bOffHand in ebx and the target creature in ebp throughout (3 locals); ours
// keeps the total in ebp and spills pTargetCreature to a 4th stack slot.

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    void Add(T t);                              // 0x00671C00 (folded)
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                         // 0x004D69F0
    void SetObjectID(int nIndex, unsigned long oid);  // 0x004D7780
    void SetInteger(int nIndex, int nValue);    // 0x004D76B0
    char pad_0x0[0x34];
};

class CSWSAttackResultB {
public:
    void Clear();                               // 0x004D2EC0
    unsigned char m_nThreatRoll;                // +0x00 (0xe0)
    int m_bThreat;                              // +0x04 (0xe4)
    unsigned char m_nConfirmRoll;               // +0x08 (0xe8)
    int m_bCritical;                            // +0x0c (0xec)
    char pad_0x10;
    unsigned char m_nCriticalMultiplier;        // +0x11 (0xf1)
    char pad_0x12[0x2];
};

class CSWSCombatAttackData {
public:
    char pad_0x0[0x3a];
    unsigned char m_nWeaponAttackType;          // 0x3a
    char pad_0x3b[0x11];
    int m_bCoupDeGrace;                         // 0x4c
    char pad_0x50[0xc];
    unsigned char m_nAttackResult;              // 0x5c
    char pad_0x5d[0x43];
    CExoArrayList<CSWCCMessageData *> m_alstFeedback;   // 0xa0
    char pad_0xac[0x15];
    unsigned char m_nToHitRoll;                 // 0xc1
    unsigned char m_nToHitMod;                  // 0xc2
    char pad_0xc3[0x15];
    int m_bThreatOnNatural20;                   // 0xd8
    int m_bMissedOnNatural1;                    // 0xdc
    CSWSAttackResultB m_resultB;                // 0xe0
    unsigned char m_nDefense;                   // 0xf4
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData *GetAttack(int nAttack);   // 0x004D2B50
    int GetOffHandAttack();                     // 0x004D2A00
    int GetWeaponAttackType();                  // 0x004D3DA0
    char pad_0x0[0x96c];
    unsigned char m_nCurrentAttack;             // 0x96c
};

class CSWRules {
public:
    unsigned short RollDice(unsigned char nDice, unsigned char nSides);  // 0x00550B60
};
extern CSWRules *g_pRules;                      // 0x007A3A28
extern int g_nDebugAttackRoll;                  // 0x00832874
extern const unsigned char IMMUNITY_TYPE_CRITICAL_HIT;  // 0x00746970

class CSWSCreature;

class CSWSCreatureStats {
public:
    int GetAttackModifierVersus(CSWSCreature *pTarget);     // 0x005A7EA0
    short GetArmorClassVersus(CSWSCreature *pAttacker, int bFlag);  // 0x005AD0E0
    int GetCriticalHitRoll(int bOffHand);                   // 0x005A5130
    unsigned char GetCriticalHitMultiplier(int bOffHand);   // 0x005A52B0
    int GetEffectImmunity(unsigned char nType, CSWSCreature *pVersus);  // 0x005A6960
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();      // 0x30
};

class CSWSObject : public CGameObject {
public:
    unsigned long m_idSelf;                     // 0x04
};

class CSWSCreature : public CSWSObject {
public:
    void ResolveAttackRoll(CSWSObject *pTarget, int bOverride, unsigned char nOverrideResult);
    int ResolveDefensiveEffects(CSWSObject *pTarget, int bHit);  // 0x005B8E50
    void ResolveTargetAttackOfOpportunity(CSWSCreature *pTarget); // 0x005B8820 (placeholder)

    char pad_0x8[0x8d4 - 0x8];
    int m_bAutoHit;                             // 0x8d4
    char pad_0x8d8[0x9c8 - 0x8d8];
    CSWSCombatRound *m_pcCombatRound;           // 0x9c8
    char pad_0x9cc[0xa74 - 0x9cc];
    CSWSCreatureStats *m_pStats;                // 0xa74
};

// FUNCTION: SWKOTOR 0x005BACA0
void CSWSCreature::ResolveAttackRoll(CSWSObject *pTarget, int bOverride,
                                     unsigned char nOverrideResult)
{
    if (!pTarget) {
        return;
    }
    CSWSCombatRound *pRound = m_pcCombatRound;
    CSWSCombatAttackData *pAttack = pRound->GetAttack(pRound->m_nCurrentAttack);
    pAttack->m_resultB.Clear();
    int bOffHand = pRound->GetOffHandAttack();
    pAttack->m_nWeaponAttackType = m_pcCombatRound->GetWeaponAttackType();
    int nRoll = g_pRules->RollDice(1, 20);
    if (g_nDebugAttackRoll) {
        nRoll = g_nDebugAttackRoll;
    }
    int nAttackMod = m_pStats->GetAttackModifierVersus(pTarget->AsSWSCreature());
    CSWSCreature *pTargetCreature = 0;
    int nDefense = 0;
    if (pTarget->AsSWSCreature()) {
        pTargetCreature = pTarget->AsSWSCreature();
        nDefense = pTargetCreature->m_pStats->GetArmorClassVersus(this, 0);
        pAttack->m_nDefense = (unsigned char)nDefense;
        ResolveTargetAttackOfOpportunity(pTargetCreature);
    }
    if (bOverride) {
        pAttack->m_nAttackResult = nOverrideResult;
    }
    else if (pAttack->m_bCoupDeGrace) {
        pAttack->m_nAttackResult = 3;
        pAttack->m_nToHitRoll = 20;
        pAttack->m_nToHitMod = (unsigned char)nAttackMod;
    }
    else if (m_bAutoHit) {
        pAttack->m_nAttackResult = 1;
        pAttack->m_nToHitRoll = (unsigned char)nRoll;
        pAttack->m_nToHitMod = (unsigned char)nAttackMod;
    }
    else {
        pAttack->m_nToHitRoll = (unsigned char)nRoll;
        pAttack->m_nToHitMod = (unsigned char)nAttackMod;
        int bHit = 0;
        if (nAttackMod + nRoll >= nDefense) {
            bHit = 1;
        }
        if (!ResolveDefensiveEffects(pTarget, bHit)) {
            if (nRoll != 1) {
                pAttack->m_bMissedOnNatural1 = 0;
                if (nAttackMod + nRoll >= nDefense || nRoll == 20) {
                    if (nRoll == 20) {
                        pAttack->m_bThreatOnNatural20 = 1;
                    }
                    else {
                        pAttack->m_bThreatOnNatural20 = 0;
                    }
                    int nThreat = m_pStats->GetCriticalHitRoll(bOffHand);
                    pAttack->m_resultB.m_nThreatRoll = nThreat;
                    if (nRoll >= nThreat) {
                        pAttack->m_resultB.m_bThreat = 1;
                        int nConfirm = g_pRules->RollDice(1, 20);
                        pAttack->m_resultB.m_nConfirmRoll = (unsigned char)nConfirm;
                        if (nConfirm + nAttackMod >= nDefense) {
                            pAttack->m_resultB.m_bCritical = 1;
                            pAttack->m_resultB.m_nCriticalMultiplier =
                                m_pStats->GetCriticalHitMultiplier(bOffHand);
                            if (pTargetCreature &&
                                pTargetCreature->m_pStats->GetEffectImmunity(
                                    IMMUNITY_TYPE_CRITICAL_HIT, this)) {
                                CSWCCMessageData *pData = new CSWCCMessageData();
                                pData->SetObjectID(0, pTargetCreature->m_idSelf);
                                pData->SetInteger(0, 0x7e);
                                pAttack->m_alstFeedback.Add(pData);
                                pAttack->m_nAttackResult = 1;
                            }
                            else {
                                pAttack->m_nAttackResult = 2;
                            }
                        }
                        else {
                            pAttack->m_resultB.m_bCritical = 0;
                            pAttack->m_resultB.m_nCriticalMultiplier = 0;
                            pAttack->m_nAttackResult = 1;
                        }
                    }
                    else {
                        pAttack->m_resultB.m_bThreat = 0;
                        pAttack->m_resultB.m_bCritical = 0;
                        pAttack->m_resultB.m_nConfirmRoll = 0;
                        pAttack->m_resultB.m_nCriticalMultiplier = 0;
                        pAttack->m_nAttackResult = 1;
                    }
                }
                else {
                    pAttack->m_nAttackResult = 4;
                }
            }
            else {
                pAttack->m_bMissedOnNatural1 = 1;
                pAttack->m_nAttackResult = 4;
            }
        }
    }
}
