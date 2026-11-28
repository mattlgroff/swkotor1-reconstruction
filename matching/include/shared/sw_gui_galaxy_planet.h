// sw_gui_galaxy_planet.h -- CSWGuiGalaxyPlanet
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/k/sw_gui_galaxy_map_ctor.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_GUI_GALAXY_PLANET_H
#define SHARED_SW_GUI_GALAXY_PLANET_H

#include "shared/res_ref.h"

struct CSWGuiGalaxyPlanet {
    int m_nNameStrRef;                              // 0x00
    int m_nDescriptionStrRef;                       // 0x04
    CResRef m_cModel;                               // 0x08
};

#endif
