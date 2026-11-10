// Worker L matching headers: the conversation camera behaviour
// CSWBehaviorCameraDialog (vtable 0x00756AE8, 0x9C bytes, GetType 0x106D,
// 0x006BB590-0x006BD980). CAurBehavior/Gob/GobRef follow
// matching/include/n/sw_behavior_camera.h.
#ifndef L_SW_BEHAVIOR_CAMERA_DIALOG_H
#define L_SW_BEHAVIOR_CAMERA_DIALOG_H

#define L_CAM_VSLOTS4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Remove(T value) throw();   // 0x00445080
    void Add(T value);
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    float MagnitudeSquared() const { return x * x + y * y + z * z; }
    void Normalize();                               // 0x004AB130
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};

// View object returned by Gob slot +0x04 for camera objects.
class CAurCameraView {
public:
    L_CAM_VSLOTS4(c0a) L_CAM_VSLOTS4(c0b) L_CAM_VSLOTS4(c0c) L_CAM_VSLOTS4(c0d)
    virtual void c1a0();
    virtual void SetViewAngle(float fAngle);                            // +0x44
};

class Gob {
public:
    virtual ~Gob();
    virtual CAurCameraView* GetCameraView();                            // +0x04
    virtual void g08(); virtual void g0c();
    L_CAM_VSLOTS4(g1) L_CAM_VSLOTS4(g2) L_CAM_VSLOTS4(g3) L_CAM_VSLOTS4(g4)
    virtual void g50(); virtual void g54(); virtual void g58();
    virtual Vector SetPosition(Vector vPosition);                       // +0x5c
    virtual Quaternion SetOrientation(Quaternion qOrientation);         // +0x60
    virtual Vector GetPosition();                                       // +0x64
    virtual Quaternion GetOrientation();                                // +0x68

    CExoArrayList<Gob**> m_lWatchers;                                   // +0x04
};

// Back-referenced Gob pointer: registers its slot with the Gob's watcher
// list so the Gob can clear it when it dies.
class GobRef {
public:
    Gob* m_p;

    GobRef(Gob* p = 0)
    {
        m_p = p;
        if (m_p)
            m_p->m_lWatchers.Add(&m_p);
    }
    ~GobRef()
    {
        if (m_p) {
            CExoArrayList<Gob**>& l = m_p->m_lWatchers;
            l.Remove(&m_p);
        }
    }
    void Clear()
    {
        if (m_p)
            m_p->m_lWatchers.Remove(&m_p);
        m_p = 0;
    }
};

class CAurBehavior {
public:
    CExoArrayList<GobRef*> m_lDependencies;     // +0x04
    Gob* m_pGob;                                // +0x10

    CAurBehavior(Gob* pGob);                    // 0x0049F110
    virtual ~CAurBehavior();                    // 0x0049FA70
    virtual void UpdateScenes();                // +0x04
    virtual int Control(float fDelta);          // +0x08
    virtual void Message(char* szMsg);          // +0x0c
    virtual int GetType();                      // +0x10
};

// One line of action between two dialogue participants (descriptive).
struct CSWLineOfAction {
    unsigned long m_oidFrom;                    // +0x00
    unsigned long m_oidTo;                      // +0x04
    int m_nSide;                                // +0x08 (0 = unused slot)
};

class CSWBehaviorCameraDialog : public CAurBehavior {
public:
    CSWBehaviorCameraDialog(Gob* pGob, unsigned long nShot);   // 0x006BB670
    virtual ~CSWBehaviorCameraDialog();         // 0x006BB750
    virtual int GetType();                      // +0x10 (0x006BB740)

    void SetShot(int nType, Vector vPosition, Quaternion qOrientation,
                 float fFieldOfView);           // 0x006BB590
    int GetLineOfActionSide(unsigned long oidA, unsigned long oidB);   // 0x006BB610 (descriptive)
    int AddLineOfAction(unsigned long oidA, unsigned long oidB);       // 0x006BCEC0 (descriptive)
    int CalculateLineOfActionSide();            // 0x006BC600 (descriptive)

    int m_n14;                                  // +0x14
    int m_n18;                                  // +0x18
    int m_n1C;                                  // +0x1c
    int m_n20;                                  // +0x20
    int m_n24;                                  // +0x24
    int m_nShotType;                            // +0x28
    int m_n2C;                                  // +0x2c
    int m_n30;                                  // +0x30
    int m_n34;                                  // +0x34
    unsigned long m_nShot;                      // +0x38
    int m_bFirstFrame;                          // +0x3c (descriptive)
    Vector m_vPosition;                         // +0x40
    Quaternion m_qOrientation;                  // +0x4c
    unsigned long m_oidSpeaker;                 // +0x5c (descriptive)
    unsigned long m_oidListener;                // +0x60 (descriptive)
    GobRef m_rSpeaker;                          // +0x64
    GobRef m_rListener;                         // +0x68
    CSWLineOfAction m_aLines[4];                // +0x6c
};

#endif
