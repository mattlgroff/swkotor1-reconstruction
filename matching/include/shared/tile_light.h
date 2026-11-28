// tile_light.h -- TileLight
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/h/room.cpp); the other copies' members fall on its members.
#ifndef SHARED_TILE_LIGHT_H
#define SHARED_TILE_LIGHT_H

#include "shared/vector.h"

class TileRoom;

class TileLight {
public:
    char pad_0x0[0x4c];
    Vector  m_vPosition;                         // 0x4c
    char pad_0x58[0x4];
    int  m_bDirty;                               // 0x5c
    float  m_fRadius;                            // 0x60
    char pad_0x64[0x8];
    int  m_bActive;                              // 0x6c
    char pad_0x70[0x4];
    int  m_nType;                                // 0x74
    char pad_0x78[0x174];
    TileRoom* m_pRoom;                          // +0x1ec
};

#endif
