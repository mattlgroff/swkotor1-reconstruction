// InputBinary binary-model relocation (0x004a0060-0x004a11b0).
#include "i/input_binary.h"

#define RELOC(p, base) (*(char**)&(p) += (base))

// FUNCTION: SWKOTOR 0x004a0060
void InputBinary::ResetAnim(MdlNodeAnimMesh* pNode, int nBase, int nRaw)
{
    if (pNode->m_nAnimVerts)
        RELOC(pNode->m_pAnimVerts, nBase);
    else
        pNode->m_pAnimVerts = 0;
    if (pNode->m_nAnimTVerts)
        RELOC(pNode->m_pAnimTVerts, nBase);
    else
        pNode->m_pAnimTVerts = 0;
    if (pNode->m_nAnimNormals)
        RELOC(pNode->m_pAnimNormals, nBase);
    else
        pNode->m_pAnimNormals = 0;
    if (pNode->m_nVertexSets)
        RELOC(pNode->m_pVertexSets, nBase);
    else
        pNode->m_pVertexSets = 0;
    if (pNode->m_nTextureSets)
        RELOC(pNode->m_pTextureSets, nBase);
    else
        pNode->m_pTextureSets = 0;
    pNode->m_pfnRender = Thunk43d450;
    pNode->m_pfnPrepare = Thunk43c320;
    ResetTriMeshParts(pNode, nBase, nRaw);
}

// FUNCTION: SWKOTOR 0x004a0100
void InputBinary::ResetDangly(MdlNodeDanglyMesh* pNode, int nBase, int nRaw)
{
    ResetTriMeshParts(pNode, nBase, nRaw);
    pNode->m_pfnRender = Thunk43bd40;
    pNode->m_pfnPrepare = Thunk43c130;
    if (pNode->m_lConstraints.num)
        RELOC(pNode->m_lConstraints.element, nBase);
    else
        pNode->m_lConstraints.element = 0;
    if (pNode->m_nVertices > 0)
        RELOC(pNode->m_pVertexData, nBase);
}

// FUNCTION: SWKOTOR 0x004a0160
void InputBinary::ResetMdlNodePartsLite(MdlNode* pNode, int nBase, int nRaw)
{
    if (pNode->m_lChildren.num) {
        for (int i = 0; i < pNode->m_lChildren.num; i++)
            ResetMdlNodeLite(pNode->m_lChildren.element[i], nBase, nRaw);
    }
}

// FUNCTION: SWKOTOR 0x004a01b0
void InputBinary::ResetSkin(MdlNodeSkin* pNode, int nBase, int nRaw)
{
    ResetTriMeshParts(pNode, nBase, nRaw);
    pNode->m_pfnRender = ThunkSkinGenVertices;
    pNode->m_pfnPrepare = Thunk43c1a0;
    if (pNode->m_nBoneMap)
        RELOC(pNode->m_pBoneMap, nBase);
    else
        pNode->m_pBoneMap = 0;
    if (pNode->m_nBoneQuats)
        RELOC(pNode->m_pBoneQuats, nBase);
    else
        pNode->m_pBoneQuats = 0;
    if (pNode->m_nBoneTrans)
        RELOC(pNode->m_pBoneTrans, nBase);
    else
        pNode->m_pBoneTrans = 0;
    if (pNode->m_nBoneConst)
        RELOC(pNode->m_pBoneConst, nBase);
    else
        pNode->m_pBoneConst = 0;
}

// FUNCTION: SWKOTOR 0x004a0240
void InputBinary::ResetTriMesh(MdlNodeTriMesh* pNode, int nBase, int nRaw)
{
    pNode->m_pfnRender = Thunk439df0;
    pNode->m_pfnPrepare = Thunk43c090;
    ResetTriMeshParts(pNode, nBase, nRaw);
}

#pragma inline_recursion(on)
// FUNCTION: SWKOTOR 0x004a0260
inline void InputBinary::ResetAABBTree(AABB* pAABB, int nBase, int nRaw)
{
    if (pAABB->m_pLeft) {
        RELOC(pAABB->m_pLeft, nBase);
        ResetAABBTree(pAABB->m_pLeft, nBase, nRaw);
    }
    if (pAABB->m_pRight) {
        RELOC(pAABB->m_pRight, nBase);
        ResetAABBTree(pAABB->m_pRight, nBase, nRaw);
    }
}

