// CSWSCreatureStats::GetEffectLinkImmunity (b005b_f2_faction).

class C2DA {
public:
    int GetINTEntry(int nColumn, int nRow, int *pValue);
};

class CTwoDimArrays {
public:
    char pad_0x0[0xdc];
    C2DA *m_pImmunity2DA;            // +0xdc
};

class CSWRules {
public:
    char pad_0x0[0xb8];
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

class CSWSCreature {
public:
    void SendFeedbackMessage(int nMessage, CGameEffect *pEffect);
    char pad_0x0[0x4];
    unsigned long m_idSelf;          // +0x4
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

class CSWSCreatureStats {
public:
    int GetEffectImmunityColumn(unsigned short nType, int nSubType);
    int GetEffectImmunity(int nImmunityType, CSWSCreature *pVersus);
    int GetEffectLinkImmunity(CGameEffect *pEffect);

    char pad_0x0[0x24];
    CSWSCreature *m_pBaseCreature;   // +0x24
};

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
