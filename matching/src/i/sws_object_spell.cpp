// CSWSObject spell helpers: BroadcastSpellData (0x004CB890) and
// DoSpellImmunity (0x004CCFC0).
#include <math.h>
#include "i/sws_object.h"

class CSWSItem {
public:
    char pad_0x0[0xc];
    int m_nBaseItem;                                // 0x0c (placeholder name)
};

class CSWSPlayer {
public:
    char pad_0x0[0x4];
    unsigned long m_nPlayerID;                      // 0x04
};

extern const int EFFECT_TYPE_SPELL_IMMUNITY;        // 0x00746744 (placeholder name)
extern const float BROADCAST_SPELL_RANGE_SQUARED;   // 0x007455BC (placeholder name)

// FUNCTION: SWKOTOR 0x004CB890
void CSWSObject::BroadcastSpellData(void* pSpell, CSWCCMessageData* pData)
{
    CGameObject* pArea = NULL;
    CGameObjectArray* pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
    pArray->GetGameObject(m_oidArea, &pArea);
    if (pArea == NULL || pArea->AsSWSArea() == NULL) {
        return;
    }
    unsigned long oidPC = g_pAppManager->m_pServerExoApp->GetFirstPCObject();
    CSWSCreature* pPC = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidPC);
    if (pPC == NULL) {
        return;
    }
    CSWSPlayer* pPlayer =
        (CSWSPlayer*)g_pAppManager->m_pServerExoApp->GetClientObjectByObjectID(oidPC);
    Vector vDiff = pPC->m_vPosition - m_vPosition;
    if (pPlayer == NULL ||
        !(vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z <=
          BROADCAST_SPELL_RANGE_SQUARED)) {
        return;
    }
    if (AsSWSCreature() && pData->GetObjectID(1) != 0x7f000000) {
        CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(pData->GetObjectID(1));
        if (pItem) {
            pData->SetInteger(3, pItem->m_nBaseItem);
        }
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    g_pAppManager->m_pServerExoApp->GetNWSMessage()->SendServerToPlayerCCMessage(nPlayerId, 8,
                                                                                 pData, NULL);
}

// FUNCTION: SWKOTOR 0x004CCFC0
int CSWSObject::DoSpellImmunity(CSWSObject* pCaster)
{
    int nSpellId = pCaster->m_nLastSpellId;
    if (AsSWSCreature() && m_appliedEffects.num > 0) {
        for (int i = AsSWSCreature()->GetCreatureStats()->m_nSpellImmunityIndex;
             i < m_appliedEffects.num &&
             m_appliedEffects[i]->m_nType <= EFFECT_TYPE_SPELL_IMMUNITY;
             i++) {
            CGameEffect* pEffect = m_appliedEffects[i];
            if (pEffect->m_nType == EFFECT_TYPE_SPELL_IMMUNITY &&
                (pEffect->GetInteger(0) == nSpellId || pEffect->GetInteger(0) == -1)) {
                CSWCCMessageData* pData = new CSWCCMessageData();
                CSWCCMessageData* pCopy = new CSWCCMessageData();
                pData->SetInteger(1, nSpellId);
                pData->SetObjectID(0, m_idSelf);
                pData->CopyTo(pCopy);
                if (AsSWSCreature()) {
                    AsSWSCreature()->SendFeedbackMessage(0x44, pData);
                }
                else if (pData) {
                    delete pData;
                }
                if (pCaster->AsSWSCreature()) {
                    pCaster->AsSWSCreature()->SendFeedbackMessage(0x44, pCopy);
                    return 2;
                }
                if (pCopy) {
                    delete pCopy;
                }
                return 2;
            }
        }
    }
    return -1;
}

class CSWSpell {
public:
    char pad_0x0[0xf4];
    int m_bHasProjectile;                           // 0xf4 (placeholder name)
    char pad_0xf8[0x108 - 0xf8];
    int m_nProjectileType;                          // 0x108 (placeholder name)
};



float Magnitude(const Vector& v);                   // 0x004AB100 (cdecl)

// FUNCTION: SWKOTOR 0x004CB9E0
void CSWSObject::CalculateLastSpellProjectileTime(unsigned char nType)
{
    Vector vDelta(0.0f, 0.0f, 0.0f);
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(m_nLastSpellId);
    if (pSpell == NULL) {
        return;
    }
    if (pSpell->m_bHasProjectile == 0) {
        m_nLastSpellProjectileTime = 0;
        return;
    }
    if (m_oidLastSpellTarget != OBJECT_INVALID_ID) {
        CGameObject* pTarget;
        CGameObjectArray* pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
        if (pArray->GetGameObject(m_oidLastSpellTarget, &pTarget) == GAME_OBJECT_VALID) {
            vDelta = ((CSWSObject*)pTarget)->m_vPosition - m_vPosition;
        }
    }
    else {
        vDelta = m_vLastSpellTarget - m_vPosition;
    }
    float fSpeed = (float)log(Magnitude(vDelta)) * 3.0f + 2.0f;
    if (nType == 0) {
        nType = (unsigned char)pSpell->m_nProjectileType;
    }
    switch (nType) {
    case 1:
        fSpeed = fSpeed + fSpeed;
        break;
    case 3:
        m_nLastSpellProjectileTime = 2000;
        return;
    case 5:
        fSpeed = fSpeed * 1.5f;
        break;
    case 7:
        fSpeed = Magnitude(vDelta) * 0.5f;
        break;
    case 8:
        fSpeed = fSpeed * 0.4f;
        break;
    }
    if (fSpeed <= 0.0f) {
        m_nLastSpellProjectileTime = 1;
        return;
    }
    m_nLastSpellProjectileTime = (int)(Magnitude(vDelta) / fSpeed * 1000.0f);
    if (pSpell->m_nProjectileType == 6) {
        m_nLastSpellProjectileTime += 2500;
    }
}
