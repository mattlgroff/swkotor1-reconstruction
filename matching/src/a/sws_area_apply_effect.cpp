// CSWSArea::ApplyEffect (0x0050C6B0): area-of-effect, summon, visual
// effect and link effects applied at a location in the area.
#include "a/sws_area.h"

class CGameEffect {
public:
    ~CGameEffect();                                 // 0x00503590
    int GetInteger(int nIndex);                     // 0x00503690
    void SetFloat(int nIndex, float fValue);        // 0x005036D0
    void SetObjectID(int nIndex, unsigned long oid); // 0x005036F0
    CExoString GetString(int nIndex);               // 0x00503700
    void UpdateLinked();                            // 0x00504240
    void SetLinked(CGameEffect* pLeft, CGameEffect* pRight); // 0x00504810

    unsigned short GetType() { return m_nType; }
    unsigned short GetSubType() { return m_nSubType; }
    unsigned short GetDurationType() { return m_nSubType & 7; }
    float GetDuration() { return m_fDuration; }
    unsigned long GetCreator() { return m_oidCreator; }
    unsigned long GetSpellId() { return m_nSpellId; }

    char pad_0x0[0x8];
    unsigned short m_nType;                         // 0x08
    unsigned short m_nSubType;                      // 0x0a
    float m_fDuration;                              // 0x0c
    char pad_0x10[0x18 - 0x10];
    unsigned long m_oidCreator;                     // 0x18
    unsigned long m_nSpellId;                       // 0x1c
    char pad_0x20[0x28 - 0x20];
    CGameEffect* m_pLinkLeft;                       // 0x28
    CGameEffect* m_pLinkRight;                      // 0x2c
};

class CExoArrayListObjectId {
public:
    void AddUnique(unsigned long oid);              // 0x006449D0
};

class CSWSObjectAoEList : public CSWSObject {
public:
    void AddAreaOfEffect(unsigned long oid) { m_aAreaOfEffects.AddUnique(oid); }

    char pad_0x9c[0x13c - 0x9c];
    CExoArrayListObjectId m_aAreaOfEffects;         // 0x13c (placeholder name)
};

class CSWSCreatureEffects {
public:
    void ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bArg3); // 0x004D14F0
};

extern const int g_nEffectAreaOfEffect;  // 0x007466F8
extern const int g_nEffectSummonCreature; // 0x00746694
extern const int g_nEffectVisualEffect;  // 0x007466F4
extern const int g_nEffectLink;          // 0x0074671C

// FUNCTION: SWKOTOR 0x0050C6B0
void CSWSArea::ApplyEffect(CGameEffect* pEffect, Vector* pPosition, Vector* pOrientation)
{
    if (pEffect->GetType() == g_nEffectAreaOfEffect) {
        CSWSAreaOfEffectObject* pAoE = new CSWSAreaOfEffectObject(0x7f000000);
        pAoE->SetCreator(pEffect->GetCreator());
        if ((pEffect->m_nSubType & 0x18) == 8 && pEffect->m_oidCreator != 0x7f000000) {
            CGameObject* pCreator = GetServerExoApp()->GetGameObject(pEffect->m_oidCreator);
            if (pCreator && pCreator->AsSWSObject()) {
                ((CSWSObjectAoEList*)pCreator->AsSWSObject())->AddAreaOfEffect(pAoE->m_idSelf);
            }
        }
        pAoE->SetShape(pEffect->GetInteger(0));
        pAoE->SetDuration((unsigned char)pEffect->GetDurationType(), pEffect->GetDuration());
        pAoE->SetOrientation(*pOrientation);
        pAoE->SetEffectSpellId(pEffect->GetSpellId());
        if (pEffect->GetString(0) != "") {
            pAoE->m_sScriptOnEnter = pEffect->GetString(0);
        }
        if (pEffect->GetString(1) != "") {
            pAoE->m_sScriptOnExit = pEffect->GetString(1);
        }
        if (pEffect->GetString(2) != "") {
            pAoE->m_sScriptHeartbeat = pEffect->GetString(2);
        }
        pAoE->AddToArea(this, pPosition->x, pPosition->y, pPosition->z, 1);
    } else if (pEffect->GetType() == g_nEffectSummonCreature) {
        CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
        if (pCreature) {
            pEffect->SetObjectID(0, m_idSelf);
            pEffect->SetFloat(0, pPosition->x);
            pEffect->SetFloat(1, pPosition->y);
            pEffect->SetFloat(2, pPosition->z);
            ((CSWSCreatureEffects*)pCreature)->ApplyEffect(pEffect, 0, 0);
            return;
        }
    } else if (pEffect->GetType() == g_nEffectVisualEffect) {
        PlayVisualEffect((unsigned short)pEffect->GetInteger(0), pPosition);
    } else if (pEffect->GetType() == g_nEffectLink) {
        pEffect->UpdateLinked();
        if (pEffect->m_pLinkLeft) {
            ApplyEffect(pEffect->m_pLinkLeft, pPosition, pOrientation);
        }
        if (pEffect->m_pLinkRight) {
            ApplyEffect(pEffect->m_pLinkRight, pPosition, pOrientation);
        }
        pEffect->SetLinked(NULL, NULL);
    }
    delete pEffect;
}
