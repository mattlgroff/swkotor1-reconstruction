// CSWCAnimBaseHead: head model on top of a virtual CSWCAnimBaseTW base
// (0x0069B030-0x0069BC80). Every override handles part 0xfe (the head)
// and then forwards to the CSWCAnimBaseTW implementation.

#include "k/sw_c_anim_base_head.h"

// FUNCTION: SWKOTOR 0x0069B030
CSWCAnimBaseHead::~CSWCAnimBaseHead()
{
    UnloadModel();
}

// FUNCTION: SWKOTOR 0x0069B060
CAurObject* CSWCAnimBaseHead::GetModel(unsigned char nPart)
{
    switch (nPart) {
    case 0xfe:
        return m_pModelHead;
    case 0xff:
        return CSWCAnimBaseTW::GetModel(0xff);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069B0A0
int CSWCAnimBaseHead::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    char szModel[20];
    switch (nPart) {
    case 0xff:
        if (!CSWCAnimBaseTW::LoadModel(cResRef, 0xff)) {
            return 0;
        }
        if (m_pModelHead) {
            m_pModelHead->AttachToObject(m_pModel, "HEADHOOK", 0);
        }
        return 1;
    case 0xfc:
    case 0xfd:
        return CSWCAnimBaseTW::LoadModel(cResRef, nPart) != 0;
    case 0xfe:
        cResRef.CopyToString(szModel);
        m_pModelHead = NewCAurObject(szModel, "head", 0, 0);
        if (!m_pModelHead) {
            return 0;
        }
        m_cReplaceTextureHead = cResRef;
        m_bReplaceTextureIndexedHead = 0;
        m_pModelHead->AttachToObject(m_pModel, "HEADHOOK", 0);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069B1C0 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHead@@UAEHABVCResRef@@0H@Z
int CSWCAnimBaseHead::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore)
{
    char szOld[20];
    char szNew[20];
    cOld.CopyToString(szOld);
    cNew.CopyToString(szNew);
    _strlwr(szNew);
    _strlwr(szOld);
    if (m_pModelHead) {
        m_pModelHead->ReplaceTexture(szOld, szNew);
    }
    if (bStore) {
        m_cReplaceTextureHead = cNew;
        m_bReplaceTextureIndexedHead = 0;
    }
    return CSWCAnimBaseTW::ReplaceTexture(cOld, cNew, bStore);
}

// FUNCTION: SWKOTOR 0x0069B250 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHead@@UAEHEABVCResRef@@0H@Z
int CSWCAnimBaseHead::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore)
{
    char szOld[20];
    char szNew[20];
    if (nPart == 0xfe) {
        if (!m_pModelHead) {
            return 0;
        }
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModelHead->ReplaceTexture(szOld, szNew);
        if (bStore) {
            m_cReplaceTextureHead = cNew;
            m_bReplaceTextureIndexedHead = 0;
        }
        return 1;
    }
    return CSWCAnimBaseTW::ReplaceTexture(nPart, cOld, cNew, bStore);
}

// FUNCTION: SWKOTOR 0x0069B310 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHead@@UAEHEABVCResRef@@0HPAGHH@Z
int CSWCAnimBaseHead::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    char szOld[20];
    char szNew[20];
    if (nPart == 0xfe) {
        if (!m_pModelHead) {
            return 0;
        }
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModelHead->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
        if (bStore) {
            m_cReplaceTextureHead = cNew;
            m_bReplaceTextureIndexedHead = 1;
            m_nTextureCountHead = (unsigned char)nCount;
            m_nTextureStartHead = (unsigned char)nStart;
            memcpy(m_aTextureIndicesHead, pIndices + nStart, nCount * 2);
        }
        return 1;
    }
    return CSWCAnimBaseTW::ReplaceTexture(nPart, cOld, cNew, nCount, pIndices, bStore, nStart);
}

