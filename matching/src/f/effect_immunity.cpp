// CSWSCreatureStats::GetEffectLinkImmunity (b005b_f2_faction).

class C2DA {
public:
    int GetINTEntry(int nColumn, int nRow, int *pValue);
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
};

class CTwoDimArrays {
public:
    char pad_0x0[0xdc];
    C2DA *m_pImmunity2DA;            // +0xdc
};

class CSWRules {
public:
    char pad_0x0[0xaa];
    unsigned char m_nRacialTypeInvalid;  // +0xaa (placeholder name)
    char pad_0xab[0xb8 - 0xab];
    CTwoDimArrays *m_p2DArrays;      // +0xb8
    CTwoDimArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;

class CGameEffect {
public:
    CGameEffect();
    int GetInteger(int nIndex);
    void SetObjectID(int nIndex, unsigned long oid);
    void CopyEffect(CGameEffect *pEffect);
    char pad_0x0[0x8];
    unsigned short m_nType;          // +0x8
    char pad_0xa[0xe];
    unsigned long m_oidCreator;      // +0x18
    char pad_0x1c[0xc];
    CGameEffect *m_pLinkLeft;        // +0x28
    CGameEffect *m_pLinkRight;       // +0x2c
    char pad_0x30[0x4];
};

class CSWSCreatureStats;

class CSWSCreature {
public:
    void SendFeedbackMessage(int nMessage, CGameEffect *pEffect);
    char pad_0x0[0x4];
    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x124 - 0x8];
    CExoArrayList<CGameEffect *> m_appliedEffects;  // +0x124
    char pad_0x130[0xa74 - 0x130];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

extern const int EFFECT_TRUETYPE_DAMAGE_IMMUNITY;
extern const int EFFECT_TRUETYPE_IMMUNITY;
extern const int IMMUNITY_TYPE_COUNT;
extern const int IMMUNITY_TYPE_MIND_SPELLS;
extern const int IMMUNITY_TYPE_POISON;
extern const int IMMUNITY_TYPE_DISEASE;
extern const int IMMUNITY_TYPE_FEAR;
extern const int IMMUNITY_TYPE_TRAP;
extern const int IMMUNITY_TYPE_PARALYSIS;
extern const int IMMUNITY_TYPE_BLINDNESS;
extern const int IMMUNITY_TYPE_DEAFNESS;
extern const int IMMUNITY_TYPE_SLOW;
extern const int IMMUNITY_TYPE_ENTANGLE;
extern const int IMMUNITY_TYPE_SILENCE;
extern const int IMMUNITY_TYPE_STUN;
extern const int IMMUNITY_TYPE_SLEEP;
extern const int IMMUNITY_TYPE_CHARM;
extern const int IMMUNITY_TYPE_DOMINATE;
extern const int IMMUNITY_TYPE_CONFUSED;
extern const int IMMUNITY_TYPE_CURSED;
extern const int IMMUNITY_TYPE_DAZED;
extern const int IMMUNITY_TYPE_ABILITY_DECREASE;
extern const int IMMUNITY_TYPE_NONE;    // 0x007468FC

class CSWSCreatureStats {
public:
    int GetEffectImmunityColumn(unsigned short nType, int nSubType);
    int GetEffectImmunity(unsigned char nImmunityType, CSWSCreature *pVersus);
    int GetEffectLinkImmunity(CGameEffect *pEffect);
    unsigned char GetSimpleAlignmentGoodEvil()
    {
        if (m_nGoodEvil <= 40)
            return 3;
        if (m_nGoodEvil >= 60)
            return 2;
        return 1;
    }

