// Worker L matching headers: the mini-game tracking behaviours
// (swmgbehavior.o): CSWMGBehaviorTrack (vtable 0x00757FB8, type 0xAAAC) and
// CSWMGBehaviorTrackAndFire (vtable 0x00757FDC, type 0xAAAA). Both are
// CAurBehaviorAttach subclasses; the base layout follows e/behavior.cpp.
#ifndef L_SW_MG_BEHAVIOR_H
#define L_SW_MG_BEHAVIOR_H

#ifdef L_MG_BEHAVIOR_DTORS
#define L_MGB_VTABLE __declspec(novtable)
#else
#define L_MGB_VTABLE
#endif

class CAurObject;
class CAurPart;

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    void Remove(T t) throw();                       // 0x00445080
};

class Vector;
class Quaternion;

class Gob {
public:
    virtual ~Gob();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58();
    virtual Vector SetPosition(Vector vPosition);   // 0x5c
    virtual void v60();
    virtual Vector GetPosition();                   // 0x64
    virtual Quaternion GetOrientation();            // 0x68
    CExoArrayList<Gob**> m_lWatchers;               // 0x04
};

// Safe pointer to a Gob (registers itself in the Gob's watcher list).
class GobRef {
public:
    Gob* m_p;
    GobRef() { m_p = 0; }
    ~GobRef()
    {
        if (m_p) {
            m_p->m_lWatchers.Remove(&m_p);
        }
    }
    GobRef& operator=(CAurObject* p);               // 0x00447FA0
};

class CSWMGTargettingInfo {
public:
    CSWMGTargettingInfo();                          // 0x00673A30
    float m_fHorizSpread;                           // 0x00
    float m_fVertSpread;                            // 0x04
    float m_fSensingRadius;                         // 0x08
    float m_fInaccuracy;                            // 0x0c
};


class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    float x, y, z;
};

inline Vector operator*(float f, const Vector& v)
{
    return Vector(f * v.x, f * v.y, f * v.z);
}

class Quaternion {
public:
    Vector GetYAxis() const;                        // 0x0044F420
    float w, x, y, z;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    int IsValid() const;                            // 0x00406030
    char* GetResRef() const;                        // 0x00641DB0 (returns this)
    CResRef operator=(const CResRef& r);            // 0x00406120
    char m_resRef[16];
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* szName, float fSpeed, int nFlags, float fStart);   // 0x18
    virtual void v1C(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual Vector GetPosition();                   // 0x64
};

class CExoSound {
public:
    int PlayOneShotSound(const CResRef& cRes, unsigned char nGroup, int a, unsigned char nVolume, int b,
                         int c);                    // 0x005D5E00
    int Play3DOneShotSound(const CResRef& cRes, Vector vPos, float fDelay, unsigned char nGroup,
                           unsigned long nFlags, unsigned char nVolume, float fPitch,
                           float fVariance);        // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CAurBehaviorAttach {
public:
    CAurBehaviorAttach(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart);   // 0x0049FB10
    virtual ~CAurBehaviorAttach();                  // 0x0049FCE0
    virtual void UpdateScenes();                    // 0x04
    virtual int Control(float fDelta);              // 0x08
    virtual void Message(char* szMsg);              // 0x0c
    virtual int GetType();                          // 0x10
    virtual void v14(); virtual void v18(); virtual void v1C(); virtual void v20();
    virtual int Fire();                             // 0x24 (descriptive; m calls it Restart)
    void AddDependency(CAurObject* pObject);        // 0x0049F130
    char pad_0x4[0x10 - 0x4];
    CAurObject* m_pObject;                          // 0x10
    char pad_0x14[0x3c - 0x14];
};

class CAurBehavior {
public:
    CAurBehavior(Gob* pGob);                        // 0x0049F110
    virtual ~CAurBehavior();                        // 0x0049FA70
    virtual void UpdateScenes();                    // 0x04
    virtual int Control(float fDelta);              // 0x08 (0x0049F270)
    virtual void Message(char* szMsg);              // 0x0c
    virtual int GetType();                          // 0x10
    char pad_0x4[0x10 - 0x4];
    Gob* m_pGob;                                    // 0x10
};

class CSWMiniHitInfo {
public:
    CSWMiniHitInfo();                               // 0x00673A10
    int m_nDamage;                                  // 0x00
    float m_fSpeed;                                 // 0x04
    float m_fRateOfFire;                            // 0x08
    float m_fLifespan;                              // 0x0c
    int m_nTargetMask;                              // 0x10
    int m_n14;                                      // 0x14
    CResRef m_cSound;                               // 0x18
};

class CSWTrackFollower;

// The bullet behaviour (vtable 0x00757F98, type 0xAAAB; m/ names it
// CSWMiniBulletInfo after its GetUserData(0xAAAB) use).
class CSWMiniBulletInfo : public CAurBehavior {
public:
    CSWMiniBulletInfo(Gob* pGob, CSWTrackFollower* pOwner, const CSWMiniHitInfo& cHit);   // 0x006DAFE0
    virtual int Control(float fDelta);              // 0x08 (0x006DB090)
    virtual int GetType();                          // 0x10 (0x006DB080)

    CSWMiniHitInfo m_cHit;                          // 0x14
    CSWTrackFollower* m_pOwner;                     // 0x3c
};

// The mini-game gun emitter (vtable 0x00757F70, type 0xAAAD, 0x54 bytes).
class CAurGunEmitter : public CAurBehaviorAttach {
public:
    CAurGunEmitter(CAurObject* pGun, CAurObject* pModel, CAurPart* pPart);   // 0x006DADF0
    virtual int Control(float fDelta);              // 0x08 (0x006DAE70)
    virtual int GetType();                          // 0x10 (0x006DAE60)
    virtual int Fire();                             // 0x24 (0x006DAEC0)

    CResRef m_cFireSound;                           // 0x3c
    float m_fRate;                                  // 0x4c
    float m_fTimer;                                 // 0x50
};

class L_MGB_VTABLE CSWMGBehaviorTrack : public CAurBehaviorAttach {
public:
    CSWMGBehaviorTrack(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart,
                       CAurObject* pTarget);        // 0x006DB4F0
    virtual ~CSWMGBehaviorTrack();                  // 0x006DB630
    virtual int Control(float fDelta);              // 0x08 (0x006DB180)
    virtual int GetType();                          // 0x10 (0x006DB580)

    CSWMGTargettingInfo m_cInfo;                    // 0x3c
    unsigned char m_bOnTarget;                      // 0x4c (descriptive)
    char pad_0x4d[0x3];
    float m_f50;                                    // 0x50
    GobRef m_pTarget;                               // 0x54
    Vector m_vOffset;                               // 0x58
};

class CSWMGBehaviorTrackAndFire : public CSWMGBehaviorTrack {
public:
    CSWMGBehaviorTrackAndFire(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart,
                              CAurObject* pTarget); // 0x006DB590
    virtual int Control(float fDelta);              // 0x08 (0x006DB3D0)
    virtual int GetType();                          // 0x10 (0x006DB600)

    CResRef m_cFireSound;                           // 0x64
    float m_fRate;                                  // 0x74
    float m_fTimer;                                 // 0x78
};

#endif
