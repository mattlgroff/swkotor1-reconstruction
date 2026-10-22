// animation runs, Gob events, LookAtAnimation and part lists
// (0x004818C0 .. 0x004885E0).

#include "e/mdlanim.h"

extern "C" int __cdecl _stricmp(const char*, const char*);

struct AnimEvent {
    float fTime;                // +0x00
    char szName[0x20];          // +0x04
};

class Animation {
public:
    char pad_0x0[8];
    char m_szName[0x20];        // +0x08
    MdlNode* m_pRootNode;       // +0x28
    char pad_0x2c[0x24];
    float m_fTimeScale;         // +0x50: also the animation length
    float m_fTransition;        // +0x54
    char pad_0x58[0x20];
    CExoArrayList<AnimEvent> m_lEvents;     // +0x78
    char pad_0x84[4];

    Animation(char* szName);    // 0x004851E0
};

class Model {
public:
    char pad_0x0[0x58];
    CExoArrayList<Animation*> m_lAnimations;    // +0x58
    Model* m_pSuperModel;       // +0x64
};

// Expanded three levels deep in the original.
#pragma inline_recursion(on)
#pragma inline_depth(3)

// FUNCTION: SWKOTOR 0x004822A0
Animation* FindAnimation(Model* pModel, char* szName)
{
    if (!szName)
        return 0;
    int i;
    for (i = 0; i < pModel->m_lAnimations.num; i++) {
        if (_stricmp(pModel->m_lAnimations[i]->m_szName, szName) == 0)
            return pModel->m_lAnimations[i];
    }
    if (pModel->m_pSuperModel)
        return FindAnimation(pModel->m_pSuperModel, szName);
    char szDefault[8] = "default";
    for (i = 0; i < pModel->m_lAnimations.num; i++) {
        if (_stricmp(pModel->m_lAnimations[i]->m_szName, szDefault) == 0)
            return pModel->m_lAnimations[i];
    }
    if (pModel->m_pSuperModel)
        return FindAnimation(pModel->m_pSuperModel, szDefault);
    return 0;
}

#pragma inline_recursion(off)
#pragma inline_depth()

// One playing animation in Gob::m_lRuns (0x24 bytes).
class AnimRun {
public:
    Animation* m_pAnimation;    // +0x00
    float m_fSpeed;             // +0x04
    int m_nFlags;               // +0x08
    float m_f0c;                // +0x0c
    float m_fTime;              // +0x10
    float m_f14;                // +0x14
    float m_fTransition;        // +0x18
    float m_fWeight;            // +0x1c
    int m_n20;                  // +0x20
    AnimRun(Animation* pAnimation) throw();     // 0x00481890
};

// Event track of one playing animation.
class OOAnimRun {
public:
    Animation* m_pAnimation;    // +0x00
    float m_fSpeed;             // +0x04
    float m_fTime;              // +0x08
    float m_f0c;                // +0x0c
    float m_f10;                // +0x10
    float m_fWeight;            // +0x14
    float m_f18;                // +0x18
    float m_fBlend;             // +0x1c
    float m_f20;                // +0x20
    int m_nFlags;               // +0x24
    float* m_pEventTimes;       // +0x28
    char** m_pEventNames;       // +0x2c
    int m_nNumEvents;           // +0x30

    OOAnimRun(Animation* pAnimation, char** pNames, float* pTimes, int nCount, int nFlags);
};

// FUNCTION: SWKOTOR 0x004818C0
OOAnimRun::OOAnimRun(Animation* pAnimation, char** pNames, float* pTimes, int nCount, int nFlags)
{
    m_pAnimation = pAnimation;
    m_fSpeed = 1.0f;
    m_fTime = 0.0f;
    m_f10 = 0.0f;
    m_f0c = 0.0f;
    m_fWeight = 1.0f;
    m_f18 = 0.0f;
    m_f20 = 0.0f;
    m_fBlend = 1.0f;
    m_pEventTimes = new float[nCount];
    m_pEventNames = new char*[nCount];
    for (int i = 0; i < nCount; i++) {
        m_pEventTimes[i] = pTimes[i] * pAnimation->m_fTimeScale;
        m_pEventNames[i] = pNames[i];
    }
    m_nNumEvents = nCount;
    m_nFlags = nFlags;
}

