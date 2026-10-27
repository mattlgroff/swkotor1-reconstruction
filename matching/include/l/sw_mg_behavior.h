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

class Gob {
public:
    virtual ~Gob();
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
    float x, y, z;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

class CAurBehaviorAttach {
public:
    CAurBehaviorAttach(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart);   // 0x0049FB10
    virtual ~CAurBehaviorAttach();                  // 0x0049FCE0
    virtual void UpdateScenes();                    // 0x04
    virtual int Control(float fDelta);              // 0x08
    virtual void Message(char* szMsg);              // 0x0c
    virtual int GetType();                          // 0x10
    void AddDependency(CAurObject* pObject);        // 0x0049F130
    char pad_0x4[0x3c - 0x4];
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
