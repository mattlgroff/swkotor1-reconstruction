// Camera (0x0045BEF0 .. 0x0045D930).

extern "C" {
void __stdcall gluPerspective(double fovy, double aspect, double zNear, double zFar);
}

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    CExoArrayList(int n);
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);
    void SetSize(int n)
    {
        Allocate(n);
        num = n;
    }
};

class Plane {
public:
    float x, y, z, d;

    Plane() { x = 0.0f; y = 0.0f; z = 0.0f; d = 0.0f; }
};

// FUNCTION: SWKOTOR 0x0045C270 SYMBOL: ?Allocate@?$CExoArrayList@VPlane@@@@QAEXH@Z
template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* old = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

// FUNCTION: SWKOTOR 0x0045D790 SYMBOL: ??0?$CExoArrayList@VPlane@@@@QAE@H@Z
template <class T>
CExoArrayList<T>::CExoArrayList(int n)
{
    num = 0;
    array_size = 0;
    element = 0;
    if (n)
        Allocate(n);
}

template class CExoArrayList<Plane>;

class CameraTarget {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual int IsActive();                     // +0x1c
    char pad_0x4[0x2c];
    float m_f30;                                // +0x30
    char pad_0x34[8];
    float m_f3C;                                // +0x3c
    char pad_0x40[4];
    float m_f44;                                // +0x44
};

class CAurCamera {
public:
    virtual ~CAurCamera();                      // 0x0045C240
};

class Gob {
public:
    Gob(char* szName);                          // 0x00449440
    virtual ~Gob();                             // 0x0044B2F0
    char pad_0x4[0x184];
    CameraTarget* m_pTarget;                    // +0x188
    char pad_0x18c[0x40];
};

struct ViewAngleAnimation {
    float m_fDuration;                          // +0x00
    float m_fTime;                              // +0x04
    float m_fStart;                             // +0x08
    float m_fEnd;                               // +0x0c
    int m_n10;                                  // +0x10
};

class Camera : public CAurCamera, public Gob {
public:
    float m_fViewAngle;                         // +0x1d0
    ViewAngleAnimation* m_pAnimation;           // +0x1d4
    float m_fAspect;                            // +0x1d8
    float m_fNear;                              // +0x1dc
    float m_fFar;                               // +0x1e0
    int m_n1E4;                                 // +0x1e4
    int m_n1E8;                                 // +0x1e8
    int m_n1EC;                                 // +0x1ec
    int m_n1F0;                                 // +0x1f0
    int m_n1F4;                                 // +0x1f4
    CExoArrayList<Plane> m_lPlanes;             // +0x1f8
    Plane m_p204;                               // +0x204
    Plane m_p214;                               // +0x214
    Plane m_p224;                               // +0x224

    Camera(char* szName);                       // 0x0045D800
    virtual ~Camera();                          // 0x0045C330
    void BeginViewAngleAnimation(float fEnd, float fDuration, int n);
    float GetTarget3C();
    float GetTarget44();
    float GetTarget30();
};

// FUNCTION: SWKOTOR 0x0045BEF0
void Camera::BeginViewAngleAnimation(float fEnd, float fDuration, int n)
{
    if (m_pAnimation) {
        delete m_pAnimation;
        m_fViewAngle = 45.0f;
    }
    m_pAnimation = new ViewAngleAnimation;
    m_pAnimation->m_fDuration = fDuration;
    m_pAnimation->m_fTime = 0.0f;
    m_pAnimation->m_fEnd = fEnd;
    m_pAnimation->m_fStart = m_fViewAngle;
    m_pAnimation->m_n10 = n;
}

// FUNCTION: SWKOTOR 0x0045C170
float Camera::GetTarget3C()
{
    if (m_pTarget && m_pTarget->IsActive())
        return m_pTarget->m_f3C;
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0045C1A0
float Camera::GetTarget44()
{
    if (m_pTarget && m_pTarget->IsActive())
        return m_pTarget->m_f44;
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0045C1D0
float Camera::GetTarget30()
{
    if (m_pTarget && m_pTarget->IsActive())
        return m_pTarget->m_f30;
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0045C240
CAurCamera::~CAurCamera()
{
}

// FUNCTION: SWKOTOR 0x0045C300
void _gluPerspective(double fovy, double aspect, double zNear, double zFar)
{
    gluPerspective(fovy, aspect, zNear, zFar);
}

// FUNCTION: SWKOTOR 0x0045C330
Camera::~Camera()
{
}

// FUNCTION: SWKOTOR 0x0045D800
Camera::Camera(char* szName)
    : Gob(szName)
{
    m_fViewAngle = 45.0f;
    m_fNear = 0.1f;
    m_fFar = 100.0f;
    m_fAspect = 1.0f;
    m_pAnimation = 0;
    m_n1F0 = 0;
    m_n1EC = 0;
    m_n1E8 = 0;
    m_n1E4 = 0;
    m_lPlanes.SetSize(6);
}

// FUNCTION: SWKOTOR 0x0045D910 SYMBOL: ??_GCamera@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0045D930
CAurCamera* NewCAurCamera(char* szName)
{
    return new Camera(szName);
}