class MaxTree {
public:
    ~MaxTree();                 // 0x0043F730
};

class AnimTreeRun {
public:
    virtual ~AnimTreeRun();
    int m_n04;
    MaxTree* m_pTree;           // +0x08
};

#pragma inline_recursion(on)
#pragma inline_depth(1)

// Fires the events between fStart and fEnd (bit 1: backward, bit 2: from the
// start), splitting wrapped ranges at +-1e9.
// FUNCTION: SWKOTOR 0x004826B0
void AnimateEvents(Gob* pGob, Animation* pAnim, float fStart, float fEnd, int nFlags)
{
    if (nFlags & 2) {
        nFlags &= ~2;
        AnimateEvents(pGob, pAnim, -1e9f, 1e9f, nFlags);
        return;
    }
    if (nFlags & 1) {
        if (fStart < fEnd) {
            AnimateEvents(pGob, pAnim, fStart, -1e9f, nFlags);
            AnimateEvents(pGob, pAnim, 1e9f, fEnd, nFlags);
            return;
        }
        for (int i = pAnim->m_lEvents.num - 1; i >= 0; i--) {
            if (pAnim->m_lEvents.element[i].fTime >= fEnd && pAnim->m_lEvents.element[i].fTime < fStart)
                pGob->DoEvent(pAnim->m_lEvents.element[i].szName, 0);
        }
    }
    else {
        if (fStart > fEnd) {
            AnimateEvents(pGob, pAnim, fStart, 1e9f, nFlags);
            AnimateEvents(pGob, pAnim, -1e9f, fEnd, nFlags);
            return;
        }
        for (int i = 0; i < pAnim->m_lEvents.num; i++) {
            if (pAnim->m_lEvents.element[i].fTime <= fEnd && pAnim->m_lEvents.element[i].fTime > fStart)
                pGob->DoEvent(pAnim->m_lEvents.element[i].szName, 0);
        }
    }
}

#pragma inline_recursion(off)
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x00481DA0
AnimTreeRun::~AnimTreeRun()
{
    if (m_pTree)
        delete m_pTree;
}

extern int g_bEventTimeFilter;      // 0x00798B14

// FUNCTION: SWKOTOR 0x00486560
void Gob::DoEvent(char* szEvent, void* pData)
{
    int i = m_lHandlers.num - 1;
    GobWatch watch(this);
    while (watch.m_pGob && i >= 0) {
        GobEventHandler* pHandler = m_lHandlers.element[i];
        if (pHandler->szEvent[0] == 0 || pHandler->szEvent[0] == '*'
            || _stricmp(pHandler->szEvent, szEvent) == 0) {
            if (!g_bEventTimeFilter || pHandler->fTime > m_fTime
                || (pHandler->bAlways && CanHandleEvents()))
                pHandler->pfnCallback(this, szEvent, pData ? pData : pHandler->pDefaultData);
        }
        i--;
    }
}

class ModelRef {
public:
    void* m_p;
    ModelRef() { m_p = 0; }
    ~ModelRef();
    ModelRef& operator=(char* szName);      // 0x00447FA0
};

class AnimationBase {
public:
    unsigned char m_bActive;    // +0x04
    Animation* m_pAnimation;    // +0x08
    AnimRun* m_pRun;            // +0x0c
    AnimationBase() { m_pAnimation = 0; m_pRun = 0; m_bActive = 0; }
    ~AnimationBase();
};

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

class LookAtAnimation : public AnimationBase {
public:
    virtual ~LookAtAnimation();
    virtual unsigned char IsDone();     // +0x04
    virtual void Start();               // +0x08
    ModelRef m_rModel;          // +0x10
    int m_n14;                  // +0x14
    int m_n18;                  // +0x18
    Part* m_pTarget;            // +0x1c
    float m_fYaw;               // +0x20
    float m_fPitch;             // +0x24
    float m_fX;                 // +0x28
    float m_fY;                 // +0x2c
    float* m_pKey1;             // +0x30
    float* m_pKey0;             // +0x34
    unsigned char m_b38;        // +0x38

