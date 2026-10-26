// CSWCItem lifetime, CSWCObject overrides, model alignment, name and power
// state (0x00646230-0x00647130).
#include "m/sw_c_item_object.h"

// FUNCTION: SWKOTOR 0x00646230
CSWCItem::CSWCItem(unsigned long nBaseItem) : CSWCObject(OBJECT_TYPE_ITEM)
{
    m_n150 = -1;
    m_n14c = -1;
    m_n154 = -1;
    m_n158 = -1;
    m_nStackSize = -1;
    m_n148 = 0;
    m_n15d = 0xff;
    m_n15c = 0xff;
    m_nBaseItem = nBaseItem;
    m_n15f = 0xff;
    m_n15e = 0xff;
    m_nNumActiveProperties = 0;
    m_pActiveProperties = 0;
    m_bIdentified = 0;
    m_n168 = 0;
    m_n188 = 0;
    m_n10c = 0;
    m_n11c = 1;
    m_oidEquippedBy = OBJECT_INVALID;
    m_oid164 = OBJECT_INVALID;
    for (int i = 0; i < 8; i++) {
        m_nPropertyUses[i] = 0;
    }
    m_nActivePropertyIndex = 0xff;
    m_n11d = 0;
    m_pSpecialAttack = 0;
    m_n198 = 0;
    m_pPowerSound = 0;
    m_bPowered = 0;
    m_n1a0 = 0;
}

// FUNCTION: SWKOTOR 0x00646380
CSWItem* CSWCItem::AsSWItem()
{
    return this;
}

// FUNCTION: SWKOTOR 0x00646390
void CSWCItem::UpdateAI()
{
    unsigned long nDay, nTick, nElapsedDay, nElapsedTick;
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    pApp->GetWorldTimer()->GetWorldTime(&nDay, &nTick);
    pApp = g_pAppManager->m_pClientExoApp;
    pApp->GetWorldTimer()->SubtractWorldTimes(nDay, nTick, m_nDay, m_nTick, &nElapsedDay, &nElapsedTick);
    m_nElapsed = nElapsedTick;
    UpdatePopUpBubble();
    if (FadeObject()) {
        g_pAppManager->m_pClientExoApp->GetClientAIMaster()->GetAIList()->RemoveObject(this);
        CGameObject* pRemoved;
        unsigned long oid = m_oid;
        pApp = g_pAppManager->m_pClientExoApp;
        if (pApp->GetGameObjectArray()->Delete(oid, &pRemoved) == g_nObjectArraySuccess && pRemoved) {
            delete this;
            return;
        }
    }
    if (m_pSpecialAttack) {
        float fTime = 0.0f;
        m_pSpecialAttack->GetAnimationTime("impact", &fTime, 0);
        if (fTime == -1.0f) {
            m_pSpecialAttack->AttachToObject(0, "", 0);
            delete m_pSpecialAttack;
            m_pSpecialAttack = 0;
            pApp = g_pAppManager->m_pClientExoApp;
            pApp->GetClientAIMaster()->RemoveObject(this);
        }
    }
    m_nDay = nDay;
    m_nTick = nTick;
}

