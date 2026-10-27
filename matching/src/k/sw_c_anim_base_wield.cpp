// CSWCAnimBaseWield: wielded items on top of a virtual CSWCAnimBaseTW base
// (0x00699DD0-0x0069B020). Right / left hand items are looked up by object
// id; "dummyright" / "dummyleft" models stand in for them until
// ShowWieldedItems swaps the real item models back in.

#include "k/sw_c_anim_base_wield.h"

// FUNCTION: SWKOTOR 0x00699DD0
CSWCAnimBaseWield::CSWCAnimBaseWield()
{
    m_oidRight = g_oidInvalid;
    m_oidLeft = g_oidInvalid;
    m_nAnimType = 2;
    m_bCheckItems = 0;
    m_cRightModel = "";
    m_cLeftModel = "";
    m_pRightDummy = 0;
    m_pLeftDummy = 0;
    m_bDummiesShown = 0;
    m_n4C = 0;
    m_bSaberIgnited = 0;
    m_pSound = 0;
}

// FUNCTION: SWKOTOR 0x00699ED0
CSWCAnimBaseWield::~CSWCAnimBaseWield()
{
    if (m_pSound) {
        delete m_pSound;
        m_pSound = 0;
    }
    if (m_pRightDummy) {
        m_pRightDummy->AttachToObject(0, 0, 0);
        m_pRightDummy->SetVisible(0);
        delete m_pRightDummy;
        m_pRightDummy = 0;
    }
    if (m_pLeftDummy) {
        m_pLeftDummy->AttachToObject(0, 0, 0);
        m_pLeftDummy->SetVisible(0);
        delete m_pLeftDummy;
        m_pLeftDummy = 0;
    }
    UnloadModel();
}

// FUNCTION: SWKOTOR 0x00699FC0
void CSWCAnimBaseWield::SetCheckItems(int b)
{
    m_bCheckItems = b;
}

