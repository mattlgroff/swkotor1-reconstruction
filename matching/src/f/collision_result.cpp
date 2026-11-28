// Out-of-line copy of the CSWCollisionResult constructor (0x00581320); the
// walkmesh queries in room_surface_mesh.cpp inline it.

#include "shared/vector.h"

struct CSWCollisionResult {
    CSWCollisionResult(unsigned long nMask);
    unsigned long m_nMask;           // +0x00
    int m_n04;                       // +0x04
    int m_bHit;                      // +0x08
    int m_nFace;                     // +0x0c
    Vector m_vHit;                   // +0x10
    unsigned long m_nBit;            // +0x1c
};

// FUNCTION: SWKOTOR 0x00581320
CSWCollisionResult::CSWCollisionResult(unsigned long nMask)
{
    m_nFace = -1;
    m_nMask = nMask;
    m_n04 = 0;
    m_bHit = 0;
    m_nBit = 0;
    m_vHit = Vector(0.0f, 0.0f, 0.0f);
}
