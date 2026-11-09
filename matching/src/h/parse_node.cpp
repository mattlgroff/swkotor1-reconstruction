// ParseNode: the "node <type> <name>" block reader of the ASCII model
// parser. TU-local minimal node classes (sizes from the allocations).

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);  // 0x00438A50 / 0x004642B0
    void Allocate(int n);                       // 0x005E03C0
    void Remove(T value);                       // 0x00445080
    T& operator[](int i) { return element[i]; }
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
};

struct Vector { float x, y, z; };
struct Quaternion { float w, x, y, z; };

extern "C" int __cdecl sscanf(const char*, const char*, ...);
extern "C" int __cdecl _stricmp(const char*, const char*);
extern "C" int __cdecl strncmp(const char*, const char*, unsigned int);
extern "C" unsigned int __cdecl strlen(const char*);
extern "C" char* __cdecl strcpy(char*, const char*);
#pragma intrinsic(strlen, strcpy)

extern unsigned char g_bBinaryModel;        // 0x007B927C
char* AurResGetNextLine();                  // 0x0044BFA0

struct NewController;
class MdlNode;
class Model;
class Animation;
class MdlNodeAABB;
class MdlNodeAnimMesh;
class MdlNodeCamera;
class MdlNodeDanglyMesh;
class MdlNodeEmitter;
class MdlNodeLight;
class MdlNodeLightsaber;
class MdlNodeReference;
class MdlNodeSkin;
class MdlNodeTriMesh;

class MaxTree {
public:
    char pad_0x0[0x28];
    MdlNode* m_pRoot;                           // +0x28
    int m_nNodeCount;                           // +0x2c
    char pad_0x30[0x84 - 0x30];

    Model* AsModel();                           // 0x0043E1C0
    Animation* AsAnimation();                   // 0x0043E1D0
};

class Model : public MaxTree {
public:
    char pad_0x84[0xb8 - 0x84];
    CExoArrayList<char*> m_lNodeNames;          // +0xb8
};

class Animation : public MaxTree {
public:
    Model* m_pModel;                            // +0x84
};

class MdlNode {
public:
    unsigned short m_w0;                        // +0x00
    unsigned short m_nNodeNumber;               // +0x02
    int m_nNameIndex;                           // +0x04
    MaxTree* m_pTree;                           // +0x08
    MdlNode* m_pParent;                         // +0x0c
    Vector m_vPosition;                         // +0x10
    Quaternion m_qOrientation;                  // +0x1c
    CExoArrayList<MdlNode*> m_lChildren;        // +0x2c
    CExoArrayList<NewController> m_lControllers;    // +0x38
    CExoArrayList<float> m_lControllerData;     // +0x44

    MdlNode(char* szName);                      // 0x00445210
    ~MdlNode();                                 // 0x0044A040
    void InternalParseField(char* s);           // 0x0043FA10
    void InternalPostProcess();                 // 0x0043FB90
    MdlNodeAABB* AsMdlNodeAABB();               // 0x0043E340
    MdlNodeAnimMesh* AsMdlNodeAnimMesh();       // 0x0043E360
    MdlNodeCamera* AsMdlNodeCamera();           // 0x0043E370
    MdlNodeDanglyMesh* AsMdlNodeDanglyMesh();   // 0x0043E380
    MdlNodeEmitter* AsMdlNodeEmitter();         // 0x0043E3C0
    MdlNodeLight* AsMdlNodeLight();             // 0x0043E3D0
    MdlNodeLightsaber* AsMdlNodeLightsaber();   // 0x0043E3A0
    MdlNodeReference* AsMdlNodeReference();     // 0x0043E3E0
    MdlNodeSkin* AsMdlNodeSkin();               // 0x0043E3F0
    MdlNodeTriMesh* AsMdlNodeTriMesh();         // 0x0043E400
};

class MdlNodeCamera : public MdlNode {
public:
    MdlNodeCamera(char* szName);                // 0x0044A280
};

class MdlNodeEmitter : public MdlNode {
public:
    char pad_0x50[0x130 - 0x50];
    MdlNodeEmitter(char* szName);               // 0x0044A300
};

class MdlNodeLight : public MdlNode {
public:
    char pad_0x50[0xac - 0x50];
    MdlNodeLight(char* szName);                 // 0x0044A3F0
    ~MdlNodeLight();                            // 0x00448F80
};

class MdlNodeReference : public MdlNode {
public:
    char pad_0x50[0x74 - 0x50];
    MdlNodeReference(char* szName);             // 0x0044A5C0
};

class MdlNodeTriMesh : public MdlNode {
public:
    char pad_0x50[0x19c - 0x50];
    MdlNodeTriMesh(char* szName);               // 0x0044A640
    ~MdlNodeTriMesh();                          // 0x00448BF0
};

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    char pad_0x19c[0x1d4 - 0x19c];
    MdlNodeAnimMesh(char* szName);              // 0x0044ABC0
    ~MdlNodeAnimMesh();                         // 0x00448DA0
};

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    char pad_0x19c[0x1b8 - 0x19c];
    MdlNodeDanglyMesh(char* szName);            // 0x0044AE00
    ~MdlNodeDanglyMesh();                       // 0x004490F0
};

class MdlNodeAABB : public MdlNodeTriMesh {
public:
    char pad_0x19c[0x1a0 - 0x19c];
    MdlNodeAABB(char* szName);                  // 0x0044A8B0
    ~MdlNodeAABB();                             // 0x00449170
};

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    char pad_0x19c[0x200 - 0x19c];
    MdlNodeSkin(char* szName);                  // 0x0044AC70
    ~MdlNodeSkin();                             // 0x00448E60
};