// FUNCTION: SWKOTOR 0x004a0460
void InputBinary::ResetLightsaber(MdlNodeLightsaber* pNode, int nBase, int nRaw)
{
    ResetTriMeshParts(pNode, nBase, nRaw);
    pNode->m_pfnRender = Thunk439df0;
    pNode->m_pfnPrepare = Thunk43c090;
    PoolRequest(0, SABER_NUM_VERTS * 32, 2, 0x1012, &pNode->m_nVertexPool);
    PoolRequest(0, SABER_NUM_SEGMENTS * SABER_NUM_STRIPS * 20, 2, 0x1010, &pNode->m_nTexPool);
    RELOC(pNode->m_p19c, nBase);
    RELOC(pNode->m_p1a0, nBase);
    RELOC(pNode->m_p1a4, nBase);
}

// FUNCTION: SWKOTOR 0x004a0510
void InputBinary::ResetLightsaberLite(MdlNodeLightsaber* pNode, int nBase, int nRaw)
{
    ResetTriMeshPartsLite(pNode, nBase, nRaw);
    PoolRequest(0, SABER_NUM_VERTS * 32, 2, 0x1012, &pNode->m_nVertexPool);
    PoolRequest(0, SABER_NUM_SEGMENTS * SABER_NUM_STRIPS * 20, 2, 0x1010, &pNode->m_nTexPool);
}

// FUNCTION: SWKOTOR 0x004a05e0
void InputBinary::ResetLight(MdlNodeLight* pNode, int nBase, int nRaw)
{
    ResetMdlNodeParts(pNode, nBase, nRaw);
    if (pNode->m_lTextureNames.num) {
        RELOC(pNode->m_lTextureNames.element, nBase);
        for (int i = 0; i < pNode->m_lTextureNames.num; i++)
            RELOC(pNode->m_lTextureNames.element[i], nBase);
    } else {
        pNode->m_lTextureNames.element = 0;
    }
    if (pNode->m_lFlareSizes.num)
        RELOC(pNode->m_lFlareSizes.element, nBase);
    else
        pNode->m_lFlareSizes.element = 0;
    if (pNode->m_lFlarePositions.num)
        RELOC(pNode->m_lFlarePositions.element, nBase);
    else
        pNode->m_lFlarePositions.element = 0;
    if (pNode->m_lFlareColors.num)
        RELOC(pNode->m_lFlareColors.element, nBase);
    else
        pNode->m_lFlareColors.element = 0;
    pNode->m_lTextures.InlineSetSize(pNode->m_lTextureNames.num);
    int i = pNode->m_lTextureNames.num;
    while (i) {
        i--;
        pNode->m_lTextures[i] = AurTextureGetReference(pNode->m_lTextureNames[i], 0);
    }
    pNode->AddToFreeList(pNode);
}

// ResetMdlNodePartsLite, inlined here only.
static __forceinline void InlineResetMdlNodePartsLite(InputBinary* pInput, MdlNode* pNode,
                                                     int nBase, int nRaw)
{
    if (pNode->m_lChildren.num) {
        for (int i = 0; i < pNode->m_lChildren.num; i++)
            pInput->ResetMdlNodeLite(pNode->m_lChildren.element[i], nBase, nRaw);
    }
}

// FUNCTION: SWKOTOR 0x004a07b0
void InputBinary::ResetLightLite(MdlNodeLight* pNode, int nBase, int nRaw)
{
    InlineResetMdlNodePartsLite(this, pNode, nBase, nRaw);
    pNode->m_lTextures.SetSize(pNode->m_lTextureNames.num);
    int i = pNode->m_lTextureNames.num;
    while (i) {
        i--;
        pNode->m_lTextures[i] = AurTextureGetReference(pNode->m_lTextureNames[i], 0);
    }
    pNode->AddToFreeList(pNode);
}

