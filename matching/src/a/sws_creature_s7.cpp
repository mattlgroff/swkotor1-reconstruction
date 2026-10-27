// Worker A round s7: CSWSCreature range/experience helpers.
#include "a/sws_creature.h"

extern "C" double __cdecl ceil(double x);

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

template <class T> class CExoLinkedList {
public:
    T* GetHead() { return (T*)m_pList->GetHead(); }
    struct Internal {
        void* GetHead() { void* p = m_pHead; if (p) { p = ((CExoLinkedListNode*)p)->pObject; } return p; }
        CExoLinkedListNode* m_pHead;
    };
    Internal* m_pList;
};

// FUNCTION: SWKOTOR 0x004F1310
float CSWSCreature::DesiredAttackRange(unsigned long oidTarget, int bPersonalSpace)
{
    float fRange = 0.0f;
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject) {
        if (pObject->AsSWSCreature()) {
            CSWSCreature* pTarget = pObject->AsSWSCreature();
            if (bPersonalSpace) {
                float fSpace = pTarget->m_pFollowInfo->m_fPersonalSpace;
                return fSpace + m_pFollowInfo->m_fPersonalSpace + 0.7f;
            }
            float fValue = 0.0f;
            g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColPrefAttackDist, &fValue);
            fRange = fValue + 1.6f;
            g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(pTarget->m_nAppearanceType, g_nAppearanceColPrefAttackDist, &fValue);
            return fValue + fRange;
        }
        Vector vPosition;
        if (pObject->AsSWSDoor()) {
            vPosition = pObject->AsSWSDoor()->m_vPosition;
        } else if (pObject->AsSWSPlaceable()) {
            vPosition = pObject->AsSWSPlaceable()->m_vPosition;
        } else {
            return 1.5f;
        }
        GetUseRange(oidTarget, vPosition, fRange, 0);
    }
    return fRange;
}

// FUNCTION: SWKOTOR 0x004EF930
void CSWSCreature::ReceiveExperience(int nXP)
{
    int nNPCPercent = 100;
    int nPCPercent = 100;
    float fMultiplier = 1.0f;
    CSWPartyTable* pParty = GetServerExoApp()->GetPartyTable();
    C2DA* pTable = g_pRules->Get2DArrays()->m_pPartyTable;
    if (pParty) {
        int nNPC = pParty->GetNPCIndexOfObject(m_idSelf);
        if (nNPC != -1 && pTable) {
            {
                CExoString sColumn("PercentXP");
                pTable->GetINTEntry(nNPC, sColumn, &nNPCPercent);
            }
            m_sTag.CStr();
            fMultiplier = nNPCPercent * 0.01f;
        }
        {
            CExoString sColumn("PercentXP");
            pTable->GetINTEntry(9, sColumn, &nPCPercent);
        }
        fMultiplier = nPCPercent * 0.01f * fMultiplier;
    }
    m_sTag.CStr();
    GetStats()->AddExperience((int)ceil(nXP * fMultiplier));
}

