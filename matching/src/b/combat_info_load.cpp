// CCombatInformation::LoadData: the CombatInfo struct of a saved creature.
// The two lists are read from the parent struct (as in the original).
#define B_COMBAT_INFO_NODE_INLINE
#include "b/server_info.h"
#include "b/res_gff.h"

// FUNCTION: SWKOTOR 0x00552350
int CCombatInformation::LoadData(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cInfo;
    CResStruct cElement;
    CResList cList;
    int bSuccess;
    int i;
    if (!pGFF->GetStructFromStruct(&cInfo, pStruct, "CombatInfo")) {
        return 0;
    }
    m_n0 = pGFF->ReadFieldBYTE(&cInfo, "NumAttacks", bSuccess, GetNumAttacks());
    m_n1 = pGFF->ReadFieldCHAR(&cInfo, "OnHandAttackMod", bSuccess, GetOnHandAttackMod());
    m_n2 = pGFF->ReadFieldCHAR(&cInfo, "OnHandDamageMod", bSuccess, GetOnHandDamageMod());
    m_n3 = pGFF->ReadFieldCHAR(&cInfo, "OffHandAttackMod", bSuccess, GetOffHandAttackMod());
    m_n4 = pGFF->ReadFieldCHAR(&cInfo, "OffHandDamageMod", bSuccess, GetOffHandDamageMod());
    m_n5 = pGFF->ReadFieldBYTE(&cInfo, "ForceResistance", bSuccess, GetForceResistance());
    // The original drops this value.
    pGFF->ReadFieldBYTE(&cInfo, "ArcaneSpellFail", bSuccess, GetArcaneSpellFail());
    m_n7 = pGFF->ReadFieldBYTE(&cInfo, "ArmorCheckPen", bSuccess, GetArmorCheckPen());
    m_n8 = pGFF->ReadFieldBYTE(&cInfo, "UnarmedDamDice", bSuccess, GetUnarmedDamDice());
    m_n9 = pGFF->ReadFieldBYTE(&cInfo, "UnarmedDamDie", bSuccess, GetUnarmedDamDie());
    m_nA = pGFF->ReadFieldBYTE(&cInfo, "OnHandCritRng", bSuccess, GetOnHandCritRng());
    m_nB = pGFF->ReadFieldBYTE(&cInfo, "OnHandCritMult", bSuccess, GetOnHandCritMult());
    m_nC = pGFF->ReadFieldBYTE(&cInfo, "OffHandWeaponEq", bSuccess, GetOffHandWeaponEq());
    m_n10 = pGFF->ReadFieldBYTE(&cInfo, "OffHandCritRng", bSuccess, GetOffHandCritRng());
    m_n11 = pGFF->ReadFieldBYTE(&cInfo, "OffHandCritMult", bSuccess, GetOffHandCritMult());
    m_oid30 = pGFF->ReadFieldDWORD(&cInfo, "LeftEquip", bSuccess, GetLeftEquip());
    m_oid2C = pGFF->ReadFieldDWORD(&cInfo, "RightEquip", bSuccess, GetRightEquip());
    m_s3C = pGFF->ReadFieldCExoString(&cInfo, "LeftString", bSuccess, "");
    m_s34 = pGFF->ReadFieldCExoString(&cInfo, "RightString", bSuccess, "");
    m_n44 = pGFF->ReadFieldBYTE(&cInfo, "DamageDice", bSuccess, 0);
    m_n45 = pGFF->ReadFieldBYTE(&cInfo, "DamageDie", bSuccess, 0);
    if (pGFF->GetList(&cList, pStruct, "AttackList") == 1) {
        for (i = 0; i < (int)pGFF->GetListCount(&cList); i++) {
            pGFF->GetListElement(&cElement, &cList, i);
            CCombatInformationNode* pNode = new CCombatInformationNode;
            pNode->m_n0 = pGFF->ReadFieldCHAR(&cElement, "Modifier", bSuccess, 0);
            pNode->m_n2 = pGFF->ReadFieldBYTE(&cElement, "WeaponWield", bSuccess, 0);
            pNode->m_n3 = pGFF->ReadFieldBYTE(&cElement, "VersusGoodEvil", bSuccess, 0);
            pNode->m_n4 = pGFF->ReadFieldBYTE(&cElement, "VersusRace", bSuccess, g_pRules->GetAA());
            m_lNodes14.Add(pNode);
        }
    }
    if (pGFF->GetList(&cList, pStruct, "DamageList") == 1) {
        for (i = 0; i < (int)pGFF->GetListCount(&cList); i++) {
            pGFF->GetListElement(&cElement, &cList, i);
            CCombatInformationNode* pNode = new CCombatInformationNode;
            pNode->m_n0 = pGFF->ReadFieldCHAR(&cElement, "Modifier", bSuccess, 0);
            pNode->m_n1 = pGFF->ReadFieldBYTE(&cElement, "ModifierType", bSuccess, 0);
            pNode->m_n2 = pGFF->ReadFieldBYTE(&cElement, "WeaponWield", bSuccess, 0);
            pNode->m_n3 = pGFF->ReadFieldBYTE(&cElement, "VersusGoodEvil", bSuccess, 0);
            pNode->m_n4 = pGFF->ReadFieldBYTE(&cElement, "VersusRace", bSuccess, g_pRules->GetAA());
            m_lNodes20.Add(pNode);
        }
    }
    return 1;
}
