// CAuroraInterface resource lookups, 0x0070E080-0x0070EB60. Get*Internal
// reuses a wrapper already tagged on the resource data; Release*Internal
// unloads one and deletes it once nothing demands the resource.
#include "o/aurora_textures.h"

// FUNCTION: SWKOTOR 0x0070E080
CAuroraModel* CAuroraInterface::GetModelInternal(const CExoString& sName, int bAutoRequest, int* pbCached,
                                                 CExoLinkedListNode** ppNode)
{
    CAuroraModel* pModel = GetModelResHelper(sName, bAutoRequest, pbCached, ppNode);
    if (pModel) {
        if (bAutoRequest || pModel->Load()) {
            return pModel;
        }
        if (!pModel->m_pRes || pModel->m_pRes->GetDemands() < 1) {
            delete pModel;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E0E0
void CAuroraInterface::ReleaseModelInternal(CAuroraModel* pModel)
{
    if (pModel->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pModel->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2002 && pInfo->m_pResHelper == pModel) {
            pModel->Unload();
            if (pModel->m_pRes->GetDemands() < 1) {
                delete pModel;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070E130
CAuroraTexture* CAuroraInterface::GetTextureInternal(const CExoString& sName)
{
    CAuroraTexture* pTexture = new CAuroraTexture(CResRef(sName), 0);
    if (pTexture && pTexture->m_pRes && pTexture->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3 && pInfo->m_pResHelper) {
            CAuroraTexture* pLoaded = (CAuroraTexture*)pInfo->m_pResHelper;
            delete pTexture;
            pTexture = pLoaded;
        }
    }
    if (pTexture) {
        if (!pTexture->Load()) {
            if (!pTexture->m_pRes || pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        } else {
            return pTexture;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E220
CAurora16BitTexture* CAuroraInterface::Get16BitTextureInternal(const CExoString& sName)
{
    CAurora16BitTexture* pTexture = new CAurora16BitTexture(CResRef(sName), 0);
    if (pTexture && pTexture->m_pRes && pTexture->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2059 && pInfo->m_pResHelper) {
            CAurora16BitTexture* pLoaded = (CAurora16BitTexture*)pInfo->m_pResHelper;
            delete pTexture;
            pTexture = pLoaded;
        }
    }
    if (pTexture) {
        if (!pTexture->Load()) {
            if (!pTexture->m_pRes || pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        } else {
            return pTexture;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E310
CAuroraLayeredTexture* CAuroraInterface::GetLayeredTextureInternal(const CExoString& sName)
{
    CAuroraLayeredTexture* pTexture = new CAuroraLayeredTexture(CResRef(sName), 0);
    if (pTexture && pTexture->m_pRes && pTexture->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 6 && pInfo->m_pResHelper) {
            CAuroraLayeredTexture* pLoaded = (CAuroraLayeredTexture*)pInfo->m_pResHelper;
            delete pTexture;
            pTexture = pLoaded;
        }
    }
    if (pTexture) {
        if (!pTexture->Load()) {
            if (!pTexture->m_pRes || pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        } else {
            return pTexture;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E400
CAuroraCompressedTexture* CAuroraInterface::GetCompressedTextureInternal(const CExoString& sName)
{
    CAuroraCompressedTexture* pTexture = new CAuroraCompressedTexture(CResRef(sName), 0);
    if (pTexture && pTexture->m_pRes && pTexture->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2033 && pInfo->m_pResHelper) {
            CAuroraCompressedTexture* pLoaded = (CAuroraCompressedTexture*)pInfo->m_pResHelper;
            delete pTexture;
            pTexture = pLoaded;
        }
    }
    if (pTexture) {
        if (!pTexture->Load()) {
            if (!pTexture->m_pRes || pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        } else {
            return pTexture;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E4F0
CAuroraProcessedTexture* CAuroraInterface::GetProcessedTextureInternal(const CExoString& sName)
{
    CAuroraProcessedTexture* pTexture = new CAuroraProcessedTexture(CResRef(sName), 0);
    if (pTexture && pTexture->m_pRes && pTexture->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3007 && pInfo->m_pResHelper) {
            CAuroraProcessedTexture* pLoaded = (CAuroraProcessedTexture*)pInfo->m_pResHelper;
            delete pTexture;
            pTexture = pLoaded;
        }
    }
    if (pTexture) {
        if (!pTexture->Load()) {
            if (!pTexture->m_pRes || pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        } else {
            return pTexture;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E5E0
CAuroraTXI* CAuroraInterface::GetTXIInternal(const CExoString& sName)
{
    CAuroraTXI* pTXI = new CAuroraTXI(CResRef(sName), 0);
    if (pTXI && pTXI->m_pRes && pTXI->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pTXI->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2022 && pInfo->m_pResHelper) {
            CAuroraTXI* pLoaded = (CAuroraTXI*)pInfo->m_pResHelper;
            delete pTXI;
            pTXI = pLoaded;
        }
    }
    if (pTXI) {
        if (!pTXI->Load()) {
            if (!pTXI->m_pRes || pTXI->m_pRes->GetDemands() < 1) {
                delete pTXI;
            }
        } else {
            return pTXI;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E6D0
CAuroraVisibility* CAuroraInterface::GetVisibilityInternal(const CExoString& sName)
{
    CAuroraVisibility* pVisibility = new CAuroraVisibility(CResRef(sName), 0);
    if (pVisibility && pVisibility->m_pRes && pVisibility->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pVisibility->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3001 && pInfo->m_pResHelper) {
            CAuroraVisibility* pLoaded = (CAuroraVisibility*)pInfo->m_pResHelper;
            delete pVisibility;
            pVisibility = pLoaded;
        }
    }
    if (pVisibility) {
        if (!pVisibility->Load()) {
            if (!pVisibility->m_pRes || pVisibility->m_pRes->GetDemands() < 1) {
                delete pVisibility;
            }
        } else {
            return pVisibility;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E7C0
CAuroraModelX* CAuroraInterface::GetModelXInternal(const CExoString& sName)
{
    CAuroraModelX* pModelX = new CAuroraModelX(CResRef(sName), 0);
    if (pModelX && pModelX->m_pRes && pModelX->m_pRes->GetDemands() > 0) {
        CResHelperInfo* pInfo = pModelX->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3008 && pInfo->m_pResHelper) {
            CAuroraModelX* pLoaded = (CAuroraModelX*)pInfo->m_pResHelper;
            delete pModelX;
            pModelX = pLoaded;
        }
    }
    if (pModelX) {
        if (!pModelX->Load()) {
            if (!pModelX->m_pRes || pModelX->m_pRes->GetDemands() < 1) {
                delete pModelX;
            }
        } else {
            return pModelX;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070E8B0
CAuroraTexture* CAuroraInterface::GetTexture(const CExoString& sName)
{
    return GetTextureInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070E8C0
void CAuroraInterface::ReleaseTextureInternal(CAuroraTexture* pTexture)
{
    if (pTexture->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3 && pInfo->m_pResHelper == pTexture) {
            pTexture->Unload();
            if (pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070E910
CAurora16BitTexture* CAuroraInterface::Get16BitTexture(const CExoString& sName)
{
    return Get16BitTextureInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070E920
void CAuroraInterface::Release16BitTextureInternal(CAurora16BitTexture* pTexture)
{
    if (pTexture->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2059 && pInfo->m_pResHelper == pTexture) {
            pTexture->Unload();
            if (pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070E970
CAuroraLayeredTexture* CAuroraInterface::GetLayeredTexture(const CExoString& sName)
{
    return GetLayeredTextureInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070E980
void CAuroraInterface::ReleaseLayeredTextureInternal(CAuroraLayeredTexture* pTexture)
{
    if (pTexture->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 6 && pInfo->m_pResHelper == pTexture) {
            pTexture->Unload();
            if (pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070E9D0
CAuroraCompressedTexture* CAuroraInterface::GetCompressedTexture(const CExoString& sName)
{
    return GetCompressedTextureInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070E9E0
void CAuroraInterface::ReleaseCompressedTextureInternal(CAuroraCompressedTexture* pTexture)
{
    if (pTexture->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2033 && pInfo->m_pResHelper == pTexture) {
            pTexture->Unload();
            if (pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070EA30
CAuroraProcessedTexture* CAuroraInterface::GetProcessedTexture(const CExoString& sName)
{
    return GetProcessedTextureInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070EA40
void CAuroraInterface::ReleaseProcessedTextureInternal(CAuroraProcessedTexture* pTexture)
{
    if (pTexture->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pTexture->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3007 && pInfo->m_pResHelper == pTexture) {
            pTexture->Unload();
            if (pTexture->m_pRes->GetDemands() < 1) {
                delete pTexture;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070EA90
CAuroraTXI* CAuroraInterface::GetTXI(const CExoString& sName)
{
    return GetTXIInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070EAA0
void CAuroraInterface::ReleaseTXIInternal(CAuroraTXI* pTXI)
{
    if (pTXI->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pTXI->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 2022 && pInfo->m_pResHelper == pTXI) {
            pTXI->Unload();
            if (pTXI->m_pRes->GetDemands() < 1) {
                delete pTXI;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070EAF0
CAuroraVisibility* CAuroraInterface::GetVisibility(const CExoString& sName)
{
    return GetVisibilityInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070EB00
void CAuroraInterface::ReleaseVisibilityInternal(CAuroraVisibility* pVisibility)
{
    if (pVisibility->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pVisibility->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3001 && pInfo->m_pResHelper == pVisibility) {
            pVisibility->Unload();
            if (pVisibility->m_pRes->GetDemands() < 1) {
                delete pVisibility;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0070EB50
CAuroraModelX* CAuroraInterface::GetModelX(const CExoString& sName)
{
    return GetModelXInternal(sName);
}

// FUNCTION: SWKOTOR 0x0070EB60
void CAuroraInterface::ReleaseModelXInternal(CAuroraModelX* pModelX)
{
    if (pModelX->m_pRes->GetDemands() >= 1) {
        CResHelperInfo* pInfo = pModelX->GetPtrToResHelperInfo();
        if (pInfo && pInfo->m_nResType == 3008 && pInfo->m_pResHelper == pModelX) {
            pModelX->Unload();
            if (pModelX->m_pRes->GetDemands() < 1) {
                delete pModelX;
            }
        }
    }
}
