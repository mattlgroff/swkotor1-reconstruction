// WindManager construction, teardown, timer and noise (aurwindmanager.o,
// 0x004A57C0-0x004A6710). Fuller layout than i/wind_manager.h, kept
// separate so the older single-function files stay as they are.
#ifndef I_WIND_MANAGER_LIFE_H
#define I_WIND_MANAGER_LIFE_H

class Vector {
public:
    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }

    float x;
    float y;
    float z;
};

inline Vector operator*(float f, const Vector& v)
{
    return Vector(f * v.x, f * v.y, f * v.z);
}

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
    void Allocate(int n);
    __forceinline void RemoveInline(T value)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == value)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
};

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

class PointSourceWind {
public:
    PointSourceWind()                                   // 0x004A58C0
    {
        m_vPosition = Vector();
        m_fRadius = 0.0f;
        m_fTimeLeft = 0.0f;
        m_fStrength = 0.0f;
    }
    ~PointSourceWind() {}

    Vector m_vPosition;                                 // +0x00
    float m_fRadius;                                    // +0x0c
    float m_fTimeLeft;                                  // +0x10
    float m_fStrength;                                  // +0x14
};

class PartEmitter;

template <class T>
class SafePointer {
public:
    T* m_p;
};

// Registration list of the SafePointers aimed at the owner; the
// destructor clears every one of them.
template <class T>
class SafePointerList : public CExoArrayList<SafePointer<T>*> {
public:
    ~SafePointerList()
    {
        for (int i = num - 1; i >= 0; i--) {
            SafePointer<T>* p = element[i];
            if (p->m_p)
                p->m_p->m_lSafePointers.RemoveInline(p);
            p->m_p = 0;
        }
        if (element) {
            delete[] element;
            element = 0;
        }
        array_size = 0;
        num = 0;
    }
};

class WindManager {
public:
    WindManager();                                      // 0x004A6640
    ~WindManager();                                     // 0x004A6710
    void FillNoiseField(float* pField);                 // 0x004A57C0
    void UpdateTimer(float fDelta);                     // 0x004A5E30

    SafePointerList<WindManager> m_lSafePointers;       // +0x00
    CExoArrayList<PointSourceWind> m_lPointSourceWinds; // +0x0c
    Vector m_vGlobalWind;                               // +0x18
    Vector m_vWindStep;                                 // +0x24
    float m_afNoise[2][0x100];                          // +0x30
    float* m_pCurrentNoise;                             // +0x830
    float* m_pNextNoise;                                // +0x834
    float m_fMinimumDeviation;                          // +0x838
    float m_fMaximumDeviation;                          // +0x83c
    float m_fBlend;                                     // +0x840
    float m_fTimer;                                     // +0x844
    float m_fPeriod;                                    // +0x848
    CExoArrayList<PartEmitter*> m_lEmitters;            // +0x84c
};

#endif
