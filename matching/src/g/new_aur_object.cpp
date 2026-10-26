// NewCAurObject: create a Gob instance of a model (found by name or loaded
// from open files), with the optional "_x" / "_z" variant trees.
// Behavioural reference: src/units/0665/b313627324-model_instance.cpp.
#include "g/gob.h"

typedef struct _iobuf FILE;

extern "C" char* __cdecl strcpy(char* d, const char* s);
extern "C" size_t __cdecl strlen(const char* s);
#pragma intrinsic(strcpy, strlen)

class MdlNode {
public:
    void ProcessSkinSeams();                    // 0x00438fa0
};

extern unsigned char g_bModelGate;              // 0x007b8fc0
extern int g_bModelVariants;                    // 0x00828068

Model* __cdecl LoadModel(FILE* pMdl, FILE* pMdx);     // 0x00464200
void __cdecl CreateReferenceObjects(Gob* pGob, Part* pPart);    // 0x004448c0
void __cdecl PartInitMaterial(Gob* pGob, Part* pPart, char* szPrefix);   // 0x00442d70

// FUNCTION: SWKOTOR 0x00449cc0
// NONMATCH (35): flow, calls and frame are exact; only the variant model
// registers are swapped (original: _x in ebp, _z in ebx, with '_' kept in bl).
Gob* __cdecl NewCAurObject(char* szModel, char* szName, FILE* pMdl, FILE* pMdx)
{
    unsigned char bGate = g_bModelGate;
    Model* pModelX = 0;
    Model* pModelZ = 0;
    g_bModelGate = 0;
    Model* pModel;
    if (!pMdl) {
        pModel = FindModel(szModel);
        if (g_bModelVariants) {
            char szVariant[64];
            strcpy(szVariant, szModel);
            int n = strlen(szModel);
            szVariant[n + 2] = 0;
            szVariant[n] = '_';
            szVariant[n + 1] = 'x';
            pModelX = FindModel(szVariant);
            strcpy(szVariant, szModel);
            n = strlen(szModel);
            szVariant[n] = '_';
            szVariant[n + 1] = 'z';
            szVariant[n + 2] = 0;
            pModelZ = FindModel(szVariant);
        }
    } else {
        pModel = LoadModel(pMdl, pMdx);
    }
    if (!pModel) {
        g_bModelGate = bGate;
        return 0;
    }
    if (!szName || !*szName)
        szName = pModel->m_szName;
    Gob* pGob = new Gob(szName);
    pGob->m_pModel = pModel;
    pModel->m_nRefCount++;
    pGob->m_pModelX = pModelX;
    if (pModelX)
        pModelX->m_nRefCount++;
    pGob->m_pModelZ = pModelZ;
    if (pModelZ)
        pModelZ->m_nRefCount++;
    Part* pRoot = CreateInstanceTreeR(pModel->m_pRoot, pGob);
    pRoot->Finalize(pRoot);
    pGob->m_pRoot = pRoot;
    pRoot->m_pNode->ProcessSkinSeams();
    pGob->m_pRoot->m_pGob = pGob;
    if (pModelX) {
        Part* pPart = CreateInstanceTreeR(pModelX->m_pRoot, pGob);
        pPart->Finalize(pPart);
        pGob->m_pRootX = pPart;
        pPart->m_pGob = pGob;
    }
    if (pModelZ) {
        Part* pPart = CreateInstanceTreeR(pModelZ->m_pRoot, pGob);
        pPart->Finalize(pPart);
        pGob->m_pRootZ = pPart;
        pPart->m_pGob = pGob;
    }
    CreateReferenceObjects(pGob, pGob->m_pRoot);
    if (pGob->m_pRootX)
        CreateReferenceObjects(pGob, pGob->m_pRootX);
    if (pGob->m_pRootZ)
        CreateReferenceObjects(pGob, pGob->m_pRootZ);
    pGob->DoEvent("_EmitterTarget", 0);
    PartInitMaterial(pGob, pGob->m_pRoot, "");
    if (pGob->m_pRootX)
        PartInitMaterial(pGob, pGob->m_pRootX, "");
    if (pGob->m_pRootZ)
        PartInitMaterial(pGob, pGob->m_pRootZ, "");
    g_bModelGate = bGate;
    pGob->UpdateBoundingBox();
    pGob->m_bDirty = 1;
    pGob->Update014(0);
    return pGob;
}
