// CSWCAnimBaseHeadWield: head model plus wielded items, built from the
// virtual bases CSWCAnimBaseHead and CSWCAnimBaseWield over one shared
// CSWCAnimBaseTW (0x00698EC0-0x00699D70). The overrides pick the Head or
// Wield implementation by part, or run both.

#include "k/sw_c_anim_base_head_wield.h"

// (also matches; annotated in matching/src/pilot3/animbase.cpp) 0x00698EC0
CSWCAnimBaseHeadWield::CSWCAnimBaseHeadWield()
{
    m_nAnimType = 3;
}

// FUNCTION: SWKOTOR 0x00698F90
CAurObject* CSWCAnimBaseHeadWield::GetModel(unsigned char nPart)
{
    switch (nPart) {
    case 0xf4:
    case 0xf5:
    case 0xf6:
    case 0xf7:
    case 0xf8:
    case 0xf9:
    case 0xfa:
    case 0xfb:
        return CSWCAnimBaseWield::GetModel(nPart);
    case 0xfe:
        return CSWCAnimBaseHead::GetModel(nPart);
    case 0xff:
        return CSWCAnimBaseWield::GetModel(nPart);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699000
int CSWCAnimBaseHeadWield::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    if (nPart == 0xfe) {
        if (!CSWCAnimBaseHead::LoadModel(cResRef, 0xfe)) {
            return 0;
        }
        AttachWieldedItems();
        return 1;
    }
    if (CSWCAnimBaseWield::LoadModel(cResRef, nPart)) {
        AttachWieldedItems();
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699080 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@UAEHEABVCResRef@@0H@Z
int CSWCAnimBaseHeadWield::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore)
{
    switch (nPart) {
    case 0xfe:
        return CSWCAnimBaseHead::ReplaceTexture(nPart, cOld, cNew, bStore);
    case 0xff:
        return CSWCAnimBaseTW::ReplaceTexture(nPart, cOld, cNew, bStore);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006990C0 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@UAEHABVCResRef@@0H@Z
int CSWCAnimBaseHeadWield::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore)
{
    if (CSWCAnimBaseTW::ReplaceTexture(cOld, cNew, bStore) && CSWCAnimBaseHead::ReplaceTexture(cOld, cNew, bStore)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699120 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@UAEHEABVCResRef@@0HPAGHH@Z
int CSWCAnimBaseHeadWield::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    switch (nPart) {
    case 0xfe:
        // The head result is dropped: the original returns 0 here.
        CSWCAnimBaseHead::ReplaceTexture(nPart, cOld, cNew, nCount, pIndices, bStore, nStart);
        break;
    case 0xff:
        return CSWCAnimBaseTW::ReplaceTexture(nPart, cOld, cNew, nCount, pIndices, bStore, nStart);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699180 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@UAEHABVCResRef@@0HPAGHH@Z
int CSWCAnimBaseHeadWield::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    if (CSWCAnimBaseTW::ReplaceTexture(cOld, cNew, nCount, pIndices, bStore, nStart) && CSWCAnimBaseHead::ReplaceTexture(cOld, cNew, nCount, pIndices, bStore, nStart)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699200
int CSWCAnimBaseHeadWield::RestoreTexture()
{
    return CSWCAnimBaseHead::RestoreTexture();
}

// FUNCTION: SWKOTOR 0x00699210
int CSWCAnimBaseHeadWield::RestoreTexturePart(unsigned char nPart)
{
    return CSWCAnimBaseHead::RestoreTexturePart(nPart);
}

// FUNCTION: SWKOTOR 0x00699220
void CSWCAnimBaseHeadWield::UnloadModel()
{
    CSWCAnimBaseTW::UnloadModel();
    CSWCAnimBaseHead::UnloadModel();
}

// FUNCTION: SWKOTOR 0x00699250
void CSWCAnimBaseHeadWield::UnloadModelPart(unsigned char nPart)
{
    if (nPart == 0xff) {
        CSWCAnimBaseTW::UnloadModelPart(0xff);
    }
    else {
        CSWCAnimBaseHead::UnloadModelPart(nPart);
    }
}

// FUNCTION: SWKOTOR 0x00699290
int CSWCAnimBaseHeadWield::SetWieldedItem(unsigned long oid, const CExoString& sNode, unsigned char nHand, float fAlpha)
{
    return CSWCAnimBaseWield::SetWieldedItem(oid, sNode, nHand, fAlpha);
}

// FUNCTION: SWKOTOR 0x006992A0
void CSWCAnimBaseHeadWield::SetWieldedModels(const CResRef& cRight, const CResRef& cLeft)
{
    CSWCAnimBaseWield::SetWieldedModels(cRight, cLeft);
}

// FUNCTION: SWKOTOR 0x006992B0
void CSWCAnimBaseHeadWield::ShowWieldedItems()
{
    CSWCAnimBaseWield::ShowWieldedItems();
}

// FUNCTION: SWKOTOR 0x006992C0
void CSWCAnimBaseHeadWield::SetCheckItems(int b)
{
    CSWCAnimBaseWield::SetCheckItems(b);
}

// FUNCTION: SWKOTOR 0x006992D0
int CSWCAnimBaseHeadWield::SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap)
{
    if (CSWCAnimBaseTW::SetEnvMap(bEnable, bStore, cEnvMap) && CSWCAnimBaseHead::SetEnvMap(bEnable, bStore, cEnvMap)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699330
int CSWCAnimBaseHeadWield::SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap)
{
    if (nPart == 0xfe) {
        return CSWCAnimBaseHead::SetEnvMapPart(0xfe, bEnable, bStore, cEnvMap);
    }
    return CSWCAnimBaseTW::SetEnvMapPart(nPart, bEnable, bStore, cEnvMap);
}

// FUNCTION: SWKOTOR 0x00699370
int CSWCAnimBaseHeadWield::RestoreEnvMap()
{
    if (CSWCAnimBaseTW::RestoreEnvMap() && CSWCAnimBaseHead::RestoreEnvMap()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006993B0
int CSWCAnimBaseHeadWield::PlayAnim(int nAnim, float fSpeed, int a, int b)
{
    if (CSWCAnimBaseWield::PlayAnimOnModel(m_pModel, nAnim, fSpeed, a, b) && CSWCAnimBaseTW::PlayAnimOnModel(m_pModelHead, nAnim, fSpeed, a, b)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699420
int CSWCAnimBaseHeadWield::PlayAnimSet(int a, int b)
{
    if (!m_pModel && !m_pModelHead) {
        return 0;
    }
    if (PlayAnimSetOnModel(m_pModel, a, b) && PlayAnimSetOnModel(m_pModelHead, a, b)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006994A0
int CSWCAnimBaseHeadWield::SetTalk(int a, int b, int c, int d)
{
    if (a && b) {
        if (m_pModel) {
            m_pModel->SetLight("talk", a, b, c, d);
        }
        if (m_pModelHead) {
            m_pModelHead->SetLight("talk", a, b, c, d);
        }
    }
    else {
        if (m_pModel) {
            m_pModel->ClearLight("talk");
        }
        if (m_pModelHead) {
            m_pModelHead->ClearLight("talk");
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00699560
void CSWCAnimBaseHeadWield::SetFlag130(int b)
{
    if (b) {
        if (m_pModel) {
            m_pModel->v130();
        }
        if (m_pModelHead) {
            m_pModelHead->v130();
        }
    }
    else {
        if (m_pModel) {
            m_pModel->v134();
        }
        if (m_pModelHead) {
            m_pModelHead->v134();
        }
    }
}

// FUNCTION: SWKOTOR 0x006995D0
void CSWCAnimBaseHeadWield::SetFlag138(int b)
{
    if (b) {
        if (m_pModel) {
            m_pModel->v138();
        }
        if (m_pModelHead) {
            m_pModelHead->v138();
        }
    }
    else {
        if (m_pModel) {
            m_pModel->v13C();
        }
        if (m_pModelHead) {
            m_pModelHead->v13C();
        }
    }
}

// FUNCTION: SWKOTOR 0x00699640
int CSWCAnimBaseHeadWield::PlayAnimPart(unsigned char nPart, int nAnim, float fSpeed, int a, int b)
{
    if (nPart == 0xff) {
        return CSWCAnimBaseWield::PlayAnimOnPart(m_pModel, 0xff, nAnim, fSpeed, a, b);
    }
    return CSWCAnimBaseTW::PlayAnimOnPart(m_pModelHead, nPart, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x006996B0
int CSWCAnimBaseHeadWield::PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b)
{
    return CSWCAnimBase::PlayAnimOnModel(pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x006996C0
int CSWCAnimBaseHeadWield::PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b)
{
    return CSWCAnimBase::PlayAnimOnPart(pModel, nPart, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x006996D0
int CSWCAnimBaseHeadWield::SetModelV128Self(int n)
{
    if (SetModelV128(m_pModel, n) && SetModelV128(m_pModelHead, n)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699730
int CSWCAnimBaseHeadWield::SetModelV12CSelf(int n)
{
    if (SetModelV12C(m_pModel, n) && SetModelV12C(m_pModelHead, n)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00699790
void CSWCAnimBaseHeadWield::ModelVC0()
{
    CSWCAnimBaseHead::ModelVC0();
    CSWCAnimBaseWield::ModelVC0();
}

// FUNCTION: SWKOTOR 0x006997C0
void CSWCAnimBaseHeadWield::ModelVBC()
{
    CSWCAnimBaseHead::ModelVBC();
    CSWCAnimBaseWield::ModelVBC();
}

// FUNCTION: SWKOTOR 0x006997F0
void CSWCAnimBaseHeadWield::UpdateSound(int bOn)
{
    CSWCAnimBaseWield::UpdateSound(bOn);
}

// FUNCTION: SWKOTOR 0x00699800
int CSWCAnimBaseHeadWield::SetSkin(const CResRef& cSkin, int n)
{
    return CSWCAnimBaseHead::SetSkin(cSkin, n);
}

// FUNCTION: SWKOTOR 0x00699810
int CSWCAnimBaseHeadWield::ClearSkin()
{
    return CSWCAnimBaseHead::ClearSkin();
}

// FUNCTION: SWKOTOR 0x00699820
int CSWCAnimBaseHeadWield::SetFlag144(int b)
{
    return CSWCAnimBaseHead::SetFlag144(b);
}

// FUNCTION: SWKOTOR 0x00699830
void CSWCAnimBaseHeadWield::ModelVF8()
{
    CSWCAnimBaseHead::ModelVF8();
    CSWCAnimBaseWield::ModelVF8();
}

// FUNCTION: SWKOTOR 0x00699860
void CSWCAnimBaseHeadWield::ModelVFC()
{
    CSWCAnimBaseHead::ModelVFC();
    CSWCAnimBaseWield::ModelVFC();
}

// FUNCTION: SWKOTOR 0x00699AE0
void CSWCAnimBaseHeadWield::DetachHeadItem()
{
    CExoString sUnused;
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidHeadItem);
    if (pItem) {
        CAurObject* pModel = pItem->GetPartModel(0xff, 1);
        if (pModel) {
            pModel->AttachToObject(0, 0, 0);
            pModel->SetVisible(0);
        }
    }
    m_n4C = 1;
}

// FUNCTION: SWKOTOR 0x00699B90
void CSWCAnimBaseHeadWield::AttachHeadItem()
{
    if (m_n4C) {
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidHeadItem);
        if (pItem) {
            CAurObject* pModel = pItem->GetPartModel(0xff, 1);
            if (pModel) {
                pModel->AttachToObject(m_pModelHead, "GoggleHook", 0);
            }
        }
        RefreshHilite();
        m_n4C = 0;
    }
}

// FUNCTION: SWKOTOR 0x00699C10
int CSWCAnimBaseHeadWield::GetBoundingExtent(CSWGuiExtent* pExtent)
{
    if (m_pModelHead) {
        CSWGuiExtent cHead;
        CSWGuiExtent cBody;
        cHead.height = 0;
        cHead.width = 0;
        cHead.top = 0;
        cHead.left = 0;
        cBody.height = 0;
        cBody.width = 0;
        cBody.top = 0;
        cBody.left = 0;
        if (GetBoundingRect(m_pModelHead, &cHead) && CSWCAnimBase::GetBoundingExtent(&cBody)) {
            *pExtent = UnionExtents(cHead, cBody);
            return 1;
        }
        return 0;
    }
    return CSWCAnimBase::GetBoundingExtent(pExtent);
}

// FUNCTION: SWKOTOR 0x00699CE0
void CSWCAnimBaseHeadWield::SetHeadItem(unsigned long oid)
{
    m_oidHeadItem = oid;
}

CSWCAnimBaseHeadWield::~CSWCAnimBaseHeadWield()
{
    UnloadModel();
}

// FUNCTION: SWKOTOR 0x00699D40 SYMBOL: ??_GCSWCAnimBaseHeadWield@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00699D70 SYMBOL: ??_DCSWCAnimBaseHeadWield@@QAEXXZ

// vtordisp{4} thunks (compiler generated for the overrides seen through
// the virtual CSWCAnimBase base: `sub ecx, [ecx-4]; jmp`).
// FUNCTION: SWKOTOR 0x00699890 SYMBOL: ??_ECSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: SWKOTOR 0x006998A0 SYMBOL: ?GetModel@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEPAVCAurObject@@E@Z
// FUNCTION: SWKOTOR 0x006998B0 SYMBOL: ?LoadModel@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHABVCResRef@@E@Z
// FUNCTION: SWKOTOR 0x006998C0 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHABVCResRef@@0HPAGHH@Z
// FUNCTION: SWKOTOR 0x006998D0 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHEABVCResRef@@0HPAGHH@Z
// FUNCTION: SWKOTOR 0x006998E0 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHABVCResRef@@0H@Z
// FUNCTION: SWKOTOR 0x006998F0 SYMBOL: ?ReplaceTexture@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHEABVCResRef@@0H@Z
// FUNCTION: SWKOTOR 0x00699900 SYMBOL: ?RestoreTexturePart@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHE@Z
// FUNCTION: SWKOTOR 0x00699910 SYMBOL: ?RestoreTexture@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHXZ
// FUNCTION: SWKOTOR 0x00699920 SYMBOL: ?SetSkin@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x00699930 SYMBOL: ?ClearSkin@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHXZ
// FUNCTION: SWKOTOR 0x00699940 SYMBOL: ?SetFlag144@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHH@Z
// FUNCTION: SWKOTOR 0x00699950 SYMBOL: ?PlayAnimSet@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHHH@Z
// FUNCTION: SWKOTOR 0x00699960 SYMBOL: ?PlayAnim@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHHMHH@Z
// FUNCTION: SWKOTOR 0x00699970 SYMBOL: ?PlayAnimPart@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHEHMHH@Z
// FUNCTION: SWKOTOR 0x00699980 SYMBOL: ?SetTalk@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHHHHH@Z
// FUNCTION: SWKOTOR 0x00699990 SYMBOL: ?SetFlag130@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXH@Z
// FUNCTION: SWKOTOR 0x006999A0 SYMBOL: ?SetFlag138@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXH@Z
// FUNCTION: SWKOTOR 0x006999B0 SYMBOL: ?UnloadModelPart@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXE@Z
// FUNCTION: SWKOTOR 0x006999C0 SYMBOL: ?UnloadModel@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x006999D0 SYMBOL: ?SetWieldedItem@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHKABVCExoString@@EM@Z
// FUNCTION: SWKOTOR 0x006999E0 SYMBOL: ?SetWieldedModels@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXABVCResRef@@0@Z
// FUNCTION: SWKOTOR 0x006999F0 SYMBOL: ?ShowWieldedItems@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699A00 SYMBOL: ?SetCheckItems@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXH@Z
// FUNCTION: SWKOTOR 0x00699A10 SYMBOL: ?SetEnvMapPart@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHEHHABVCResRef@@@Z
// FUNCTION: SWKOTOR 0x00699A20 SYMBOL: ?SetEnvMap@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHHHABVCResRef@@@Z
// FUNCTION: SWKOTOR 0x00699A30 SYMBOL: ?RestoreEnvMap@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHXZ
// FUNCTION: SWKOTOR 0x00699A40 SYMBOL: ?ModelVC0@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699A50 SYMBOL: ?ModelVBC@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699A60 SYMBOL: ?UpdateSound@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXH@Z
// FUNCTION: SWKOTOR 0x00699A70 SYMBOL: ?SetModelV128Self@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHH@Z
// FUNCTION: SWKOTOR 0x00699A80 SYMBOL: ?SetModelV12CSelf@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHH@Z
// FUNCTION: SWKOTOR 0x00699A90 SYMBOL: ?ModelVF8@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699AA0 SYMBOL: ?ModelVFC@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699AB0 SYMBOL: ?PlayAnimOnModel@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHPAVCAurObject@@HMHH@Z
// FUNCTION: SWKOTOR 0x00699AC0 SYMBOL: ?PlayAnimOnPart@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHPAVCAurObject@@EHMHH@Z
// FUNCTION: SWKOTOR 0x00699D00 SYMBOL: ?DetachHeadItem@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699D10 SYMBOL: ?AttachHeadItem@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x00699D20 SYMBOL: ?GetBoundingExtent@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEHPAUCSWGuiExtent@@@Z
// FUNCTION: SWKOTOR 0x00699D30 SYMBOL: ?SetHeadItem@CSWCAnimBaseHeadWield@@$4PPPPPPPM@A@AEXK@Z
