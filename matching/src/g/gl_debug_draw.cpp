// GL debug drawing: flat-shaded face lists and sphere bounds
// (0x004318f0, 0x00431a50).
extern "C" {
__declspec(dllimport) void __stdcall glBegin(unsigned int mode);
__declspec(dllimport) void __stdcall glEnd();
__declspec(dllimport) void __stdcall glColor4ubv(const unsigned char* v);
__declspec(dllimport) void __stdcall glColor3f(float r, float g, float b);
__declspec(dllimport) void __stdcall glNormal3fv(const float* v);
__declspec(dllimport) void __stdcall glVertex3fv(const float* v);
__declspec(dllimport) void __stdcall glVertex3f(float x, float y, float z);
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glGetFloatv(unsigned int pname, float* p);
__declspec(dllimport) void __stdcall glPushMatrix();
__declspec(dllimport) void __stdcall glPopMatrix();
__declspec(dllimport) void __stdcall glMatrixMode(unsigned int mode);
__declspec(dllimport) void __stdcall glMultMatrixf(const float* m);
}

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
    operator const float*() const { return &x; }
    Vector& operator=(const Vector& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

// 0x20-byte model face.
class TriFace {
public:
    char pad00[0x10];
    int m_nColor;                           // +0x10
    char pad14[0x1a - 0x14];
    short m_anVerts[3];                     // +0x1a
};

Vector __cdecl TriangleNormal(const Vector& a, const Vector& b, const Vector& c);  // 0x004aca00

// FUNCTION: SWKOTOR 0x004318f0
void DrawTriangles(CExoArrayList<TriFace>* pFaces, Vector* pVerts, unsigned int* pColors)
{
    glBegin(4);
    Vector v0, v1, v2;
    for (int i = 0; i < pFaces->num; i++) {
        glColor4ubv((unsigned char*)&pColors[pFaces->element[i].m_nColor]);
        v0 = pVerts[pFaces->element[i].m_anVerts[0]];
        v1 = pVerts[pFaces->element[i].m_anVerts[1]];
        v2 = pVerts[pFaces->element[i].m_anVerts[2]];
        glNormal3fv(TriangleNormal(v0, v1, v2));
        glVertex3fv(v0);
        glVertex3fv(v1);
        glVertex3fv(v2);
    }
    glEnd();
}

// FUNCTION: SWKOTOR 0x00431a50
void DrawSphereBound(float fRadius, Vector* pCenter, float* pColor, float fScale,
                     unsigned char bBillboard)
{
    glDisable(0xde1);
    glDisable(0xb50);
    if (bBillboard) {
        float m[16];
        glGetFloatv(0xba6, m);
        float f;
        *(Vector*)&m[12] = *pCenter;
        f = m[1];
        m[1] = m[4];
        m[4] = f;
        f = m[2];
        m[2] = m[8];
        m[8] = f;
        f = m[6];
        m[6] = m[9];
        m[9] = f;
        glPushMatrix();
        glMatrixMode(0x1700);
        glMultMatrixf(m);
    }
    glColor3f(pColor[0], pColor[1], pColor[2]);
    fRadius *= fScale;
    glBegin(2);
    glVertex3f(0, fRadius, 0);
    float fDiag = fRadius * 0.707f;
    glVertex3f(fDiag, fDiag, 0);
    glVertex3f(fRadius, 0, 0);
    float fNegDiag = fRadius * -0.707f;
    glVertex3f(fDiag, fNegDiag, 0);
    float fNeg = -fRadius;
    glVertex3f(0, fNeg, 0);
    glVertex3f(fNegDiag, fNegDiag, 0);
    glVertex3f(fNeg, 0, 0);
    glVertex3f(fNegDiag, fDiag, 0);
    glEnd();
    if (bBillboard)
        glPopMatrix();
    glEnable(0xde1);
    glEnable(0xb50);
}