// FUNCTION: SWKOTOR 0x004F19E0
float CSWSCreature::CalculateDeathExperience()
{
    unsigned long oidPC = GetServerExoApp()->GetPlayerList()->GetHead()->m_oidGameObject;
    CSWSCreature* pPC = GetServerExoApp()->GetCreatureByGameObjectID(oidPC);
    if (pPC) {
        unsigned long nExperience = pPC->GetStats()->m_nExperience;
        int nLevel;
        for (int i = 20; i >= 0; i--) {
            if (nExperience >= g_pRules->m_aExperienceTable[(unsigned char)i]) {
                nLevel = i + 1;
                break;
            }
        }
        int nCR = (int)GetStats()->m_fChallengeRating;
        float fXP = 0.0f;
        g_pRules->Get2DArrays()->m_pExpTable->GetFLOATEntry(nLevel - 1, nCR + 1, &fXP);
        float fPercent = 0.0f;
        {
            CExoString sColumn("PercentXP");
            g_pRules->Get2DArrays()->GetPartyTable()->GetFLOATEntry(9, sColumn, &fPercent);
        }
        return fXP * (fPercent * 0.01f);
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x004F2C30
int CSWSCreature::ValidNearestObjectType(CGameObject* pObject, int& bHostile)
{
    if (!pObject) {
        return 0;
    }
    int bValid = 0;
    bHostile = 0;
    switch (pObject->m_nObjectType) {
    case 5: {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->GetDead() || pCreature->GetDeadTemp()) {
            break;
        }
        CSWSCreatureListEntry* pEntry = GetFromList920(pCreature->m_idSelf);
        if (!pEntry || !pEntry->m_b0) {
            if (!DoSpotDetection(pCreature, 0)) {
                break;
            }
        }
        bValid = 1;
        if (GetAIStateReputation(pCreature->m_idSelf) == 2) {
            bHostile = 1;
        }
        break;
    }
    case 9: {
        CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
        if (!pPlaceable || !pPlaceable->m_bUseable) {
            break;
        }
        if (pPlaceable->m_bHostile340 && pPlaceable->GetReputation(m_idSelf, 0) <= 10) {
            bHostile = 1;
        } else {
            bHostile = 0;
        }
        bValid = 1;
        break;
    }
    case 10: {
        CSWSDoor* pDoor = pObject->AsSWSDoor();
        if (!pDoor || pDoor->m_bOpen || pDoor->m_bLocked3C0) {
            break;
        }
        bValid = 1;
        break;
    }
    case 7: {
        CSWSTrigger* pTrigger = pObject->AsSWSTrigger();
        if (!pTrigger || !pTrigger->m_bTrapFlag) {
            break;
        }
        unsigned long oidSelf = m_idSelf;
        if (pTrigger->m_lTrapDetectedBy.Contains(oidSelf) || pTrigger->GetReputation(oidSelf, 0) >= 90 ||
            pTrigger->m_nTrapFaction == GetStats()->m_nFactionId) {
            bValid = 1;
        }
        break;
    }
    default:
        bValid = 0;
        break;
    }
    return bValid;
}

// FUNCTION: SWKOTOR 0x004EB1B0
void CSWSCreature::ClearActivities(int nMask)
{
    if ((nMask & 0x1) && (m_n9FC & 0x1) && !(m_nA00 & 0x1)) {
        SetActivity(0x1, 0, 0);
    }
    if ((nMask & 0x4) && (m_n9FC & 0x4) && !(m_nA00 & 0x4)) {
        SetActivity(0x4, 0, 0);
    }
    if ((nMask & 0x8) && (m_n9FC & 0x8) && !(m_nA00 & 0x8)) {
        SetActivity(0x8, 0, 0);
    }
    if ((nMask & 0x10) && (m_n9FC & 0x10) && !(m_nA00 & 0x10)) {
        SetActivity(0x10, 0, 0);
    }
    if ((nMask & 0x20) && (m_n9FC & 0x20) && !(m_nA00 & 0x20)) {
        SetActivity(0x20, 0, 0);
    }
    if ((nMask & 0x40) && (m_n9FC & 0x40) && !(m_nA00 & 0x40)) {
        SetActivity(0x40, 0, 0);
    }
    if ((nMask & 0x80) && (m_n9FC & 0x80) && !(m_nA00 & 0x80)) {
        SetActivity(0x80, 0, 0);
    }
    if ((nMask & 0x100) && (m_n9FC & 0x100) && !(m_nA00 & 0x100)) {
        SetActivity(0x100, 0, 0);
    }
    if ((nMask & 0x200) && (m_n9FC & 0x200) && !(m_nA00 & 0x200)) {
        SetActivity(0x200, 0, 0);
    }
    if ((nMask & 0x400) && (m_n9FC & 0x400) && !(m_nA00 & 0x400)) {
        SetActivity(0x400, 0, 0);
    }
    if ((nMask & 0x800) && (m_n9FC & 0x800) && !(m_nA00 & 0x800)) {
        SetActivity(0x800, 0, 0);
    }
    if ((nMask & 0x1000) && (m_n9FC & 0x1000) && !(m_nA00 & 0x1000)) {
        SetActivity(0x1000, 0, 0);
    }
    if ((nMask & 0x2000) && (m_n9FC & 0x2000) && !(m_nA00 & 0x2000)) {
        SetActivity(0x2000, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x004ED850
void CSWSCreature::UpdateAppearanceDependantInfo(int bMovement)
{
    int nValue;
    {
        CExoString sColumn("haslegs");
        g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(m_nAppearanceType, sColumn, &nValue);
    }
    m_bHasLegs = nValue;
    {
        CExoString sColumn("hasarms");
        g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(m_nAppearanceType, sColumn, &nValue);
    }
    m_bHasArms = nValue;
    int nInjured;
    g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(m_nAppearanceType, g_nAppearanceCol_DisableInjuredAnim,
                                                          &nInjured);
    m_bAC4 = nInjured;
    int nSize;
    g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(m_nAppearanceType, g_nAppearanceCol_SIZECATEGORY, &nSize);
    m_n4F8 = nSize;
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearance()->GetCExoStringEntry(m_nAppearanceType, g_nAppearanceCol_MODELTYPE,
                                                                 &sModelType);
    m_sModelType = sModelType;
    float fValue;
    bool bFound;
    {
        CExoString sColumn("weaponscale");
        bFound = g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, sColumn, &fValue) == 1;
    }
    if (bFound) {
        m_fWeaponScale = fValue;
    }
    else {
        m_fWeaponScale = 1.0f;
    }
    if (bMovement) {
        GetStats()->SetMovementRate(7);
    }
    {
        CExoString sColumn("PREFATCKDIST");
        g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, sColumn, &fValue);
    }
    m_fPrefAttackDist = fValue;
    m_b344 = 1;
}

// FUNCTION: SWKOTOR 0x004ED4C0
void CSWSCreature::SetPortraitId(unsigned short nPortraitId)
{
    CExoString sPrefix("po_");
    if (nPortraitId >= 0xfffe) {
        CResRef cPortrait("");
        SetPortrait(cPortrait);
    }
    else {
        CExoString sResRef;
        {
            CExoString sColumn("BaseResRef");
            g_pRules->Get2DArrays()->GetPortraits()->GetCExoStringEntry(nPortraitId, sColumn, &sResRef);
        }
        sResRef = sResRef;
        {
            CResRef cPortrait(sResRef.CStr());
            SetPortrait(cPortrait);
        }
    }
    m_nPortraitId = nPortraitId;
}

extern const unsigned long ACTION_IN_PROGRESS;      // 0x00745DBC

// FUNCTION: SWKOTOR 0x004FCE90
int CSWSCreature::AddCounterSpellActions(unsigned long oidTarget)
{
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (oidTarget == 0x7f000000) {
        return 0;
    }
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (!pTarget) {
        return 0;
    }
    if (!GetArea()) {
        return 0;
    }
    if (oidTarget == m_idSelf) {
        return 0;
    }
    if (!pTarget->GetArea()) {
        return 0;
    }
    CSWSArea* pArea = pTarget->GetArea();
    if (!pArea) {
        return ACTION_IN_PROGRESS;
    }
    unsigned long oidArea = pArea->GetId();
    vPosition = pTarget->m_vPosition;
    float fRange = g_pRules->m_fRangeLong;
    int bRun = 1;
    int bOne = 1;
    if (!(m_nPartyFollowFlags & 2)) {
        return 0;
    }
    AddActionToFront(0x32, 0xffff, 3, &oidTarget);
    AddActionToFront(0x11, 0xfffe, 3, &oidTarget, 1, &bRun, 2, &fRange, 2, &fRange, 1, &bOne);
    AddMoveToPointActionToFront(0xfffe, vPosition, oidArea, 0x7f000000, bRun, fRange, 0.0f, 0, 0, 0, 0, 0, 0,
                                Vector(0.0f, 0.0f, 0.0f), 0, 0, 0);
    m_oidCounterSpellTarget = oidTarget;
    return 1;
}

// FUNCTION: SWKOTOR 0x004F9DA0
int CSWSCreature::AddMineActions(unsigned long oidTarget, unsigned char nAction, unsigned long oidArg,
                                 const Vector& vPosition, unsigned long)
{
    if (!m_bE8) {
        return 0;
    }
    if (nAction == 0x1d && GetArea() && !GetArea()->GetPlayerCanSetMines()) {
        return 0;
    }
    if (GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
        Fn4F1460();
        if (m_n9F2 < 10) {
            ClearAllActions(1);
        }
    }
    CGameObject* pTarget = GetServerExoApp()->GetGameObject(oidTarget);
    if (!pTarget) {
        return 0;
    }
    if (nAction == 0x19 || nAction == 0x1a) {
        if (pTarget->m_nObjectType == OBJECT_TYPE_TRIGGER && !pTarget->AsSWSTrigger()->m_n2C4 &&
            pTarget->AsSWSTrigger()->m_bTrapFlag == 1) {
            return 0;
        }
        if (pTarget->m_nObjectType == OBJECT_TYPE_DOOR && !pTarget->AsSWSDoor()->m_n2FC &&
            pTarget->AsSWSDoor()->m_n2E8 == 1) {
            return 0;
        }
        if (pTarget->m_nObjectType == OBJECT_TYPE_PLACEABLE && !pTarget->AsSWSPlaceable()->m_n280 &&
            pTarget->AsSWSPlaceable()->m_n278 == 1) {
            return 0;
        }
    }
    int bCanUse = GetStats()->GetCanUseSkill(1);
    if (nAction == 0x1d) {
        if (!bCanUse) {
            return 0;
        }
        float x = vPosition.x;
        float y = vPosition.y;
        float z = vPosition.z;
        AddAction(0x1d, 0xffff, 3, &oidTarget, 3, &oidArg, 2, &x, 2, &y, 2, &z);
    }
    else {
        if (!bCanUse) {
            return 0;
        }
        AddAction(nAction, 0xffff, 3, &oidTarget);
    }
    return 1;
}
