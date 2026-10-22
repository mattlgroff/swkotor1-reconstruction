// MdlNode -> Part instancing (InternalCreateInstance per node
// type) and the MdlNode ConJoin factories.
#include "g/mdl_create.h"

// FUNCTION: SWKOTOR 0x00447b20
Part* MdlNode::InternalCreateInstance(Gob* pGob)
{
    return new Part(this);
}

// FUNCTION: SWKOTOR 0x00447b90
Part* MdlNodeLight::InternalCreateInstance(Gob* pGob)
{
    PartLight* pPart = new PartLight(this);
    pPart->m_n5c = m_na0;
    pPart->m_n6c = m_n9c;
    pPart->m_n70 = m_bAffectDynamic98;
    pPart->m_n68 = m_n94;
    pPart->m_n48 = 0;
    if (pGob) {
        if (m_bAffectDynamic98 || m_n9c)
            pGob->m_bHasLights = 1;
        if (!m_bAffectDynamic98)
            pGob->m_lLights.Add(pPart);
    }
    return pPart;
}

// FUNCTION: SWKOTOR 0x00447c80
Part* MdlNodeCamera::InternalCreateInstance(Gob* pGob)
{
    return new PartCamera(this);
}

// FUNCTION: SWKOTOR 0x00447ce0
Part* MdlNodeReference::InternalCreateInstance(Gob* pGob)
{
    return new PartReference(this);
}

// FUNCTION: SWKOTOR 0x00447d60
Part* MdlNodeTriMesh::InternalCreateInstance(Gob* pGob)
{
    return new PartTriMesh(this);
}

// FUNCTION: SWKOTOR 0x00447dd0
Part* MdlNodeAnimMesh::InternalCreateInstance(Gob* pGob)
{
    return new PartAnimMesh(this);
}

// FUNCTION: SWKOTOR 0x00447e30
Part* MdlNodeAABB::InternalCreateInstance(Gob* pGob)
{
    return new PartAABB(this);
}

// FUNCTION: SWKOTOR 0x00447e90
Part* MdlNodeDanglyMesh::InternalCreateInstance(Gob* pGob)
{
    return new PartDanglyMesh(this);
}

// FUNCTION: SWKOTOR 0x004480c0
Part* MdlNodeLightsaber::InternalCreateInstance(Gob* pGob)
{
    return new PartLightsaber(this);
}

// FUNCTION: SWKOTOR 0x0043fd70
MdlNode* MdlNode::ConJoinTrigger()
{
    return new MdlNodeTrigger(GetName());
}

// FUNCTION: SWKOTOR 0x00445350
MdlNode* MdlNode::ConJoin()
{
    return new MdlNode(GetName());
}

// FUNCTION: SWKOTOR 0x0044a390
MdlNode* MdlNode::ConJoinEmitter()
{
    return new MdlNodeEmitter(GetName());
}

// FUNCTION: SWKOTOR 0x0044a4a0
MdlNode* MdlNode::ConJoinLight()
{
    return new MdlNodeLight(GetName());
}

// FUNCTION: SWKOTOR 0x0044a5e0
MdlNode* MdlNode::ConJoinReference()
{
    return new MdlNodeReference(GetName());
}

// FUNCTION: SWKOTOR 0x0044a850
MdlNode* MdlNode::ConJoinTriMesh()
{
    return new MdlNodeTriMesh(GetName());
}

// FUNCTION: SWKOTOR 0x0044ad20
MdlNode* MdlNode::ConJoinSkin()
{
    return new MdlNodeSkin(GetName());
}

// FUNCTION: SWKOTOR 0x0044ae60
MdlNode* MdlNode::ConJoinAnimMesh()
{
    return new MdlNodeAnimMesh(GetName());
}
