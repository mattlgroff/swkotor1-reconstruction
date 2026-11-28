// sound_set_list.h -- CSoundSetList
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/m/sw_sound_set.cpp); the other copies' members fall on its members.
#ifndef SHARED_SOUND_SET_LIST_H
#define SHARED_SOUND_SET_LIST_H


class CResRef;

class CSoundSetList {
public:
    CSoundSetList();
    ~CSoundSetList();
    CResRef GetSoundSetResRef(unsigned short nId);

    unsigned short m_nNumSoundSets;                 // 0x00
    CResRef* m_pResRefs;                            // 0x04
    unsigned char* m_pGenders;                      // 0x08
    unsigned long* m_pStrRefs;                      // 0x0c
};

#endif
