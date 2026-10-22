// MdlNode per-type dispatchers (0x0043f890, 0x0043fa10,
// 0x0043fb90, 0x004488f0, 0x00448a70, 0x0044b140). Each one switches on the
// exact node type and tail-calls the subclass member through the inline
// checked downcast (MdlNode::As*). Behavioural reference:
// src/batches/b0044_mdlnode_lifecycle.cpp. Self-contained: only the member
// functions the dispatchers call are declared.
#ifndef G_MDL_DISPATCH_H
#define G_MDL_DISPATCH_H

class Part;
class Gob;

class MdlNodeLight;
class MdlNodeEmitter;
class MdlNodeCamera;
class MdlNodeReference;
class MdlNodeTriMesh;
class MdlNodeSkin;
class MdlNodeAnimMesh;
class MdlNodeDanglyMesh;
class MdlNodeAABB;
class MdlNodeTrigger;
class MdlNodeLightsaber;

class MdlNode {
public:
    short m_nType;              // +0x00

    MdlNodeLight* AsMdlNodeLight()
    {
        if ((m_nType & 0x003) == 0x003) return (MdlNodeLight*)this; return 0;
    }
    MdlNodeEmitter* AsMdlNodeEmitter()
    {
        if ((m_nType & 0x005) == 0x005) return (MdlNodeEmitter*)this; return 0;
    }
    MdlNodeCamera* AsMdlNodeCamera()
    {
        if ((m_nType & 0x009) == 0x009) return (MdlNodeCamera*)this; return 0;
    }
    MdlNodeReference* AsMdlNodeReference()
    {
        if ((m_nType & 0x011) == 0x011) return (MdlNodeReference*)this; return 0;
    }
    MdlNodeTriMesh* AsMdlNodeTriMesh()
    {
        if ((m_nType & 0x021) == 0x021) return (MdlNodeTriMesh*)this; return 0;
    }
    MdlNodeSkin* AsMdlNodeSkin()
    {
        if ((m_nType & 0x061) == 0x061) return (MdlNodeSkin*)this; return 0;
    }
    MdlNodeAnimMesh* AsMdlNodeAnimMesh()
    {
        if ((m_nType & 0x0a1) == 0x0a1) return (MdlNodeAnimMesh*)this; return 0;
    }
    MdlNodeDanglyMesh* AsMdlNodeDanglyMesh()
    {
        if ((m_nType & 0x121) == 0x121) return (MdlNodeDanglyMesh*)this; return 0;
    }
    MdlNodeAABB* AsMdlNodeAABB()
    {
        if ((m_nType & 0x221) == 0x221) return (MdlNodeAABB*)this; return 0;
    }
    MdlNodeTrigger* AsMdlNodeTrigger()
    {
        if ((m_nType & 0x401) == 0x401) return (MdlNodeTrigger*)this; return 0;
    }
    MdlNodeLightsaber* AsMdlNodeLightsaber()
    {
        if ((m_nType & 0x821) == 0x821) return (MdlNodeLightsaber*)this; return 0;
    }

    // Dispatchers.
    void InternalControl(Part* pPart, float f1, int n, float f2);  // 0x0043f890
    void InternalParseField(char* szLine);                          // 0x0043fa10
    void InternalPostProcess();                                     // 0x0043fb90
    void InternalDestructor();                                      // 0x004488f0
    Part* InternalCreateInstance(Gob* pGob);                        // 0x00448a70
    MdlNode* ConJoin();                                             // 0x0044b140

    // MdlNode's own implementations.
    void Control(Part* pPart, float f1, int n, float f2);           // 0x004854d0
    void ParseField(char* szLine);                                  // 0x00465560
    void PostProcess();                                             // 0x004015a0
    void Destructor();                                              // 0x0043fdd0
    Part* CreateInstance(Gob* pGob);                                // 0x00447b20
    MdlNode* ConJoinNode();                                         // 0x00445350
};

class MdlNodeLight : public MdlNode {
public:
    void Control(Part* pPart, float f1, int n, float f2);           // 0x00485560
    void ParseField(char* szLine);                                  // 0x00469150
    void PostProcess();                                             // 0x0043dfc0
    void Destructor();                                              // 0x00448000
    Part* CreateInstance(Gob* pGob);                                // 0x00447b90
    MdlNode* ConJoinNode();                                         // 0x0044a4a0
};

class MdlNodeEmitter : public MdlNode {
public:
    void Control(Part* pPart, float f1, int n, float f2);           // 0x00485590
    void ParseField(char* szLine);                                  // 0x004658b0
    void PostProcess();                                             // 0x00437c10
    Part* CreateInstance(Gob* pGob);                                // 0x0049d5c0
    MdlNode* ConJoinNode();                                         // 0x0044a390
};

class MdlNodeCamera : public MdlNode {
public:
    void Control(Part* pPart, float f1, int n, float f2);           // 0x004855d0
    void ParseField(char* szLine);                                  // 0x00468060
    Part* CreateInstance(Gob* pGob);                                // 0x00447c80
    MdlNode* ConJoinNode();                                         // 0x0044a2a0
};

class MdlNodeReference : public MdlNode {
public:
    void Control(Part* pPart, float f1, int n, float f2);           // 0x00485600
    void ParseField(char* szLine);                                  // 0x00468070
    Part* CreateInstance(Gob* pGob);                                // 0x00447ce0
    MdlNode* ConJoinNode();                                         // 0x0044a5e0
};

class MdlNodeTriMesh : public MdlNode {
public:
    void Control(Part* pPart, float f1, int n, float f2);           // 0x00485630
    void ParseField(char* szLine);                                  // 0x00469700
    void PostProcess();                                             // 0x0043cf00
    Part* CreateInstance(Gob* pGob);                                // 0x00447d60
    MdlNode* ConJoinNode();                                         // 0x0044a850
};

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    void ParseField(char* szLine);                                  // 0x0046a2f0
    void PostProcess();                                             // 0x0043b350
    Part* CreateInstance(Gob* pGob);                                // 0x00487510
    MdlNode* ConJoinNode();                                         // 0x0044ad20
};

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    void Control(Part* pPart, float f1, int n, float f2);           // 0x00485660
    void ParseField(char* szLine);                                  // 0x0046a240
    Part* CreateInstance(Gob* pGob);                                // 0x00447dd0
    MdlNode* ConJoinNode();                                         // 0x0044ae60
};

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    void ParseField(char* szLine);                                  // 0x0046a160
    void PostProcess();                                             // 0x0043d990
    Part* CreateInstance(Gob* pGob);                                // 0x00447e90
};

class MdlNodeAABB : public MdlNodeTriMesh {
public:
    void ParseField(char* szLine);                                  // 0x0046a3c0
    Part* CreateInstance(Gob* pGob);                                // 0x00447e30
};

class MdlNodeTrigger : public MdlNode {
public:
    void ParseField(char* szLine);                                  // 0x0060e760
    Part* CreateInstance(Gob* pGob);                                // 0x0063e7c0
    MdlNode* ConJoinNode();                                         // 0x0043fd70
};

class MdlNodeLightsaber : public MdlNodeTriMesh {
public:
    void ParseField(char* szLine);                                  // 0x0046a150
    void PostProcess();                                             // 0x0043b380
    void Destructor();                                              // 0x0043fd10
    Part* CreateInstance(Gob* pGob);                                // 0x004480c0
};

#endif
