// Gob animation-model binding (vtable 0x741268 +0x20) and the
// MaxTree node-map link it uses.
extern "C" char* __cdecl strcpy(char* d, const char* s);
extern "C" char* __cdecl strcat(char* d, const char* s);
#pragma intrinsic(strcpy, strcat)

struct _iobuf;
extern "C" _iobuf* __cdecl fopen(const char* szName, const char* szMode);  // 0x006fad9d

class MdlNode;
class Model;
class Animation;

extern int g_nModelMemory;                  // 0x007a6990

int __cdecl MdlNodeCountNodes(MdlNode* pRoot, int n);              // 0x00438400
void __cdecl MdlNodeBuildMap(MdlNode* pRoot, MdlNode* pModelRoot, int* pCount);  // 0x00439a10
void __cdecl MdlNodeRemoveAnimation(MdlNode* pRoot, int nIndex);   // 0x00437db0

class MaxTree {
public:
    char pad00[0x28];
    MdlNode* m_pRoot;                       // +0x28
    char pad2c[0x54 - 0x2c];
    int m_nRefCount;                        // +0x54

    Animation* AsAnimation();               // 0x0043e1d0
    void LinkToModel(MaxTree* pModel);
};

class Animation : public MaxTree {
public:
    char pad58[0x7c - 0x58];
    int m_nKeys;                            // +0x7c
};

class Model : public MaxTree {
public:
    Animation** m_pAnimations;              // +0x58
    int m_nAnimations;                      // +0x5c
};

Model* __cdecl FindModel(char* szName);                            // 0x00464110
Model* __cdecl LoadModel(_iobuf* pFile, int n);                    // 0x00464200

// FUNCTION: SWKOTOR 0x00439d70
void MaxTree::LinkToModel(MaxTree* pModel)
{
    if (!pModel)
        return;
    MdlNode* pModelRoot = pModel->m_pRoot;
    int nCount = MdlNodeCountNodes(pModelRoot, -1) + 1;
    MdlNodeBuildMap(m_pRoot, pModelRoot, &nCount);
    if (AsAnimation()) {
        g_nModelMemory += 0x8c;
        g_nModelMemory += AsAnimation()->m_nKeys * 0x24;
        MdlNodeRemoveAnimation(m_pRoot, 1);
    }
}

class Gob {
public:
    char pad00[0x58];
    Model* m_pModel;                        // +0x58
    char pad5c[0x64 - 0x5c];
    Model* m_pAnimModel;                    // +0x64

    int SetAnimationModel(char* szName);
};

// FUNCTION: SWKOTOR 0x00440890
int Gob::SetAnimationModel(char* szName)
{
    m_pAnimModel = FindModel(szName);
    if (!m_pAnimModel) {
        char szFile[64];
        strcpy(szFile, szName);
        strcat(szFile, ".mdl");
        _iobuf* pFile = fopen(szFile, "r");
        if (pFile) {
            m_pAnimModel = LoadModel(pFile, 0);
            m_pAnimModel->LinkToModel(m_pModel);
            for (int i = 0; i < m_pModel->m_nAnimations; i++)
                m_pModel->m_pAnimations[i]->LinkToModel(m_pModel);
        } else {
            return 0;
        }
    }
    m_pAnimModel->m_nRefCount++;
    return 1;
}
