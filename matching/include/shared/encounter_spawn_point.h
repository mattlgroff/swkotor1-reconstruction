// encounter_spawn_point.h -- CEncounterSpawnPoint
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/encounter_load.cpp); the other copies' members fall on its members.
#ifndef SHARED_ENCOUNTER_SPAWN_POINT_H
#define SHARED_ENCOUNTER_SPAWN_POINT_H

#include "shared/vector.h"

class CEncounterSpawnPoint {
public:
    CEncounterSpawnPoint();
    Vector m_vPosition;              // +0x0
    float m_fOrientation;            // +0xc
};

#endif
