// CCombatInformation::SaveData: the CombatInfo struct of a saved creature.
#include "b/server_info.h"
#include "b/res_gff.h"

// FUNCTION: SWKOTOR 0x00550F30
int CCombatInformation::SaveData(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cInfo;
    CResList cList;
    CResStruct cElement;
    int i;
    pGFF->AddStructToStruct(&cInfo, pStruct, "CombatInfo", 0xcaaa);
    pGFF->WriteFieldBYTE(&cInfo, m_n0, "NumAttacks");
    pGFF->WriteFieldCHAR(&cInfo, m_n1, "OnHandAttackMod");
    pGFF->WriteFieldCHAR(&cInfo, m_n2, "OnHandDamageMod");
    pGFF->WriteFieldCHAR(&cInfo, m_n3, "OffHandAttackMod");
    pGFF->WriteFieldCHAR(&cInfo, m_n4, "OffHandDamageMod");
    pGFF->WriteFieldBYTE(&cInfo, m_n5, "ForceResistance");
    pGFF->WriteFieldBYTE(&cInfo, m_n6, "ArcaneSpellFail");
    pGFF->WriteFieldBYTE(&cInfo, m_n7, "ArmorCheckPen");
    pGFF->WriteFieldBYTE(&cInfo, m_n8, "UnarmedDamDice");
    pGFF->WriteFieldBYTE(&cInfo, m_n9, "UnarmedDamDie");
    pGFF->WriteFieldBYTE(&cInfo, m_nA, "OnHandCritRng");
    pGFF->WriteFieldBYTE(&cInfo, m_nB, "OnHandCritMult");
    pGFF->WriteFieldBYTE(&cInfo, m_nC, "OffHandWeaponEq");
    pGFF->WriteFieldBYTE(&cInfo, m_n10, "OffHandCritRng");
    pGFF->WriteFieldBYTE(&cInfo, m_n11, "OffHandCritMult");
    pGFF->WriteFieldDWORD(&cInfo, m_oid30, "LeftEquip");
    pGFF->WriteFieldDWORD(&cInfo, m_oid2C, "RightEquip");
    pGFF->WriteFieldCExoString(&cInfo, m_s3C, "LeftString");
    pGFF->WriteFieldCExoString(&cInfo, m_s34, "RightString");
    pGFF->WriteFieldBYTE(&cInfo, m_n44, "DamageDice");
    pGFF->WriteFieldBYTE(&cInfo, m_n45, "DamageDie");
    pGFF->AddList(&cList, &cInfo, "AttackList");
    for (i = 0; i < m_lNodes14.num; i++) {
        pGFF->AddListElement(&cElement, &cList, 0xaaee);
        pGFF->WriteFieldCHAR(&cElement, m_lNodes14[i]->m_n0, "Modifier");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes14[i]->m_n2, "WeaponWield");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes14[i]->m_n3, "VersusGoodEvil");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes14[i]->m_n4, "VersusRace");
    }
    pGFF->AddList(&cList, &cInfo, "DamageList");
    for (i = 0; i < m_lNodes20.num; i++) {
        pGFF->AddListElement(&cElement, &cList, 0xddee);
        pGFF->WriteFieldCHAR(&cElement, m_lNodes20[i]->m_n0, "Modifier");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes20[i]->m_n1, "ModifierType");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes20[i]->m_n2, "WeaponWield");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes20[i]->m_n3, "VersusGoodEvil");
        pGFF->WriteFieldBYTE(&cElement, m_lNodes20[i]->m_n4, "VersusRace");
    }
    return 1;
}
