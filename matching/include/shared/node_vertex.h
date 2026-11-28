// node_vertex.h -- NodeVertex
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/h/vertex.h); the other copies' members fall on its members.
#ifndef SHARED_NODE_VERTEX_H
#define SHARED_NODE_VERTEX_H

#include "shared/vector.h"

class NodeVertexAnim;
class NodeVertexDangly;

class NodeVertex {
public:
    NodeVertex(Vector* pPosition, Vector* pNormal, Vector* pV18, Vector* pV24,
               Vector* pV30, Vector* pV3C, Vector* pColor);
    bool operator==(NodeVertex& o);
    NodeVertexAnim* AsAnim();
    NodeVertexDangly* AsDangly();

    Vector m_vPosition;         // +0x00
    Vector m_vNormal;           // +0x0c
    Vector m_v18;               // +0x18
    Vector m_v24;               // +0x24
    Vector m_v30;               // +0x30
    Vector m_v3C;               // +0x3c
    Vector m_vColor;            // +0x48
    unsigned char m_nFlags;     // +0x54
    unsigned char m_nType;      // +0x55 (1 anim, 2 dangly)
};

#endif
