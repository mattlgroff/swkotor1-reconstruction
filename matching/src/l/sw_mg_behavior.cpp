// Mini-game tracking behaviours (swmgbehavior.o, 0x006DB4F0-0x006DB650).
#include "l/sw_mg_behavior.h"

// FUNCTION: SWKOTOR 0x006DB4F0
CSWMGBehaviorTrack::CSWMGBehaviorTrack(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart,
                                       CAurObject* pTarget)
    : CAurBehaviorAttach(pObject, pTo, pPart), m_vOffset(0.0f, 0.0f, 0.0f)
{
    m_bOnTarget = 0;
    m_pTarget = pTarget;
    AddDependency(pTarget);
}

// FUNCTION: SWKOTOR 0x006DB580
int CSWMGBehaviorTrack::GetType()
{
    return 0xaaac;
}

// FUNCTION: SWKOTOR 0x006DB590
CSWMGBehaviorTrackAndFire::CSWMGBehaviorTrackAndFire(CAurObject* pObject, CAurObject* pTo,
                                                     CAurPart* pPart, CAurObject* pTarget)
    : CSWMGBehaviorTrack(pObject, pTo, pPart, pTarget)
{
    m_fTimer = 0.0f;
}

// FUNCTION: SWKOTOR 0x006DB600
int CSWMGBehaviorTrackAndFire::GetType()
{
    return 0xaaaa;
}

// FUNCTION: SWKOTOR 0x006DB610 SYMBOL: ??_GCSWMGBehaviorTrack@@UAEPAXI@Z
// Emitted with the vtable (the destructor itself is in
// sw_mg_behavior_dtors_novtable.cpp).

// FUNCTION: SWKOTOR 0x006DB3D0
int CSWMGBehaviorTrackAndFire::Control(float fDelta)
{
    if (!CSWMGBehaviorTrack::Control(fDelta)) {
        return 0;
    }
    if (m_fTimer > 0.0) {
        float fTimer = m_fTimer - fDelta;
        m_fTimer = 0.0f > fTimer ? 0.0f : fTimer;
    }
    if (m_bOnTarget && m_fTimer == 0.0) {
        m_fTimer = m_fRate;
        m_pObject->PlayAnimation("fire", 1.0f, 1, 0.0f);
        m_pObject->PlayAnimation("ready", 1.0f, 0x20, 0.0f);
        if (m_cFireSound.IsValid()) {
            CResRef cSound(m_cFireSound.GetResRef());
            g_pExoSound->Play3DOneShotSound(cSound, m_pObject->GetPosition(), 0.0f, 0x18, 0, 0, 0.0f, 0.0f);
        }
    }
    return 1;
}