// FUNCTION: SWKOTOR 0x0069B410 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHead@@UAEHABVCResRef@@0HPAGHH@Z
int CSWCAnimBaseHead::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    char szOld[20];
    char szNew[20];
    cOld.CopyToString(szOld);
    cNew.CopyToString(szNew);
    _strlwr(szOld);
    _strlwr(szNew);
    // The original tests the head model the wrong way round.
    if (!m_pModelHead) {
        m_pModelHead->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
    }
    if (bStore) {
        m_cReplaceTextureHead = cNew;
        m_bReplaceTextureIndexedHead = 1;
        m_nTextureCountHead = (unsigned char)nCount;
        m_nTextureStartHead = (unsigned char)nStart;
        memcpy(m_aTextureIndicesHead, pIndices + nStart, nCount * 2);
    }
    return CSWCAnimBaseTW::ReplaceTexture(cOld, cNew, nCount, pIndices, bStore, nStart);
}

// FUNCTION: SWKOTOR 0x0069B4F0
int CSWCAnimBaseHead::RestoreTexture()
{
    if (m_pModelHead) {
        if (m_bReplaceTextureIndexedHead) {
            CResRef cEmpty("");
            ReplaceTexture(0xfe, cEmpty, m_cReplaceTextureHead, m_nTextureCountHead, m_aTextureIndicesHead, 0, m_nTextureStartHead);
        }
        else {
            m_pModelHead->RestoreTextures();
        }
    }
    return CSWCAnimBaseTW::RestoreTexture();
}

// FUNCTION: SWKOTOR 0x0069B570
int CSWCAnimBaseHead::RestoreTexturePart(unsigned char nPart)
{
    if (nPart == 0xfe) {
        if (m_pModelHead) {
            if (m_bReplaceTextureIndexedHead) {
                CResRef cEmpty("");
                ReplaceTexture(0xfe, cEmpty, m_cReplaceTextureHead, m_nTextureCountHead, m_aTextureIndicesHead, 0, m_nTextureStartHead);
                return 1;
            }
            m_pModelHead->RestoreTextures();
            return 1;
        }
        return 0;
    }
    return CSWCAnimBaseTW::RestoreTexturePart(nPart);
}

// FUNCTION: SWKOTOR 0x0069B610
void CSWCAnimBaseHead::UnloadModel()
{
    if (m_pModelHead) {
        m_pModelHead->AttachToObject(0, 0, 0);
        delete m_pModelHead;
        m_pModelHead = 0;
    }
    CSWCAnimBaseTW::UnloadModel();
}

// FUNCTION: SWKOTOR 0x0069B650
void CSWCAnimBaseHead::UnloadModelPart(unsigned char nPart)
{
    if (m_pModelHead && nPart == 0xfe) {
        m_pModelHead->AttachToObject(0, 0, 0);
        delete m_pModelHead;
        m_pModelHead = 0;
        return;
    }
    CSWCAnimBaseTW::UnloadModelPart(nPart);
}

// FUNCTION: SWKOTOR 0x0069B6A0
int CSWCAnimBaseHead::SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap)
{
    char szEnvMap[20];
    if (!m_pModelHead) {
        return 0;
    }
    if (bEnable) {
        cEnvMap.CopyToString(szEnvMap);
        m_pModelHead->SetEnvMap(szEnvMap);
    }
    else {
        m_pModelHead->SetEnvMap("");
    }
    if (bStore) {
        m_cEnvMapHead = cEnvMap;
        m_bEnvMapHead = bEnable;
    }
    return CSWCAnimBaseTW::SetEnvMap(bEnable, bStore, cEnvMap);
}

// FUNCTION: SWKOTOR 0x0069B720
int CSWCAnimBaseHead::SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap)
{
    char szEnvMap[20];
    if (nPart == 0xfe) {
        if (!m_pModelHead) {
            return 0;
        }
        if (bEnable) {
            cEnvMap.CopyToString(szEnvMap);
            m_pModelHead->SetEnvMap(szEnvMap);
        }
        else {
            m_pModelHead->SetEnvMap("");
        }
        if (bStore) {
            m_cEnvMapHead = cEnvMap;
            m_bEnvMapHead = bEnable;
        }
        return 1;
    }
    return CSWCAnimBaseTW::SetEnvMapPart(nPart, bEnable, bStore, cEnvMap);
}

