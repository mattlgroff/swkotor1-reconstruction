// Camera screen-space queries: MouseDir (0x0045CF30) and the view-volume
// hit test (0x0045D300). Minimal Camera layout; the full class is in
// camera.cpp.

#include <math.h>

extern "C" __declspec(dllimport) void __stdcall glGetIntegerv(unsigned int pname, int* params);

struct Vector {
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector& operator-=(const Vector& v);        // 0x004A9770
    void Normalize();                           // 0x004AB130
};

struct Quaternion {
    float w, x, y, z;

    Quaternion() {}
};

Vector operator*(const Quaternion& q, const Vector& v);  // 0x004A9BB0
Quaternion operator~(const Quaternion& q);               // 0x004A9B50

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

__forceinline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

int operator==(const Vector& a, const Vector& b);   // 0x004AA980
Vector planelineintersection(const Vector& normal, float d,
                             const Vector& start, const Vector& end); // 0x004AB250

struct Plane {
    float x, y, z, d;

    const Vector& Normal() const { return *(const Vector*)this; }
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CHitInfo;

class Scene {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual void V2C();
    virtual void V30();
    virtual void V34();
    virtual int HitCheck(Vector start, Vector end, Vector* out, CHitInfo* hit); // +0x38
};

class CAurCamera {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual Vector MouseDir(int x, int y) = 0;  // +0x2c
};

class Gob {
public:
    void* m_vtbl;
    char pad_0x4[0x64];
    Scene* m_pScene;                            // +0x68
    char pad_0x6c[0x0c];
    Vector m_vPosition;                         // +0x78
    Quaternion m_qOrientation;                  // +0x84
    char pad_0x94[0x1cc - 0x94];
};

struct ViewPort { int x, y, width, height; };

class Camera : public CAurCamera, public Gob {
public:
    float m_fViewAngle;                         // +0x1d0
    char pad_0x1d4[4];
    float m_fAspect;                            // +0x1d8
    float m_fNear;                              // +0x1dc
    float m_fFar;                               // +0x1e0
    ViewPort m_ViewPort;                        // +0x1e4
    char pad_0x1f4[0x1f8 - 0x1f4];
    CExoArrayList<Plane> m_lPlanes;             // +0x1f8

