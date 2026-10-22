// Vector / quaternion leaf helpers (0x004A9680-0x004AA9C0). Self-contained.
#ifndef I_VECTOR_MATH_H
#define I_VECTOR_MATH_H

extern const float g_fZero;                         // 0x0073D700 (0.0f)
extern const float g_fOne;                          // 0x0073D764 (1.0f)

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    Vector& operator+=(const Vector& v);                    // 0x004A9740
    Vector& operator-=(const Vector& v);                    // 0x004A9770
    Vector& operator*=(float f);                            // 0x004A97A0
    Vector& operator/=(float f);                            // 0x004A97D0
    void Normalize();                                       // 0x004AB130

    float x;
    float y;
    float z;
};

// w first: x87 code multiplies/conjugates element 0 as the scalar part.
class Quaternion {
public:
    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ)
    {
        w = fW;
        x = fX;
        y = fY;
        z = fZ;
    }

    float w;
    float x;
    float y;
    float z;
};

struct Plane {
    Vector n;
    float d;
};

struct Matrix3 {
    float m[9];
};

class VertexCache {
public:
    VertexCache(int nSize);                                 // 0x004A9680
    ~VertexCache();                                         // 0x004A96C0

    int* m_pEntries;                                        // +0x00
    int m_nSize;                                            // +0x04
};

float FUN_004a96d0(float f);                                                // 0x004A96D0
float Round(float fValue, float fStep);                                     // 0x004A96E0
float FUN_004a9710(const Vector& v, float f, const Plane& p);               // 0x004A9710
float FUN_004a9810(const Vector& v, float f, const Vector& n);              // 0x004A9810
Vector FUN_004a9850(const Vector& n, float d, const Vector& v);             // 0x004A9850
Quaternion FUN_004a9b50(const Quaternion& q);                               // 0x004A9B50
Quaternion FUN_004a9b80(const Quaternion& q, float f);                      // 0x004A9B80
Quaternion FUN_004a9d00(const Quaternion& a, const Quaternion& b);          // 0x004A9D00
float FUN_004a9d30(const Quaternion& a, const Quaternion& b);               // 0x004A9D30
Vector FUN_004a9a60(const Matrix3& m, const Vector& v);                     // 0x004A9A60
Quaternion FUN_004a9ac0(const Quaternion& a, const Quaternion& b);          // 0x004A9AC0
int operator==(const Vector& a, const Vector& b);                           // 0x004AA980

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

void Outcode(Vector* pPoint, Vector* pMin, Vector* pMax, unsigned char* pCode); // 0x004AA160
float FUN_004aa0f0(const Vector& v);                                        // 0x004AA0F0
float FUN_004aa130(const Vector& v);                                        // 0x004AA130
Plane* FUN_004aaab0(const Vector& n, const float& d);                       // 0x004AAAB0
float FUN_004aadf0(float f);                                                // 0x004AADF0
float FUN_004aae30(float f);                                                // 0x004AAE30
Vector FUN_004aae70(Vector a, Vector b);                                    // 0x004AAE70
Vector FUN_004aaee0(Vector a, Vector b);                                    // 0x004AAEE0
float FUN_004aaf50(const Vector& a, const Vector& b, const Plane& p);       // 0x004AAF50
Vector FUN_004aafc0(const CExoArrayList<Vector>& l);                        // 0x004AAFC0
Vector FUN_004ab060(const CExoArrayList<Vector>& l);                        // 0x004AB060
float FUN_004ab100(const Vector* v);                                        // 0x004AB100
Vector FUN_004ab1e0(const Vector& a, const Vector& b);                      // 0x004AB1E0
Vector FUN_004ab210(const Vector& a, const Vector& b);                      // 0x004AB210

Vector lineproject(const Vector& a, const Vector& b, const Vector& p);      // 0x004A98B0
float FUN_004aa020(const Quaternion& q);                                    // 0x004AA020
float FUN_004aaae0(float a, float b, float c, float d, float t);            // 0x004AAAE0
Vector NormalizeVector(const Vector& v);                                    // 0x004AB180
Quaternion normalize(Quaternion q);                                         // 0x004A9D60

extern const float g_afSinTable[32];                // 0x00743FE8
extern const float g_afCosTable[32];                // 0x00744070

#endif
