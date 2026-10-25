// controller key parsing and the loaded-model lookup
// (0x00463950 .. 0x004640F0).

#define H_ALLOCATE_OUT_OF_LINE
#include "h/texctl.h"
#include <math.h>
#include <stdio.h>

class Model;

class MaxTree {
public:
    char pad_0x0[8];
    char m_szName[64];      // +0x08
    char pad_0x48[4];
    unsigned char m_nType;  // +0x4c

    Model* AsModel();                           // 0x0043E1C0
    static void operator delete(void* p);       // 0x0044AEC0
};

class Model : public MaxTree {
public:
    ~Model();                                   // 0x0043F790
};

class IODispatcher {
public:
    static IODispatcher* GetRef();              // 0x004A0580
    MaxTree* ReadSync(char* szName);            // 0x004A15B0
    MaxTree* ReadSync(char* szName, int nFlags);    // 0x004A15D0
};

extern Model* g_pLoadingModel;              // 0x007FBAE4

static inline Model* GetLoadedModel(MaxTree* pTree)
{
    if (pTree)
        return pTree->AsModel();
    return g_pLoadingModel;
}

extern CExoArrayList<Model*> g_lModels;     // 0x007A8A64
extern float g_fKeyEpsilon;                 // 0x0078E58C

struct NewController {
    int data[4];
};

template void CExoArrayList<AurGenericKey>::Allocate(int n);
template void CExoArrayList<AurKeyString>::Allocate(int n);

// FUNCTION: SWKOTOR 0x00463950 SYMBOL: ?Allocate@?$CExoArrayList@UAurGenericKey@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x004639B0 SYMBOL: ?Allocate@?$CExoArrayList@UAurKeyString@@@@QAEXH@Z

// FUNCTION: SWKOTOR 0x00463A30
void ParseQuaternion(char* s, Quaternion* pOut)
{
    float fAngle;
    Vector vAxis(0.0f, 0.0f, 0.0f);
    sscanf(s, "%f %f %f %f", &vAxis.x, &vAxis.y, &vAxis.z, &fAngle);
    if (vAxis.x == 0.0f && vAxis.y == 0.0f && vAxis.z == 0.0f)
        *pOut = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    else
        *pOut = Quaternion(vAxis, fAngle);
}

// FUNCTION: SWKOTOR 0x00463B30
void ParseGenericKey(char* s, AurGenericKey* pKey)
{
    pKey->nNumValues = sscanf(s, "%f %f %f %f %f %f %f %f %f %f",
                              &pKey->fValues[0], &pKey->fValues[1], &pKey->fValues[2],
                              &pKey->fValues[3], &pKey->fValues[4], &pKey->fValues[5],
                              &pKey->fValues[6], &pKey->fValues[7], &pKey->fValues[8],
                              &pKey->fValues[9]);
    if (pKey->nNumValues == 5) {
        static Vector vAxis(0.0f, 0.0f, 0.0f);
        vAxis.x = pKey->fValues[1];
        vAxis.y = pKey->fValues[2];
        vAxis.z = pKey->fValues[3];
        Quaternion q(vAxis, pKey->fValues[4]);
        pKey->fValues[1] = q.x;
        pKey->fValues[2] = q.y;
        pKey->fValues[3] = q.z;
        pKey->fValues[4] = q.w;
    }
}

// FUNCTION: SWKOTOR 0x00463F80
void CullUnnecessaryKeys(CExoArrayList<AurGenericKey>* pList)
{
    for (int i = pList->num - 2; i > 1; i--) {
        if (fabs(pList->element[i].fValues[1] - pList->element[i - 1].fValues[1]) < g_fKeyEpsilon
            && fabs(pList->element[i].fValues[2] - pList->element[i - 1].fValues[2]) < g_fKeyEpsilon
            && fabs(pList->element[i].fValues[3] - pList->element[i - 1].fValues[3]) < g_fKeyEpsilon
            && fabs(pList->element[i].fValues[4] - pList->element[i - 1].fValues[4]) < g_fKeyEpsilon
            && fabs(pList->element[i - 2].fValues[1] - pList->element[i - 1].fValues[1]) < g_fKeyEpsilon
            && fabs(pList->element[i - 2].fValues[2] - pList->element[i - 1].fValues[2]) < g_fKeyEpsilon
            && fabs(pList->element[i - 2].fValues[3] - pList->element[i - 1].fValues[3]) < g_fKeyEpsilon
            && fabs(pList->element[i - 2].fValues[4] - pList->element[i - 1].fValues[4]) < g_fKeyEpsilon)
            pList->DelIndex(i - 1);
    }
}

// FUNCTION: SWKOTOR 0x00464090
Model* BinarySearchModel(char* szName, int nLow, int nHigh)
{
    while (nHigh >= nLow) {
        int nMid = (nLow + nHigh) / 2;
        int c = _stricmp(szName, g_lModels.element[nMid]->m_szName);
        if (c == 0)
            return g_lModels.element[nMid];
        if (c < 0)
            nHigh = nMid - 1;
        else
            nLow = nMid + 1;
    }
    return 0;
}

