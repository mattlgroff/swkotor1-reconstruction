// CSWCCreature members, worker C s5 (translation-unit-local layout).

#include "c/exo_base.h"

#include <stdlib.h>

struct Vector {
    float x, y, z;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    char m_resRef[16];
};

class CExoSound {
public:
    int PlayOneShotSoundAt(const CResRef& cResRef, Vector vPosition, float fVolume, unsigned long nPriority,
                           unsigned long a, unsigned long b, unsigned long c, float fPitch);   // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

struct CSWBaseItemData {
    char pad_0x0[0x8c];
    unsigned char m_nWeaponSoundRow;                // 0x8c (descriptive)
};

class CSWSItem {
public:
    int GetIsFinesseWeapon();                       // 0x00555690
};

class CSWCItem {
public:
    CSWBaseItemData* GetBaseItem();                 // 0x005B4790
    CSWSItem* GetServerItem();                      // 0x00646C90
};

class CClientExoApp {
public:
    CSWCItem* GetItemByGameObjectID(unsigned long oid);   // 0x005ED5A0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413270
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);                          // 0x00413350
};

class CTwoDimArrays {
public:
    char pad_0x0[0x60];
    C2DA* m_pWeaponSounds;                          // 0x60

    C2DA* GetWeaponSounds() { return m_pWeaponSounds; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8

    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nWeaponSoundPitchColumn;               // 0x007A21A4 (descriptive)

class CSWCCombatAttackData;

class CExoResMan {
public:
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize);   // 0x00408BC0
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CSWCCreatureStats {
public:
    CExoString GetFullName();                       // 0x00647CE0

    char pad_0x0[0x14];
    CExoString m_sFirstName;                        // 0x14
    CExoString m_sLastName;                         // 0x1c
};

class CSWCCreature {
public:
    void SetFirstAndLastName(const CExoString& sFirst, const CExoString& sLast);
    CExoString GetSaveCreatureFilename(const CExoString& sUnused1, const CExoString& sUnused2);
    int PlaySwingSound(const CExoString& sEvent, CSWCCombatAttackData* pData, int nCount);
    // Both callees are the shared empty body at 0x0060E760 (RET 4).
    void DisplayName(const CExoString& sName);      // 0x0060E760
    void NameChanged(unsigned long oid);            // 0x0060E760

    void* m_vtable;                                 // 0x00
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x24 - 0x8];
    Vector m_vPosition;                             // 0x24
    char pad_0x30[0x230 - 0x30];
    unsigned long m_oidRightWeapon;                 // 0x230
    unsigned long m_oidLeftWeapon;                  // 0x234
    char pad_0x238[0x2f8 - 0x238];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

// Descriptive name: stores the first/last name and builds "[full name]".
// FUNCTION: SWKOTOR 0x0060D600
void CSWCCreature::SetFirstAndLastName(const CExoString& sFirst, const CExoString& sLast)
{
    CExoString sName;
    CSWCCreatureStats* pStats = m_pStats;
    pStats->m_sFirstName = sFirst;
    pStats->m_sLastName = sLast;
    sName = "[";
    sName = sName + m_pStats->GetFullName() + "]";
    DisplayName(sName);
    unsigned long oid = m_idSelf;
    NameChanged(oid);
}

// Builds a unique creature resref from the stats name ("<first><last>",
// trimmed to 16 alphanumerics, numbered on collision). The two parameters
// are not read in this build.
// FUNCTION: SWKOTOR 0x0060C540
CExoString CSWCCreature::GetSaveCreatureFilename(const CExoString& sUnused1, const CExoString& sUnused2)
{
    CExoString sName;
    CExoString sFile;
    int nNumber = 1;
    CExoString sNumber;
    sName = m_pStats->m_sFirstName + m_pStats->m_sLastName;
    sName = sName.SubString(0, 16);
    sName.StripNonAlphaNumeric(1, 0, 0);
    if (!sName[0]) {
        sName = "0";
    }
    sFile = sName;
    for (;;) {
        CResRef cResRef(sFile.CStr());
        if (!g_pExoResMan->Exists(cResRef, 2015, 0)) {
            break;
        }
        sNumber = CExoString(nNumber);
        sName = sName.SubString(0, 16 - sNumber.GetLength());
        sFile = sName + sNumber;
        nNumber++;
    }
    return sFile;
}

// Header name PlaySwingSound (Mac: CSWCCreature::PlaySwingSound / Swing);
// the attack-data argument is not read.
// NONMATCH (32 diff insns): register assignment only (the original keeps
// `this` in edi and the items/index in ebp; ours swaps them). Separate item
// locals and a byte row do not change it.
// FUNCTION: SWKOTOR 0x0060DC50
int CSWCCreature::PlaySwingSound(const CExoString& sEvent, CSWCCombatAttackData* pData, int nCount)
{
    int nRow = 0;
    CSWCItem* pItem = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidRightWeapon);
    if (pItem && pItem->GetBaseItem()) {
        nRow = pItem->GetBaseItem()->m_nWeaponSoundRow;
    }
    pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidLeftWeapon);
    if (pItem && pItem->GetBaseItem()) {
        CSWSItem* pServerItem = pItem->GetServerItem();
        if (pServerItem && pServerItem->GetIsFinesseWeapon() == 1) {
            nRow = pItem->GetBaseItem()->m_nWeaponSoundRow;
        }
    }
    int nIndex = rand() % nCount;
    CExoString sSound;
    CExoString sColumn = nIndex;
    sColumn = sEvent + sColumn;
    g_pRules->Get2DArrays()->GetWeaponSounds()->GetCExoStringEntry(nRow, sColumn, &sSound);
    float fPitch = 0.0f;
    g_pRules->Get2DArrays()->GetWeaponSounds()->GetFLOATEntry(nRow, g_nWeaponSoundPitchColumn, &fPitch);
    return g_pExoSound->PlayOneShotSoundAt(sSound, m_vPosition, 1.5f, 15, 0, 0, 0, fPitch);
}