// FUNCTION: SWKOTOR 0x00699FD0
int CSWCAnimBaseWield::PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b)
{
    if (nPart != 0xff) {
        return 0;
    }
    return PlayAnimOnModel(pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x0069A010
void CSWCAnimBaseWield::ShowWieldedItems()
{
    if (!m_bDummiesShown) {
        return;
    }
    if (m_pRightDummy) {
        m_pRightDummy->AttachToObject(0, 0, 0);
        m_pRightDummy->SetVisible(0);
        delete m_pRightDummy;
        m_pRightDummy = 0;
    }
    if (m_pLeftDummy) {
        m_pLeftDummy->AttachToObject(0, 0, 0);
        m_pLeftDummy->SetVisible(0);
        delete m_pLeftDummy;
        m_pLeftDummy = 0;
    }
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
    if (pItem) {
        CAurObject* pModel = pItem->GetPartModel(0xff, 1);
        if (pModel) {
            pModel->AttachToObject(m_pModel, m_sRightNode.CStr(), 0);
        }
        if (m_pSound) {
            m_pSound->Play();
        }
    }
    pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidLeft);
    if (pItem) {
        CAurObject* pModel = pItem->GetPartModel(0xff, 1);
        if (pModel) {
            pModel->AttachToObject(m_pModel, m_sLeftNode.CStr(), 0);
        }
    }
    RefreshHilite();
    m_cRightModel = "";
    m_cLeftModel = "";
    m_bDummiesShown = 0;
}

// FUNCTION: SWKOTOR 0x0069A170
void CSWCAnimBaseWield::ModelVC0()
{
    CSWCAnimBaseTW::ModelVC0();
}

// FUNCTION: SWKOTOR 0x0069A180
void CSWCAnimBaseWield::ModelVBC()
{
    CSWCAnimBaseTW::ModelVBC();
}

// FUNCTION: SWKOTOR 0x0069A190
void CSWCAnimBaseWield::UpdateSound(int bOn)
{
    if (bOn && m_bSaberIgnited) {
        if (!m_pSound) {
            m_pSound = new CExoSoundSource("al_cv_firetorch1");
            if (!m_pSound) {
                return;
            }
            m_pSound->Set3D(1);
            m_pSound->SetPriorityGroup(6);
            m_pSound->SetVolume(10);
            m_pSound->SetLooping(1);
            m_pSound->Play();
            if (!m_pSound) {
                return;
            }
        }
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
        if (pItem && pItem->GetPartModel(0xff, 1)) {
            m_pSound->SetPosition(pItem->GetPartModel(0xff, 1)->GetPosition(0));
            return;
        }
        if (m_pModel) {
            m_pSound->SetPosition(m_pModel->GetPosition(0));
        }
        return;
    }
    if (m_pSound) {
        delete m_pSound;
        m_pSound = 0;
    }
}

// FUNCTION: SWKOTOR 0x0069A3E0
// NONMATCH: 17 diff insns, register allocation only: for the second part
// call (0xfe) of each item the original keeps the model in edi and spills
// the vtable, we spill the model. Tried one shared / three separate part
// locals.
int CSWCAnimBaseWield::PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b)
{
    if (fSpeed == 0.0f) {
        fSpeed = 1.0f;
    }
    CAurObject* pPart;
    CSWCItem* pRight = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
    CSWCItem* pLeft = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidLeft);
    if (pRight || pLeft) {
        CExoString sAnim = GetAnimationName(nAnim);
        if (pRight && pRight->GetBaseItem()->m_nWeaponWield != 0x1d) {
            pPart = pRight->GetPartModel(0xff, 1);
            if (pPart) {
                pPart->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            }
            pPart = pRight->GetPartModel(0xfe, 1);
            if (pPart) {
                pPart->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            }
            pPart = pRight->GetPartModel(0xfd, 1);
            if (pPart) {
                pPart->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            }
        }
        if (pLeft) {
            pPart = pLeft->GetPartModel(0xff, 1);
            if (pPart) {
                pPart->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            }
            pPart = pLeft->GetPartModel(0xfe, 1);
            if (pPart) {
                pPart->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            }
            pPart = pLeft->GetPartModel(0xfd, 1);
            if (pPart) {
                pPart->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            }
        }
    }
    return CSWCAnimBaseTW::PlayAnimOnModel(pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x0069A650
CAurObject* CSWCAnimBaseWield::GetItemModel(unsigned long oid, unsigned char nPart)
{
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
    if (!pItem) {
        return 0;
    }
    return pItem->GetPartModel(nPart, 1);
}

// FUNCTION: SWKOTOR 0x0069A680
int CSWCAnimBaseWield::IsItemValid(unsigned long oid)
{
    if (!m_bCheckItems) {
        return 1;
    }
    return g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid) != 0;
}

// FUNCTION: SWKOTOR 0x0069A6B0
void CSWCAnimBaseWield::SetWieldedModels(const CResRef& cRight, const CResRef& cLeft)
{
    CExoString sModel;
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
    if (pItem) {
        CAurObject* pPart = pItem->GetPartModel(0xff, 1);
        if (pPart) {
            pPart->AttachToObject(0, 0, 0);
            pPart->SetVisible(0);
            if (m_pSound) {
                m_pSound->Stop();
            }
        }
    }
    pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidLeft);
    if (pItem) {
        CAurObject* pPart = pItem->GetPartModel(0xff, 1);
        if (pPart) {
            pPart->AttachToObject(0, 0, 0);
            pPart->SetVisible(0);
        }
    }
    if (m_cRightModel != cRight) {
        if (m_pRightDummy) {
            m_pRightDummy->AttachToObject(0, 0, 0);
            m_pRightDummy->SetVisible(0);
            delete m_pRightDummy;
            m_pRightDummy = 0;
        }
        if (*cRight.GetResRef()) {
            cRight.CopyToString(sModel);
            m_pRightDummy = NewCAurObject(sModel.CStr(), "dummyright", 0, 0);
            if (m_pRightDummy) {
                m_pRightDummy->AttachToObject(m_pModel, "rhand", 0);
            }
        }
        m_cRightModel = cRight;
    }
    if (m_cLeftModel != cLeft) {
        if (m_pLeftDummy) {
            m_pLeftDummy->AttachToObject(0, 0, 0);
            m_pLeftDummy->SetVisible(0);
            delete m_pLeftDummy;
            m_pLeftDummy = 0;
        }
        if (*cLeft.GetResRef()) {
            cLeft.CopyToString(sModel);
            m_pLeftDummy = NewCAurObject(sModel.CStr(), "dummyleft", 0, 0);
            if (m_pLeftDummy) {
                m_pLeftDummy->AttachToObject(m_pModel, "lhand", 0);
            }
        }
        m_cLeftModel = cLeft;
    }
    m_bDummiesShown = 1;
}