// FUNCTION: SWKOTOR 0x00646500
int CSWCItem::LoadModel(const CResRef& cResRef, unsigned char nPart, int)
{
    if (!m_pModel) {
        m_pModel = new CSWCAnimBase;
        if (!m_pModel) {
            return 0;
        }
    }
    if (!GetAnimationBase()->LoadModel(cResRef, nPart)) {
        return 0;
    }
    CAurObject* pModel = GetAnimationBase()->GetModel(nPart);
    if (pModel) {
        pModel->SetClientObject(this);
        pModel->SetObjectType(OBJECT_TYPE_ITEM);
    }
    pModel->SetPickMode(5);
    if (nPart == 0xff) {
        m_fCanSeeHeight = 0.5f;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00646930
void CSWCItem::SetArea(CSWCArea* pArea)
{
    int bMoved;
    if (pArea) {
        if (m_pModel) {
            CAurObject* pAuxiliary = GetModel(0xfc, 1);
            if (pAuxiliary) {
                pAuxiliary->AttachToObject(0, 0, 0);
                pAuxiliary->SetVisible(0);
            }
        }
        if (m_pArea == pArea) {
            goto noMove;
        }
        bMoved = 1;
    } else {
        AttachAuxiliaryModel();
noMove:
        bMoved = 0;
    }
    CSWCObject::SetArea(pArea);
    if (bMoved) {
        SetPosition(m_vPosition);
    }
}

// FUNCTION: SWKOTOR 0x00646A30
void CSWCItem::AlignToNormal(const Vector& vNormal)
{
    Quaternion q;
    float fRoll = 0.0f;
    float fPitch = 0.0f;
    if (GetBaseItem()->m_nModelType == 1) {
        fRoll = 90.0f;
    } else if (GetBaseItem()->m_nModelType == 2) {
        fPitch = 90.0f;
    }
    q = EulerToQuaternion(GetFacing(m_vOrientation), fPitch, fRoll);
    if (g_bItemGroundAlign) {
        q = RotationArc(Vector(0.0f, 0.0f, 1.0f), vNormal) * q;
    }
    if (m_pModel) {
        m_pModel->SetOrientation(q);
    }
}

// FUNCTION: SWKOTOR 0x00646CD0
CSWCItem::~CSWCItem()
{
    if (m_pModel) {
        m_pModel->UnloadPart(0xfc);
    }
    DestroyAnimationBase();
    if (m_pActiveProperties) {
        free(m_pActiveProperties);
        m_pActiveProperties = 0;
    }
    if (m_pSpecialAttack) {
        m_pSpecialAttack->AttachToObject(0, "", 0);
        delete m_pSpecialAttack;
        m_pSpecialAttack = 0;
    }
    delete m_pPowerSound;
    m_pPowerSound = 0;
}

// FUNCTION: SWKOTOR 0x00646DD0
void CSWCItem::SetOrientation(const Vector& v)
{
    m_vOrientation = v;
    Vector vNormal(0.0f, 0.0f, 1.0f);
    if (g_bItemGroundAlign && m_pArea) {
        Vector vPosition = m_vPosition;
        vPosition.z = 0.0f;
        GetGroundHeight(&vPosition, 0.0f);
    }
    AlignToNormal(vNormal);
}

// FUNCTION: SWKOTOR 0x00646E70
CExoString CSWCItem::GetName(int bFullName)
{
    STR_RES strRes;
    CExoString sName;
    if (!bFullName) {
        return m_sDescription;
    }
    if (m_sDescription.GetLength() > 0 && m_bIdentified) {
        return m_sDescription;
    }
    g_pTlkTable->Fetch(GetBaseItem()->m_nNameStrRef, strRes, g_nGender);
    sName = strRes.m_sText;
    if (!m_bIdentified) {
        g_pTlkTable->Fetch(0, strRes, g_nGender);
        sName = sName + " " + strRes.m_sText;
    }
    return sName;
}

// FUNCTION: SWKOTOR 0x00647030
void CSWCItem::SetPosition(const Vector& v)
{
    Vector vPosition = v;
    float fHeight = g_fZero;
    vPosition.z = fHeight;
    Vector vNormal(fHeight, fHeight, 1.0f);
    if (g_bItemGroundAlign) {
        fHeight = GetGroundHeight(&vPosition, fHeight);
    } else {
        int bHitStatic = g_bHitStatic;
        g_bHitStatic = 1;
        fHeight = GetGroundHeight(&vPosition, fHeight);
        g_bHitStatic = bHitStatic;
    }
    vPosition.z = fHeight + 0.1f;
    CSWCObject::SetPosition(vPosition);
    if (g_bItemGroundAlign) {
        if (m_vOrientation.z * m_vOrientation.z + m_vOrientation.y * m_vOrientation.y
                + m_vOrientation.x * m_vOrientation.x > 1e-5f) {
            AlignToNormal(vNormal);
        }
    }
}

// FUNCTION: SWKOTOR 0x00647130
void CSWCItem::PowerItem(int bPowered, int bForce, int bAnimate, int bQuiet)
{
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    if (pApp->GetModule()->m_pArea->m_bLoading) {
        return;
    }
    if (m_oidEquippedBy != OBJECT_INVALID
        && g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(m_oidEquippedBy)) {
        if (GetEquippedBy()->m_n3d8 == 1 && !bForce) {
            return;
        }
    }
    if (bPowered == 1 && g_pAppManager->m_pClientExoApp->GetAreaNotReadyToDisplay() == bPowered
        && !g_pAppManager->m_pClientExoApp->GetGuiInGame()->IsGlobalFading()) {
        SetEquippedModelFlags(0);
        return;
    }
    SetEquippedModelFlags(bPowered);
    if (GetBaseItem()->m_bPowered != 1) {
        return;
    }
    g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(m_oidEquippedBy);
    if (bPowered != m_bPowered) {
        Vector vPosition = GetAnimationBase()->GetModel(0xff)->GetPosition();
        if (!m_pPowerSound) {
            m_pPowerSound = new CExoSoundSource(CResRef(GetBaseItem()->m_sPoweredSound));
            m_pPowerSound->Set3D(1);
            m_pPowerSound->SetPosition(vPosition, 0);
        }
        int nAnimation;
        int nLoopAnimation;
        if (bPowered) {
            nAnimation = 0x128;
            nLoopAnimation = 0x129;
            if (!bQuiet && m_pPowerSound) {
                m_pPowerSound->SetPriorityGroup(0x15);
                m_pPowerSound->SetLooping(1);
                m_pPowerSound->Set3D(1);
                m_pPowerSound->SetPosition(vPosition, bQuiet);
                m_pPowerSound->Play();
            }
        } else {
            nAnimation = 0x12a;
            nLoopAnimation = 0x164;
            if (m_pPowerSound) {
                m_pPowerSound->Stop();
            }
        }
        if (bAnimate) {
            if (!m_bPowered) {
                if (m_pModel->GetModel(0xff)) {
                    m_pModel->GetModel(0xff)->PlayAnimation("NULL", 1.0f, 0, 0);
                }
                if (!bQuiet) {
                    g_pExoSound->PlaySoundEffect(CResRef(GetBaseItem()->m_sPowerUpSound), vPosition,
                                                 bQuiet, 0x15, bQuiet, bQuiet, bQuiet, bQuiet);
                }
            } else if (!bQuiet) {
                g_pExoSound->PlaySoundEffect(CResRef(GetBaseItem()->m_sPowerDownSound), vPosition,
                                             bQuiet, 0x15, bQuiet, bQuiet, bQuiet, bQuiet);
            }
            m_pModel->PlayAnim(nAnimation, 1.0f, 1, 0);
            m_pModel->PlayAnim(nLoopAnimation, 1.0f, 0x20, 0);
        } else {
            m_pModel->PlayAnim(nLoopAnimation, 1.0f, 0, 0);
        }
    }
    m_bPowered = bPowered;
}
