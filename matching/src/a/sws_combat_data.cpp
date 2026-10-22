// CSWSCombatRoundAction clear/save/load and the CSWSCombatAttackData
// damage helpers and SaveData (0x004D1D50-0x004D2447, 0x004D2EC0;
// behavioural reference: src/unknown/sub_004d2070.cpp,
// src/batches/b004d4_combat_attack_data.cpp).
#include "a/sws_combat_round.h"

extern int g_nDamageIndex;                      // 0x008327E0
extern const float g_fInvLog10Of2;              // 0x00745FF8

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

extern "C" double __cdecl log10(double x);
#pragma intrinsic(log10)

// FUNCTION: SWKOTOR 0x004D1D50
void CSWSCombatRoundAction::Clear() throw()
{
    m_nActionTimer = 0;
    m_nAnimationTime = 0;
    m_nAnimation = 0;
    m_nActionType = 0;
    m_nNumAttacks = 0;
    m_oidTarget = OBJECT_INVALID;
    m_bRetargettable = 1;
    m_nInventorySlot = 0;
    m_oidItem = OBJECT_INVALID;
    m_nSpellId = 0;
    m_nSpellClass = 0;
    m_nSpellParam2c = 0;
    m_nSpellParam30 = 0;
    m_nSpellParam34 = 0;
    m_oidSpellTarget = OBJECT_INVALID;
    m_nSpellParam48 = 0;
    m_nSpellParam4c = 0;
    m_nSpellParam50 = 0;
    m_nParam54 = 0;
    m_nParam58 = 0;
    m_nParam5c = 0;
    m_nSpellParam60 = 0;
    m_oidItemSpellTarget = OBJECT_INVALID;
    m_nItemSpellParam68 = 0;
    m_nItemSpellParam6c = 0;
    m_nParam70 = 0;
    m_nParam72 = 0;
    m_bCutscene = 0;
    m_nCutsceneParam78 = 0;
    m_nCutsceneParam7c = 4;
    m_nCutsceneParam80 = 0;
    m_nParam84 = 0;
}

// FUNCTION: SWKOTOR 0x004D1DD0
int CSWSCombatRoundAction::SaveData(CResGFF* pRes, CResStruct* pStruct)
{
    pRes->WriteFieldINT(pStruct, m_nActionTimer, "ActionTimer");
    pRes->WriteFieldWORD(pStruct, m_nAnimation, "Animation");
    pRes->WriteFieldINT(pStruct, m_nAnimationTime, "AnimationTime");
    pRes->WriteFieldINT(pStruct, m_nNumAttacks, "NumAttacks");
    pRes->WriteFieldBYTE(pStruct, m_nActionType, "ActionType");
    pRes->WriteFieldDWORD(pStruct, m_oidTarget, "TargetId");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bRetargettable, "Retargettable");
    pRes->WriteFieldDWORD(pStruct, m_nInventorySlot, "InventorySlot");
    pRes->WriteFieldDWORD(pStruct, m_oidItem, "ItemId");
    return 1;
}

