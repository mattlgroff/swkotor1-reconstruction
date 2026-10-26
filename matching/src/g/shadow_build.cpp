// Stencil shadow volume vertices: each marked silhouette vertex and its
// extrusion away from the light. Behavioural reference:
// src/units/0806/e9d88e6c4d-m47_modelb_shadow.cpp.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memcpy)

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator*(const Vector& v, float s)
{
    return Vector(v.x * s, v.y * s, v.z * s);
}

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Plane;

Vector Normalize(const Vector& v);                                   // 0x004ab180
void ClipProjection(Vector* pPoint, Vector* pLight, Plane* pFrustum);  // 0x0046f650
void* AllocateShadowMemory(int nSize);                               // 0x00426940

extern unsigned short g_aShadowVertexMarks[];  // 0x007a4820
extern void* g_pShadowVertices;                // 0x007a47e8
extern int g_bShadowNoClip;                    // 0x007a68e0
extern int g_bShadowClampFar;                  // 0x0078d994

// Renumbers the marked vertices 1..n and writes, for each, the vertex and its
// projection away from the light; the scaled direction and a point follow
// the pairs. Returns the number of marked vertices.
// FUNCTION: SWKOTOR 0x004269f0
int BuildShadowArray(Vector* pLight, float* pLength, Plane* pFrustum, Vector* pDirection,
                     Vector* pPoint, int nVertices, Vector* pVertices)
{
    static Vector s_vDir;
    static Vector s_aPair[2];
    int nMarked = 0;
    for (int i = 0; i < nVertices; i++) {
        if (g_aShadowVertexMarks[i] == 1) {
            nMarked++;
            g_aShadowVertexMarks[i] = nMarked;
        }
    }
    int nBytes = nMarked * 24;
    g_pShadowVertices = AllocateShadowMemory(nBytes + 0x18);
    char* pArray = (char*)g_pShadowVertices;
    for (int v = 0; v < nVertices; v++) {
        unsigned short nMark = g_aShadowVertexMarks[v];
        int nSlot = nMark * 6 - 6;
        if (nMark > 0) {
            s_aPair[0] = pVertices[v];
            s_vDir = Normalize(s_aPair[0] - *pLight);
            s_aPair[1] = s_vDir * *pLength + *pLight;
            if (!g_bShadowNoClip) {
                ClipProjection(&s_aPair[0], pLight, pFrustum);
                ClipProjection(&s_aPair[1], pLight, pFrustum);
            }
            if (g_bShadowClampFar) {
                if (s_vDir * (s_aPair[0] - s_aPair[1]) > 0.0f)
                    s_aPair[1] = s_aPair[0];
            }
            memcpy(pArray + nSlot * 4, s_aPair, sizeof(s_aPair));
        }
    }
    Vector* pTail = (Vector*)(pArray + nBytes);
    *pTail++ = *pDirection;
    *pTail = *pPoint;
    return nMarked;
}
