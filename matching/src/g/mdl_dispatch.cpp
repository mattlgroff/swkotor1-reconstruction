// MdlNode per-type dispatchers. Each switches on the exact node
// type and tail-calls the subclass member through the inline checked
// downcast; unknown types fall through.
#include "g/mdl_dispatch.h"

// FUNCTION: SWKOTOR 0x0043f890
void MdlNode::InternalControl(Part* pPart, float f1, int n, float f2)
{
    switch (m_nType) {
    case 0x001: Control(pPart, f1, n, f2); return;
    case 0x003: AsMdlNodeLight()->Control(pPart, f1, n, f2); return;
    case 0x005: AsMdlNodeEmitter()->Control(pPart, f1, n, f2); return;
    case 0x009: AsMdlNodeCamera()->Control(pPart, f1, n, f2); return;
    case 0x011: AsMdlNodeReference()->Control(pPart, f1, n, f2); return;
    case 0x021: AsMdlNodeTriMesh()->Control(pPart, f1, n, f2); return;
    case 0x061: AsMdlNodeSkin()->Control(pPart, f1, n, f2); return;
    case 0x0a1: AsMdlNodeAnimMesh()->Control(pPart, f1, n, f2); return;
    case 0x121: AsMdlNodeDanglyMesh()->Control(pPart, f1, n, f2); return;
    case 0x221: AsMdlNodeAABB()->Control(pPart, f1, n, f2); return;
    case 0x401: AsMdlNodeTrigger()->Control(pPart, f1, n, f2); return;
    case 0x821: AsMdlNodeLightsaber()->Control(pPart, f1, n, f2); return;
    }
}

// FUNCTION: SWKOTOR 0x0043fa10
void MdlNode::InternalParseField(char* szLine)
{
    switch (m_nType) {
    case 0x001: ParseField(szLine); return;
    case 0x003: AsMdlNodeLight()->ParseField(szLine); return;
    case 0x005: AsMdlNodeEmitter()->ParseField(szLine); return;
    case 0x009: AsMdlNodeCamera()->ParseField(szLine); return;
    case 0x011: AsMdlNodeReference()->ParseField(szLine); return;
    case 0x021: AsMdlNodeTriMesh()->ParseField(szLine); return;
    case 0x061: AsMdlNodeSkin()->ParseField(szLine); return;
    case 0x0a1: AsMdlNodeAnimMesh()->ParseField(szLine); return;
    case 0x121: AsMdlNodeDanglyMesh()->ParseField(szLine); return;
    case 0x221: AsMdlNodeAABB()->ParseField(szLine); return;
    case 0x401: AsMdlNodeTrigger()->ParseField(szLine); return;
    case 0x821: AsMdlNodeLightsaber()->ParseField(szLine); return;
    }
}

// FUNCTION: SWKOTOR 0x0043fb90
void MdlNode::InternalPostProcess()
{
    switch (m_nType) {
    case 0x001: PostProcess(); return;
    case 0x003: AsMdlNodeLight()->PostProcess(); return;
    case 0x005: AsMdlNodeEmitter()->PostProcess(); return;
    case 0x009: AsMdlNodeCamera()->PostProcess(); return;
    case 0x011: AsMdlNodeReference()->PostProcess(); return;
    case 0x021: AsMdlNodeTriMesh()->PostProcess(); return;
    case 0x061: AsMdlNodeSkin()->PostProcess(); return;
    case 0x0a1: AsMdlNodeAnimMesh()->PostProcess(); return;
    case 0x121: AsMdlNodeDanglyMesh()->PostProcess(); return;
    case 0x221: AsMdlNodeAABB()->PostProcess(); return;
    case 0x401: AsMdlNodeTrigger()->PostProcess(); return;
    case 0x821: AsMdlNodeLightsaber()->PostProcess(); return;
    }
}

// FUNCTION: SWKOTOR 0x004488f0
void MdlNode::InternalDestructor()
{
    switch (m_nType) {
    case 0x001: Destructor(); return;
    case 0x003: AsMdlNodeLight()->Destructor(); return;
    case 0x005: AsMdlNodeEmitter()->Destructor(); return;
    case 0x009: AsMdlNodeCamera()->Destructor(); return;
    case 0x011: AsMdlNodeReference()->Destructor(); return;
    case 0x021: AsMdlNodeTriMesh()->Destructor(); return;
    case 0x061: AsMdlNodeSkin()->Destructor(); return;
    case 0x0a1: AsMdlNodeAnimMesh()->Destructor(); return;
    case 0x121: AsMdlNodeDanglyMesh()->Destructor(); return;
    case 0x221: AsMdlNodeAABB()->Destructor(); return;
    case 0x401: AsMdlNodeTrigger()->Destructor(); return;
    case 0x821: AsMdlNodeLightsaber()->Destructor(); return;
    }
}

// FUNCTION: SWKOTOR 0x00448a70
Part* MdlNode::InternalCreateInstance(Gob* pGob)
{
    switch (m_nType) {
    case 0x001: return CreateInstance(pGob);
    case 0x003: return AsMdlNodeLight()->CreateInstance(pGob);
    case 0x005: return AsMdlNodeEmitter()->CreateInstance(pGob);
    case 0x009: return AsMdlNodeCamera()->CreateInstance(pGob);
    case 0x011: return AsMdlNodeReference()->CreateInstance(pGob);
    case 0x021: return AsMdlNodeTriMesh()->CreateInstance(pGob);
    case 0x061: return AsMdlNodeSkin()->CreateInstance(pGob);
    case 0x0a1: return AsMdlNodeAnimMesh()->CreateInstance(pGob);
    case 0x121: return AsMdlNodeDanglyMesh()->CreateInstance(pGob);
    case 0x221: return AsMdlNodeAABB()->CreateInstance(pGob);
    case 0x401: return AsMdlNodeTrigger()->CreateInstance(pGob);
    case 0x821: return AsMdlNodeLightsaber()->CreateInstance(pGob);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0044b140
MdlNode* MdlNode::ConJoin()
{
    switch (m_nType) {
    case 0x001: return ConJoinNode();
    case 0x003: return AsMdlNodeLight()->ConJoinNode();
    case 0x005: return AsMdlNodeEmitter()->ConJoinNode();
    case 0x009: return AsMdlNodeCamera()->ConJoinNode();
    case 0x011: return AsMdlNodeReference()->ConJoinNode();
    case 0x021: return AsMdlNodeTriMesh()->ConJoinNode();
    case 0x061: return AsMdlNodeSkin()->ConJoinNode();
    case 0x0a1: return AsMdlNodeAnimMesh()->ConJoinNode();
    case 0x121: return AsMdlNodeDanglyMesh()->ConJoinNode();
    case 0x221: return AsMdlNodeAABB()->ConJoinNode();
    case 0x401: return AsMdlNodeTrigger()->ConJoinNode();
    case 0x821: return AsMdlNodeLightsaber()->ConJoinNode();
    }
    return 0;
}
