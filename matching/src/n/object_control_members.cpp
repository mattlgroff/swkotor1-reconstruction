#include "n/object_control_family.h"

// FUNCTION: SWKOTOR 0x0063C100
unsigned short CSWCObject::GetLoopingAnimation()
{
    return m_nLoopingAnimation;
}

// FUNCTION: SWKOTOR 0x0063CAD0
void CSWCObject::AbortFireForgetAnimation(int bKeep)
{
    m_n64 = 0;
    if (!bKeep)
        SetAnimation(m_nLoopingAnimation,0,0);
}

// FUNCTION: SWKOTOR 0x0063D830
void CSWCObject::SetLoopingAnimation(unsigned short nAnimation)
{
    if (AnimationLooping(nAnimation) == 1)
        m_nLoopingAnimation = nAnimation;
}

// FUNCTION: SWKOTOR 0x0063C9C0
void CSWCObject::RemoveTopFireForgetAnimation()
{
    for (int i = 1; i < m_nAnimationCount; ++i) {
        m_pAnimationQueue[i-1].active = m_pAnimationQueue[i].active;
        m_pAnimationQueue[i-1].animation = m_pAnimationQueue[i].animation;
        m_pAnimationQueue[i-1].day = m_pAnimationQueue[i].day;
        m_pAnimationQueue[i-1].tick = m_pAnimationQueue[i].tick;
        m_pAnimationQueue[i-1].duration = m_pAnimationQueue[i].duration;
        m_pAnimationQueue[i-1].speed = m_pAnimationQueue[i].speed;
        m_pAnimationQueue[i-1].data = m_pAnimationQueue[i].data;
    }
    if (m_nAnimationCount >= 1) {
        m_pAnimationQueue[m_nAnimationCount-1].active = 0;
        m_pAnimationQueue[m_nAnimationCount-1].animation = 10000;
        m_pAnimationQueue[m_nAnimationCount-1].day = 0;
        m_pAnimationQueue[m_nAnimationCount-1].tick = 0;
        m_pAnimationQueue[m_nAnimationCount-1].duration = 500;
        m_pAnimationQueue[m_nAnimationCount-1].speed = 1.0f;
        m_pAnimationQueue[m_nAnimationCount-1].data = 0;
        --m_nAnimationCount;
    }
}

// FUNCTION: SWKOTOR 0x0063CAB0
void CSWCObject::ClearFireForgetAnimations()
{
    for (int i = 0; i < m_nAnimationCapacity; ++i)
        RemoveTopFireForgetAnimation();
}

// FUNCTION: SWKOTOR 0x0063C0A0
int CSWCObject::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore)
{
    if (!m_pAnimationBase)
        return 0;
    return m_pAnimationBase->ReplaceTexture(nPart,cOld,cNew,bStore);
}

// FUNCTION: SWKOTOR 0x0063CE40
int CSWCObject::RestoreTexturePart(unsigned char nPart)
{
    if (!m_pAnimationBase)
        return 0;
    return m_pAnimationBase->RestoreTexturePart(nPart);
}

// FUNCTION: SWKOTOR 0x0063D2D0
void CSWCObject::SetNameInfo(const CExoString& sName)
{
    m_sNameInfo = sName;
    m_nNameInfoTime = 5000;
}

// FUNCTION: SWKOTOR 0x0063D300
void CSWCObject::GetNameInfo(CExoString* pName, int* pTime)
{
    *pName = m_sNameInfo;
    *pTime = m_nNameInfoTime;
}

// FUNCTION: SWKOTOR 0x0063D4B0
CSWSObject* CSWCObject::GetServerObject()
{
    if (!m_pServerObject) {
        if (m_bNoServerLink)
            return 0;
        CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
        if (!pServer)
            return 0;
        unsigned long nServerObject = pServer->ClientToServerObjectID(m_oidSelf);
        pServer = g_pAppManager->m_pServerExoApp;
        CGameObject* pObject = pServer->GetGameObject(nServerObject);
        if (pObject) {
            m_pServerObject = pObject->AsSWSObject();
            if (m_pServerObject)
                m_pServerObject->AttachmentFromClientObject(this);
        }
    }
    return m_pServerObject;
}