    int GetViewPortWidth() { return m_ViewPort.width; }
    int GetViewPortHeight() { return m_ViewPort.height; }
    virtual Vector MouseDir(int x, int y);
    int HitCheck(int x, int y, Vector* out, CHitInfo* hit);
    Vector MapToScreen(Vector v, bool normalized);
    float HitCheckViewVolume(const Vector* start, const Vector* end, Vector* out, int* hit);
};

// World-space direction through window pixel (x, y).
// FUNCTION: SWKOTOR 0x0045CF30
Vector Camera::MouseDir(int x, int y)
{
    ViewPort vp = m_ViewPort;
    if (!GetViewPortWidth() || !GetViewPortHeight())
        glGetIntegerv(0xba2, &vp.x);
    float t = (float)tan(m_fViewAngle * 0.0087266462f);
    x -= vp.x;
    y -= vp.y;
    Vector v;
    v.x = (x - vp.width * 0.5f) * t / (vp.height * 0.5f);
    v.y = (y - vp.height * 0.5f) * t / (vp.height * 0.5f);
    v.z = -1.0f;
    v = m_qOrientation * v;
    v.Normalize();
    return v;
}

// Nearest point where the segment crosses one of the side planes (2..5)
// inside the view volume, as depth along the forward plane (1).
// FUNCTION: SWKOTOR 0x0045D300
float Camera::HitCheckViewVolume(const Vector* start, const Vector* end, Vector* out, int* hit)
{
    float nearest = m_fFar;
    Vector point(0.0f, 0.0f, 0.0f);
    Vector zero(0.0f, 0.0f, 0.0f);
    if (hit)
        *hit = 0;
    for (int i = 2; i < 6; i++) {
        point = planelineintersection(m_lPlanes.element[i].Normal(), m_lPlanes.element[i].d, *start, *end);
        if (point == zero)
            continue;
        for (int k = 0; k < 6; k++) {
            if (k == i)
                continue;
            Plane* p = &m_lPlanes.element[k];
            if (point.y * p->y + point.z * p->z + point.x * p->x + p->d > 0.0f)
                goto next;
        }
        {
            point -= m_vPosition;
            Plane* f = &m_lPlanes.element[1];
            float depth = point.y * f->y + point.z * f->z + point.x * f->x;
            if (depth < nearest && depth > 0.0f) {
                nearest = depth;
                if (out)
                    *out = point;
                if (hit)
                    *hit = 1;
            }
        }
    next:;
    }
    return nearest;
}

// Projects a world point to window coordinates (optionally normalized to
// 0..1 with y up).
// NONMATCH: the x87 body matches; only the viewport copy prologue differs
// (original loads height before width and tests it via `mov eax, edx`),
// the same unsolved pattern as MouseDir. Register names then rotate.
// FUNCTION: SWKOTOR 0x0045D040
Vector Camera::MapToScreen(Vector v, bool normalized)
{
    ViewPort vp;
    vp.x = m_ViewPort.x;
    vp.y = m_ViewPort.y;
    vp.width = m_ViewPort.width;
    vp.height = m_ViewPort.height;
    if (!GetViewPortWidth() || !GetViewPortHeight())
        glGetIntegerv(0xba2, &vp.x);
    if (m_fViewAngle > 0.0f) {
        v = v - m_vPosition;
        v = ~m_qOrientation * v;
        Vector p = planelineintersection(Vector(0.0f, 0.0f, 1.0f), 1.0f, Vector(0.0f, 0.0f, 0.0f), v);
        float t = tan(m_fViewAngle * 0.0087266462f);
        float h = (float)vp.height;
        float hh = h * 0.5f;
        float sx = hh / t * p.x + (float)vp.width * 0.5f + vp.x;
        float sy = hh / t * p.y + vp.y + hh;
        if (normalized) {
            sx /= (float)vp.width;
            sy = 1.0f - sy / h;
        }
        return Vector(sx, sy, 0.0f);
    }
    return Vector(0.0f, 0.0f, 0.0f);
}

// Picks the scene along the ray through window pixel (x, y), between the
// near and far clip distances.
// NONMATCH: the perspective path matches apart from the frame size (one
// extra saved register); in the orthographic path the original stores the
// offsets to memory before pushing the call arguments, ours keeps them on
// the x87 stack.
// FUNCTION: SWKOTOR 0x0045CC90
int Camera::HitCheck(int x, int y, Vector* out, CHitInfo* hit)
{
    if (!m_pScene)
        return 0;
    if (m_fViewAngle > 0.0f) {
        Vector dir = MouseDir(x, y);
        Vector end = dir * m_fFar + m_vPosition;
        Vector start = dir * m_fNear + m_vPosition;
        return m_pScene->HitCheck(start, end, out, hit);
    }
    ViewPort vp;
    vp.y = m_ViewPort.y;
    vp.x = m_ViewPort.x;
    vp.height = m_ViewPort.height;
    vp.width = m_ViewPort.width;
    if (!vp.width || !vp.height)
        glGetIntegerv(0xba2, &vp.x);
    x -= vp.x;
    y -= vp.y;
    if (x < 0 || y < 0 || x >= vp.width || y >= vp.height)
        return 0;
    float ox = (x + 0.5f) / (float)vp.height * m_fAspect;
    float oy = (y + 0.5f) / (float)vp.height * m_fAspect;
    float nz = -m_fNear;
    Vector end = Vector(ox, oy, nz - m_fFar) + m_vPosition;
    Vector start = Vector(ox, oy, nz) + m_vPosition;
    return m_pScene->HitCheck(start, end, out, hit);
}
