#include "n/object_control_family.h"

// FUNCTION: SWKOTOR 0x0063C110
void CSWCObject::SetOrientation(const Vector& v)
{
    m_vOrientation = Normalize(v);
    if (m_pAnimationBase && !m_n50)
        m_pAnimationBase->SetFacing(v);
    UpdateOrientation();
}

// FUNCTION: SWKOTOR 0x0063D270
void CSWCObject::SetWind(int bOn,float fStrength)
{
    CAurObject* pModel = GetModel(255,0);
    if (pModel) {
        if (g_pAppManager->m_pClientExoApp->GetActivePauseState() != 2 && bOn)
            pModel->SetWind(bOn != 0,fStrength,0,0);
        else
            pModel->SetWind(0,0.0f,0,0);
    }
}

// FUNCTION: SWKOTOR 0x0063D1D0
void CSWCObject::NewVocalization(const CResRef& cSound,int nType)
{
    if (m_pVocalization)
        delete m_pVocalization;
    m_pVocalization = new CExoSoundSource(cSound);
    m_nVocalizationType = nType;
}

// FUNCTION: SWKOTOR 0x0063D140
void CSWCObject::SetPortraitId(unsigned short nId)
{
    CSWPortrait portrait;
    if (portrait.SetPortraitId(nId)) {
        // Slot +EC installs the portrait reference; public name is descriptive.
        SetPortrait(portrait.GetPortrait(0));
    }
    m_nPortraitId = nId;
}

// FUNCTION: SWKOTOR 0x0063D070
void CSWCObject::RemoveShadowBlob()
{
    if (m_pAnimationBase) {
        m_pAnimationBase->DestroyShadow();
        if (m_bShadowBlob)
            m_pAnimationBase->ModelVC0();
    }
}

// FUNCTION: SWKOTOR 0x0063D700
void CSWCObject::AcquireAnimationBase(CSWCObject* pSource)
{
    if (pSource) {
        if (m_pAnimationBase)
            delete m_pAnimationBase;
        if (pSource->AsSWCCreature())
            pSource->AsSWCCreature()->UnregisterAnimationCallbacks();
        m_pAnimationBase = pSource->m_pAnimationBase;
        pSource->m_pAnimationBase->GetModel(255)->SetClientObject(this);
        CAurObject* pModel = pSource->m_pAnimationBase->GetModel(255);
        unsigned char nType = m_nType;
        pModel->SetObjectType(nType);
        pSource->m_pAnimationBase = 0;
    }
}

// FUNCTION: SWKOTOR 0x0063F570
void CSWCObject::SetArea(CSWCArea* pArea)
{
    if (m_pArea != pArea) {
        if (m_pArea)
            m_pArea->m_lObjects.Remove(m_oidSelf);
        if (pArea) {
            pArea->m_lObjects.AddUnique(m_oidSelf);
            UpdateAreaFade(pArea);
        }
    }
    m_pArea = pArea;
}