    char pad_0x0[0x24];
    CSWSCreature *m_pBaseCreature;   // +0x24
    char pad_0x28[0xdc - 0x28];
    unsigned short m_nRace;          // +0xdc
    char pad_0xde[0x15e - 0xde];
    unsigned short m_nImmunityEffectStart;  // +0x15e (placeholder name)
    char pad_0x160[0x17e - 0x160];
    short m_nGoodEvil;               // +0x17e
};

// FUNCTION: SWKOTOR 0x005A6960
int CSWSCreatureStats::GetEffectImmunity(unsigned char nImmunityType, CSWSCreature *pVersus)
{
    int nRace;
    int nAlignment;
    if (pVersus) {
        nRace = pVersus->m_pStats->m_nRace;
        nAlignment = pVersus->m_pStats->GetSimpleAlignmentGoodEvil();
    } else {
        nRace = g_pRules->m_nRacialTypeInvalid;
        nAlignment = -1;
    }
    CExoArrayList<CGameEffect *> *pEffects = &m_pBaseCreature->m_appliedEffects;
    for (int i = m_nImmunityEffectStart; i < pEffects->num; i++) {
        CGameEffect *pEffect = pEffects->element[i];
        if (pEffect->m_nType > EFFECT_TRUETYPE_IMMUNITY)
            break;
        int nType = pEffect->GetInteger(0);
        int nVsRace = pEffect->GetInteger(1);
        int nVsAlignment = pEffect->GetInteger(2);
        if (pEffect->m_nType == EFFECT_TRUETYPE_IMMUNITY &&
            (nType == IMMUNITY_TYPE_NONE || nType == nImmunityType) &&
            (nVsRace == g_pRules->m_nRacialTypeInvalid || nVsRace == nRace) &&
            (!nVsAlignment || nVsAlignment == nAlignment))
            return 1;
    }
    return 0;
}

// NONMATCH (59, mostly offset shift): the original re-does `movzx` of the
// 16-bit type for the second true-type compare (ours reuses eax, 3 bytes
// shorter); the feedback tail swaps esi/edi (message vs new effect).
// FUNCTION: SWKOTOR 0x005A6A90
int CSWSCreatureStats::GetEffectLinkImmunity(CGameEffect *pEffect)
{
    unsigned short nType = pEffect->m_nType;
    int bImmune = 0;
    if (nType != EFFECT_TRUETYPE_DAMAGE_IMMUNITY && nType != EFFECT_TRUETYPE_IMMUNITY) {
        int nColumn = GetEffectImmunityColumn(nType, pEffect->GetInteger(0));
        if (nColumn) {
            for (int nRow = 0; nRow < IMMUNITY_TYPE_COUNT; nRow++) {
                int bSet;
                C2DA *p2DA = g_pRules->Get2DArrays()->m_pImmunity2DA;
                p2DA->GetINTEntry(nColumn, nRow + 1, &bSet);
                if (bSet) {
                    bImmune = GetEffectImmunity(nRow, 0);
                    if (bImmune) {
                        int nMessage;
                        if (nRow == IMMUNITY_TYPE_MIND_SPELLS)
                            nMessage = 0x84;
                        else if (nRow == IMMUNITY_TYPE_POISON)
                            nMessage = 0x85;
                        else if (nRow == IMMUNITY_TYPE_DISEASE)
                            nMessage = 0x7d;
                        else if (nRow == IMMUNITY_TYPE_FEAR)
                            nMessage = 0x80;
                        else if (nRow == IMMUNITY_TYPE_TRAP)
                            nMessage = 0x82;
                        else if (nRow == IMMUNITY_TYPE_PARALYSIS)
                            nMessage = 0x8b;
                        else if (nRow == IMMUNITY_TYPE_BLINDNESS)
                            nMessage = 0x8c;
                        else if (nRow == IMMUNITY_TYPE_DEAFNESS)
                            nMessage = 0x92;
                        else if (nRow == IMMUNITY_TYPE_SLOW)
                            nMessage = 0x90;
                        else if (nRow == IMMUNITY_TYPE_ENTANGLE)
                            nMessage = 0x91;
                        else if (nRow == IMMUNITY_TYPE_SILENCE)
                            nMessage = 0x8a;
                        else if (nRow == IMMUNITY_TYPE_STUN)
                            nMessage = 0x87;
                        else if (nRow == IMMUNITY_TYPE_SLEEP)
                            nMessage = 0x89;
                        else if (nRow == IMMUNITY_TYPE_CHARM)
                            nMessage = 0x8d;
                        else if (nRow == IMMUNITY_TYPE_DOMINATE)
                            nMessage = 0x88;
                        else if (nRow == IMMUNITY_TYPE_CONFUSED)
                            nMessage = 0x81;
                        else if (nRow == IMMUNITY_TYPE_CURSED)
                            nMessage = 0x83;
                        else if (nRow == IMMUNITY_TYPE_DAZED)
                            nMessage = 0x7e;
                        else if (nRow == IMMUNITY_TYPE_ABILITY_DECREASE)
                            nMessage = 0x7f;
                        else
                            return bImmune;
                        CSWSCreature *pCreator =
                            g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator);
                        CGameEffect *pFeedback = new CGameEffect;
                        CSWSCreature *pSelf = m_pBaseCreature;
                        pFeedback->SetObjectID(0, pSelf->m_idSelf);
                        if (pCreator) {
                            CGameEffect *pCopy = new CGameEffect;
                            pFeedback->CopyEffect(pCopy);
                            pCreator->SendFeedbackMessage(nMessage, pCopy);
                        }
                        m_pBaseCreature->SendFeedbackMessage(nMessage, pFeedback);
                        return bImmune;
                    }
                }
            }
        }
    }
    if (pEffect->m_pLinkLeft)
        bImmune = GetEffectLinkImmunity(pEffect->m_pLinkLeft);
    if (pEffect->m_pLinkRight && !bImmune)
        bImmune = GetEffectLinkImmunity(pEffect->m_pLinkRight);
    return bImmune;
}