// NONMATCH (3 insns): the original loads szName into eax after pushing
// num - 1; ours loads it into ecx first.
// FUNCTION: SWKOTOR 0x004640F0
Model* BinaryFindModel(char* szName)
{
    int nHigh = g_lModels.num - 1;
    return BinarySearchModel(szName, 0, nHigh);
}

// NONMATCH (13 insns): the ReadSync / AsModel / g_pLoadingModel merge goes
// through eax in the original (mov ebp, eax after the join); also sprintf
// argument scheduling.
// FUNCTION: SWKOTOR 0x00464110 SYMBOL: ?FindModel@@YAPAVModel@@PAD@Z
Model* FindModel(char* szName)
{
    char szBuf[256];
    if (!szName || !_stricmp(szName, "NULL"))
        return 0;
    Model* pModel = BinarySearchModel(szName, 0, g_lModels.num - 1);
    if (pModel)
        return pModel;
    Model* pSaved = g_pLoadingModel;
    g_pLoadingModel = 0;
    IODispatcher* pIO = IODispatcher::GetRef();
    pModel = GetLoadedModel(pIO->ReadSync(szName));
    g_pLoadingModel = pSaved;
    if (!pModel)
        return 0;
    if (_stricmp(pModel->m_szName, szName)) {
        FILE* f = fopen("AR_ERROR.LOG", "w");
        sprintf(szBuf, "Names Differ: %s %s", pModel->m_szName, szName);
        fprintf(f, "Names Differ: %s %s", pModel->m_szName, szName);
        fclose(f);
    }
    return pModel;
}

// NONMATCH (11 insns): same eax join as FindModel; the original then returns
// the null result from eax instead of xor eax, eax.
// FUNCTION: SWKOTOR 0x00464200
Model* LoadModel(char* szName, int nFlags)
{
    if (!szName)
        return 0;
    Model* pSaved = g_pLoadingModel;
    g_pLoadingModel = 0;
    IODispatcher* pIO = IODispatcher::GetRef();
    Model* pModel = GetLoadedModel(pIO->ReadSync(szName, nFlags));
    g_pLoadingModel = pSaved;
    if (!pModel)
        return pModel;
    for (int i = 0; i < g_lModels.num; i++) {
        if (g_lModels.element[i] != pModel
            && !_stricmp(g_lModels.element[i]->m_szName, pModel->m_szName)) {
            delete pModel;
            return g_lModels.element[i];
        }
    }
    return pModel;
}

template CExoArrayList<NewController>& CExoArrayList<NewController>::operator=(const CExoArrayList<NewController>&);
template CExoArrayList<AurGenericKey>& CExoArrayList<AurGenericKey>::operator=(const CExoArrayList<AurGenericKey>&);

// FUNCTION: SWKOTOR 0x004642B0 SYMBOL: ??4?$CExoArrayList@UNewController@@@@QAEAAV0@ABV0@@Z
// FUNCTION: SWKOTOR 0x004643B0 SYMBOL: ??4?$CExoArrayList@UAurGenericKey@@@@QAEAAV0@ABV0@@Z

class MdlNode {
public:
    char pad_0x0[0x2c];
    CExoArrayList<MdlNode*> m_lChildren;    // +0x2c

    char* GetName();                        // 0x00438A00
};

// FUNCTION: SWKOTOR 0x00464AF0
MdlNode* FindMdlNode(MdlNode* pNode, char* szName)
{
    if (!pNode)
        return 0;
    if (!_stricmp(szName, pNode->GetName()))
        return pNode;
    int n = pNode->m_lChildren.num;
    for (int i = 0; i < n; i++) {
        MdlNode* pFound = FindMdlNode(pNode->m_lChildren.element[i], szName);
        if (pFound)
            return pFound;
    }
    return 0;
}

template <class T>
void CExoArrayList<T>::Insert(T value, int nIndex)
{
    int n = num;
    Add(value);
    for (int i = n; i > nIndex; i--)
        element[i] = element[i - 1];
    element[nIndex] = value;
}

template void CExoArrayList<AurKeyString>::Insert(AurKeyString value, int nIndex);

// FUNCTION: SWKOTOR 0x00468970 SYMBOL: ?Insert@?$CExoArrayList@UAurKeyString@@@@QAEXUAurKeyString@@H@Z

// FUNCTION: SWKOTOR 0x0046A590
void BinaryInsertModel(Model* pModel, int nLow, int nHigh)
{
    while (nHigh >= nLow) {
        int nMid = (nLow + nHigh) / 2;
        if (_stricmp(pModel->m_szName, g_lModels.element[nMid]->m_szName) < 0)
            nHigh = nMid - 1;
        else
            nLow = nMid + 1;
    }
    g_lModels.Insert(pModel, nLow);
}

// FUNCTION: SWKOTOR 0x0046A5F0 SYMBOL: ?InsertModel@@YAXPAVModel@@@Z
void InsertModel(Model* pModel)
{
    BinaryInsertModel(pModel, 0, g_lModels.num - 1);
}
