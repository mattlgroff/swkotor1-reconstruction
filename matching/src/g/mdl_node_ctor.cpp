// MdlNode base constructor (its own unit so the derived
// constructors in g/mdl_ctor.cpp keep calling it out of line).
#include "g/mdl_ctor.h"

// FUNCTION: SWKOTOR 0x00445210
MdlNode::MdlNode(char* szName)
{
    m_pParent = 0;
    m_nNodeNumber = 0;
    m_nType = 1;
    g_nMdlNodesAlive++;
    m_vPosition = Vector(0, 0, 0);
    m_qOrientation = Quaternion(1.0f, 0, 0, 0);
    g_nMdlNodesTotal++;
}