// FUNCTION: SWKOTOR 0x004D1E90
int CSWSCombatRoundAction::LoadData(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    m_nActionTimer = pRes->ReadFieldINT(pStruct, "ActionTimer", bSuccess, 0);
    m_nAnimation = pRes->ReadFieldWORD(pStruct, "Animation", bSuccess, 0);
    m_nAnimationTime = pRes->ReadFieldINT(pStruct, "AnimationTime", bSuccess, 0);
    m_nNumAttacks = pRes->ReadFieldINT(pStruct, "NumAttacks", bSuccess, 0);
    m_nActionType = pRes->ReadFieldBYTE(pStruct, "ActionType", bSuccess, 0);
    m_oidTarget = pRes->ReadFieldDWORD(pStruct, "TargetId", bSuccess, 0);
    m_bRetargettable = pRes->ReadFieldBYTE(pStruct, "Retargettable", bSuccess, 0);
    m_nInventorySlot = pRes->ReadFieldDWORD(pStruct, "InventorySlot", bSuccess, 0);
    m_oidItem = pRes->ReadFieldDWORD(pStruct, "ItemId", bSuccess, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004D1F80
int CSWSCombatAttackData::GetDamage(int nFlags)
{
    if (nFlags & 0x10) return m_nDamage[4];
    if (nFlags & 0x20) return m_nDamage[5];
    if (nFlags & 0x40) return m_nDamage[6];
    if (nFlags & 0x80) return m_nDamage[7];
    if (nFlags & 0x100) return m_nDamage[8];
    if (nFlags & 0x8) return m_nDamage[3];
    if (nFlags & 0x200) return m_nDamage[9];
    if (nFlags & 0x400) return m_nDamage[10];
    if (nFlags & 0x800) return m_nDamage[11];
    if (nFlags & 0x1000) return m_nDamage[12];
    if (nFlags & 0x1) return m_nDamage[0];
    if (nFlags & 0x2) return m_nDamage[1];
    if (nFlags & 0x4) return m_nDamage[2];
    if (nFlags & 0x2000) return m_nDamage[13];
    return 0;
}

// FUNCTION: SWKOTOR 0x004D2030
void CSWSCombatAttackData::AddBaseDamage(int nDamage)
{
    if (m_nDamage[14] > 0) {
        m_nDamage[14] += nDamage;
    }
    else {
        m_nDamage[14] = nDamage;
    }
}

// FUNCTION: SWKOTOR 0x004D2060
int CSWSCombatAttackData::GetBaseDamage()
{
    return m_nDamage[14];
}

// FUNCTION: SWKOTOR 0x004D2070
void CSWSCombatAttackData::SetDamage(unsigned short nFlags, int nDamage)
{
    if (nDamage < 0) {
        nDamage = 0;
    }
    FloatToInt(&g_nDamageIndex, log10((double)nFlags) * g_fInvLog10Of2);
    m_nDamage[g_nDamageIndex] = nDamage;
}

// FUNCTION: SWKOTOR 0x004D20D0
void CSWSCombatAttackData::AddDamage(unsigned short nFlags, int nDamage, int bAddToBase)
{
    FloatToInt(&g_nDamageIndex, log10((double)nFlags) * g_fInvLog10Of2);
    int nIndex = g_nDamageIndex;
    if (m_nDamage[nIndex] > 0) {
        int nTotal = m_nDamage[nIndex] + nDamage;
        if (nTotal < 1) {
            nTotal = 1;
        }
        m_nDamage[nIndex] = nTotal;
    }
    else {
        SetDamage(nFlags, nDamage);
    }
    if (bAddToBase) {
        AddBaseDamage(nDamage);
    }
}

// FUNCTION: SWKOTOR 0x004D2180
int CSWSCombatAttackData::GetTotalDamage(int nUnused)
{
    int nTotal = 0;
    for (int i = 0; i < 14; i++) {
        if (m_nDamage[i] > -1) {
            nTotal += m_nDamage[i];
        }
    }
    return nTotal;
}

// FUNCTION: SWKOTOR 0x004D2210
int CSWSCombatAttackData::SaveData(CResGFF* pRes, CResStruct* pStruct)
{
    pRes->WriteFieldBYTE(pStruct, m_nAttackGroup, "AttackGroup");
    pRes->WriteFieldWORD(pStruct, m_nTime, "AnimationLength");
    pRes->WriteFieldDWORD(pStruct, m_nField18, "MissedBy");
    pRes->WriteFieldBYTE(pStruct, m_nAttackGroup5c, "AttackGroup5c");
    pRes->WriteFieldDWORD(pStruct, m_oidReactObject, "ReactObject");
    pRes->WriteFieldWORD(pStruct, m_nWeaponAttackType, "WeaponAttackType");
    pRes->WriteFieldWORD(pStruct, m_nField12, "Field12");
    pRes->WriteFieldWORD(pStruct, m_nField14, "Field14");
    pRes->WriteFieldBYTE(pStruct, m_nField3c, "Field3c");
    pRes->WriteFieldWORD(pStruct, m_nSpecialAttack, "SpecialAttack");
    pRes->WriteFieldBYTE(pStruct, m_nField3b, "Field3b");
    pRes->WriteFieldINT(pStruct, m_nField40, "Field40");
    pRes->WriteFieldINT(pStruct, m_nField44, "Field44");
    pRes->WriteFieldBYTE(pStruct, m_nAttackResult, "AttackResult");
    pRes->WriteFieldFLOAT(pStruct, m_vRangedTarget.x, "RangedTargetX");
    pRes->WriteFieldFLOAT(pStruct, m_vRangedTarget.y, "RangedTargetY");
    pRes->WriteFieldFLOAT(pStruct, m_vRangedTarget.z, "RangedTargetZ");
    CResList cList;
    pRes->AddList(&cList, pStruct, "DamageList");
    for (int i = 0; i < 15; i++) {
        CResStruct cElement;
        pRes->AddListElement(&cElement, &cList, 0xdaee);
        pRes->WriteFieldSHORT(&cElement, m_nDamage[i], "DamageValue");
    }
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_nField48, "Field48");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_nField4c, "Field4c");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_nField50, "Field50");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_nField58, "Field58");
    pRes->WriteFieldDWORD(pStruct, m_oidAmmunition, "AmmoItem");
    pRes->WriteFieldCExoString(pStruct, m_sAttackDebugText, "AttackDebugText");
    pRes->WriteFieldCExoString(pStruct, m_sDamageDebugText, "DamageDebugText");
    return 1;
}

static void (CSWSAttackResultB::*s_pfnClearResultB)() = &CSWSAttackResultB::Clear;

// FUNCTION: SWKOTOR 0x004D2EC0 SYMBOL: ?Clear@CSWSAttackResultB@@QAEXXZ
