// Gob::AttachToObject (Gob vtable +0x50; missing from functions.csv): attach
// this object to a part of another one through one of four behaviours.
// Behavioural reference: src/units/1470/5ac096f97b-scene3d.cpp.
#include "g/gob.h"

class AttachBehavior : public GobAttachment {
public:
    AttachBehavior(Gob* pGob, Gob* pTarget, Part* pPart);          // 0x0049fb10
    char pad04[0x3c - 0x04];
};

class AttachTranslateBehavior : public GobAttachment {
public:
    AttachTranslateBehavior(Gob* pGob, Gob* pTarget, Part* pPart); // 0x0049fdc0
    char pad04[0x4c - 0x04];
};

class AttachRotateBehavior : public GobAttachment {
public:
    AttachRotateBehavior(Gob* pGob, Gob* pTarget, Part* pPart);    // 0x0049fe00
    char pad04[0x48 - 0x04];
};

class AttachNoOrientationBehavior : public GobAttachment {
public:
    AttachNoOrientationBehavior(Gob* pGob, Gob* pTarget, Part* pPart);  // 0x0049fe40
    char pad04[0x3c - 0x04];
};

__forceinline void Gob::SetScene(Scene* pScene)
{
    if (m_pScene == pScene)
        return;
    if (m_pScene)
        m_pScene->RemoveGob(this);
    m_pScene = pScene;
    if (pScene)
        pScene->AddGob(this);
    DoEvent("SetScene", 0);
}

// FUNCTION: SWKOTOR 0x00443820
GobAttachment* Gob::AttachToObject(Gob* pObject, char* szPart, int nMode)
{
    GobAttachment* pBehavior = 0;
    Part* pPart;
    if (pObject && (pPart = pObject->FindPart(szPart)) != 0) {
        pPart->GetWorldTransform(&m_vPosition, &m_qOrientation);
        switch (nMode) {
        case 0:
            pBehavior = new AttachBehavior(this, pObject, pPart);
            break;
        case 1:
            pBehavior = new AttachTranslateBehavior(this, pObject, pPart);
            break;
        case 2:
            pBehavior = new AttachRotateBehavior(this, pObject, pPart);
            break;
        case 3:
            pBehavior = new AttachNoOrientationBehavior(this, pObject, pPart);
            break;
        }
        SetAttachment(pBehavior);
        SetScene(pObject->m_pScene);
    } else {
        SetAttachment(0);
    }
    return pBehavior;
}

// FUNCTION: SWKOTOR 0x004439a0
// Missing from functions.csv. An unknown mode leaves pBehavior unset, as in
// the original.
GobAttachment* Gob::AttachToPart(Gob* pObject, Part* pPart, int nMode)
{
    GobAttachment* pBehavior;
    pPart->GetWorldTransform(&m_vPosition, &m_qOrientation);
    switch (nMode) {
    case 0:
        pBehavior = new AttachBehavior(this, pObject, pPart);
        break;
    case 1:
        pBehavior = new AttachTranslateBehavior(this, pObject, pPart);
        break;
    case 2:
        pBehavior = new AttachRotateBehavior(this, pObject, pPart);
        break;
    case 3:
        pBehavior = new AttachNoOrientationBehavior(this, pObject, pPart);
        break;
    }
    SetAttachment(pBehavior);
    SetScene(pObject->m_pScene);
    return pBehavior;
}
