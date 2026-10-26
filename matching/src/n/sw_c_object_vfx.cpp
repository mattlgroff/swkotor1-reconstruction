// CSWCObject model forwards and the visual-effect list at +0x6C
// (CExoLinkedList<CSWCVisualEffectOnObject>), nwcobject.o.
#include "n/sw_c_object.h"

// FUNCTION: SWKOTOR 0x0063C080
CAurObject* CSWCObject::GetModel(unsigned char nPart, int b)
{
    if (!m_pAnimationBase) {
        return 0;
    }
    return m_pAnimationBase->GetModel(nPart);
}

// FUNCTION: SWKOTOR 0x0063CB00
int CSWCObject::Hilite(int a, int b, int c, int d)
{
    if (!m_pAnimationBase) {
        return 0;
    }
    return m_pAnimationBase->Hilite(a, b, d);
}

// FUNCTION: SWKOTOR 0x0063CB30
int CSWCObject::UnHilite(int a)
{
    if (!m_pAnimationBase) {
        return 0;
    }
    return m_pAnimationBase->UnHilite(a);
}

// FUNCTION: SWKOTOR 0x0063D0A0
void CSWCObject::SetGroundNormal(const Vector& v)
{
    m_vGroundNormal = v;
    if (m_pAnimationBase) {
        Vector vUp(0.0f, 0.0f, 1.0f);
        m_pAnimationBase->SetGroundNormal(RotationArc(vUp, v));
    }
}

// FUNCTION: SWKOTOR 0x0063D8C0
void CSWCObject::AddFireForgetAnimation(unsigned short nAnim, void* pData)
{
    if (AsSWCCreature()) {
        AsSWCCreature()->AddFireForgetAnimation(nAnim, pData);
        return;
    }
    if (AsSWCDoor()) {
        AsSWCDoor()->PlayAnimationAction(nAnim, pData, 0);
        return;
    }
    if (AsSWCPlaceable()) {
        AsSWCPlaceable()->PlayAnimationAction(nAnim, pData, 0);
        return;
    }
    if (AsSWCTrigger()) {
        AsSWCTrigger()->PlayAnimation(nAnim, pData, 0);
        return;
    }
    AddFireForgetAnimation(nAnim, 500, 1.0f, pData, 0);
}

// FUNCTION: SWKOTOR 0x0063D970
int CSWCObject::AddVisualEffect(unsigned short nEffect, int nDuration, unsigned long nTarget,
                                unsigned long nNode, unsigned char nA, unsigned char nB,
                                const Vector& vOffset)
{
    if (nEffect == 0x3fc && HasVisualEffectApplied(0x3fc)) {
        return 1;
    }
    CSWCVisualEffectOnObject* pEffect = new CSWCVisualEffectOnObject;
    if (!pEffect) {
        return 0;
    }
    if (!pEffect->Load(nEffect, nDuration, m_idSelf, nTarget, nNode, nA, nB, vOffset)) {
        delete pEffect;
        return 0;
    }
    m_lVisualEffects.AddTail(pEffect);
    pEffect->StartVisualEffect();
    pEffect->ShowVisualEffect(0);
    pEffect->Detach();
    ActivateHardCodedVisualWithHighestPriority(pEffect->GetType(), pEffect);
    return 1;
}

