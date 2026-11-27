// CSWSCreature::GetWeaponDischargeRow (pilot group 2).
// Needs exception handling: compile with --flags "-O2 -Gy -GX".

#include "shared/exo_string.h"

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);                  // 0x00413270
    int GetCExoStringEntry(const CExoString& sRow, const CExoString& sColumn, CExoString* pValue);    // 0x00413DE0
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);                  // 0x00414110
};

class C2DArrays {
public:
    char pad_0x00[0xc];
    C2DA* m_pRaceTable;                 // 0x0c
    char pad_0x10[0xd4];
    C2DA* m_pWeaponDischargeTable;      // 0xe4
    C2DA* m_pDroidDischargeTable;       // 0xe8

    C2DA* GetRaceTable() { return m_pRaceTable; }
    C2DA* GetWeaponDischargeTable() { return m_pWeaponDischargeTable; }
    C2DA* GetDroidDischargeTable() { return m_pDroidDischargeTable; }
};

class CSWRules {
public:
    char pad_0x00[0xb8];
    C2DArrays* m_p2DArrays;             // 0xb8

    C2DArrays* Get2DArrays() { return m_p2DArrays; }
};

#include "shared/globals.h"

class CSWSCreature {
public:
    char pad_0x00[0xa60];
    unsigned short m_nRace;             // 0xa60

    CExoString GetWeaponDischargeRow(int nWeapon);
};

// FUNCTION: SWKOTOR 0x005B4FB0
CExoString CSWSCreature::GetWeaponDischargeRow(int nWeapon)
{
    CExoString sRow(nWeapon);
    int bDroid = 0;
    g_pRules->Get2DArrays()->GetWeaponDischargeTable()->GetINTEntry(sRow, "Droid", &bDroid);
    if (bDroid) {
        CExoString sWeapon(nWeapon);
        CExoString sRace;
        CExoString sPrefix;
        g_pRules->Get2DArrays()->GetRaceTable()->GetCExoStringEntry(m_nRace, "RACE", &sRace);
        g_pRules->Get2DArrays()->GetDroidDischargeTable()->GetCExoStringEntry(sRace, sWeapon, &sPrefix);
        sRow = sPrefix + "_" + sRow;
    }
    else if (m_nRace == 3) {
        sRow = CExoString("3_") + CExoString(nWeapon);
    }
    return sRow;
}