// FUNCTION: SWKOTOR 0x004a0900
void InputBinary::ResetMdlNode(MdlNode* pNode, int nBase, int nRaw)
{
    if (pNode->m_nType == NODE_TYPE_AABB) {
        MdlNodeAABB* pAABB = (MdlNodeAABB*)pNode;
        RELOC(pAABB->m_pTree, nBase);
        ResetAABBTree(pAABB->m_pTree, nBase, nRaw);
        pAABB->m_pfnRender = Thunk439df0;
        pAABB->m_pfnPrepare = Thunk43c090;
        ResetTriMeshParts(pAABB, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_ANIM) {
        ResetAnim((MdlNodeAnimMesh*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_DUMMY) {
        ResetMdlNodeParts(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_CAMERA) {
        ResetMdlNodeParts(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_DANGLY) {
        ResetDangly((MdlNodeDanglyMesh*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_EMITTER) {
        ResetMdlNodeParts(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_LIGHT) {
        ResetLight((MdlNodeLight*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_REFERENCE) {
        ResetMdlNodeParts(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_SKIN) {
        ResetSkin((MdlNodeSkin*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_PATCH) {
        ResetMdlNodeParts(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_SABER) {
        ResetLightsaber((MdlNodeLightsaber*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_TRIMESH) {
        ResetTriMesh((MdlNodeTriMesh*)pNode, nBase, nRaw);
    }
}

// FUNCTION: SWKOTOR 0x004a0a90
void InputBinary::ResetMdlNodeLite(MdlNode* pNode, int nBase, int nRaw)
{
    if (pNode->m_nType == NODE_TYPE_AABB) {
        ResetTriMeshPartsLite((MdlNodeTriMesh*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_ANIM) {
        ResetTriMeshPartsLite((MdlNodeTriMesh*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_DUMMY) {
        ResetMdlNodePartsLite(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_CAMERA) {
        ResetCameraLite(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_DANGLY) {
        ResetTriMeshPartsLite((MdlNodeTriMesh*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_EMITTER) {
        ResetEmitterLite(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_LIGHT) {
        ResetLightLite((MdlNodeLight*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_REFERENCE) {
        ResetReferenceLite(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_SKIN) {
        ResetTriMeshPartsLite((MdlNodeTriMesh*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_PATCH) {
        ResetPatchLite(pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_SABER) {
        ResetLightsaberLite((MdlNodeLightsaber*)pNode, nBase, nRaw);
    } else if (pNode->m_nType == NODE_TYPE_TRIMESH) {
        ResetTriMeshPartsLite((MdlNodeTriMesh*)pNode, nBase, nRaw);
    }
}

// FUNCTION: SWKOTOR 0x004a0b60
void InputBinary::ResetMdlNodeParts(MdlNode* pNode, int nBase, int nRaw)
{
    RELOC(pNode->m_pTree, nBase);
    if (pNode->m_pParent)
        RELOC(pNode->m_pParent, nBase);
    else
        pNode->m_pParent = 0;
    if (pNode->m_lChildren.num) {
        RELOC(pNode->m_lChildren.element, nBase);
        for (int i = 0; i < pNode->m_lChildren.num; i++) {
            RELOC(pNode->m_lChildren.element[i], nBase);
            ResetMdlNode(pNode->m_lChildren.element[i], nBase, nRaw);
        }
    } else {
        pNode->m_lChildren.element = 0;
    }
    if (pNode->m_lControllerKeys.num)
        RELOC(pNode->m_lControllerKeys.element, nBase);
    else
        pNode->m_lControllerKeys.element = 0;
    if (pNode->m_lControllerData.num)
        RELOC(pNode->m_lControllerData.element, nBase);
    else
        pNode->m_lControllerData.element = 0;
}

// FUNCTION: SWKOTOR 0x004a0c00
void InputBinary::ResetTriMeshParts(MdlNodeTriMesh* pNode, int nBase, int nRaw)
{
    ResetMdlNodeParts(pNode, nBase, nRaw);
    if (pNode->m_nVertices > 0)
        RELOC(pNode->m_pVertices, nBase);
    else
        pNode->m_pVertices = 0;
    if (pNode->m_lFaces.num)
        RELOC(pNode->m_lFaces.element, nBase);
    else
        pNode->m_lFaces.element = 0;
    if (pNode->m_lIndexLists.num)
        RELOC(pNode->m_lIndexLists.element, nBase);
    else
        pNode->m_lIndexLists.element = 0;
    if (pNode->m_lIndexCounts.num)
        RELOC(pNode->m_lIndexCounts.element, nBase);
    else
        pNode->m_lIndexCounts.element = 0;
    int i;
    for (i = 0; i < pNode->m_lIndexLists.num; i++)
        RELOC(pNode->m_lIndexLists.element[i], nBase);
    if (pNode->m_lIndexOffsets.num)
        RELOC(pNode->m_lIndexOffsets.element, nBase);
    else
        pNode->m_lIndexOffsets.element = 0;
    if (pNode->m_lIndexPools.num)
        RELOC(pNode->m_lIndexPools.element, nBase);
    else
        pNode->m_lIndexPools.element = 0;
    if (pNode->m_lIndexData.num)
        RELOC(pNode->m_lIndexData.element, nBase);
    else
        pNode->m_lIndexData.element = 0;
    for (i = 0; i < pNode->m_lIndexPools.num; i++) {
        char* pSrc = (char*)pNode->m_lIndexData.element[i];
        PoolRequest(1, pNode->m_lIndexCounts.element[i] * 2, 2, 0, &pNode->m_lIndexPools.element[i]);
        void* pDst = PoolLock(pNode->m_lIndexPools.element[i], 0, 0, 1);
        memcpy(pDst, pSrc + nBase, pNode->m_lIndexCounts.element[i] * 2);
        PoolUnlock(pNode->m_lIndexPools.element[i]);
        pNode->AddToFreeTokenList((char*)pNode->m_lIndexPools.element[i]);
    }
    if (pNode->m_nVertexPool != -1) {
        char* pSrc = (char*)pNode->m_nVertexData;
        PoolRequest(1, pNode->m_nVertexCount * 2, 2, 0, &pNode->m_nVertexPool);
        void* pDst = PoolLock(pNode->m_nVertexPool, 0, 0, 1);
        memcpy(pDst, pSrc + nBase, pNode->m_nVertexCount * 2);
        PoolUnlock(pNode->m_nVertexPool);
        pNode->AddToFreeTokenList((char*)pNode->m_nVertexPool);
    }
    g_nMeshFaces += pNode->m_nVertices;
    g_nMeshVertBytes += pNode->m_nVertexSize * pNode->m_nVertices;
}

// FUNCTION: SWKOTOR 0x004a0e30
void InputBinary::ResetTriMeshPartsLite(MdlNodeTriMesh* pNode, int nBase, int nRaw)
{
    ResetMdlNodePartsLite(pNode, nBase, nRaw);
    int i;
    for (i = 0; i < pNode->m_lIndexPools.num; i++) {
        char* pSrc = (char*)pNode->m_lIndexData.element[i];
        PoolRequest(1, pNode->m_lIndexCounts.element[i] * 2, 2, 0, &pNode->m_lIndexPools.element[i]);
        void* pDst = PoolLock(pNode->m_lIndexPools.element[i], 0, 0, 1);
        memcpy(pDst, pSrc + nBase, pNode->m_lIndexCounts.element[i] * 2);
        PoolUnlock(pNode->m_lIndexPools.element[i]);
        pNode->AddToFreeTokenList((char*)pNode->m_lIndexPools.element[i]);
    }
    if (pNode->m_nVertexPool != -1) {
        char* pSrc = (char*)pNode->m_nVertexData;
        PoolRequest(1, pNode->m_nVertexCount * 2, 2, 0, &pNode->m_nVertexPool);
        void* pDst = PoolLock(pNode->m_nVertexPool, 0, 0, 1);
        memcpy(pDst, pSrc + nBase, pNode->m_nVertexCount * 2);
        PoolUnlock(pNode->m_nVertexPool);
        pNode->AddToFreeTokenList((char*)pNode->m_nVertexPool);
    }
    g_nMeshFaces += pNode->m_nVertices;
    g_nMeshVertBytes += pNode->m_nVertexSize * pNode->m_nVertices;
}

// FUNCTION: SWKOTOR 0x004a0fb0
void InputBinary::ResetAnimation(Animation* pAnim, int nBase, int nRaw)
{
    pAnim->m_pModel = (Model*)nBase;
    if (pAnim->m_lEvents.num)
        RELOC(pAnim->m_lEvents.element, nBase);
    else
        pAnim->m_lEvents.element = 0;
    RELOC(pAnim->m_pRoot, nBase);
    ResetMdlNode(pAnim->m_pRoot, nBase, nRaw);
    ResolveAnimationNodes(pAnim->m_pRoot, 1);
    g_nModelMemory += 0x8c;
    g_nModelMemory += pAnim->m_lEvents.num * 36;
    pAnim->m_pfnParse = AnimationParseField;
    pAnim->m_pfnField = MaxTreeParseField;
}

// FUNCTION: SWKOTOR 0x004a1030
Model* InputBinary::Reset(Model* pModel, int nRaw, void* pResource)
{
    pModel->m_pResource = pResource;
    pModel->m_nType = (unsigned char)(pModel->GetType() | 0x80);
    pModel->m_pSuperModel = FindModel(pModel->m_szSuperModel);
    if (pModel->m_pSuperModel)
        pModel->m_pSuperModel->AddRef(pModel);
    pModel->ResetFreeLists();
    char* pSrc = (char*)pModel->m_nVertexPool;
    PoolRequest(0, pModel->m_nVertexPoolSize, 2, 0, &pModel->m_nVertexPool);
    void* pDst = PoolLock(pModel->m_nVertexPool, 0, 0, 1);
    memcpy(pDst, pSrc + nRaw, pModel->m_nVertexPoolSize);
    PoolUnlock(pModel->m_nVertexPool);
    pModel->AddToFreeTokenList((char*)pModel->m_nVertexPool);
    int nBase = (int)pModel;
    if (pModel->m_lNames.num)
        RELOC(pModel->m_lNames.element, nBase);
    else
        pModel->m_lNames.element = 0;
    int i;
    for (i = 0; i < pModel->m_lNames.num; i++)
        RELOC(pModel->m_lNames.element[i], nBase);
    if (pModel->m_lAnimations.num)
        RELOC(pModel->m_lAnimations.element, nBase);
    else
        pModel->m_lAnimations.element = 0;
    for (i = 0; i < pModel->m_lAnimations.num; i++) {
        RELOC(pModel->m_lAnimations.element[i], nBase);
        ResetAnimation(pModel->m_lAnimations.element[i], nBase, nRaw);
    }
    if (pModel->m_pa8)
        RELOC(pModel->m_pa8, nBase);
    RELOC(pModel->m_pRoot, nBase);
    ResetMdlNode(pModel->m_pRoot, nBase, nRaw);
    pModel->m_pfnParse = ModelParseHeader;
    pModel->m_pfnField = ModelParseField;
    InsertModel(pModel);
    return pModel;
}

// FUNCTION: SWKOTOR 0x004a11b0
Model* InputBinary::ResetLite(Model* pModel, int nRaw, void* pResource)
{
    pModel->m_pSuperModel = FindModel(pModel->m_szSuperModel);
    if (pModel->m_pSuperModel)
        pModel->m_pSuperModel->AddRef(pModel);
    pModel->ResetFreeLists();
    for (int i = 0; i < pModel->m_lAnimations.num; i++) {
        Animation* pAnim = pModel->m_lAnimations.element[i];
        ResetMdlNodeLite(pAnim->m_pRoot, (int)pModel, nRaw);
        ResolveAnimationNodes(pAnim->m_pRoot, 1);
        g_nModelMemory += 0x8c;
        g_nModelMemory += pAnim->m_lEvents.num * 36;
    }
    ResetMdlNodeLite(pModel->m_pRoot, (int)pModel, nRaw);
    InsertModel(pModel);
    return pModel;
}