// FUNCTION: SWKOTOR 0x0069B7C0
int CSWCAnimBaseHead::RestoreEnvMap()
{
    char szEnvMap[20];
    if (!m_pModelHead) {
        return 0;
    }
    if (m_bEnvMapHead) {
        m_cEnvMapHead.CopyToString(szEnvMap);
        m_pModelHead->SetEnvMap(szEnvMap);
    }
    else {
        m_pModelHead->SetEnvMap("");
    }
    return CSWCAnimBaseTW::RestoreEnvMap();
}

// FUNCTION: SWKOTOR 0x0069B810
void CSWCAnimBaseHead::ModelVC0()
{
    if (m_pModelHead) {
        m_pModelHead->vC0();
    }
    CSWCAnimBaseTW::ModelVC0();
}

// FUNCTION: SWKOTOR 0x0069B840
void CSWCAnimBaseHead::ModelVBC()
{
    if (m_pModelHead) {
        m_pModelHead->vBC();
    }
    CSWCAnimBaseTW::ModelVBC();
}

// FUNCTION: SWKOTOR 0x0069B870
int CSWCAnimBaseHead::SetSkin(const CResRef& cSkin, int n)
{
    if (!m_pModelHead) {
        return 0;
    }
    char szSkin[20];
    cSkin.CopyToString(szSkin);
    _strlwr(szSkin);
    m_pModelHead->v150(szSkin, n);
    return CSWCAnimBaseTW::SetSkin(cSkin, n);
}

// FUNCTION: SWKOTOR 0x0069B8E0
int CSWCAnimBaseHead::ClearSkin()
{
    if (!m_pModelHead) {
        return 0;
    }
    m_pModelHead->v154();
    return CSWCAnimBaseTW::ClearSkin();
}

// FUNCTION: SWKOTOR 0x0069B910
int CSWCAnimBaseHead::SetFlag144(int b)
{
    if (!m_pModelHead) {
        return 0;
    }
    m_pModelHead->v144(b != 0);
    return CSWCAnimBaseTW::SetFlag144(b);
}

// FUNCTION: SWKOTOR 0x0069B950
void CSWCAnimBaseHead::ModelVF8()
{
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidHeadItem);
    if (m_pModelHead) {
        m_pModelHead->vF8();
    }
    if (pItem && pItem->GetPartModel(0xff, 1)) {
        pItem->GetPartModel(0xff, 1)->vF8();
    }
    CSWCAnimBaseTW::ModelVF8();
}

// FUNCTION: SWKOTOR 0x0069B9C0
void CSWCAnimBaseHead::ModelVFC()
{
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidHeadItem);
    if (m_pModelHead) {
        m_pModelHead->vFC();
    }
    if (pItem && pItem->GetPartModel(0xff, 1)) {
        pItem->GetPartModel(0xff, 1)->vFC();
    }
    CSWCAnimBaseTW::ModelVFC();
}

// FUNCTION: SWKOTOR 0x00699AD0
void CSWCAnimBaseHead::SetHeadItem(unsigned long oid)
{
    m_oidHeadItem = oid;
}

// FUNCTION: SWKOTOR 0x0069BB80
CSWCAnimBaseHead::CSWCAnimBaseHead()
{
    m_pModelHead = 0;
    m_nAnimType = 1;
    m_cReplaceTextureHead = "";
    m_bReplaceTextureIndexedHead = 0;
    m_nTextureCountHead = 0;
    m_nTextureStartHead = 0;
    memset(m_aTextureIndicesHead, 0, sizeof(m_aTextureIndicesHead));
    m_cEnvMapHead = "";
    m_bEnvMapHead = 0;
    m_oidHeadItem = 0x7f000000;
}

// FUNCTION: SWKOTOR 0x0069BC80 SYMBOL: ??_GCSWCAnimBaseHead@@UAEPAXI@Z