// FUNCTION: SWKOTOR 0x0063DAA0
void CSWCObject::StopAllVisualEffects()
{
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    while (pEffect) {
        pEffect->StopVisualEffect(1);
        pEffect = m_lVisualEffects.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x0063DAE0
void CSWCObject::KillAllVisualEffects()
{
    while (!m_lVisualEffects.IsEmpty()) {
        CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.RemoveHead();
        if (pEffect) {
            pEffect->Reset();
            delete pEffect;
        }
    }
}

// FUNCTION: SWKOTOR 0x0063DB20
int CSWCObject::RefreshVisualEffects(int a)
{
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    while (pos) {
        if (!pEffect->m_bC0 && pEffect->m_nA8) {
            pEffect->DetachAll();
            pEffect->ShowVisualEffect(0);
        }
        pEffect = m_lVisualEffects.GetNext(pos);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0063DB90
void CSWCObject::ActivateHardCodedVisualWithHighestPriority(unsigned char nType,
                                                            CSWCVisualEffectOnObject* pNew)
{
    unsigned char nBest = 0xff;
    CSWCVisualEffectOnObject* pBest = 0;
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    while (pos) {
        if (!pEffect->m_bC0 && pEffect->GetType() == nType) {
            if (pEffect->m_nPriority == 0) {
                if (pEffect->m_bDC &&
                    !g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->GetIsDark()) {
                    if (pEffect->m_bAttached) {
                        pEffect->Detach();
                    }
                }
                else if (!pEffect->m_bAttached) {
                    pEffect->ReAttachVisualEffect();
                }
            }
            else if (pEffect->m_nPriority <= nBest) {
                nBest = pEffect->m_nPriority;
                pBest = pEffect;
            }
        }
        pEffect = m_lVisualEffects.GetNext(pos);
    }

    pEffect = m_lVisualEffects.GetHead();
    pos = m_lVisualEffects.GetHeadPos();
    while (pos) {
        if (!pEffect->m_bC0 && pEffect->GetType() == nType && pEffect->m_nPriority) {
            if (pEffect == pBest) {
                if (pEffect->GetType() == 4) {
                    if (pEffect->m_bDC &&
                        !g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->GetIsDark()) {
                        if (pEffect->m_bAttached && !pEffect->m_pBC) {
                            pEffect->StopVisualEffect(0);
                        }
                    }
                    else {
                        if (pEffect->m_pBC) {
                            pEffect->StartVisualEffect();
                        }
                        if (!pEffect->m_bAttached) {
                            pEffect->ReAttachVisualEffect();
                        }
                    }
                }
                else if (!pEffect->m_bAttached) {
                    pEffect->ReAttachVisualEffect();
                }
            }
            else if (pEffect->m_bAttached) {
                pEffect->Detach();
            }
        }
        pEffect = m_lVisualEffects.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x0063DD50
void CSWCObject::DetachVisualEffectOfSameType(CSWCVisualEffectOnObject* pNew)
{
    CExoLinkedListPosition pos = m_lVisualEffects.GetTailPos();
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetTail();
    while (pos) {
        if (pNew->GetType() != 7 && pEffect->GetType() == pNew->GetType() && pNew != pEffect &&
            pEffect->m_nPriority && !pEffect->m_bC0) {
            if (pEffect->m_bAttached) {
                pEffect->Detach();
            }
            return;
        }
        pEffect = m_lVisualEffects.GetPrev(pos);
    }
}

// FUNCTION: SWKOTOR 0x0063DDF0
int CSWCObject::HasVisualEffectApplied(int nEffect)
{
    CExoLinkedListPosition pos = m_lVisualEffects.GetTailPos();
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetTail();
    while (pos) {
        if (pEffect->m_nEffect == nEffect) {
            return 1;
        }
        pEffect = m_lVisualEffects.GetPrev(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0063DE50
float CSWCObject::GetVisualEffectAlpha()
{
    float fAlpha = -1.0f;
    unsigned char nBest = 0xff;
    CExoLinkedListPosition pos = m_lVisualEffects.GetTailPos();
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetTail();
    while (pos) {
        if (pEffect->GetType() == 0xd && !pEffect->m_bC0 && pEffect->m_bAttached &&
            pEffect->m_nPriority < nBest) {
            fAlpha = pEffect->m_fAlpha;
            nBest = pEffect->m_nPriority;
        }
        pEffect = m_lVisualEffects.GetPrev(pos);
    }
    return fAlpha;
}

// FUNCTION: SWKOTOR 0x0063E450
void CSWCObject::DetachVisualEffects()
{
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    while (pos) {
        if (!pEffect->m_bC0 && pEffect->m_nA8) {
            pEffect->Detach();
        }
        pEffect = m_lVisualEffects.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x0063E4B0
void CSWCObject::ReAttachVisualEffects()
{
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    while (pos) {
        if (!pEffect->m_bC0 && pEffect->m_nA8) {
            pEffect->ReAttachVisualEffect();
        }
        pEffect = m_lVisualEffects.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x0063F400
int CSWCObject::DeleteVisualEffect(unsigned short nEffect)
{
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    while (pos) {
        if (pEffect->m_nEffect == nEffect && !pEffect->m_bC0) {
            DetachVisualEffectOfSameType(pEffect);
            pEffect->StopVisualEffect(1);
            ActivateHardCodedVisualWithHighestPriority(pEffect->GetType(), 0);
            return 1;
        }
        pEffect = m_lVisualEffects.GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0063F490
void CSWCObject::UpdateVisualEffectsList()
{
    CExoLinkedListPosition pos = m_lVisualEffects.GetHeadPos();
    CSWCVisualEffectOnObject* pEffect = m_lVisualEffects.GetHead();
    while (pEffect) {
        pEffect->Update(m_nDeltaTime);
        ActivateHardCodedVisualWithHighestPriority(pEffect->GetType(), 0);
        if (pEffect->IsReady()) {
            if (!pEffect->m_bC8) {
                DetachVisualEffectOfSameType(pEffect);
                ActivateHardCodedVisualWithHighestPriority(pEffect->GetType(), 0);
            }
            CExoLinkedListPosition posNext = pos;
            CSWCVisualEffectOnObject* pNext = m_lVisualEffects.GetNext(posNext);
            m_lVisualEffects.Remove(pos);
            pEffect->DestroyModels();
            delete pEffect;
            pEffect = pNext;
            pos = posNext;
        }
        else {
            pEffect = m_lVisualEffects.GetNext(pos);
        }
    }
}
