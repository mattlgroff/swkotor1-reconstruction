// CSWCObject construction, destruction and the small vtable members that
// live between them in nwcobject.o (0x0063E530-0x0063E7F0, 0x0063F5C0).
#include "n/sw_c_object.h"

// FUNCTION: SWKOTOR 0x0063D7B0 SYMBOL: ??0?$CExoLinkedList@VCSWCVisualEffectOnObject@@@@QAE@XZ


// FUNCTION: SWKOTOR 0x0063E530
CSWCObject::CSWCObject(unsigned char nObjectType)
    : CGameObject(nObjectType, 0x7f000000)
{
    m_pArea = 0;
    m_pAnimationBase = 0;
    m_nF4 = 0;
    m_nCurrentAnimation = 0;
    m_nPendingAnimation = 0;
    m_nLoopingAnimation = 0;
    m_n64 = 0;
    m_nAnimationCapacity = 8;
    m_pAnimationQueue = new FireForgetAnimation[8];
    m_nFadeState = 1;
    m_nDesiredFadeState = 1;
    m_nAnimationCount = 0;
    m_n1C = 0;
    m_fAnimationSpeed = 1.0f;
    m_nFadeStartDay = 0;
    m_nFadeStartTime = 0;
    m_fFadeAlpha = 1.0f;
    m_bTimedFade = 0;
    m_n9C = 0;
    m_nFadeDuration = 0;
    m_nA0 = 0;
    m_n0C = 0;
    m_n10 = 0;
    m_n14 = 0;
    m_n18 = 0;
    m_pVocalization = 0;
    m_pStreamingVocalization = 0;
    m_nVocalizationType = 0;
    m_nC0 = 0;
    m_nC4 = 0;
    m_fA4 = 2.1f;
    m_nBA = 10000;
    m_oidBC = OBJECT_INVALID;
    m_bShadowBlob = 1;
    m_vGroundNormal = Vector(0.0f, 0.0f, 1.0f);
    m_n7C = 0;
    m_bNoServerLink = 0;
    m_nNameInfoTime = 0;
    m_n50 = 0;
    m_n54 = 0;
    m_pServerObject = 0;
    m_nPortraitId = 0xffff;
}

// FUNCTION: SWKOTOR 0x0063E730
CExoString CSWCObject::GetName()
{
    return CExoString(g_sEmpty);
}

// FUNCTION: SWKOTOR 0x0063E760
CAurObject* CSWCObject::GetCameraFollowPart()
{
    return m_pAnimationBase->GetModel(0xff);
}

// FUNCTION: SWKOTOR 0x0063E770
CResRef CSWCObject::GetPortrait(unsigned char n)
{
    return m_cPortrait.GetPortrait(n);
}

// FUNCTION: SWKOTOR 0x0063E790
unsigned short CSWCObject::GetPortraitId()
{
    return m_nPortraitId;
}

// FUNCTION: SWKOTOR 0x0063E7A0
int CSWCObject::UnknownD4()
{
    return 1;
}

// FUNCTION: SWKOTOR 0x0063E7B0
void CSWCObject::BarkString(int a, int b, int c, int d)
{
    BarkBubble(a, b, c, d);
}

// FUNCTION: SWKOTOR 0x0063E7C0
int CSWCObject::Unknown128(int a)
{
    return 0;
}

// FUNCTION: SWKOTOR 0x0063E7D0
void CSWCObject::ClientServerSyncHandler_Position(const Vector& v, int a, int b)
{
    SetPosition(v);
}

// FUNCTION: SWKOTOR 0x0063E7E0
void CSWCObject::ClientServerSyncHandler_Orientation(const Vector& v)
{
    SetOrientation(v);
}

// FUNCTION: SWKOTOR 0x0063E7F0
int CSWCObject::Unknown11C()
{
    return 0;
}

// FUNCTION: SWKOTOR 0x0063E800
CSWCObject::~CSWCObject()
{
    g_pAppManager->m_pClientExoApp->RemoveObjectFromHitcheckIgnoreList(m_idSelf);
    KillAllVisualEffects();
    CClientAIMaster* pAIMaster = g_pAppManager->m_pClientExoApp->GetClientAIMaster();
    pAIMaster->RemoveObject(this);
    if (m_pVocalization) {
        delete m_pVocalization;
        m_pVocalization = 0;
    }
    if (m_pStreamingVocalization) {
        delete m_pStreamingVocalization;
        m_pStreamingVocalization = 0;
    }
    if (m_pAnimationQueue) {
        delete[] m_pAnimationQueue;
        m_pAnimationQueue = 0;
    }
    if (m_pServerObject) {
        m_pServerObject->DetachFromClientObject();
        m_pServerObject = 0;
    }
}

// FUNCTION: SWKOTOR 0x0063F5C0 SYMBOL: ??_GCSWCObject@@UAEPAXI@Z
