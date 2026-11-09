// CServerInfo (server options) and CCombatInformationNode (only what worker B touches).
#ifndef B_SERVER_INFO_H
#define B_SERVER_INFO_H

#include "b/exo_string.h"
#include "b/exo_array_list.h"
#ifdef B_COMBAT_INFO_NODE_INLINE
#include "b/sw_rules.h"
#endif

struct CServerOption {
    char* m_szName;                                 // 0x0
    char* m_szValue;                                // 0x4
    char pad_0x8[0x8];
};

class CServerInfo {
public:
    int FindOptionIndex(CExoString& sName, CExoString& sValue);   // 0x0054E9D0
    void SetDifficulty(int nLevel);                 // 0x0054EA70 (name guessed)
    void SetModuleName(CExoString& sName);          // 0x0054EAA0

    char pad_0x0[0xfc];
    CExoString m_sModuleName;                       // 0xfc
    unsigned long m_bSpecialModule : 1;             // 0x104 bit 0 (name guessed)
    int m_nDifficulty;                              // 0x108 (name guessed)
    CServerOption m_aOptions[50];                   // 0x10c
};

class CCombatInformationNode {
public:
#ifdef B_COMBAT_INFO_NODE_INLINE
    // Inlined into CCombatInformation::operator= (out of line at 0x0054F260).
    CCombatInformationNode()
    {
        m_n0 = 0;
        m_n1 = 0;
        m_n2 = 0;
        m_n3 = 0;
        m_n4 = g_pRules->m_nAA;
    }
    CCombatInformationNode& operator=(const CCombatInformationNode& other)
    {
        if (this != &other) {
            m_n0 = other.m_n0;
            m_n1 = other.m_n1;
            m_n2 = other.m_n2;
            m_n3 = other.m_n3;
            m_n4 = other.m_n4;
        }
        return *this;
    }
#else
    CCombatInformationNode();                       // 0x0054F260
#endif
    int operator!=(CCombatInformationNode& other);  // 0x0054F280

    unsigned char m_n0;
    unsigned char m_n1;
    unsigned char m_n2;
    unsigned char m_n3;
    unsigned char m_n4;
};

class CCombatInformation {
public:
    CCombatInformation();                           // 0x00552280
    ~CCombatInformation();                          // 0x00550E00
    int operator!=(CCombatInformation& other);      // 0x005512D0
    CCombatInformation& operator=(CCombatInformation& other); // 0x00552A00
    int SaveData(class CResGFF* pGFF, struct CResStruct* pStruct); // 0x00550F30
    int LoadData(class CResGFF* pGFF, struct CResStruct* pStruct); // 0x00552350
    // Byte getters named after the saved field labels (LoadData passes the
    // current values through them as defaults).
    unsigned char GetNumAttacks() { return m_n0; }
    unsigned char GetOnHandAttackMod() { return m_n1; }
    unsigned char GetOnHandDamageMod() { return m_n2; }
    unsigned char GetOffHandAttackMod() { return m_n3; }
    unsigned char GetOffHandDamageMod() { return m_n4; }
    unsigned char GetForceResistance() { return m_n5; }
    unsigned char GetArcaneSpellFail() { return m_n6; }
    unsigned char GetArmorCheckPen() { return m_n7; }
    unsigned char GetUnarmedDamDice() { return m_n8; }
    unsigned char GetUnarmedDamDie() { return m_n9; }
    unsigned char GetOnHandCritRng() { return m_nA; }
    unsigned char GetOnHandCritMult() { return m_nB; }
    unsigned char GetOffHandCritRng() { return m_n10; }
    unsigned char GetOffHandCritMult() { return m_n11; }
    int GetOffHandWeaponEq() { return m_nC; }
    unsigned long GetLeftEquip() { return m_oid30; }
    unsigned long GetRightEquip() { return m_oid2C; }

    unsigned char m_n0;                             // 0x00
    unsigned char m_n1;
    unsigned char m_n2;
    unsigned char m_n3;
    unsigned char m_n4;
    unsigned char m_n5;
    unsigned char m_n6;
    unsigned char m_n7;
    unsigned char m_n8;
    unsigned char m_n9;
    unsigned char m_nA;
    unsigned char m_nB;
    int m_nC;                                       // 0x0c
    unsigned char m_n10;                            // 0x10
    unsigned char m_n11;                            // 0x11
    CExoArrayList<CCombatInformationNode*> m_lNodes14;  // 0x14
    CExoArrayList<CCombatInformationNode*> m_lNodes20;  // 0x20
    unsigned long m_oid2C;                          // 0x2c
    unsigned long m_oid30;                          // 0x30
    CExoString m_s34;                               // 0x34
    CExoString m_s3C;                               // 0x3c
    unsigned char m_n44;                            // 0x44
    unsigned char m_n45;                            // 0x45
};

#endif
