// CSWSObject leaves, destructor and effect list management
// (vtable 0x00745F18, 0x004D0170-0x004D1D47; behavioural reference:
// src/batches/b004ca_sws_object_dialog_effects.cpp).
#include "a/sws_object.h"

// FUNCTION: SWKOTOR 0x004D0170
CResRef CSWSObject::GetDialogResref()
{
    return CResRef("");
}

// FUNCTION: SWKOTOR 0x004D0190
CExoLocString* CSWSObject::GetFirstName()
{
    return &m_sLocName;
}

// FUNCTION: SWKOTOR 0x004D01A0
unsigned short CSWSObject::GetGender(int nUnused)
{
    return m_nE0;
}

// FUNCTION: SWKOTOR 0x004D01B0
unsigned long CSWSObject::GetLastTriggered()
{
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004D01C0
CResRef CSWSObject::GetTemplateResRef()
{
    return m_cTemplateResRef;
}

// FUNCTION: SWKOTOR 0x004D01F0
void CSWSObject::SetTemplateResRef(const CResRef& cResRef)
{
    m_cTemplateResRef = cResRef;
}

// FUNCTION: SWKOTOR 0x004D0210
unsigned short CSWSObject::Get30()
{
    return m_n30;
}

// NONMATCH (10): the effect loop init is `lea ebp,[eax-1]; cmp ebp,ebx; jl` in the
// original; every loop form tried gives `dec ebp; js` (j >= 0, 0 <= j, !(j < 0)).
// FUNCTION: SWKOTOR 0x004D0220
CSWSObject::~CSWSObject()
{
    if (m_p34) {
        delete m_p34;
        m_p34 = NULL;
    }
    int i;
    for (i = 0; i < m_lstListenExpressions.num; i++) {
        delete m_lstListenExpressions.element[i];
    }
    ClearMatchedExpressionStrings();
    GetServerExoApp()->GetObjectArray()->Delete(m_idSelf);
    for (i = m_appliedEffects.num - 1; i >= 0; i--) {
        delete m_appliedEffects.element[i];
    }
    m_appliedEffects.SetSize(0);
    if (m_p1AC) {
        delete[] m_p1AC;
        m_p1AC = NULL;
    }
    if (m_p168) {
        delete[] m_p168;
        m_p168 = NULL;
    }
    while (!m_lQueuedActions.IsEmpty()) {
        CSWSObjectActionNode* pNode = m_lQueuedActions.RemoveHead();
        delete pNode;
    }
    if (m_p224) {
        m_p224->Destroy();
        m_p224 = NULL;
    }
}

// FUNCTION: SWKOTOR 0x004D0480
void CSWSObject::LoadListenData(CResGFF* pRes, CResStruct* pStruct)
{
    CExoString sExpression;
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "ExpressionList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cElement;
            int bSuccess;
            pRes->GetListElement(&cElement, &cList, i);
            int nID = pRes->ReadFieldINT(&cElement, "ExpressionId", bSuccess, 0);
            {
                CExoString sDefault("");
                sExpression = pRes->ReadFieldCExoString(&cElement, "ExpressionString", bSuccess, sDefault);
            }
            SetListenExpression(sExpression, nID);
        }
    }
}

// FUNCTION: SWKOTOR 0x004D05C0
void CSWSObject::RemoveEffect(CGameEffect* pEffect)
{
    CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
    if (pAIMaster->OnEffectRemoved(this, pEffect) == g_nTRUE) {
        m_appliedEffects.DelIndex(m_appliedEffects.IndexOf(pEffect));
        unsigned long oidCreator = pEffect->m_oidCreator;
        CGameObject* pCreator = GetServerExoApp()->GetGameObject(oidCreator);
        if (pCreator && pCreator->AsSWSObject()) {
            unsigned long oidSelf = m_idSelf;
            pCreator->AsSWSObject()->RemoveEffectTarget(oidSelf);
        }
        if (m_nObjectType == g_nObjectTypeCreature) {
            OnEffectListChanged(pEffect);
        }
        delete pEffect;
        if (m_nObjectType == g_nObjectTypeCreature) {
            UpdateEffectPtrs();
        }
    }
}

