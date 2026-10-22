// MdlNode subclass InternalControl overrides (0x00485560 .. 0x00485630).
#include "e/mdlanim.h"

class PartEmitter {
public:
    void CalculateRadius();                 // 0x0048E1B0
};

class MdlNodeLight : public MdlNode {
public:
    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

class MdlNodeEmitter : public MdlNode {
public:
    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

class MdlNodeCamera : public MdlNode {
public:
    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

class MdlNodeReference : public MdlNode {
public:
    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

// FUNCTION: SWKOTOR 0x00485560
void MdlNodeLight::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    pPart->v24();
    MdlNode::InternalControl(pPart, fPrevTime, fTime, fBlend);
}

// FUNCTION: SWKOTOR 0x00485590
void MdlNodeEmitter::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    PartEmitter* pEmitter = pPart->AsEmitter();
    MdlNode::InternalControl(pPart, fPrevTime, fTime, fBlend);
    pEmitter->CalculateRadius();
}

// FUNCTION: SWKOTOR 0x004855D0
void MdlNodeCamera::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    pPart->v28();
    MdlNode::InternalControl(pPart, fPrevTime, fTime, fBlend);
}

// FUNCTION: SWKOTOR 0x00485600
void MdlNodeReference::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    pPart->AsReference();
    MdlNode::InternalControl(pPart, fPrevTime, fTime, fBlend);
}

// FUNCTION: SWKOTOR 0x00485630
void MdlNodeTriMesh::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    pPart->UpdateTransform();
    MdlNode::InternalControl(pPart, fPrevTime, fTime, fBlend);
}