// FUNCTION: SWKOTOR 0x0069A8F0
void CSWCAnimBaseWield::ModelVF8()
{
    CAurObject* pPart = GetItemModelInline(m_oidLeft);
    if (pPart) {
        pPart->vF8();
    }
    pPart = GetItemModelInline(m_oidRight);
    if (pPart) {
        pPart->vF8();
    }
    CSWCAnimBaseTW::ModelVF8();
}

// FUNCTION: SWKOTOR 0x0069A980
void CSWCAnimBaseWield::ModelVFC()
{
    CAurObject* pPart = GetItemModelInline(m_oidLeft);
    if (pPart) {
        pPart->vFC();
    }
    pPart = GetItemModelInline(m_oidRight);
    if (pPart) {
        pPart->vFC();
    }
    CSWCAnimBaseTW::ModelVFC();
}

// No head item at this layer: both overrides are empty and the linker
// folded them (and their two vtordisp thunks) into shared stubs.
void CSWCAnimBaseWield::DetachHeadItem()
{
}

void CSWCAnimBaseWield::AttachHeadItem()
{
}

// FUNCTION: SWKOTOR 0x0069AA60 SYMBOL: ??_GCSWCAnimBaseWield@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0069AA90
CAurObject* CSWCAnimBaseWield::GetModel(unsigned char nPart)
{
    switch (nPart) {
    case 0xfb:
        return GetItemModel(m_oidRight, 0xff);
    case 0xfa:
        return GetItemModel(m_oidRight, 0xfe);
    case 0xf9:
        return GetItemModel(m_oidRight, 0xfd);
    case 0xf8:
        return GetItemModel(m_oidRight, 0xfc);
    case 0xf7:
        return GetItemModel(m_oidLeft, 0xff);
    case 0xf6:
        return GetItemModel(m_oidLeft, 0xfe);
    case 0xf5:
        return GetItemModel(m_oidLeft, 0xfd);
    case 0xf4:
        return GetItemModel(m_oidLeft, 0xfc);
    }
    return CSWCAnimBaseTW::GetModel(nPart);
}

// FUNCTION: SWKOTOR 0x0069AB80
int CSWCAnimBaseWield::SetWieldedItem(unsigned long oid, const CExoString& sNode, unsigned char nHand, float fAlpha)
{
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
    CAurObject* pNew;
    if (!pItem) {
        pNew = 0;
    }
    else {
        pNew = pItem->GetPartModel(0xff, 1);
    }
    CAurObject* pOld;
    if (nHand == 0) {
        pOld = GetItemModelInline(m_oidRight);
        if (oid != m_oidRight && pOld) {
            pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
            if (pItem && pItem->GetBaseItem()->m_nWeaponWield == 0x1d) {
                pOld->PlayAnimation("unequip", 1.0f, 3, 0);
                m_bSaberIgnited = 0;
            }
            pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
            if (pItem && !pItem->GetKeepAttached()) {
                pOld->AttachToObject(0, 0, 0);
                pOld->SetVisible(0);
                pOld->SetAlpha(1.0f, 1);
            }
        }
        if (pNew) {
            if (!m_bCheckItems || g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid)) {
                pNew->AttachToObject(m_pModel, sNode.CStr(), 0);
                pNew->SetAlpha(fAlpha, 1);
                m_sRightNode = sNode;
                pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
                if (pItem && pItem->GetBaseItem()->m_nWeaponWield == 0x1d) {
                    pNew->PlayAnimation("equip", 1.0f, 3, 0);
                    m_bSaberIgnited = 1;
                }
            }
        }
        else {
            if (m_oidRight != m_oidLeft && pOld) {
                pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidRight);
                if (pItem && !pItem->GetKeepAttached()) {
                    pOld->AttachToObject(0, 0, 0);
                    pOld->SetVisible(0);
                    pOld->SetAlpha(1.0f, 1);
                }
            }
            m_sRightNode = "";
        }
        m_oidRight = oid;
    }
    else if (nHand == 1) {
        pOld = GetItemModelInline(m_oidLeft);
        if (oid != m_oidLeft && pOld) {
            pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidLeft);
            if (pItem && !pItem->GetKeepAttached()) {
                pOld->AttachToObject(0, 0, 0);
                pOld->SetVisible(0);
                pOld->SetAlpha(1.0f, 1);
            }
        }
        if (pNew) {
            if (IsItemValid(oid)) {
                pNew->AttachToObject(m_pModel, sNode.CStr(), 0);
                pNew->SetAlpha(fAlpha, 1);
                m_sLeftNode = sNode;
            }
        }
        else {
            if (m_oidLeft != m_oidRight && pOld) {
                pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidLeft);
                if (pItem && !pItem->GetKeepAttached()) {
                    pOld->AttachToObject(0, 0, 0);
                    pOld->SetVisible(0);
                    pOld->SetAlpha(1.0f, 1);
                }
            }
            m_sLeftNode = "";
        }
        m_oidLeft = oid;
    }
    else {
        return 0;
    }
    RefreshHilite();
    return 1;
}

