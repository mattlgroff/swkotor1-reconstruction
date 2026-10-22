// Pilot 4 arm B: 0x00647660, client creature stats race name.
#include "pilot4/b/common.h"

class CSWRace {
public:
    CExoString GetNameText();                   // 0x005C78F0
    char pad_0x0[0x34];
};

class CSWRules {
public:
    char pad_0x0[0xb0];
    CSWRace *m_lstRaces;                        // 0xb0
};

extern CSWRules *g_pRules;                      // 0x007A3A28

class CSWCCreatureStats {
public:
    CExoString GetRaceName();
    char pad_0x0[0x24];
    unsigned short m_nRace;                     // 0x24
    char pad_0x26[0x2];
    CExoString m_sSubRace;                      // 0x28
};

// FUNCTION: SWKOTOR 0x00647660
CExoString CSWCCreatureStats::GetRaceName()
{
    if (m_sSubRace == CExoString("")) {
        return g_pRules->m_lstRaces[m_nRace].GetNameText();
    }
    return m_sSubRace;
}
