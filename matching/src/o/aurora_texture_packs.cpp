// CAuroraInterface resource access, texture packs, model requests and
// the constructor / destructor, 0x0070C920-0x0070D9C0.
#include "o/aurora_textures.h"

extern "C" int __cdecl sprintf(char* buf, const char* fmt, ...);
extern "C" char* __cdecl _strupr(char* s);
extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"



class Model;
Model* BinaryFindModel(char* szName);               // 0x004640F0
void AurTextureFreeLayers();                        // 0x0041EBD0
int AurTextureSetMem(int nMem);                     // 0x00421D90
// Folded onto the shared "return 0" leaf 0x0063E7F0; real name unknown.
int AurInterfaceStub(int nArg1, int nArg2);

#include "shared/globals.h"
extern CExoLinkedList<CAuroraModel> g_lRequestedModels; // 0x00834094

#include "shared/aurora_callbacks.h"

// FUNCTION: SWKOTOR 0x0070C920
unsigned char* CAuroraInterface::GetResource(const char* szName, const char* szType, int* pSize)
{
    CExoString sName;
    if (!szType || !pSize) {
        return 0;
    }
    if (*szType == '.') {
        szType++;
    }
    char szExt[256];
    sprintf(szExt, "%s", szType);
    _strupr(szExt);
    sName = szName;
    sName = sName.LowerCase();
    if (!strcmp(szExt, "MDL")) {
        CAuroraModel* pModel = GetModelInternal(sName, 0, 0, 0);
        if (!pModel) {
            return 0;
        }
        *pSize = pModel->GetModelDataSize();
        return pModel->GetModelData();
    } else if (!strcmp(szExt, "MDX")) {
        CAuroraModelX* pModelX = GetModelX(sName);
        if (!pModelX) {
            return 0;
        }
        *pSize = pModelX->GetModelXDataSize();
        return pModelX->GetModelXData();
    } else if (!strcmp(szExt, "TXI")) {
        CAuroraTXI* pTXI = GetTXI(sName);
        if (!pTXI) {
            CAuroraProcessedTexture* pTexture = GetProcessedTexture(sName);
            if (!pTexture) {
                return 0;
            }
            *pSize = -1 - pTexture->GetProcessedTextureTXISize();
            return pTexture->GetProcessedTextureTXIPtr();
        }
        *pSize = pTXI->GetTXIDataSize();
        return (unsigned char*)pTXI->GetTXIData();
    } else if (!strcmp(szExt, "TGA")) {
        CAuroraTexture* pTexture = GetTexture(sName);
        if (!pTexture) {
            return 0;
        }
        *pSize = pTexture->GetTextureDataSize();
        return pTexture->GetTextureData();
    } else if (!strcmp(szExt, "TPC")) {
        CAuroraProcessedTexture* pTexture = GetProcessedTexture(sName);
        if (!pTexture) {
            return 0;
        }
        *pSize = pTexture->GetProcessedTextureDataSize();
        return pTexture->GetProcessedTextureDataPtr();
    } else if (!strcmp(szExt, "VIS")) {
        CAuroraVisibility* pVisibility = GetVisibility(sName);
        if (!pVisibility) {
            return 0;
        }
        *pSize = pVisibility->GetVisibilityDataSize();
        return pVisibility->GetVisibilityData();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070CF30
int CAuroraInterface::LoadTexturePack(unsigned char nIndex, const CExoString& sName)
{
    if (nIndex >= 4) {
        return 0;
    }
    if (!sName.GetLength()) {
        return 0;
    }
    if (!g_pExoResMan->AddEncapsulatedResourceFile(sName, 2)) {
        return 0;
    }
    g_asTexturePacks[nIndex] = sName;
    return 1;
}

// FUNCTION: SWKOTOR 0x0070CF80
int CAuroraInterface::UnloadTexturePack(unsigned char nIndex)
{
    CExoString sPack;
    if (nIndex >= 4) {
        return 0;
    }
    sPack = g_asTexturePacks[nIndex];
    if (sPack.GetLength()) {
        if (!g_pExoResMan->RemoveEncapsulatedResourceFile(sPack)) {
            return 0;
        }
        g_asTexturePacks[nIndex] = g_sEmpty;
    }
    return 1;
}

// An out-of-range index yields CExoString(int 0), i.e. "0".
// FUNCTION: SWKOTOR 0x0070D060
CExoString CAuroraInterface::GetCurrentTexturePack(unsigned char nIndex)
{
    if (nIndex >= 4) {
        return 0;
    }
    return g_asTexturePacks[nIndex];
}

// FUNCTION: SWKOTOR 0x0070D340
CAuroraInterface::~CAuroraInterface()
{
    while (!g_lRequestedModels.IsEmpty()) {
        CAuroraModel* pModel = g_lRequestedModels.RemoveHead();
        if (pModel) {
            pModel->Unload();
            delete pModel;
        }
    }
    AurTextureFreeLayers();
}

// FUNCTION: SWKOTOR 0x0070D800
int CAuroraInterface::SetTexturePack(unsigned char nIndex, const CExoString& sName)
{
    if (nIndex >= 4) {
        return 0;
    }
    if (sName == GetCurrentTexturePack(nIndex)) {
        return 1;
    }
    if (!UnloadTexturePack(nIndex)) {
        return 0;
    }
    return LoadTexturePack(nIndex, sName);
}

// FUNCTION: SWKOTOR 0x0070D8D0
void CAuroraInterface::RequestModel(char* szName)
{
    int bCached = 0;
    CExoLinkedListNode* pNode;
    CAuroraModel* pModel;
    {
        CExoString sName(szName);
        pModel = GetModelInternal(sName, 1, &bCached, &pNode);
    }
    if (pModel && bCached) {
        if (pNode) {
            g_lRequestedModels.AddBefore(pModel, pNode);
        } else {
            g_lRequestedModels.AddTail(pModel);
        }
    }
}

// FUNCTION: SWKOTOR 0x0070D980
int RenderInterface::IsResident(char* szName)
{
    bool bResident;
    if (szName && _stricmp(szName, "NULL")) {
        bResident = BinaryFindModel(szName) != 0;
    } else {
        bResident = false;
    }
    return bResident;
}

// FUNCTION: SWKOTOR 0x0070D9C0
CAuroraInterface::CAuroraInterface()
{
    g_pfnAurResGet = GetResource;
    g_pfnAurResFree = ReleaseResource;
    g_pfnGetRaster = GetRaster;
    g_pfnReadRasterHeader = ReadRasterHeader;
    g_pfnReleaseRaster = ReleaseRaster;
    g_pfnGetLayeredRaster = GetLayeredRaster;
    g_pfnReleaseLayeredRaster = ReleaseLayeredRaster;
    g_pfnGetCompressedRaster = GetCompressedRaster;
    g_pfnReadCompressedRasterHeader = ReadCompressedRasterHeader;
    g_pfnGet16BitRaster = Get16BitRaster;
    g_pfnGet16BitRasterHeader = Get16BitRasterHeader;
    g_pfnGetProcessedRaster = GetProcessedRaster;
    g_pfnReadProcessedRasterHeader = ReadProcessedRasterHeader;
    AurInterfaceStub(0, 0);
    AurTextureSetMem(g_nMaxTextureMemory);
}

// FUNCTION: SWKOTOR 0x0070D2D0 SYMBOL: ??0?$CExoLinkedList@VCAuroraModel@@@@QAE@XZ
template class CExoLinkedList<CAuroraModel>;

// The requested-model list is sorted by lower-case name: a hit is reused
// (and unlisted unless auto-requested), a miss returns its insert position.
// FUNCTION: SWKOTOR 0x0070DE20
CAuroraModel* CAuroraInterface::GetModelResHelper(const CExoString& sName, int bAutoRequest, int* pbCached,
                                                  CExoLinkedListNode** ppNode)
{
    CExoString sLowerName;
    CExoString sModelName;
    sLowerName = sName.LowerCase();
    CExoLinkedListNode* pPosition = g_lRequestedModels.GetHeadPos();
    CAuroraModel* pModel = g_lRequestedModels.GetHead();
    if (ppNode) {
        *ppNode = 0;
    }
    while (pPosition) {
        if (pModel) {
            pModel->m_cResRef.CopyToString(sModelName);
            sModelName = sModelName.LowerCase();
            if (sModelName == sLowerName) {
                if (pbCached) {
                    *pbCached = 0;
                }
                if (ppNode) {
                    *ppNode = 0;
                }
                if (bAutoRequest) {
                    return pModel;
                }
                g_lRequestedModels.Remove(pPosition);
                break;
            }
            if (sModelName > sLowerName) {
                if (pbCached) {
                    *pbCached = 1;
                }
                if (ppNode) {
                    *ppNode = pPosition;
                }
                pModel = 0;
                break;
            }
        }
        pModel = g_lRequestedModels.GetNext(pPosition);
    }
    if (!pModel) {
        pModel = new CAuroraModel(sName, bAutoRequest);
        if (!pModel) {
            return 0;
        }
    }
    if (pModel->m_pRes && pModel->m_pRes->GetDemands() > 0) {
        unsigned char* pTag = (unsigned char*)pModel->GetPtrToResHelperInfo();
        if (pTag && *(unsigned short*)pTag == 2002) {
            pTag += sizeof(unsigned short);
            CAuroraModel* pExisting = *(CAuroraModel**)pTag;
            if (pExisting) {
                if (pbCached) {
                    *pbCached = 0;
                }
                delete pModel;
                pModel = pExisting;
            }
        }
    }
    return pModel;
}
