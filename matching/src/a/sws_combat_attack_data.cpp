// CSWSCombatAttackData, CSWSCombatSubAttack and their
// CExoArrayList instantiations (0x004D27E0-0x004D5CB0; behavioural reference:
// src/batches/b004d4_combat_attack_data.cpp, b004d42_combat_round_lifecycle.cpp).
#include "a/sws_combat_round.h"

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}

template <class T>
void CExoArrayList<T>::DelIndex(int nIndex)
{
    num--;
    for (; nIndex < num; nIndex++) {
        element[nIndex] = element[nIndex + 1];
    }
}

template void CExoArrayList<unsigned short>::Allocate(int n);
template void CExoArrayList<unsigned short>::Add(unsigned short t);
template void CExoArrayList<CSWSCombatSubAttack>::Allocate(int n);
template void CExoArrayList<CSWSCombatSubAttack>::Add(CSWSCombatSubAttack t);
template void CExoArrayList<CSWSCombatSubAttack>::DelIndex(int nIndex);
template CExoArrayList<CSWSCombatSubAttack>::~CExoArrayList();

// FUNCTION: SWKOTOR 0x004D2FA0 SYMBOL: ?Allocate@?$CExoArrayList@G@@QAEXH@Z
// FUNCTION: SWKOTOR 0x004D4330 SYMBOL: ?Add@?$CExoArrayList@G@@QAEXG@Z
// FUNCTION: SWKOTOR 0x004D43B0 SYMBOL: ?Allocate@?$CExoArrayList@VCSWSCombatSubAttack@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x004D57A0 SYMBOL: ?Add@?$CExoArrayList@VCSWSCombatSubAttack@@@@QAEXVCSWSCombatSubAttack@@@Z
// FUNCTION: SWKOTOR 0x004D2EE0 SYMBOL: ?DelIndex@?$CExoArrayList@VCSWSCombatSubAttack@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x004D4370 SYMBOL: ??1?$CExoArrayList@VCSWSCombatSubAttack@@@@QAE@XZ

// FUNCTION: SWKOTOR 0x004D27E0
void CSWSCombatAttackData::ResolveSubAttackToAttack(const CSWSCombatSubAttack& sub)
{
    m_nAttackGroup5c = sub.m_nAttackGroup;
    m_nSpecialAttack = sub.m_nSpecialAttack;
    m_nAttackResult = sub.m_nAttackResult;
    m_vRangedTarget = sub.m_vRangedTarget;
    m_nWeaponAttackType = (unsigned short)sub.m_nWeaponAttackType;
    m_nAnimationLength = sub.m_nAnimationLength;
    for (int i = 0; i < 15; i++) {
        m_nDamage[i] = sub.m_nDamage[i];
    }
    m_resultA = sub.m_resultA;
    m_resultB = sub.m_resultB;
    m_resultC = sub.m_resultC;
    m_resultD = sub.m_resultD;
    m_resultE = sub.m_resultE;
}

// FUNCTION: SWKOTOR 0x004D3010
void CSWSCombatAttackData::ClearAttackData()
{
    m_nAttackGroup = 0xff;
    m_nAnimationLength = 0;
    m_nTime = 0;
    m_nField18 = 0;
    for (int j = 0; j < 15; j++) {
        m_nDamage[j] = -1;
    }
    m_nAttackGroup5c = 0;
    m_nField60 = 0;
    m_oidReactObject = OBJECT_INVALID;
    m_nWeaponAttackType = 0;
    m_nField12 = 0x2711;
    m_nField14 = 0;
    m_nField3c = 0;
    m_nSpecialAttack = 0;
    m_nField3b = 0;
    m_nField66 = 0xff;
    m_vRangedTarget = Vector();
    m_nAttackResult = 0;
    m_nField44 = 0;
    m_nField40 = 0;
    m_nField48 = 0;
    m_nField4c = 0;
    m_nField50 = 0;
    m_nField54 = 0;
    m_nField58 = 0;
    m_oidAmmunition = OBJECT_INVALID;
    m_sAttackDebugText = "";
    m_sDamageDebugText = "";
    for (int i = 0; i < m_alstOnHitEffects.num; i++) {
        delete m_alstOnHitEffects.element[i];
    }
    m_alstOnHitEffects.num = 0;
    m_alstList94.num = 0;
    m_alstListA0.num = 0;
    m_alstSubAttacks.num = 0;
    m_nFieldB8 = 0;
    m_nFieldBC = 0;
    m_resultA.Clear();
    m_resultB.Clear();
    m_resultC.Clear();
    m_resultD.Clear();
    m_resultE.Clear();
    m_resultC2.Clear();
}

