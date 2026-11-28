// encounter_list_entry.h -- CEncounterListEntry
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/encounter_load.cpp); the other copies' members fall on its members.
#ifndef SHARED_ENCOUNTER_LIST_ENTRY_H
#define SHARED_ENCOUNTER_LIST_ENTRY_H

#include "shared/res_ref.h"

class CEncounterListEntry {
public:
    CEncounterListEntry()
    {
        m_cResRef = "";
        m_fCR = 0.0f;
        m_fPoints = 0.0f;
        m_bSingleSpawn = 0;
        m_bAlreadyUsed = 0;
        m_bTried = 0;
    }
    CEncounterListEntry &operator=(const CEncounterListEntry &o);
    ~CEncounterListEntry() {}
    CResRef m_cResRef;               // +0x0
    float m_fCR;                     // +0x10
    float m_fPoints;                 // +0x14
    int m_bAlreadyUsed;              // +0x18
    int m_bTried;                    // +0x1c
    int m_bSingleSpawn;              // +0x20
};

#endif