    LookAtAnimation(char* szModel, int a, int b, Part* pTarget, float fYaw, float fPitch, float fX, float fY);
    void BuildAnimation();
};

// FUNCTION: SWKOTOR 0x00487020
LookAtAnimation::LookAtAnimation(char* szModel, int a, int b, Part* pTarget, float fYaw, float fPitch,
                                 float fX, float fY)
{
    m_rModel = szModel;
    m_n18 = a;
    m_n14 = b;
    m_pTarget = pTarget;
    m_fYaw = ABS(fYaw) < 89.0f ? ABS(fYaw) : 89.0f;
    m_fPitch = ABS(fPitch) < 89.0f ? ABS(fPitch) : 89.0f;
    m_fX = ABS(fX);
    m_fY = ABS(fY);
    m_pKey1 = 0;
    m_pKey0 = 0;
    m_b38 = 0;
    BuildAnimation();
}

extern "C" int __cdecl sprintf(char*, const char*, ...);
AnimRun* GetCurrentRun(Gob* pGob);                  // 0x00484CA0
float GetAnimationWeight(Model* pModel, Animation* pAnim);  // 0x00484CE0

// FUNCTION: SWKOTOR 0x00485BD0
int Gob::PlayAnimation(char* szName, float fSpeed, int nFlags, float fStart)
{
    if (!szName || !*szName || !_stricmp("NULL", szName)) {
        int i = m_lRuns.num;
        while (i) {
            i--;
            if (m_pLookAt && m_pLookAt->m_pRun == m_lRuns.element[i]) {
                m_pLookAt->m_pRun = 0;
                delete m_pLookAt;
                m_pLookAt = 0;
            }
            delete m_lRuns.element[i];
        }
        m_lRuns.num = 0;
        return 1;
    }

    AnimRun* pRun = 0;
    if (!(nFlags & 0x40)) {
        for (int i = m_lRuns.num - 1; i >= 0; i--) {
            if (!_stricmp(m_lRuns.element[i]->m_pAnimation->m_szName, szName)) {
                pRun = m_lRuns.element[i];
                if (nFlags & 0x400) {
                    pRun->m_nFlags |= 0x401;
                    return 1;
                }
                break;
            }
        }
    }
    if (pRun) {
        m_lRuns.Remove(pRun);
    }
    else if (!_stricmp(szName, "current")) {
        pRun = GetCurrentRun(this);
        if (pRun)
            m_lRuns.Remove(pRun);
    }

    if (nFlags & 0x80) {
        if (!pRun)
            return 0;
        char szEvent[0x40];
        sprintf(szEvent, "done%s", pRun->m_pAnimation->m_szName);
        DoEvent(szEvent, 0);
        if (m_pLookAt && m_pLookAt->m_pRun == pRun) {
            m_pLookAt->m_pRun = 0;
            delete m_pLookAt;
            m_pLookAt = 0;
        }
        delete pRun;
        return 1;
    }

    Animation* pAnim = 0;
    Model* pModel;
    if (m_pAnimModel) {
        pAnim = FindAnimation(m_pAnimModel, szName);
        pModel = m_pAnimModel;
    }
    if (!pAnim) {
        pAnim = FindAnimation(m_pModel, szName);
        pModel = m_pModel;
    }
    if (fStart < 0.0f)
        fStart = 0.0f;
    if (pAnim) {
        while (fStart > pAnim->m_fTimeScale)
            fStart -= pAnim->m_fTimeScale;
    }
    if (!pRun) {
        if (!pAnim)
            return 0;
        pRun = new AnimRun(pAnim);
        if (fSpeed == 0.0f)
            pRun->m_fTime = 0.0f;
        if (fSpeed < 0.0f)
            pRun->m_fTime = pAnim->m_fTimeScale + 1e-7f;
        if (fSpeed > 0.0f)
            pRun->m_fTime = -1e-7f;
        if (fStart != 0.0f)
            pRun->m_fTime = fStart;
        if (!(nFlags & 2)) {
            pRun->m_f14 = 0.0f;
            pRun->m_fTransition = pRun->m_pAnimation->m_fTransition;
        }
    }
    pRun->m_fWeight = GetAnimationWeight(pModel, pAnim);
    pRun->m_n20 = 0;
    pRun->m_fSpeed = fSpeed;
    pRun->m_nFlags = nFlags;

    int nPos = 0;
    int bFront = nFlags & 8;
    if (bFront) {
        for (; nPos < m_lRuns.num; nPos++) {
            if (!(m_lRuns.element[nPos]->m_nFlags & 0x200))
                break;
        }
    }
    else {
        for (; nPos < m_lRuns.num; nPos++) {
            int nRunFlags = m_lRuns.element[nPos]->m_nFlags;
            if (!(nRunFlags & 8) && ((nFlags & 0x20) || !(nRunFlags & 0x20)))
                break;
        }
    }
    m_lRuns.Insert(pRun, nPos);
    if ((nFlags & 0x10) && m_lRuns.num > nPos + 1) {
        pRun->m_fTime = m_lRuns.element[nPos + 1]->m_fTime;
        return 1;
    }
    if (!bFront && !(nFlags & 0x20)) {
        char szEvent[0x100];
        while (m_lRuns.num > nPos + 1) {
            AnimRun* pOld = m_lRuns.element[nPos + 1];
            sprintf(szEvent, "done%s", pOld->m_pAnimation->m_szName);
            m_lRuns.Remove(pOld);
            DoEvent(szEvent, 0);
            delete pOld;
        }
        sprintf(szEvent, "start%s", pRun->m_pAnimation->m_szName);
        DoEvent(szEvent, 0);
    }
    return 1;
}

