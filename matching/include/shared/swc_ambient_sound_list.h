// swc_ambient_sound_list.h -- CSWCAmbientSoundList
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_c_ambient_sound.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_AMBIENT_SOUND_LIST_H
#define SHARED_SWC_AMBIENT_SOUND_LIST_H

#include "shared/exo_string.h"

class CSWCAmbientSoundList {
public:
    CSWCAmbientSoundList();                         // 0x0068D3D0
    ~CSWCAmbientSoundList();                        // 0x0068D430

    CExoString m_sResource;                         // 0x00
    CExoString m_sStinger1;                         // 0x08
    CExoString m_sStinger2;                         // 0x10
    CExoString m_sStinger3;                         // 0x18
};

#endif