// FUNCTION: SWKOTOR 0x004D32D0
void CSWSCombatAttackData::RemoveTopSubAttack()
{
    if (m_alstSubAttacks.num > 0) {
        m_alstSubAttacks.DelIndex(0);
    }
}

// FUNCTION: SWKOTOR 0x004D32F0
CSWSCombatSubAttack::CSWSCombatSubAttack()
{
    m_resultA.Clear();
    m_resultB.Clear();
    m_resultC.Clear();
    m_resultD.Clear();
    m_resultE.m_nBytes[0] = 0; m_resultE.m_nBytes[1] = 0;
    m_resultE.m_nBytes[2] = 0; m_resultE.m_nBytes[3] = 0;
    m_resultE.m_nBytes[4] = 0; m_resultE.m_nBytes[5] = 0;
    m_resultE.m_nBytes[6] = 0; m_resultE.m_nBytes[7] = 0;
    m_resultE.m_nField10 = 0;
    m_resultE.m_nField14 = 0;
    m_resultE.m_oidTarget = OBJECT_INVALID;
    m_resultE.m_nByte0c = 0xff;
}

// FUNCTION: SWKOTOR 0x004D4480
CSWSCombatAttackData::~CSWSCombatAttackData()
{
}

// FUNCTION: SWKOTOR 0x004D4560
CSWSCombatSubAttack CSWSCombatAttackData::GetTopSubAttack()
{
    CSWSCombatSubAttack sub;
    if (m_alstSubAttacks.num > 0) {
        sub = m_alstSubAttacks.element[0];
    }
    return sub;
}

// FUNCTION: SWKOTOR 0x004D45B0
void CSWSCombatAttackData::ResolveTopSubAttackToAttack()
{
    ResolveSubAttackToAttack(GetTopSubAttack());
}

// FUNCTION: SWKOTOR 0x004D45E0
int CSWSCombatAttackData::IsSubAttackReady(unsigned long nTime)
{
    int bReady = 0;
    CSWSCombatSubAttack sub = GetTopSubAttack();
    if (m_nTime - nTime > sub.m_nTime) {
        bReady = 1;
    }
    return bReady;
}

// FUNCTION: SWKOTOR 0x004D57F0
CSWSCombatAttackData::CSWSCombatAttackData()
{
    m_resultA.Clear();
    m_resultB.Clear();
    m_resultC.Clear();
    m_resultD.Clear();
    m_resultE.Clear();
    m_resultC2.Clear();
    ClearAttackData();
}

// FUNCTION: SWKOTOR 0x004D5A50
void CSWSCombatAttackData::ResolveAttackToMeleeSubAttack(int nAttackType, unsigned long nTime)
{
    CSWSCombatSubAttack sub;
    sub.m_nAttackType = nAttackType;
    sub.m_nTime = nTime;
    sub.m_nAttackGroup = m_nAttackGroup5c;
    sub.m_nAttackResult = m_nAttackResult;
    sub.m_nSpecialAttack = m_nSpecialAttack;
    sub.m_bRanged = 0;
    for (int i = 0; i < 15; i++) {
        sub.m_nDamage[i] = m_nDamage[i];
        m_nDamage[i] = -1;
    }
    sub.m_resultA = m_resultA;
    sub.m_resultB = m_resultB;
    sub.m_resultC = m_resultC;
    sub.m_resultD = m_resultD;
    sub.m_resultE = m_resultE;
    m_alstSubAttacks.Add(sub);
}

// FUNCTION: SWKOTOR 0x004D5B60
void CSWSCombatAttackData::ResolveAttackToRangedSubAttack(int nAttackType, unsigned long nTime,
                                                          int nAnimationLength, int nParam14,
                                                          int nParam2c)
{
    CSWSCombatSubAttack sub;
    sub.m_nAttackType = nAttackType;
    sub.m_nTime = nTime;
    sub.m_nAnimationLength = nAnimationLength;
    sub.m_nParam14 = nParam14;
    sub.m_nParam2c = nParam2c;
    sub.m_nAttackGroup = m_nAttackGroup5c;
    sub.m_nAttackResult = m_nAttackResult;
    sub.m_nSpecialAttack = m_nSpecialAttack;
    sub.m_vRangedTarget = m_vRangedTarget;
    sub.m_nWeaponAttackType = m_nWeaponAttackType;
    sub.m_bRanged = 1;
    for (int i = 0; i < 15; i++) {
        sub.m_nDamage[i] = m_nDamage[i];
        m_nDamage[i] = -1;
    }
    sub.m_resultA = m_resultA;
    sub.m_resultB = m_resultB;
    sub.m_resultC = m_resultC;
    sub.m_resultD = m_resultD;
    sub.m_resultE = m_resultE;
    m_alstSubAttacks.Add(sub);
}