// The lookingParent chain's child lists grow inline here.
template <> inline void CExoArrayList<MdlNode*>::Allocate(int n)
{
    MdlNode** old = element;
    array_size = n;
    element = new MdlNode*[n];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

// FUNCTION: SWKOTOR 0x00486140
void LookAtAnimation::BuildAnimation()
{
    m_pAnimation = new Animation("LookAtAnimation");
    MdlNode* pLeaf = new MdlNode("looking");
    pLeaf->m_nId = m_pTarget->m_nNodeId;
    MdlNode* pNode = pLeaf;
    for (Part* pPart = m_pTarget->m_pParent; pPart; pPart = pPart->m_pParent) {
        MdlNode* pParent = new MdlNode("lookingParent");
        pNode->m_pParent = pParent;
        pParent->m_lChildren.Add(pNode);
        pParent->m_nId = pPart->m_nNodeId;
        pNode = pParent;
    }
    pNode->m_pParent = 0;
    pNode->m_pAnimation = m_pAnimation;
    m_pAnimation->m_pRootNode = pNode;

    CExoArrayList<float>& lData = pLeaf->m_lControllerData;
    NewController controller;
    controller.m_nTimeIndex = lData.num;
    lData.Add(0.0f);
    lData.Add(0.25f);
    controller.m_nDataIndex = lData.num;
    lData.Add(0.0f);
    lData.Add(0.0f);
    lData.Add(0.0f);
    lData.Add(1.0f);
    lData.Add(0.0f);
    lData.Add(0.0f);
    lData.Add(0.0f);
    lData.Add(1.0f);
    controller.m_nColumns = 4;
    controller.m_nFlags = 0;
    controller.m_nNumKeys = 2;
    controller.m_nType = 0x14;
    pLeaf->m_lControllers.Add(controller);
    m_pKey0 = &lData.element[controller.m_nDataIndex];
    m_pKey1 = &lData.element[controller.m_nDataIndex + 4];
    m_pAnimation->m_fTimeScale = m_fX;
    m_pAnimation->m_fTransition = m_fX;
}

// The original expands the recursion ten levels deep (then calls itself).
#pragma inline_recursion(on)
#pragma inline_depth(10)

// FUNCTION: SWKOTOR 0x004881F0
void MakeInlinePartList(Part* pPart, CExoArrayList<Part*>* pList)
{
    pList->Add(pPart);
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        MakeInlinePartList(pPart->m_lChildren[i], pList);
}

#pragma inline_recursion(off)
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x004884D0
int FindPartIndexDFS(Part* pRoot, char* szName)
{
    CExoArrayList<Part*> lParts;
    if (_stricmp("root", szName) == 0)
        return 0;
    MakeInlinePartList(pRoot, &lParts);
    for (int i = 0; i < lParts.num; i++) {
        if (_stricmp(lParts.element[i]->GetName(), szName) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x00481850
unsigned char LookAtAnimation::IsDone()
{
    return m_pRun->m_n20 == 0;
}

// FUNCTION: SWKOTOR 0x00481860
void LookAtAnimation::Start()
{
    m_bActive = 1;
}

// FUNCTION: SWKOTOR 0x00481870
void Gob::StartLookAt()
{
    if (m_pLookAt)
        m_pLookAt->Start();
}

// FUNCTION: SWKOTOR 0x00482250
void Gob::ReplayOverlay(char* szName)
{
    for (int i = 0; i < m_lRuns.num; i++) {
        if (m_lRuns.element[i]->m_nFlags & 0x200) {
            PlayAnimation(szName, 1.0f, 0x400, 0);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x00482640
void Gob::RemoveCallback(int nId)
{
    GobEventHandler* pHandler = (GobEventHandler*)nId;
    for (int i = 0; i < m_lHandlers.num; i++) {
        if (pHandler == m_lHandlers.element[i]) {
            m_lHandlers.DelIndex(i);
            delete pHandler;
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x00484C40
void Gob::AddLastRunFlags(int nFlags)
{
    int i = m_lRuns.num - 1;
    if (i >= 0)
        m_lRuns.element[i]->m_nFlags |= nFlags;
}

// FUNCTION: SWKOTOR 0x00484C70
void Gob::RemoveLastRunFlags(int nFlags)
{
    int i = m_lRuns.num - 1;
    if (i >= 0) {
        AnimRun* pRun = m_lRuns.element[i];
        if (pRun->m_nFlags && nFlags)
            pRun->m_nFlags -= nFlags;
    }
}

// FUNCTION: SWKOTOR 0x00484D20
AnimRun* Gob::FindRun(char* szName, float* pTime, float* pLength)
{
    AnimRun* pRun = 0;
    for (int i = 0; i < m_lRuns.num; i++) {
        if (!_stricmp(m_lRuns.element[i]->m_pAnimation->m_szName, szName)) {
            pRun = m_lRuns.element[i];
            break;
        }
    }
    if (pRun) {
        if (pTime)
            *pTime = pRun->m_fTime;
        if (pLength)
            *pLength = pRun->m_pAnimation->m_fTimeScale;
    }
    else {
        Animation* pAnim = 0;
        if (m_pAnimModel)
            pAnim = FindAnimation(m_pAnimModel, szName);
        if (!pAnim)
            pAnim = FindAnimation(m_pModel, szName);
        if (pTime)
            *pTime = -1.0f;
        if (pLength) {
            if (pAnim)
                *pLength = pAnim->m_fTimeScale;
            else
                *pLength = -1.0f;
        }
    }
    return pRun;
}

// FUNCTION: SWKOTOR 0x00484E40
int Gob::AddCallback(GobCallback pfn, char* szEvent, void* pData, float fTime, int bAlways)
{
    GobEventHandler* pHandler = new GobEventHandler(pfn, szEvent, pData, fTime, bAlways);
    m_lHandlers.Add(pHandler);
    return (int)pHandler;
}

// FUNCTION: SWKOTOR 0x00485130
void Gob::PlayOverlayAnimation(char* szName, char** pNames, float* pTimes, int nCount, int nFlags)
{
    Animation* pAnim = FindAnimation(m_pModel, szName);
    if (pAnim) {
        AnimRun* pRun = (AnimRun*)new OOAnimRun(pAnim, pNames, pTimes, nCount, nFlags);
        pRun->m_fTime = 0.0f;
        pRun->m_f14 = 0.0f;
        pRun->m_fTransition = pAnim->m_fTransition;
        pRun->m_nFlags = 0x209;
        m_lRuns.Insert(pRun, 0);
    }
}
