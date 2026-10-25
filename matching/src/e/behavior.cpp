// CAurBehavior and its attach / camera-follow subclasses
// (0x0049EB40 .. 0x0049FFF0).

#include "e/mdlanim.h"

extern "C" int __cdecl _stricmp(const char* a, const char* b);

char* MsgGetWord(char* szMsg);                       // 0x00463530
void MsgParseFloat(char* szText, float* pOut);       // 0x004635C0
void MsgParseInt(char* szText, int* pOut);           // 0x004635E0
void MsgParseVector(char* szText, Vector* pOut);     // 0x00463570

Vector operator*(const Quaternion& q, const Vector& v);             // 0x004A9BB0
Quaternion operator*(const Quaternion& a, const Quaternion& b);     // 0x004A9AC0
Quaternion EulerToQuaternion(float fA, float fB, float fC);         // 0x004ACAC0

class CApp {
public:
    VSLOTS16(a0) VSLOTS16(a1) VSLOTS4(a2a) VSLOTS4(a2b)
    virtual void a2c0(); virtual void a2c1();
    virtual unsigned char IsRunning();              // +0xa8
};
extern CApp* g_pApp;                                // 0x007B92A0

// Heap / member safe pointer to a Gob: registers its slot in the Gob's
// watcher list, the Gob nulls it when it dies.
class GobRef {
public:
    Gob* m_p;

    GobRef() { m_p = 0; }
    GobRef(Gob* p)
    {
        m_p = p;
        if (p)
            p->m_lWatchers.Add(&m_p);
    }
    ~GobRef()
    {
        if (m_p) {
            CExoArrayList<Gob**>& l = m_p->m_lWatchers;
            l.Remove(&m_p);
        }
    }
    GobRef& operator=(Gob* p);                      // 0x00447FA0
    operator Gob*() const { return m_p; }
    Gob* operator->() const { return m_p; }
};

