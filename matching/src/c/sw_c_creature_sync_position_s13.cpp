// CSWCCreature::ClientServerSyncHandler_Position, worker C s13
// (translation-unit-local layout). Creature override of vtable slot 0x12c;
// missing from functions.csv.

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);    // 0x00413350
};

struct CTwoDimArrays {
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nAppearanceCol_WALKDIST;               // 0x007A212C
extern int g_nAppearanceCol_RUNDIST;                // 0x007A2120

class CSWCCreatureAppearance {
public:
    float GetRunRate();                             // 0x006975D0

    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
};

class CExoSoundSource {
public:
    void SetPosition(const Vector& v, float fRadius);   // 0x005D59E0
};

class CExoStreamingSoundSource {
public:
    void SetProperty0x5C(const Vector& v);          // 0x005D62C0
};

class CAurAnimationBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual float GetAnimationLength(unsigned short nAnim);     // 0x38 (descriptive)
    virtual float GetAnimationTime(unsigned short nAnim);       // 0x3c (descriptive)
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50();
    virtual void SetAnimationSpeed(unsigned short nAnim, float fSpeed, int a, float fTime);  // 0x54 (descriptive)

    char pad_0x4[0xcc - 0x4];
    int m_bRunLooksAwful;                           // 0xcc (descriptive)
};

class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108();
    virtual void SetGroundNormal(const Vector& v);  // 0x10c
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128();
    virtual void ClientServerSyncHandler_Position(const Vector& v, unsigned long nAnimation, float fDelta);  // 0x12c

    unsigned short GetCurrentAnimation();           // 0x0063C060
    CAurAnimationBase* GetAnimationBase();          // 0x0063C070
    float GetAnimationLength(unsigned short nAnim); // 0x0063C0E0 (no zero extension at the call)
    int AnimationStationary(unsigned short nAnim);  // 0x0063C170
    int AnimationWalk(unsigned short nAnim);        // 0x0063C240
    int AnimationRun(unsigned short nAnim);         // 0x0063C2A0

    char pad_0x4[0x68 - 0x4];
    CAurAnimationBase* m_pAnimationBase;            // 0x68
    char pad_0x6c[0xba - 0x6c];
    unsigned short m_nBA;                           // 0xba
    char pad_0xbc[0xcc - 0xbc];
    CExoStreamingSoundSource* m_pStreamingVocalization;  // 0xcc
};

class CSWCCreature : public CSWCObject {
public:
    virtual void ClientServerSyncHandler_Position(const Vector& v, unsigned long nAnimation, float fDelta);  // 0x12c

    void MoveCreatureToPosition(Vector* pPosition); // 0x0060BDF0
    float GetMovementScale(float fRate);            // 0x0060BF60
    void TurnToDirection(Vector* pDirection);       // 0x00611850 (descriptive)

    char pad_0xd0[0x17c - 0xd0];
    CExoSoundSource* m_pLoopingSound;               // 0x17c
    char pad_0x180[0x1c4 - 0x180];
    Vector m_vDesiredDirection;                     // 0x1c4
    Vector m_vDesiredOrientation;                   // 0x1d0
    Vector m_vDesiredPosition;                      // 0x1dc
    char pad_0x1e8[0x1f8 - 0x1e8];
    float m_fMovementScale;                         // 0x1f8 (descriptive)
    char pad_0x1fc[0x20c - 0x1fc];
    float m_fWalkRate;                              // 0x20c (descriptive)
    float m_fRunRate;                               // 0x210 (descriptive)
    char pad_0x214[0x21c - 0x214];
    CSWCCreatureAppearance* m_pAppearance;          // 0x21c
};

