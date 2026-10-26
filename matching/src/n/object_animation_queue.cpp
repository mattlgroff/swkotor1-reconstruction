#include "n/object_control_family.h"

// FUNCTION: SWKOTOR 0x0063BFD0
FireForgetAnimation::FireForgetAnimation()
{
    active = 0;
    animation = 10000;
    day = 0;
    tick = 0;
    duration = 500;
    speed = 1.0f;
    data = 0;
}

// FUNCTION: SWKOTOR 0x0063C610
int CSWCObject::GetTopFireForgetAnimation(unsigned short* pAnimation,float* pSpeed,unsigned long* pDuration,void** pData)
{
    unsigned long nDay = m_pAnimationQueue[0].day;
    unsigned long nTick = m_pAnimationQueue[0].tick;
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    unsigned long nElapsed = pClient->GetWorldTimer()->GetTimeDifferenceFromWorldTime(nDay,nTick);
    if (!m_pAnimationQueue[0].active) {
        *pAnimation = 10000;
        *pData = 0;
        *pSpeed = 1.0f;
        *pDuration = 500;
        return 0;
    }
    if (nElapsed == 0) {
        unsigned long nNowDay,nNowTick;
        pClient = g_pAppManager->m_pClientExoApp;
        pClient->GetWorldTimer()->GetWorldTime(&nNowDay,&nNowTick);
        pClient = g_pAppManager->m_pClientExoApp;
        if (pClient->GetWorldTimer()->CompareWorldTimes(nNowDay,nNowTick,nDay,nTick) == -1) {
            *pAnimation = 10000;
            *pData = (void*)nElapsed;
            *pSpeed = 1.0f;
            *pDuration = 500;
            return 0;
        }
    }
    *pAnimation = m_pAnimationQueue[0].animation;
    *pData = m_pAnimationQueue[0].data;
    unsigned long nTotal = m_pAnimationQueue[0].duration;
    int nRemaining = nTotal - nElapsed;
    if (nRemaining < 50)
        nRemaining = 50;
    *pDuration = nRemaining;
    *pSpeed = (float)nTotal * m_pAnimationQueue[0].speed / (float)nRemaining;
    return 1;
}

// FUNCTION: SWKOTOR 0x0063C760
void CSWCObject::AddFireForgetAnimation(unsigned short nAnimation,unsigned long nDuration,float fSpeed,void* pData,unsigned long nDelay)
{
    AnimationFireAndForget(nAnimation);
    unsigned long nDay,nTick;
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    pClient->GetWorldTimer()->GetWorldTime(&nDay,&nTick);
    if (m_nAnimationCount == m_nAnimationCapacity) {
        FireForgetAnimation* pNew = new FireForgetAnimation[m_nAnimationCapacity+8];
        for (int i = 0; i < m_nAnimationCapacity; ++i) {
            pNew[i].active = m_pAnimationQueue[i].active;
            pNew[i].animation = m_pAnimationQueue[i].animation;
            pNew[i].day = m_pAnimationQueue[i].day;
            pNew[i].tick = m_pAnimationQueue[i].tick;
            pNew[i].duration = m_pAnimationQueue[i].duration;
            pNew[i].speed = m_pAnimationQueue[i].speed;
            pNew[i].data = m_pAnimationQueue[i].data;
        }
        delete [] m_pAnimationQueue;
        m_nAnimationCapacity += 8;
        m_pAnimationQueue = pNew;
    }
    unsigned long nStartDay,nStartTick;
    pClient = g_pAppManager->m_pClientExoApp;
    pClient->GetWorldTimer()->AddWorldTimes(nDay,nTick,0,nDelay,&nStartDay,&nStartTick);
    int i;
    for (i = m_nAnimationCount - 1; i >= 0; --i) {
        pClient = g_pAppManager->m_pClientExoApp;
        if (pClient->GetWorldTimer()->CompareWorldTimes(nStartDay,nStartTick,m_pAnimationQueue[i].day,m_pAnimationQueue[i].tick) != -1)
            break;
        m_pAnimationQueue[i+1].active = m_pAnimationQueue[i].active;
        m_pAnimationQueue[i+1].animation = m_pAnimationQueue[i].animation;
        m_pAnimationQueue[i+1].day = m_pAnimationQueue[i].day;
        m_pAnimationQueue[i+1].tick = m_pAnimationQueue[i].tick;
        m_pAnimationQueue[i+1].duration = m_pAnimationQueue[i].duration;
        m_pAnimationQueue[i+1].speed = m_pAnimationQueue[i].speed;
        m_pAnimationQueue[i+1].data = m_pAnimationQueue[i].data;
    }
    ++i;
    m_pAnimationQueue[i].active = 1;
    m_pAnimationQueue[i].animation = nAnimation;
    m_pAnimationQueue[i].day = nStartDay;
    m_pAnimationQueue[i].tick = nStartTick;
    m_pAnimationQueue[i].duration = nDuration;
    m_pAnimationQueue[i].speed = fSpeed;
    m_pAnimationQueue[i].data = pData;
    ++m_nAnimationCount;
}
