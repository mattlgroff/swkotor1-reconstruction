// The mini-game bullet behaviour (vtable 0x00757F98, type 0xAAAB): carries
// the firing gun bank's hit info and owner on the bullet model.
#include "l/sw_mg_behavior.h"

// FUNCTION: SWKOTOR 0x006DAFE0
CSWMiniBulletInfo::CSWMiniBulletInfo(Gob* pGob, CSWTrackFollower* pOwner, const CSWMiniHitInfo& cHit)
    : CAurBehavior(pGob)
{
    m_cHit = cHit;
    m_pOwner = pOwner;
}

// FUNCTION: SWKOTOR 0x006DB080
int CSWMiniBulletInfo::GetType()
{
    return 0xaaab;
}

// FUNCTION: SWKOTOR 0x006DB090
int CSWMiniBulletInfo::Control(float fDelta)
{
    if (!CAurBehavior::Control(fDelta)) {
        return 0;
    }
    m_cHit.m_fLifespan -= fDelta;
    if (m_cHit.m_fLifespan <= 0.0f) {
        if (m_pGob) {
            delete m_pGob;
        }
        return 0;
    }
    fDelta *= m_cHit.m_fSpeed;
    m_pGob->SetPosition(fDelta * m_pGob->GetOrientation().GetYAxis() + m_pGob->GetPosition());
    return 1;
}
