// CSWCCreature::ForceMenuAction (nwccreature.o), rewritten by worker C s14
// with a translation-unit-local layout.

class CExoString {
public:
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

struct Vector {
    float x, y, z;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
};

class CTwoDimArrays {
public:
    char pad_0x0[0xf4];
    C2DA* m_pSpells;                                // 0xf4 (spells.2da)
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCMessage {
public:
    int SendPlayerToServerInput_UseItem(unsigned long nItem, unsigned char a, unsigned char b,
                                        unsigned long oidTarget, const Vector& vTarget);   // 0x00677BD0
    int SendPlayerToServerInput_CastSpell(unsigned long nSpell, unsigned char nClass, unsigned char a,
                                          unsigned char b, int c, const Vector& vTarget,
                                          unsigned long oidTarget);   // 0x006776A0
};

class CClientExoApp {
public:
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
    int ShowTutorialWindow(unsigned char nWindow, unsigned long oidCreature, unsigned long nClass,
                           unsigned long nSpell);   // 0x005EDF40
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

extern const unsigned short RACIAL_TYPE_DROID;      // 0x0074BF10

class CSWCCreatureStats {
public:
    unsigned char GetClass(unsigned char nIndex);   // 0x00647730

    char pad_0x0[0x24];
    unsigned short m_nRace;                         // 0x24
};

class CSWCCreature {
public:
    void ForceMenuAction(unsigned long nAction, CSWCCreature* pCreature);

    void* m_vtable;                                 // 0x00
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x24 - 0x8];
    Vector m_vPosition;                             // 0x24
    char pad_0x30[0x2f8 - 0x30];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

// Radial-menu force power callback: casts the spell with the creature's
// first force class, or (droids) uses it as an item ability.
// NONMATCH (66 insns): the original keeps `this` in ebp and re-reads the
// class byte's slot for each GetClass push; ours caches that slot in ebp
// and spills `this`, so the two ShowTutorialWindow pushes are not hoisted.
// FUNCTION: SWKOTOR 0x0060F330
void CSWCCreature::ForceMenuAction(unsigned long nAction, CSWCCreature* pCreature)
{
    unsigned long nSpell = nAction & 0xdfffffff;
    CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
    if (!pMessage) {
        return;
    }
    CSWCCreatureStats* pStats = pCreature->m_pStats;
    unsigned char nClass;
    for (nClass = 0; nClass < 2; nClass++) {
        if (pStats->GetClass(nClass) == 3 || pStats->GetClass(nClass) == 4 || pStats->GetClass(nClass) == 5) {
            break;
        }
    }
    if (nClass >= 2) {
        if (pStats->m_nRace == RACIAL_TYPE_DROID) {
            pMessage->SendPlayerToServerInput_UseItem(nSpell, 0, 0, m_idSelf, m_vPosition);
        }
        return;
    }
    int nHostile = 0;
    int bFound = g_pRules->m_p2DArrays->m_pSpells->GetINTEntry(nSpell, CExoString("FORCEHOSTILE"), &nHostile);
    int bShown;
    if (bFound) {
        bShown = g_pAppManager->m_pClientExoApp->ShowTutorialWindow(4, m_idSelf, nClass, nSpell);
    } else {
        bShown = g_pAppManager->m_pClientExoApp->ShowTutorialWindow(3, m_idSelf, nClass, nSpell);
    }
    if (!bShown) {
        Vector vTarget;
        vTarget.x = 0.0f;
        vTarget.y = 0.0f;
        vTarget.z = 0.0f;
        pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
        if (pMessage) {
            pMessage->SendPlayerToServerInput_CastSpell(nSpell, nClass, 0, 0, 0, vTarget, m_idSelf);
        }
    }
}