// Camera offset: a plain triple with the implicit (block) copy.
class Offset {
public:
    float x, y, z;
    Offset(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class AttachHook {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void Apply(Vector* pPosition, Quaternion* pOrientation);   // +0x0c
};

class CAurBehavior {
public:
    CExoArrayList<GobRef*> m_lDependencies;     // +0x04
    Gob* m_pGob;                                // +0x10

    CAurBehavior(Gob* pGob);
    virtual ~CAurBehavior();
    virtual void UpdateScenes();                // +0x04
    virtual int Control(float fDelta);          // +0x08
    virtual void Message(char* szMsg);          // +0x0c
    virtual int GetType();                      // +0x10

    void AddDependency(Gob* pGob);
    void RemoveDependency(Gob* pGob);
};

class CAurBehaviorAttach : public CAurBehavior {
public:
    GobRef m_pTarget;                           // +0x14
    AttachHook* m_pHook;                        // +0x18
    int m_n1c;                                  // +0x1c
    int m_bDieWithoutParent;                    // +0x20
    int m_nUpdateScenesId;                      // +0x24
    int m_nUpdatePositionId;                    // +0x28
    int m_nColorShiftId;                        // +0x2c
    int m_nIlluminationId;                      // +0x30
    int m_nRenderId;                            // +0x34
    int m_nViewVolumeId;                        // +0x38

    CAurBehaviorAttach(Gob* pGob, Gob* pTarget, AttachHook* pHook);
    virtual ~CAurBehaviorAttach();
    virtual void UpdateScenes();
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void UpdatePosition();              // +0x20

    Vector GetTargetPosition();
};

class CAurBehaviorAttachTranslate : public CAurBehaviorAttach {
public:
    Vector m_vTranslation;                      // +0x3c
    int m_bWorldSpace;                          // +0x48

    CAurBehaviorAttachTranslate(Gob* pGob, Gob* pTarget, AttachHook* pHook);
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();
};

class CAurBehaviorAttachRotate : public CAurBehaviorAttach {
public:
    Vector m_vRotation;                         // +0x3c

    CAurBehaviorAttachRotate(Gob* pGob, Gob* pTarget, AttachHook* pHook);
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();
};

class CAurBehaviorAttachNoOrientation : public CAurBehaviorAttach {
public:
    CAurBehaviorAttachNoOrientation(Gob* pGob, Gob* pTarget, AttachHook* pHook);
    virtual int Control(float fDelta);
    virtual int GetType();
};

class CAurBehaviorCameraFollow : public CAurBehavior {
public:
    GobRef m_pTarget;                           // +0x14
    Vector m_vTargetPosition;                   // +0x18
    Offset m_vOffset;                           // +0x24
    float m_fDist;                              // +0x30
    float m_fSpeed;                             // +0x34
    float m_fSpring;                            // +0x38
    float m_fYaw;                               // +0x3c
    float m_fTurn;                              // +0x40
    float m_fPitch;                             // +0x44
    float m_fTilt;                              // +0x48
    float m_fClampYaw;                          // +0x4c
    float m_fMinYaw;                            // +0x50
    float m_fMaxYaw;                            // +0x54
    float m_fClampPitch;                        // +0x58
    float m_fMinPitch;                          // +0x5c
    float m_fMaxPitch;                          // +0x60
    float m_fClampDist;                         // +0x64
    float m_fMinDist;                           // +0x68
    float m_fMaxDist;                           // +0x6c

    CAurBehaviorCameraFollow(Gob* pGob, Gob* pTarget);
    virtual ~CAurBehaviorCameraFollow();
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();
};

void UpdateDependentPosition(Gob* pGob, char* szEvent, void* pData);    // 0x0049EB90
void PropagateColorShift(Gob* pGob, char* szEvent, void* pData);        // 0x0049EBB0
void PropagateIllumination(Gob* pGob, char* szEvent, void* pData);      // 0x0049EC00
void PropagateRender(Gob* pGob, char* szEvent, void* pData);            // 0x0049EC50
void PropagateViewVolume(Gob* pGob, char* szEvent, void* pData);        // 0x0049EC90

// FUNCTION: SWKOTOR 0x0049EB40
void UpdateScenesAttach(Gob* pGob, char* szEvent, void* pData)
{
    CAurBehaviorAttach* pBehavior = (CAurBehaviorAttach*)pData;
    if (!_stricmp(szEvent, "UpdateScenes")) {
        pBehavior->UpdateScenes();
        return;
    }
    if (pBehavior->m_bDieWithoutParent && !_stricmp(szEvent, "Dying")) {
        if (pBehavior->m_pGob)
            delete pBehavior->m_pGob;
    }
}

// FUNCTION: SWKOTOR 0x0049EB90
void UpdateDependentPosition(Gob* pGob, char* szEvent, void* pData)
{
    if (!_stricmp(szEvent, "UpdateDependentPosition"))
        ((CAurBehaviorAttach*)pData)->UpdatePosition();
}

// FUNCTION: SWKOTOR 0x0049EBB0
void PropagateColorShift(Gob* pGob, char* szEvent, void* pData)
{
    if (!_stricmp(szEvent, "propagatecolorshift"))
        ((Gob*)pData)->SetColorShift(pGob->m_cColorShift, pGob->m_nColorShiftMode, 1);
}

// FUNCTION: SWKOTOR 0x0049EC00
void PropagateIllumination(Gob* pGob, char* szEvent, void* pData)
{
    if (!_stricmp(szEvent, "propagateillumination"))
        ((Gob*)pData)->SetIllumination(pGob->m_cIllumination, 1);
}

// FUNCTION: SWKOTOR 0x0049EC50
void PropagateRender(Gob* pGob, char* szEvent, void* pData)
{
    if (!_stricmp(szEvent, "propagaterender")) {
        Gob* pChild = (Gob*)pData;
        if (pChild->m_pAttachment)
            pChild->m_pAttachment->Render(0);
        pChild->Render(1);
    }
}

// FUNCTION: SWKOTOR 0x0049EC90
void PropagateViewVolume(Gob* pGob, char* szEvent, void* pData)
{
    if (!_stricmp(szEvent, "propagateviewvolume")) {
        if (pGob->CanHandleEvents())
            ((Gob*)pData)->UpdateViewVolume();
    }
}

// FUNCTION: SWKOTOR 0x0049ECD0
void CAurBehaviorAttach::UpdatePosition()
{
    Gob* pGob = m_pGob;
    m_pHook->Apply(&pGob->m_vPosition, &pGob->m_qOrientation);
    pGob->DoEvent("UpdateDependentPosition", 0);
}

// FUNCTION: SWKOTOR 0x0049ED00
void CAurBehaviorAttach::Message(char* szMsg)
{
    if (!_stricmp("diewithoutparent", MsgGetWord(szMsg)))
        MsgParseInt(szMsg + 16, &m_bDieWithoutParent);
}

// FUNCTION: SWKOTOR 0x0049ED40
void CAurBehaviorAttachTranslate::Message(char* szMsg)
{
    if (!_stricmp("diewithoutparent", MsgGetWord(szMsg)))
        MsgParseInt(szMsg + 16, &m_bDieWithoutParent);
    if (!_stricmp("m_vTranslation", MsgGetWord(szMsg)))
        MsgParseVector(szMsg + 14, &m_vTranslation);
}

// FUNCTION: SWKOTOR 0x0049EDA0
void CAurBehaviorAttachRotate::Message(char* szMsg)
{
    if (!_stricmp("diewithoutparent", MsgGetWord(szMsg)))
        MsgParseInt(szMsg + 16, &m_bDieWithoutParent);
    if (!_stricmp("m_vRotation", MsgGetWord(szMsg)))
        MsgParseVector(szMsg + 11, &m_vRotation);
}

// FUNCTION: SWKOTOR 0x0049EE00
void CAurBehaviorCameraFollow::Message(char* szMsg)
{
    if (!_stricmp("turn", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 4, &m_fTurn);
    if (!_stricmp("spring", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 6, &m_fSpring);
    if (!_stricmp("dist", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 4, &m_fDist);
    if (!_stricmp("offset", MsgGetWord(szMsg)))
        MsgParseVector(szMsg + 6, (Vector*)&m_vOffset);
    if (!_stricmp("yaw", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 3, &m_fYaw);
    if (!_stricmp("pitch", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 5, &m_fPitch);
    if (!_stricmp("tilt", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 4, &m_fTilt);
    if (!_stricmp("speed", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 5, &m_fSpeed);
    if (!_stricmp("minyaw", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 6, &m_fMinYaw);
    if (!_stricmp("maxyaw", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 6, &m_fMaxYaw);
    if (!_stricmp("minpitch", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 8, &m_fMinPitch);
    if (!_stricmp("maxpitch", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 8, &m_fMaxPitch);
    if (!_stricmp("mindist", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 7, &m_fMinDist);
    if (!_stricmp("maxdist", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 7, &m_fMaxDist);
    if (!_stricmp("clampyaw", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 8, &m_fClampYaw);
    if (!_stricmp("clamppitch", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 10, &m_fClampPitch);
    if (!_stricmp("clampdist", MsgGetWord(szMsg)))
        MsgParseFloat(szMsg + 9, &m_fClampDist);
}

// FUNCTION: SWKOTOR 0x0049F0C0
void CAurBehaviorAttach::UpdateScenes()
{
    if (m_pTarget)
        m_pGob->SetScene(m_pTarget->m_pScene);
}

// NONMATCH (8 insns): the original forms &target->m_vPosition first and
// copies through esi/edx (push esi); ours copies straight from ecx+0x78.
// FUNCTION: SWKOTOR 0x0049F0E0
Vector CAurBehaviorAttach::GetTargetPosition()
{
    return m_pTarget->m_vPosition;
}

// FUNCTION: SWKOTOR 0x0049F110
CAurBehavior::CAurBehavior(Gob* pGob)
{
    m_pGob = pGob;
}

// FUNCTION: SWKOTOR 0x0049F130
void CAurBehavior::AddDependency(Gob* pGob)
{
    m_lDependencies.Add(new GobRef(pGob));
}

// NONMATCH (4 insns): DelIndex count lands in edx in the original, ebx here.
// FUNCTION: SWKOTOR 0x0049F1F0
void CAurBehavior::RemoveDependency(Gob* pGob)
{
    for (int i = 0; i < m_lDependencies.num; i++) {
        if (m_lDependencies.element[i]->m_p == pGob) {
            delete m_lDependencies.element[i];
            m_lDependencies.DelIndex(i);
            return;
        }
    }
}

// NONMATCH (38 insns): the original loads m_pGob straight into edi for the
// local GobRef (array_size read before num), and the else branch keeps the
// null tests of `delete pDep` (ours folds them: pDep->m_p is known 0).
// FUNCTION: SWKOTOR 0x0049F270
int CAurBehavior::Control(float fDelta)
{
    GobRef pGob(m_pGob);
    for (int i = m_lDependencies.num - 1; i >= 0; i--) {
        GobRef* pDep = m_lDependencies.element[i];
        if (*pDep) {
            (*pDep)->Control(fDelta);
        }
        else {
            delete pDep;
            m_lDependencies.DelIndex(i);
        }
    }
    if (pGob && pGob->GetBehavior(-1) == this)
        return 1;
    return 0;
}

// NONMATCH (19 insns): the original caches m_pGob in ebp before the local
// GobRef and keeps the ref in a different stack slot; ours uses esi.
// FUNCTION: SWKOTOR 0x0049F3B0
int CAurBehaviorAttach::Control(float fDelta)
{
    unsigned char bRun = 1;
    if (g_pApp && !g_pApp->IsRunning())
        bRun = 0;
    GobRef pRef(m_pGob);
    Gob* pGob = pRef.m_p;
    if (bRun && !CAurBehavior::Control(fDelta))
        return 0;
    if (pRef->GetBehavior(-1) == this) {
        if (!m_pTarget) {
            if (m_bDieWithoutParent) {
                if (m_pGob)
                    delete m_pGob;
            }
            else {
                pGob->Detach(0);
            }
        }
        else {
            m_pHook->Apply(&pGob->m_vPosition, &pGob->m_qOrientation);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0049F4F0
int CAurBehaviorAttachTranslate::Control(float fDelta)
{
    int nResult = CAurBehaviorAttach::Control(fDelta);
    if (nResult) {
        if (m_bWorldSpace)
            m_pGob->SetPosition(m_pGob->GetPosition() + m_vTranslation);
        else
            m_pGob->SetPosition(m_pGob->GetPosition() + m_pGob->GetOrientation() * m_vTranslation);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x0049F5F0
int CAurBehaviorAttachRotate::Control(float fDelta)
{
    int nResult = CAurBehaviorAttach::Control(fDelta);
    if (nResult)
        m_pGob->SetOrientation(m_pGob->GetOrientation()
                               * EulerToQuaternion(m_vRotation.z, m_vRotation.x, m_vRotation.y));
    return nResult;
}

// FUNCTION: SWKOTOR 0x0049F670
int CAurBehaviorAttachNoOrientation::Control(float fDelta)
{
    Quaternion qOrientation = m_pGob->GetOrientation();
    int nResult = CAurBehaviorAttach::Control(fDelta);
    if (nResult)
        m_pGob->SetOrientation(qOrientation);
    return nResult;
}

// FUNCTION: SWKOTOR 0x0049FA70
CAurBehavior::~CAurBehavior()
{
    int nCount = m_lDependencies.num;
    for (int i = 0; i < nCount; i++)
        delete m_lDependencies.element[i];
    if (m_lDependencies.element) {
        delete[] m_lDependencies.element;
        m_lDependencies.element = 0;
    }
    m_lDependencies.array_size = 0;
    m_lDependencies.num = 0;
}

// FUNCTION: SWKOTOR 0x0049FB10
CAurBehaviorAttach::CAurBehaviorAttach(Gob* pGob, Gob* pTarget, AttachHook* pHook)
    : CAurBehavior(pGob)
{
    AddDependency(pTarget);
    m_pTarget = pTarget;
    m_pHook = pHook;
    m_nUpdateScenesId = 0;
    m_nUpdatePositionId = 0;
    m_nColorShiftId = 0;
    m_nIlluminationId = 0;
    m_nRenderId = 0;
    m_nViewVolumeId = 0;
    m_n1c = 0;
    m_bDieWithoutParent = 0;
    Gob* pChild = m_pGob;
    pChild->SetScene(0);
    Gob* pParent = m_pTarget;
    pChild->SetScene(pParent->m_pScene);
    m_pHook->Apply(&pChild->m_vPosition, &pChild->m_qOrientation);
    pChild->DoEvent("UpdateDependentPosition", 0);
    pParent->AttachChild(pChild);
    if (pChild->m_fScale / pChild->m_fBaseScale != pParent->m_fScale)
        pChild->SetScale(pChild->m_fBaseScale * pParent->m_fScale, 0);
    m_nUpdateScenesId = m_pTarget->AddCallback(UpdateScenesAttach, "*", this, 9999.0f, 0);
    m_nUpdatePositionId = m_pTarget->AddCallback(UpdateDependentPosition, "UpdateDependentPosition", this, 9999.0f, 0);
    m_nColorShiftId = m_pTarget->AddCallback(PropagateColorShift, "propagatecolorshift", m_pGob, 9999.0f, 0);
    m_nIlluminationId = m_pTarget->AddCallback(PropagateIllumination, "propagateillumination", m_pGob, 9999.0f, 0);
    m_nRenderId = m_pTarget->AddCallback(PropagateRender, "propagaterender", m_pGob, 9999.0f, 0);
    m_nViewVolumeId = m_pTarget->AddCallback(PropagateViewVolume, "propagateviewvolume", m_pGob, 9999.0f, 0);
}

// FUNCTION: SWKOTOR 0x0049FCD0
int CAurBehaviorAttach::GetType()
{
    return 1001;
}

// FUNCTION: SWKOTOR 0x0049FCE0
CAurBehaviorAttach::~CAurBehaviorAttach()
{
    Gob* pTarget = m_pTarget;
    if (pTarget) {
        if (m_pGob) {
            CAurBehaviorAttach* pAttach;
            if (m_pGob->m_pAttachment && (pAttach = m_pGob->m_pAttachment->GetAttachBehavior()) != 0) {
                if ((Gob*)pAttach->m_pTarget != (Gob*)m_pTarget)
                    pTarget->DetachChild(m_pGob);
            }
            else {
                pTarget->DetachChild(m_pGob);
            }
        }
        pTarget->RemoveCallback(m_nUpdateScenesId);
        pTarget->RemoveCallback(m_nUpdatePositionId);
        pTarget->RemoveCallback(m_nColorShiftId);
        pTarget->RemoveCallback(m_nIlluminationId);
        pTarget->RemoveCallback(m_nRenderId);
        pTarget->RemoveCallback(m_nViewVolumeId);
    }
}

// FUNCTION: SWKOTOR 0x004A0030
void __fastcall thunk_FUN_0049fce0(void* pThis, void*)
{
	((CAurBehaviorAttach*)pThis)->CAurBehaviorAttach::~CAurBehaviorAttach();
}

// FUNCTION: SWKOTOR 0x004A0010
void* __fastcall behavior_attach_scalar_delete_004a0010(void* pThis, void*, unsigned int flags)
{
    thunk_FUN_0049fce0(pThis, 0);
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}

// FUNCTION: SWKOTOR 0x0049FDC0
CAurBehaviorAttachTranslate::CAurBehaviorAttachTranslate(Gob* pGob, Gob* pTarget, AttachHook* pHook)
    : CAurBehaviorAttach(pGob, pTarget, pHook), m_vTranslation(0.0f, 0.0f, 0.0f)
{
}

// FUNCTION: SWKOTOR 0x0049FDF0
int CAurBehaviorAttachTranslate::GetType()
{
    return 1002;
}

// FUNCTION: SWKOTOR 0x0049FE00
CAurBehaviorAttachRotate::CAurBehaviorAttachRotate(Gob* pGob, Gob* pTarget, AttachHook* pHook)
    : CAurBehaviorAttach(pGob, pTarget, pHook), m_vRotation(0.0f, 0.0f, 0.0f)
{
}

// FUNCTION: SWKOTOR 0x0049FE30
int CAurBehaviorAttachRotate::GetType()
{
    return 1003;
}

// FUNCTION: SWKOTOR 0x0049FE40
CAurBehaviorAttachNoOrientation::CAurBehaviorAttachNoOrientation(Gob* pGob, Gob* pTarget, AttachHook* pHook)
    : CAurBehaviorAttach(pGob, pTarget, pHook)
{
}

// FUNCTION: SWKOTOR 0x0049FE70
int CAurBehaviorAttachNoOrientation::GetType()
{
    return 1008;
}

// FUNCTION: SWKOTOR 0x0049FE80
CAurBehaviorCameraFollow::CAurBehaviorCameraFollow(Gob* pGob, Gob* pTarget)
    : CAurBehavior(pGob), m_vTargetPosition(0.0f, 0.0f, 0.0f), m_vOffset(0.0f, 0.0f, 0.0f)
{
    AddDependency(pTarget);
    m_pTarget = pTarget;
    m_vTargetPosition = pTarget->GetPosition();
    m_vOffset = Offset(0.0f, 0.0f, 1.0f);
    m_fYaw = 0.0f;
    m_fTurn = 0.0f;
    m_fTilt = 0.0f;
    m_fSpeed = 0.0f;
    m_fMinYaw = 0.0f;
    m_fClampYaw = 0.0f;
    m_fMinPitch = 0.0f;
    m_fClampPitch = 0.0f;
    m_fMinDist = 0.0f;
    m_fClampDist = 0.0f;
    m_fMaxYaw = 360.0f;
    m_fMaxPitch = 360.0f;
    m_fSpring = 1.0f;
    m_fDist = 8.0f;
    m_fPitch = 45.0f;
    m_fMaxDist = 10.0f;
}

// FUNCTION: SWKOTOR 0x0049FF90
int CAurBehaviorCameraFollow::GetType()
{
    return 1005;
}

// FUNCTION: SWKOTOR 0x0049FFA0
CAurBehaviorCameraFollow::~CAurBehaviorCameraFollow()
{
}

// FUNCTION: SWKOTOR 0x004A0040
void* __fastcall behavior_camera_follow_scalar_delete_004a0040(void* pThis, void*, unsigned int flags)
{
    CAurBehaviorCameraFollow* pBehavior = (CAurBehaviorCameraFollow*)pThis;
    pBehavior->CAurBehaviorCameraFollow::~CAurBehaviorCameraFollow();
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}

// FUNCTION: SWKOTOR 0x0049FFD0 SYMBOL: ??_GCAurBehavior@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0049FFF0 SYMBOL: ??_GCAurBehaviorAttach@@UAEPAXI@Z
