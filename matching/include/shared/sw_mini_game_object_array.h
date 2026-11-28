// sw_mini_game_object_array.h -- CSWMiniGameObjectArray
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_mini_game_object.h); the other copies' members fall on its members.
#ifndef SHARED_SW_MINI_GAME_OBJECT_ARRAY_H
#define SHARED_SW_MINI_GAME_OBJECT_ARRAY_H

#include "shared/exo_string.h"

class CSWMiniGameObject;

class CSWMiniGameObjectArray {
public:
    CSWMiniGameObjectArray();                       // 0x0066BF10
    CSWMiniGameObject* GetObject(unsigned long nId, unsigned char nType);   // 0x0066BF30
    CSWMiniGameObject* GetObjectByTag(const CExoString& sTag);             // 0x0066BFB0
    int GetFreeSlot();                              // 0x0066C060

    int m_nNext;                                    // 0x00
    CSWMiniGameObject* m_pObjects[0xff];            // 0x04
};

#endif