// FUNCTION: SWKOTOR 0x00611F50
// NONMATCH (28 diff insns): only the last SetAnimationSpeed time argument; the
// original multiplies the current-animation length by (time / length), cl
// reassociates to (length / ...) * time or keeps a named length on the FPU.
void CSWCCreature::ClientServerSyncHandler_Position(const Vector& v, unsigned long nAnimation, float fDelta)
{
    m_vDesiredPosition = v;
    MoveCreatureToPosition((Vector*)&v);
    SetGroundNormal(Vector(0.0f, 0.0f, v.z));
    if (AnimationStationary(nAnimation)) {
        return;
    }
    float fDistance = 0.0f;
    float fRate;
    if (AnimationWalk(nAnimation)) {
        fRate = m_fWalkRate;
        g_pRules->m_p2DArrays->m_pAppearanceTable->GetFLOATEntry(
            m_pAppearance->m_nAppearanceType, g_nAppearanceCol_WALKDIST, &fDistance);
    } else if (AnimationRun(nAnimation)) {
        fRate = m_fRunRate;
        if (m_pAppearance->GetRunRate() > fRate) {
            GetAnimationBase()->m_bRunLooksAwful = 1;
        } else {
            GetAnimationBase()->m_bRunLooksAwful = 0;
        }
        g_pRules->m_p2DArrays->m_pAppearanceTable->GetFLOATEntry(
            m_pAppearance->m_nAppearanceType, g_nAppearanceCol_RUNDIST, &fDistance);
    }
    fDistance *= 1000.0f;
    float fScale = GetMovementScale(fRate);
    m_fMovementScale = fScale;
    float fSpeed;
    {
        float fLength = GetAnimationLength(GetCurrentAnimation());
        if (fDistance != 0.0f && fLength != 0.0f) {
            fSpeed = fRate * 0.001f / (fDistance / fLength);
        } else {
            fSpeed = 0.0f;
        }
    }
    if (fDelta < 0.5f && nAnimation == 0x2714) {
        float fLength = m_pAnimationBase->GetAnimationLength(m_nBA);
        g_pRules->m_p2DArrays->m_pAppearanceTable->GetFLOATEntry(
            m_pAppearance->m_nAppearanceType, g_nAppearanceCol_WALKDIST, &fDistance);
        fDistance *= 1000.0f;
        if (fLength != 0.0f && fDistance != 0.0f) {
            fSpeed = fRate * 0.001f / (fDistance / fLength);
        } else {
            fSpeed = 0.0f;
        }
        float fTime = m_pAnimationBase->GetAnimationTime(GetCurrentAnimation());
        if (fTime > 0.0f) {
            CAurAnimationBase* pAnim = m_pAnimationBase;
            fTime /= pAnim->GetAnimationLength(GetCurrentAnimation());
            m_pAnimationBase->SetAnimationSpeed(m_nBA, fSpeed * fScale, 0, fTime * fLength * 0.001f);
        } else {
            m_pAnimationBase->SetAnimationSpeed(m_nBA, fSpeed * fScale, 0, 0.0f);
        }
    } else {
        if (m_pAnimationBase->m_bRunLooksAwful == 1) {
            float fLength = m_pAnimationBase->GetAnimationLength(m_nBA);
            g_pRules->m_p2DArrays->m_pAppearanceTable->GetFLOATEntry(
                m_pAppearance->m_nAppearanceType, g_nAppearanceCol_WALKDIST, &fDistance);
            fDistance *= 1000.0f;
            if (fLength != 0.0f && fDistance != 0.0f) {
                fSpeed = fRate * 0.001f / (fDistance / fLength);
            } else {
                fSpeed = 0.0f;
            }
        }
        float fTime = m_pAnimationBase->GetAnimationTime(m_nBA);
        if (fTime > 0.0f) {
            float fLength = m_pAnimationBase->GetAnimationLength(m_nBA);
            CAurAnimationBase* pAnim = m_pAnimationBase;
            float fCurLength = pAnim->GetAnimationLength(GetCurrentAnimation());
            m_pAnimationBase->SetAnimationSpeed(GetCurrentAnimation(), fSpeed * fScale, 0,
                fTime / fLength * fCurLength * 0.001f);
        } else {
            m_pAnimationBase->SetAnimationSpeed(GetCurrentAnimation(), fSpeed * fScale, 0, 0.0f);
        }
    }
    if (m_pLoopingSound) {
        m_pLoopingSound->SetPosition(v, 1.5f);
    }
    if (m_pStreamingVocalization) {
        m_pStreamingVocalization->SetProperty0x5C(v);
    }
    if (AnimationStationary(nAnimation)) {
        TurnToDirection(&m_vDesiredOrientation);
    } else {
        TurnToDirection(&m_vDesiredDirection);
    }
}
