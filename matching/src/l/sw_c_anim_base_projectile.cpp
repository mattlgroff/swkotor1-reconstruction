// Client animation base of projectiles (CSWCAnimBaseProjectile, vtable
// 0x00759170, 0x006EA470-0x006EA690). The constructor emits the vtable, so
// the scalar deleting destructor comes from this translation unit.
#include "l/sw_c_projectile.h"

// FUNCTION: SWKOTOR 0x006EA470
CSWCAnimBaseProjectile::CSWCAnimBaseProjectile()
{
    m_nAnimType = 7;
}

// FUNCTION: SWKOTOR 0x006EA490
CSWCAnimBaseProjectile::~CSWCAnimBaseProjectile()
{
    CSWCAnimBase::UnloadModel();
}

// FUNCTION: SWKOTOR 0x006EA4E0
int CSWCAnimBaseProjectile::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    char szModel[17];
    cResRef.CopyToString(szModel);
    m_pModel = NewCAurObject(szModel, "projectile", 0, 0);
    if (m_pModel == 0) {
        return 0;
    }
    m_cModelResRef = cResRef;
    m_bReplaceTextureIndexed = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x006EA550
int CSWCAnimBaseProjectile::PlayAnimation(unsigned short nAnim, float fSpeed, int a, int nTransition)
{
    if (fSpeed == 0.0f) {
        fSpeed = 1.0f;
    }
    switch (nAnim) {
    case 0:
        m_pModel->PlayAnimation("default", fSpeed, a, nTransition);
        return 1;
    case 1:
        m_pModel->PlayAnimation("travel01", fSpeed, a, nTransition);
        return 1;
    case 2:
        m_pModel->PlayAnimation("impact01", fSpeed, a | 1, nTransition);
        return 1;
    case 3:
        m_pModel->PlayAnimation("throwout", fSpeed, a, nTransition);
        return 1;
    case 4:
        m_pModel->PlayAnimation("throwback", fSpeed, a, nTransition);
        return 1;
    default:
        m_pModel->PlayAnimation("", 1.0f, 0, nTransition);
        return 1;
    }
}

// FUNCTION: SWKOTOR 0x006EA680 SYMBOL: ??_GCSWCAnimBaseProjectile@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006EA6A0
int CSWCAnimBaseProjectile::SetFacing(const Vector& vDirection)
{
    if (m_pModel == 0) {
        return 0;
    }
    if (MagnitudeSquared(vDirection) <= 0.00001f) {
        return 0;
    }
    Vector vNormal = NormalizeVector(vDirection);
    m_pModel->SetOrientation(EulerToQuaternion(GetYaw(vNormal), Pitch(vNormal), 0.0f));
    return 1;
}