// FUNCTION: SWKOTOR 0x0069AF10
void CSWCAnimBaseWield::AttachWieldedItems()
{
    CAurObject* pPart = GetItemModelInline(m_oidRight);
    if (pPart) {
        pPart->AttachToObject(m_pModel, m_sRightNode.CStr(), 0);
    }
    pPart = GetItemModelInline(m_oidLeft);
    if (pPart) {
        pPart->AttachToObject(m_pModel, m_sLeftNode.CStr(), 0);
    }
    RefreshHilite();
}

// FUNCTION: SWKOTOR 0x0069AFE0
int CSWCAnimBaseWield::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    if (CSWCAnimBaseTW::LoadModel(cResRef, nPart)) {
        AttachWieldedItems();
        return 1;
    }
    return 0;
}

// vtordisp{4} thunks (compiler generated for the overrides seen through
// the virtual CSWCAnimBase base: `sub ecx, [ecx-4]; jmp`).
// FUNCTION: SWKOTOR 0x0069A330 SYMBOL: ??_ECSWCAnimBaseWield@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: SWKOTOR 0x0069A340 SYMBOL: ?ShowWieldedItems@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x0069A350 SYMBOL: ?SetCheckItems@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXH@Z
// FUNCTION: SWKOTOR 0x0069A360 SYMBOL: ?ModelVC0@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x0069A370 SYMBOL: ?ModelVBC@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x0069A380 SYMBOL: ?UpdateSound@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXH@Z
// FUNCTION: SWKOTOR 0x0069A390 SYMBOL: ?PlayAnimOnPart@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEHPAVCAurObject@@EHMHH@Z
// FUNCTION: SWKOTOR 0x0069AA10 SYMBOL: ?SetWieldedModels@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXABVCResRef@@0@Z
// FUNCTION: SWKOTOR 0x0069AA20 SYMBOL: ?DetachHeadItem@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x0069AA30 SYMBOL: ?ModelVF8@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x0069AA40 SYMBOL: ?ModelVFC@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEXXZ
// FUNCTION: SWKOTOR 0x0069AA50 SYMBOL: ?PlayAnimOnModel@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEHPAVCAurObject@@HMHH@Z
// FUNCTION: SWKOTOR 0x0069AFC0 SYMBOL: ?GetModel@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEPAVCAurObject@@E@Z
// FUNCTION: SWKOTOR 0x0069AFD0 SYMBOL: ?SetWieldedItem@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEHKABVCExoString@@EM@Z
// FUNCTION: SWKOTOR 0x0069B020 SYMBOL: ?LoadModel@CSWCAnimBaseWield@@$4PPPPPPPM@A@AEHABVCResRef@@E@Z
