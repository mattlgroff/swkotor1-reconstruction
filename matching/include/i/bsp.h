// BSP tree (bsp.o, 0x004ACED0-0x004AD7E0). Self-contained copy of the
// node layout; i/bsp_traverse.h keeps the minimal one for 0x004ACEA0.
#ifndef I_BSP_H
#define I_BSP_H

class Vector {
public:
    Vector() {}
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

inline Vector operator-(const Vector& v)
{
    return Vector(-v.x, -v.y, -v.z);
}

class Plane {
public:
    Plane() {}
    Plane(const Vector& vNormal, float fDist) : n(vNormal), d(fDist) {}

    Vector n;
    float d;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int n);
    ~CExoArrayList();                                   // 0x0047ABC0
    void Allocate(int n);                               // 0x0045C270 (Plane)
    void Add(T t);
};

class nodedata;

class BSPNode : public Plane {
public:
    BSPNode(Vector vNormal, float fDist);               // 0x004AD630
    ~BSPNode();                                         // 0x004ACED0

    BSPNode* m_pFront;                                  // +0x10
    BSPNode* m_pBack;                                   // +0x14
    int m_bLeaf;                                        // +0x18
    Vector m_vMin;                                      // +0x1c
    Vector m_vMax;                                      // +0x28
    CExoArrayList<Plane> m_lPlanes34;                   // +0x34
    CExoArrayList<Plane> m_lPlanes40;                   // +0x40
    CExoArrayList<Vector> m_lPoints;                    // +0x4c
    nodedata* m_pData;                                  // +0x58
};

float FUN_004aaf50(const Vector& a, const Vector& b, const Plane& p);  // 0x004AAF50 BoxPlaneSide
Vector planelineintersection(const Vector& vNormal, float fDist,
                             const Vector& vStart, const Vector& vEnd);   // 0x004AB250

extern const float g_fZero;                             // 0x0073D700
extern const float g_fBoxEpsilon;                       // 0x00744104 (-0.02f)

#endif