// FUNCTION: SWKOTOR 0x004D06C0
int CSWSObject::RemoveEffectById(unsigned __int64 nID)
{
    int bRemoved = 0;
    CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
    int i = 0;
    while (i < m_appliedEffects.num) {
        CGameEffect* pEffect = m_appliedEffects[i];
        if (pEffect->m_nID == nID) {
            unsigned long oidCreator = pEffect->m_oidCreator;
            CGameObject* pCreator = GetServerExoApp()->GetGameObject(oidCreator);
            if (pAIMaster->OnEffectRemoved(this, pEffect) == g_nTRUE) {
                int j = 0;
                for (; j < m_appliedEffects.num; j++) {
                    if (m_appliedEffects.element[j] == pEffect) {
                        break;
                    }
                }
                m_appliedEffects.num--;
                for (; j < m_appliedEffects.num; j++) {
                    m_appliedEffects.element[j] = m_appliedEffects.element[j + 1];
                }
                if (m_nObjectType == g_nObjectTypeCreature) {
                    UpdateEffectPtrs();
                }
                if (m_nObjectType == g_nObjectTypeCreature) {
                    OnEffectListChanged(pEffect);
                }
                delete pEffect;
                bRemoved = 1;
                if (pCreator && pCreator->AsSWSObject()) {
                    unsigned long oidSelf = m_idSelf;
                    pCreator->AsSWSObject()->RemoveEffectTarget(oidSelf);
                }
            }
            else {
                i++;
            }
        }
        else {
            i++;
        }
    }
    return bRemoved;
}

