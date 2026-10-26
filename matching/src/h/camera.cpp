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

struct Quaternion { float w, x, y, z; };
struct Vector { float x, y, z; };
class CHitInfo;
class GobAttachment;
class Part;
class Scene;

class Gob {
public:
    Gob(char* szName);                          // 0x00449440
    virtual ~Gob();                             // 0x0044B2F0
    Gob* IntersectLine(Vector start, Vector end, Vector* hit, CHitInfo* info); // 0x0043EB80
    GobAttachment* GetAttachment(int index); // 0x0043F130
    void SetAttachment(GobAttachment* p); // 0x0043F1D0
    void SetAttachmentLight(int n); // 0x0043F5A0
    GobAttachment* AttachToPart(Gob* gob, Part* part, int n); // 0x004439A0
    GobAttachment* AttachToObject(Gob* gob, char* name, int n); // 0x00443820
    void SetScene(Scene* scene); // 0x0043F200
    char pad_0x4[0x74];
    Vector m_vPosition;
    Quaternion m_qOrientation;                  // +0x84
    char pad_0x94[0xF4];
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
    Vector SetPosition(Vector p);
    Vector GetPosition();
    Gob* AsAurObject();
    GobAttachment* GetAttachment(int index);
    void SetAttachment(GobAttachment* p);
    void SetAttachmentLight(int n);
    GobAttachment* AttachToPart(Gob* gob, Part* part, int n);
    void EndViewAngleAnimation(bool bReset);
    void SetViewPort(int x, int y, int width, int height);
    Quaternion SetOrientation(Quaternion q);
    Quaternion GetOrientation();
    int GetViewPortHeight();
    void GetClipDist(float* pNear, float* pFar);
    float GetViewAngle();
    Plane GetViewVolume(int index);
    void SetFadePlanes(const Plane* a, const Plane* b);
    void SetClipDist(float nearClip, float farClip);
    void SetViewAngle(float angle);
    int AttachToScene(Scene* scene);
    GobAttachment* AttachToObject(Gob* gob, char* name, int n);
    int ViewVolumeSet();
    Gob* HitCheck(Vector start, Vector end, Vector* hit);
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

// FUNCTION: SWKOTOR 0x0045BF60
void Camera::EndViewAngleAnimation(bool bReset)
{
    delete m_pAnimation;
    m_pAnimation = 0;
    if (bReset) m_fViewAngle = 45.0f;
}

// FUNCTION: SWKOTOR 0x0045BFA0
void Camera::SetViewPort(int x, int y, int width, int height)
{
    m_n1E4 = x;
    m_n1E8 = y;
    if (width <= 0) width = 1;
    m_n1EC = width;
    if (height <= 0) height = 1;
    m_n1F0 = height;
}

// FUNCTION: SWKOTOR 0x0045C060
Quaternion Camera::SetOrientation(Quaternion q)
{
    m_qOrientation = q;
    return q;
}

// FUNCTION: SWKOTOR 0x0045C470
Quaternion Camera::GetOrientation()
{
    return m_qOrientation;
}

// FUNCTION: SWKOTOR 0x0045C4A0
int Camera::GetViewPortHeight()
{
    return m_n1F0;
}

// FUNCTION: SWKOTOR 0x0045C4B0
void Camera::GetClipDist(float* pNear, float* pFar)
{
    *pNear = m_fNear;
    *pFar = m_fFar;
}

// FUNCTION: SWKOTOR 0x0045BFE0
Gob* Camera::HitCheck(Vector start, Vector end, Vector* hit)
{
    return Gob::IntersectLine(start, end, hit, 0);
}

// FUNCTION: SWKOTOR 0x0045C4D0
float Camera::GetViewAngle()
{
    return m_fViewAngle;
}

// FUNCTION: SWKOTOR 0x0045D760
Plane Camera::GetViewVolume(int index)
{
    return m_lPlanes.element[index];
}

// FUNCTION: SWKOTOR 0x0045C4E0
void Camera::SetFadePlanes(const Plane* a, const Plane* b)
{
    m_p214 = *a;
    m_p224 = *b;
}

// FUNCTION: SWKOTOR 0x0045C530
int Camera::ViewVolumeSet()
{
    return m_n1F4;
}

// FUNCTION: SWKOTOR 0x0045C030
Vector Camera::SetPosition(Vector p) { m_vPosition = p; return p; }

// FUNCTION: SWKOTOR 0x0045C450
Vector Camera::GetPosition() { return m_vPosition; }

// FUNCTION: SWKOTOR 0x0045C440
Gob* Camera::AsAurObject() { return this; }

// FUNCTION: SWKOTOR 0x0045C160
void Camera::SetAttachmentLight(int n) { Gob::SetAttachmentLight(n); }

// FUNCTION: SWKOTOR 0x0045C210
GobAttachment* Camera::AttachToPart(Gob* gob, Part* part, int n)
{ return Gob::AttachToPart(gob, part, n); }

// FUNCTION: SWKOTOR 0x0045C220
GobAttachment* Camera::GetAttachment(int index) { return Gob::GetAttachment(index); }

// FUNCTION: SWKOTOR 0x0045C230
void Camera::SetAttachment(GobAttachment* p) { Gob::SetAttachment(p); }

// FUNCTION: SWKOTOR 0x0045BEC0
void Camera::SetClipDist(float nearClip, float farClip)
{
    m_fNear = nearClip;
    m_fFar = farClip;
}

// FUNCTION: SWKOTOR 0x0045BEE0
void Camera::SetViewAngle(float angle)
{
    m_fViewAngle = angle;
}

// FUNCTION: SWKOTOR 0x0045C0A0
int Camera::AttachToScene(Scene* scene)
{
    Gob::SetScene(scene);
    return 1;
}

// FUNCTION: SWKOTOR 0x0045C200
GobAttachment* Camera::AttachToObject(Gob* gob, char* name, int n)
{
    return Gob::AttachToObject(gob, name, n);
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

// FUNCTION: SWKOTOR 0x0045C250 SYMBOL: ??_GCAurCamera@@UAEPAXI@Z

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
