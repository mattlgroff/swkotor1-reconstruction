// Worker A round s7: CSWSCombatAttackData::LoadData (nwscombatround.cpp).
#include "a/sws_combat_round.h"

// FUNCTION: SWKOTOR 0x004D2450
int CSWSCombatAttackData::LoadData(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    m_nAttackGroup = pRes->ReadFieldBYTE(pStruct, "AttackGroup", bSuccess, 0);
    m_nTime = pRes->ReadFieldWORD(pStruct, "AnimationLength", bSuccess, 0);
    m_nField18 = pRes->ReadFieldDWORD(pStruct, "MissedBy", bSuccess, 0);
    m_nAttackGroup5c = pRes->ReadFieldBYTE(pStruct, "AttackResult", bSuccess, 0);
    m_oidReactObject = pRes->ReadFieldDWORD(pStruct, "ReactObject", bSuccess, 0);
    m_nWeaponAttackType = pRes->ReadFieldWORD(pStruct, "ReaxnDelay", bSuccess, 0);
    m_nField12 = pRes->ReadFieldWORD(pStruct, "ReaxnAnimation", bSuccess, 0);
    m_nField14 = pRes->ReadFieldWORD(pStruct, "ReaxnAnimLength", bSuccess, 0);
    m_nField3c = pRes->ReadFieldBYTE(pStruct, "Concealment", bSuccess, 0);
    m_nSpecialAttack = pRes->ReadFieldWORD(pStruct, "AttackType", bSuccess, 0);
    m_nField3b = pRes->ReadFieldBYTE(pStruct, "AttackMode", bSuccess, 0);
    m_nField40 = pRes->ReadFieldINT(pStruct, "RangedAttack", bSuccess, 0);
    m_nField44 = pRes->ReadFieldINT(pStruct, "SneakAttack", bSuccess, 0);
    m_nAttackResult = pRes->ReadFieldBYTE(pStruct, "WeaponAttackType", bSuccess, 0);
    m_vRangedTarget.x = pRes->ReadFieldFLOAT(pStruct, "RangedTargetX", bSuccess, 0.0f);
    m_vRangedTarget.y = pRes->ReadFieldFLOAT(pStruct, "RangedTargetY", bSuccess, 0.0f);
    m_vRangedTarget.z = pRes->ReadFieldFLOAT(pStruct, "RangedTargetZ", bSuccess, 0.0f);
    CResList cList;
    CResStruct cElement;
    if (pRes->GetList(&cList, pStruct, "DamageList") == 1) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            if (pRes->GetListElement(&cElement, &cList, i) == 1) {
                m_nDamage[i] = pRes->ReadFieldSHORT(&cElement, "DamageValue", bSuccess, 0);
            }
        }
    }
    m_nField48 = pRes->ReadFieldBYTE(pStruct, "KillingBlow", bSuccess, 0);
    m_nField4c = pRes->ReadFieldBYTE(pStruct, "CoupDeGrace", bSuccess, 0);
    m_nField50 = pRes->ReadFieldBYTE(pStruct, "CriticalThreat", bSuccess, 0);
    m_nField58 = pRes->ReadFieldBYTE(pStruct, "AttackDeflected", bSuccess, 0);
    m_oidAmmunition = pRes->ReadFieldDWORD(pStruct, "AmmoItem", bSuccess, 0);
    {
        CExoString sDefault("");
        m_sAttackDebugText = pRes->ReadFieldCExoString(pStruct, "AttackDebugText", bSuccess, sDefault);
    }
    {
        CExoString sDefault("");
        m_sDamageDebugText = pRes->ReadFieldCExoString(pStruct, "DamageDebugText", bSuccess, sDefault);
    }
    return 1;
}