// FUNCTION: SWKOTOR 0x004D0820
void CSWSObject::RemoveEffectByCreator(unsigned long oidCreator)
{
    if (m_appliedEffects.num > 0) {
        for (int i = 0; i < m_appliedEffects.num; i++) {
            CGameEffect* pEffect = m_appliedEffects[i];
            if (pEffect->m_oidCreator == oidCreator) {
                RemoveEffect(pEffect);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004D0870
void CSWSObject::RemoveSomeEffectsOfDurationType(unsigned short nType)
{
    unsigned long nRemoved = 0;
    for (unsigned long i = 0; i < (unsigned long)m_appliedEffects.num; i++) {
        CGameEffect* pEffect = m_appliedEffects[i];
        if (pEffect->GetDurationType() == nType) {
            RemoveEffectById(pEffect->m_nID);
            i = 0;
            if (++nRemoved > 20) {
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004D08D0
void CSWSObject::RemoveEffectByType(unsigned long nSpellId, int nType, int nValue)
{
    for (int i = 0; i < m_appliedEffects.num; i++) {
        CGameEffect* pEffect = m_appliedEffects[i];
        if (pEffect->m_nSpellId == nSpellId && pEffect->m_nType == nType &&
            pEffect->GetInteger(0) == nValue) {
            RemoveEffect(m_appliedEffects[i]);
        }
    }
}

// FUNCTION: SWKOTOR 0x004D0940
void CSWSObject::ClearAllEffects(int bKeepOwn)
{
    if (m_appliedEffects.num != 0) {
        for (int i = m_appliedEffects.num - 1; i >= 0; i--) {
            CGameEffect* pEffect = m_appliedEffects[i];
            if (pEffect && pEffect->GetDurationType() != g_nDurationEquipped &&
                pEffect->GetDurationType() != g_nDurationInnate &&
                pEffect->m_nType != g_nEffectType7466A0 &&
                (!bKeepOwn || pEffect->m_oidCreator != m_idSelf)) {
                RemoveEffect(pEffect);
                i = m_appliedEffects.num;
            }
        }
        m_bE8 = 1;
    }
}

// FUNCTION: SWKOTOR 0x004D09C0 SYMBOL: ??_GCSWSObject@@UAEPAXI@Z

// NONMATCH (47): the original keeps 1 as an immediate (cmp eax,1 / cmp byte,1) and
// pushes ebp only around the dead loops; ours caches 1 in ebx and saves ebp up front.
// FUNCTION: SWKOTOR 0x004D13A0
int CSWSObject::DoSpellLevelAbsorption(CSWSObject* pCaster)
{
    if (pCaster && pCaster->AsSWSCreature() && m_appliedEffects.num > 0) {
        int nSpellId = pCaster->m_nLastSpellId;
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpellId);
        if (pSpell) {
            if (pCaster->AsSWSCreature()->m_n964 != 1) {
                unsigned char nClass = pCaster->m_nLastSpellCastClass;
                int nLevel = pCaster->AsSWSCreature()->m_pStats->GetSpellLevel(nClass);
                if (nLevel != 0xff) {
                    pSpell->SetSpellLevel(nLevel);
                }
            }
            if (pSpell->m_n140 != 1) {
                return -1;
            }
            int i;
            for (i = AsSWSCreature()->m_pStats->m_nSpellLevelAbsorptionStart;
                 i < m_appliedEffects.num && m_appliedEffects[i]->m_nType <= g_nEffectType746780; i++) {
            }
            for (i = AsSWSCreature()->m_pStats->m_nSpellLevelAbsorptionStart;
                 i < m_appliedEffects.num && m_appliedEffects[i]->m_nType <= g_nEffectType746780; i++) {
            }
            if (AsSWSCreature()) {
                pCaster->AsSWSCreature();
            }
        }
    }
    return -1;
}

// NONMATCH (19): the effect-type test re-checks type == 0x7466F4 after the 0x746788
// test with the constant held in ecx; MSVC folds that re-check in every form tried.
// FUNCTION: SWKOTOR 0x004D14F0
void CSWSObject::ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bFlag)
{
    unsigned long nCurrentDay;
    unsigned long nCurrentTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCurrentDay, &nCurrentTime);
    pEffect->m_bShowIcon = (m_b1EC || !pEffect->m_bExpose) ? 1 : 0;
    if (GetServerExoApp()->GetServerAIMaster()->OnEffectApplied(this, pEffect, bLoadingGame) != g_nTRUE) {
        int nType = pEffect->m_nType;
        if (nType == g_nEffectType7466F4 || nType == g_nEffectType746788) {
            if (nType == g_nEffectType7466F4 &&
                (pEffect->GetInteger(0) == 0x1388 || pEffect->GetInteger(0) == 0x1389)) {
                m_n148 = 0;
            }
        }
        else {
            m_n148 = 0;
        }
        if (pEffect->GetDurationType() == g_nDurationTemporary && !bLoadingGame) {
            unsigned long nExpiryDay;
            unsigned long nExpiryTime;
            float fDuration = pEffect->m_fDuration;
            unsigned long nDays = GetServerExoApp()->GetWorldTimer()->GetCalendarDayFromSeconds(fDuration);
            fDuration = pEffect->m_fDuration;
            unsigned long nTime = GetServerExoApp()->GetWorldTimer()->GetTimeOfDayFromSeconds(fDuration);
            GetServerExoApp()->GetWorldTimer()->AddWorldTimes(nCurrentDay, nCurrentTime, nDays, nTime,
                                                              &nExpiryDay, &nExpiryTime);
            pEffect->SetExpiryTime(nExpiryDay, nExpiryTime);
        }
        int i = 0;
        while (i < m_appliedEffects.num && pEffect->m_nType >= m_appliedEffects[i]->m_nType) {
            i++;
        }
        m_appliedEffects.Insert(pEffect, i);
        if (m_nObjectType == g_nObjectTypeCreature) {
            UpdateEffectPtrs();
            OnEffectListChanged(pEffect);
        }
        if ((pEffect->m_nSubType & 0x18) == 8) {
            unsigned long oidCreator = pEffect->m_oidCreator;
            CGameObject* pCreator = GetServerExoApp()->GetGameObject(oidCreator);
            if (pCreator && pCreator->AsSWSObject()) {
                unsigned long oidSelf = m_idSelf;
                pCreator->AsSWSObject()->m_lstEffectTargets.Add(oidSelf);
            }
        }
    }
    else {
        if (pEffect->m_nType == g_nEffectType74671C && bFlag == 1 && m_n148 == 1) {
            RemoveEffectById(pEffect->m_nID);
            return;
        }
        delete pEffect;
    }
}

// FUNCTION: SWKOTOR 0x004D1BE0
void CSWSObject::LoadEffectList(CResGFF* pRes, CResStruct* pStruct)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "EffectList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cElement;
            pRes->GetListElement(&cElement, &cList, i);
            CGameEffect* pEffect = new CGameEffect(0);
            pEffect->LoadGameEffect(pRes, &cElement);
            if (pEffect->m_bShowIcon == 1 || pEffect->GetDurationType() == g_nDurationInnate) {
                delete pEffect;
            }
            else {
                ApplyEffect(pEffect, 1, 0);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004D1CF0
void CSWSObject::LoadObjectState(CResGFF* pRes, CResStruct* pStruct)
{
    LoadEffectList(pRes, pStruct);
    m_ScriptVars.LoadVarTable(pRes, pStruct);
    m_VarTable.LoadVarTable(pRes, pStruct);
    LoadActionQueue(pRes, pStruct);
    int bSuccess;
    m_bE8 = pRes->ReadFieldBYTE(pStruct, "Commandable", bSuccess, 1);
}