class MdlNodeLightsaber : public MdlNodeTriMesh {
public:
    char pad_0x19c[0x1b0 - 0x19c];
    MdlNodeLightsaber(char* szName);            // 0x0044AD80
    ~MdlNodeLightsaber();                       // 0x00449090
};

// FUNCTION: SWKOTOR 0x004680E0
MdlNode* ParseNode(char* s, MaxTree* pTree)
{
    char szType[0x40];
    char szName[0x40];
    sscanf(s, "%s %s", szType, szName);
    MdlNode* pNode;
    if (!_stricmp(szType, "camera"))
        pNode = new MdlNodeCamera(szName);
    else if (!_stricmp(szType, "emitter"))
        pNode = new MdlNodeEmitter(szName);
    else if (!_stricmp(szType, "light"))
        pNode = new MdlNodeLight(szName);
    else if (!_stricmp(szType, "trimesh"))
        pNode = new MdlNodeTriMesh(szName);
    else if (!_stricmp(szType, "animmesh"))
        pNode = new MdlNodeAnimMesh(szName);
    else if (!_stricmp(szType, "danglymesh"))
        pNode = new MdlNodeDanglyMesh(szName);
    else if (!_stricmp(szType, "reference"))
        pNode = new MdlNodeReference(szName);
    else if (!_stricmp(szType, "aabb"))
        pNode = new MdlNodeAABB(szName);
    else if (!_stricmp(szType, "skin"))
        pNode = new MdlNodeSkin(szName);
    else if (!_stricmp(szType, "lightsaber"))
        pNode = new MdlNodeLightsaber(szName);
    else
        pNode = new MdlNode(szName);
    pNode->m_pTree = pTree;

    int bDone = 0;
    while (!bDone) {
        char* szLine = AurResGetNextLine();
        if (!szLine) {
            bDone = 1;
        } else {
            while (*szLine == ' ' || *szLine == '\t')
                szLine++;
            if (!strncmp(szLine, "endnode", 7))
                bDone = 1;
            else
                pNode->InternalParseField(szLine);
        }
    }

    if (pTree->AsAnimation()) {
        MdlNode* pNew = new MdlNode(szName);
        pNew->m_lControllerData = pNode->m_lControllerData;
        pNew->m_nNodeNumber = pNode->m_nNodeNumber;
        pNew->m_lControllers = pNode->m_lControllers;
        pNew->m_pTree = pTree;
        MdlNode* pParent = pNode->m_pParent;
        if (pParent) {
            pNew->m_pParent = pParent;
            pParent->m_lChildren.Remove(pNode);
            pNew->m_pParent->m_lChildren.Add(pNew);
        } else if (pTree->m_pRoot == pNode) {
            pTree->m_pRoot = pNew;
        }
        MdlNode* pOld = pNode;
        pNode = pNew;
        if (pOld->AsMdlNodeAABB())
            delete pOld->AsMdlNodeAABB();
        else if (pOld->AsMdlNodeAnimMesh())
            delete pOld->AsMdlNodeAnimMesh();
        else if (pOld->AsMdlNodeCamera())
            delete pOld->AsMdlNodeCamera();
        else if (pOld->AsMdlNodeDanglyMesh())
            delete pOld->AsMdlNodeDanglyMesh();
        else if (pOld->AsMdlNodeEmitter())
            delete pOld->AsMdlNodeEmitter();
        else if (pOld->AsMdlNodeLight())
            delete pOld->AsMdlNodeLight();
        else if (pOld->AsMdlNodeLightsaber())
            delete pOld->AsMdlNodeLightsaber();
        else if (pOld->AsMdlNodeReference())
            delete pOld->AsMdlNodeReference();
        else if (pOld->AsMdlNodeSkin())
            delete pOld->AsMdlNodeSkin();
        else if (pOld->AsMdlNodeTriMesh())
            delete pOld->AsMdlNodeTriMesh();
        else
            delete pOld;
    }

    if (pNode->m_pTree->AsModel()) {
        char* szCopy = new char[strlen(szName) + 1];
        strcpy(szCopy, szName);
        pNode->m_pTree->AsModel()->m_lNodeNames.Add(szCopy);
        pNode->m_nNameIndex = pNode->m_pTree->AsModel()->m_lNodeNames.num - 1;
    } else {
        int i = pNode->m_pTree->AsAnimation()->m_pModel->m_lNodeNames.num;
        while (i--) {
            if (!_stricmp(pNode->m_pTree->AsAnimation()->m_pModel->m_lNodeNames[i], szName)) {
                pNode->m_nNameIndex = i;
                break;
            }
        }
        if (i == -1) {
            char* szCopy = new char[strlen(szName) + 1];
            strcpy(szCopy, szName);
            pNode->m_pTree->AsAnimation()->m_pModel->m_lNodeNames.Add(szCopy);
            pNode->m_nNameIndex = pNode->m_pTree->AsAnimation()->m_pModel->m_lNodeNames.num - 1;
        }
    }

    if (!pNode->m_pParent && pTree->m_pRoot != pNode)
        throw 0;
    pNode->m_nNodeNumber = pTree->m_nNodeCount++;
    if (!g_bBinaryModel)
        pNode->InternalPostProcess();
    return pNode;
}
