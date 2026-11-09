// Worker A round s9: CSWSCreature forced movement and helpers.
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004EDBA0
int CSWSCreature::ForceMoveToPoint(CSWSForcedAction* pForced)
{
    CSWSObjectActionNode* pNode = new CSWSObjectActionNode();
    unsigned long oid;
    int bSafe = 1;
    float fRadius = 1.0f;
    float fFacing = 0.0f;
    pNode->m_n70 = 1;
    if (pForced->m_oidTarget == 0x7f000000) {
        pNode->m_nActionId = 5;
        pNode->m_nGroupId = pForced->m_nGroupId;
        oid = pForced->m_oidArea;
        Vector v = pForced->GetPosition();
        AddActionNodeParameter(pNode, 0, 2, &v.x);
        v = pForced->GetPosition();
        AddActionNodeParameter(pNode, 1, 2, &v.y);
        Vector v2 = pForced->GetPosition();
        AddActionNodeParameter(pNode, 2, 2, &v2.z);
        AddActionNodeParameter(pNode, 3, 3, &oid);
        AddActionNodeParameter(pNode, 4, 1, &bSafe);
        AddActionNodeParameter(pNode, 5, 2, &fRadius);
        AddActionNodeParameter(pNode, 6, 2, &fFacing);
        AIActionJumpToPoint(pNode);
    } else {
        pNode->m_nActionId = 0x30;
        pNode->m_nGroupId = pForced->m_nGroupId;
        oid = pForced->m_oidTarget;
        AddActionNodeParameter(pNode, 0, 3, &oid);
        AddActionNodeParameter(pNode, 1, 1, &bSafe);
        AIActionJumpToObject(pNode);
    }
    delete pNode;
    return 1;
}

class CSWSpell {
public:
    char pad_0x0[0x140];
    unsigned char m_n140;                           // 0x140 (placeholder name)
    char pad_0x141[0x144 - 0x141];
    int m_n144;                                     // 0x144 (placeholder name)
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

static inline void ClearList(CExoArrayList<unsigned long>* pList)
{
    if (pList->element) {
        delete[] pList->element;
        pList->element = 0;
        pList->array_size = 0;
    }
    pList->num = 0;
}

static inline void AddToList(CExoArrayList<unsigned long>* pList, unsigned long n)
{
    if (pList->num == pList->array_size) {
        pList->Allocate(pList->array_size ? pList->array_size * 2 : 16);
    }
    pList->element[pList->num] = n;
    pList->num++;
}

// NONMATCH (16): the original keeps a dead store of the GetFromList920 result in the loop counter slot
// (only a volatile local reproduces it, 12 left: eax/edx rotation). The message data is never sent.
// FUNCTION: SWKOTOR 0x004FB9E0
void CSWSCreature::BroadcastSpellCast(unsigned long nSpell, int a2, int a3, int a4, unsigned long oidTarget)
{
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
    CSWSArea* pArea = GetArea();
    if (!pArea) {
        return;
    }
    CExoArrayList<unsigned long>* pList = &pArea->m_lPlayers;
    if (!pList) {
        return;
    }
    ClearList(&m_lSpellCastSeenBy);
    CSWCCMessageData* pData = new CSWCCMessageData();
    pData->SetObjectID(0, m_idSelf);
    pData->SetObjectID(1, 0x7f000000);
    pData->SetObjectID(2, oidTarget);
    pData->SetInteger(0, nSpell);
    pData->SetInteger(2, 1);
    CSWSCreatureListEntry* pEntry;
    for (int i = 0; i < pList->num; i++) {
        CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(pList->element[i]);
        if (pCreature) {
            Vector v = pCreature->m_vPosition - m_vPosition;
            float fRange = g_pRules->m_fRangeLong;
            if (v.MagnitudeSquared() > fRange * fRange) {
                continue;
            }
            if (pSpell->m_n140 == 1 && pSpell->m_n144 == 0) {
                pEntry = pCreature->GetFromList920(m_idSelf);
            } else {
                AddToList(&m_lSpellCastSeenBy, pCreature->m_idSelf);
            }
        }
    }
    if (pData) {
        delete pData;
    }
}

struct CClientOptions {
    char pad_0x0[0x4];
    unsigned char m_nDifficulty;                    // 0x04
};

extern int g_nDamageMultiplier;                     // 0x007A1B30 (descriptive name)
extern const int EFFECT_TYPE_TEMPORARY_HITPOINTS;   // 0x007466B8 (descriptive name)

// NONMATCH (51): the original reads the effect type word twice (no CSE) and picks other scratch
// registers for the app-manager/rules loads; for/while forms and hoisted locals did not change it.
// FUNCTION: SWKOTOR 0x004F3830
void CSWSCreature::DoDamage(int nDamage, int bNoFloaty)
{
    int nAmount;
    int nValue;
    CGameEffect* pEffect;
    int i;
    CSWPartyTable* pParty = GetServerExoApp()->GetPartyTable();
    if ((pParty && pParty->GetIsNPCObjectMember(m_idSelf)) || m_b9D4) {
        int nDifficulty = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nDifficulty;
        float fMultiplier;
        int bFound;
        {
            CExoString sColumn("Multiplier");
            bFound = g_pRules->Get2DArrays()->GetDifficultyOptions()->GetFLOATEntry(nDifficulty, sColumn,
                                                                                    &fMultiplier);
        }
        if (bFound) {
            nAmount = (int)(nDamage * fMultiplier);
        } else {
            nAmount = nDamage;
        }
    } else {
        nAmount = nDamage;
    }
    if (!m_bA88 && g_nDamageMultiplier > 1) {
        nAmount = g_nDamageMultiplier * nAmount;
    }
    if (m_nTemporaryHitPoints > 0) {
        int nRemaining = 0;
        CExoArrayList<CGameEffect*>* pEffects = &AsSWSCreature()->m_appliedEffects;
        if (pEffects->num > 0) {
            for (i = m_pStats->m_nTemporaryHitPointEffectStart; i < pEffects->num; i++) {
                pEffect = pEffects->element[i];
                if (pEffect->GetType() > EFFECT_TYPE_TEMPORARY_HITPOINTS) {
                    break;
                }
                if (pEffect->GetType() == EFFECT_TYPE_TEMPORARY_HITPOINTS) {
                    nValue = pEffect->GetInteger(0);
                    if (nValue > nAmount) {
                        nValue -= nAmount;
                        nAmount = 0;
                        pEffect->SetInteger(0, nValue);
                        nRemaining += nValue;
                    } else {
                        nAmount -= nValue;
                        AsSWSCreature()->RemoveEffectById(pEffect->GetID());
                    }
                }
            }
        }
        m_nTemporaryHitPointsDword = nRemaining;
    }
    if (GetServerExoApp()->GetDifficultyOption(5) == 1 && m_b9D4 == 1) {
        if (GetCurrentHitPoints(1) - nAmount < 1) {
            nAmount = GetCurrentHitPoints(1) - 1;
            if (nAmount < 0) {
                nAmount = 0;
            }
        }
    }
    CClientExoApp* pClient = GetClientExoApp();
    unsigned long oidClient = pClient->ServerToClientObjectId(m_idSelf);
    if (pClient->GetPlayerCreatureId() == oidClient && !bNoFloaty) {
        pClient->AddFloatyText(oidClient, 0, nAmount);
    }
    DamageHitPoints(nAmount);
}
