// CSWSObject: animation, hit point, damage immunity, dialog delay and
// hostile actor helpers (0x004CAC30-0x004CC0C0).
#include "i/sws_object.h"

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    int AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDay,
                      unsigned long* pTime);                        // 0x004ADF50
};

extern const unsigned long g_nMillisecondsPerSecond;               // 0x00744174 (placeholder name)

static inline int ClampImmunity(int nImmunity)
{
    if (nImmunity < -100) {
        return -100;
    }
    if (nImmunity > 100) {
        return 100;
    }
    return nImmunity;
}

// FUNCTION: SWKOTOR 0x004CAC30
void CSWSObject::AddActionNodeParameter(CSWSObjectActionNode* pNode, unsigned long nIndex,
                                        unsigned long nType, void* pValue)
{
    pNode->m_nNumParams = (unsigned short)(nIndex + 1);
    if (nIndex >= 13) {
        return;
    }
    pNode->m_nParamType[nIndex] = nType;
    if (nType == 1) {
        pNode->m_pParam[nIndex] = (void*)*(int*)pValue;
    }
    else if (nType == 2) {
        pNode->m_pParam[nIndex] = (void*)*(unsigned long*)pValue;
    }
    else if (nType == 4) {
        CExoString* pString = new CExoString;
        pNode->m_pParam[nIndex] = pString;
        *pString = *(CExoString*)pValue;
    }
    else if (nType == 3) {
        pNode->m_pParam[nIndex] = (void*)*(unsigned long*)pValue;
    }
    else if (nType == 5) {
        pNode->m_pParam[nIndex] = pValue;
    }
}

// FUNCTION: SWKOTOR 0x004CAD20
void CSWSObject::SetPortraitId(unsigned short nPortraitId)
{
    CExoString sPrefix("po_");
    if (nPortraitId >= 0xfffe) {
        CResRef cPortrait("");
        SetPortrait(cPortrait);
    }
    else {
        CExoString sPortrait;
        {
            CExoString sColumn("BaseResRef");
            C2DA* pPortraits = g_pRules->m_p2DArrays->m_pPortraits;
            pPortraits->GetCExoStringEntry(nPortraitId, sColumn, &sPortrait);
        }
        sPortrait = sPrefix + sPortrait;
        {
            CResRef cPortrait(sPortrait.CStr());
            SetPortrait(cPortrait);
        }
    }
    m_nPortraitId = nPortraitId;
}

// FUNCTION: SWKOTOR 0x004CAE60
int CSWSObject::GetIsAnimationStationary(unsigned short nAnimation)
{
    if (nAnimation == 10002 || nAnimation == 10003 || nAnimation == 10004 ||
        nAnimation == 10078 || nAnimation == 10079 || nAnimation == 10085 ||
        nAnimation == 10084 || nAnimation == 10087 || nAnimation == 10086 ||
        nAnimation == 10093 || nAnimation == 10094 || nAnimation == 10133) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004CAEC0
short CSWSObject::GetCurrentHitPoints(int bExcludeTemporary)
{
    if (bExcludeTemporary == 1) {
        return (short)m_nCurrentHitPoints;
    }
    return (short)(m_nCurrentHitPoints + m_nTemporaryHitPoints);
}

// FUNCTION: SWKOTOR 0x004CAEE0
int CSWSObject::GetDamageImmunity(unsigned char nType)
{
    char nImmunity = m_pDamageImmunity[nType];
    if (nImmunity < 100) {
        if ((int)nImmunity < -100) {
            return -100;
        }
        if (nImmunity < 100) {
            return nImmunity;
        }
    }
    return 100;
}

// FUNCTION: SWKOTOR 0x004CAF20
void CSWSObject::SetDamageImmunity(unsigned short nFlags, char nValue)
{
    if (nValue < -100) {
        nValue = -100;
    }
    else if (nValue > 100) {
        nValue = 100;
    }
    for (int i = 0; i < 15; i++) {
        if ((unsigned short)(nFlags >> i) & 1) {
            m_pDamageImmunity[i] = nValue;
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004CAF70
int CSWSObject::GetDamageImmunityByFlags(unsigned short nFlags)
{
    short nImmunity = 0;
    for (int i = 0; i < 15; i++) {
        if ((unsigned short)(nFlags >> i) & 1) {
            if (nImmunity == 0 || (char)GetDamageImmunity((unsigned char)i) < nImmunity) {
                nImmunity = (char)GetDamageImmunity((unsigned char)i);
            }
        }
    }
    return ClampImmunity(nImmunity);
}

// FUNCTION: SWKOTOR 0x004CB020
unsigned char CSWSObject::GetDamageLevel()
{
    float fRatio;
    if (AsSWSCreature()) {
        CSWSCreature* pCreature = AsSWSCreature();
        CSWSCreature* pCreature2 = AsSWSCreature();
        float fHitPoints = pCreature->GetCurrentHitPoints(0);
        fRatio = fHitPoints / pCreature2->GetMaxHitPoints(1);
    }
    else {
        float fHitPoints = GetCurrentHitPoints(0);
        fRatio = fHitPoints / GetMaxHitPoints(1);
    }
    if (fRatio >= 0.95f) {
        return 0;
    }
    if (fRatio >= 0.75f) {
        return 1;
    }
    if (fRatio >= 0.5f) {
        return 2;
    }
    if (fRatio >= 0.25f) {
        return 3;
    }
    if (fRatio <= 0.0f) {
        return 5;
    }
    return 4;
}

// FUNCTION: SWKOTOR 0x004CB1A0
void CSWSObject::SetDialogDelay(float fSeconds)
{
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&m_n40, &m_n44);
    GetServerExoApp()->GetWorldTimer()->AddWorldTimes(
        m_n40, m_n44, 0, (unsigned long)(g_nMillisecondsPerSecond * fSeconds),
        (unsigned long*)&m_n38, (unsigned long*)&m_n3c);
}

// FUNCTION: SWKOTOR 0x004CC0C0
void CSWSObject::SetLastHostileActor(unsigned long oidActor, int bForce)
{
    if (bForce == 1) {
        m_oidLastHostileActor = oidActor;
        return;
    }
    if (m_bf8 == 1) {
        m_oidLastHostileActor = 0x7f000000;
        return;
    }
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oidActor);
    if (pObject == NULL) {
        return;
    }
    CSWSCreature* pCreature;
    if (pObject->m_nObjectType == OBJECT_TYPE_AREA_OF_EFFECT) {
        pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
            pObject->AsSWSAreaOfEffectObject()->m_oidCreator);
    }
    else if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE && pObject->m_idSelf != m_idSelf) {
        pCreature = pObject->AsSWSCreature();
    }
    else {
        pCreature = NULL;
    }
    if (pCreature == NULL || pCreature->GetIsDead() == 1 || pCreature->GetDeadTemp() == 1) {
        m_oidLastHostileActor = 0x7f000000;
        return;
    }
    if (AsSWSCreature() && GetArea()) {
        unsigned long nFaction = pCreature->m_pStats->m_nFactionID;
        if (nFaction == AsSWSCreature()->m_pStats->m_nFactionID) {
            return;
        }
    }
    m_oidLastHostileActor = pCreature->m_idSelf;
}
