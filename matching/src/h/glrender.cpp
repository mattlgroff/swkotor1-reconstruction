// GL render helpers (0x0046EF10-0x0046F870).

#include "h/renderpaths.h"

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

namespace GLRender {
void DrawAxisAlignedFrame(const Vector& a, const Vector& b, const Vector& c);
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);
}

extern int g_nTransformMode;        // 0x0078D494
extern int g_nTransformCount;       // 0x00827FA8
extern float g_aTransformMatrix[16];    // 0x008280A0
extern float g_fRadToDeg;           // 0x00742EF8

// FUNCTION: SWKOTOR 0x0046EF10
void GLRender::DrawAxisAlignedFrame(const Vector& a, const Vector& b, const Vector& c)
{
    glDisable(0xde1);
    glDisable(0xb50);
    glColor3f(c.x, c.y, c.z);
    glBegin(2);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(a.x, b.y, a.z);
    glVertex3f(b.x, b.y, a.z);
    glVertex3f(b.x, a.y, a.z);
    glEnd();
    glBegin(2);
    glVertex3f(a.x, a.y, b.z);
    glVertex3f(b.x, a.y, b.z);
    glVertex3f(b.x, b.y, b.z);
    glVertex3f(a.x, b.y, b.z);
    glEnd();
    glBegin(1);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(a.x, a.y, b.z);
    glVertex3f(b.x, a.y, a.z);
    glVertex3f(b.x, a.y, b.z);
    glVertex3f(b.x, b.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glVertex3f(a.x, b.y, a.z);
    glVertex3f(a.x, b.y, b.z);
    glEnd();
    glEnable(0xb50);
    glEnable(0xde1);
}

// FUNCTION: SWKOTOR 0x0046F060
void GLRender::Transform(const Vector* pPos, const Quaternion* pOri, int bForce)
{
    if (!g_nTransformMode && !bForce)
        return;
    g_nTransformCount++;
    if (g_nTransformMode > 0) {
        BuildMatrix(g_aTransformMatrix, pPos, pOri);
        glMultMatrixf(g_aTransformMatrix);
        return;
    }
    static Vector s_vPos(0.0f, 0.0f, 0.0f);
    static Quaternion s_qOri(1.0f, 0.0f, 0.0f, 0.0f);
    s_vPos = *pPos;
    glTranslatef(s_vPos.x, s_vPos.y, s_vPos.z);
    s_qOri = *pOri;
    Vector vAxis = s_qOri.GetAxis();
    glRotatef(acos(s_qOri.w) * 2.0 * g_fRadToDeg, vAxis.x, vAxis.y, vAxis.z);
}

extern Vector g_aWalkMeshColors[32];    // 0x0078E6A8

// FUNCTION: SWKOTOR 0x0046F200
unsigned long* GetWalkMeshColors()
{
    static unsigned long* s_pColors = new unsigned long[32];
    unsigned long* p = s_pColors;
    for (int i = 0; i < 32; i++) {
        int r, g, b;
        FloatToInt(&r, g_aWalkMeshColors[i].x * 255.0f);
        FloatToInt(&g, g_aWalkMeshColors[i].y * 255.0f);
        FloatToInt(&b, g_aWalkMeshColors[i].z * 255.0f);
        p[i] = ((((b | 0xffffff00) << 8) | g) << 8) | r;
    }
    return s_pColors;
}

// FUNCTION: SWKOTOR 0x0046F300
unsigned long* GetInverseWalkMeshColors()
{
    static unsigned long* s_pColors = new unsigned long[32];
    unsigned long* p = s_pColors;
    for (int i = 0; i < 32; i++) {
        int r, g, b;
        FloatToInt(&r, (1.0f - g_aWalkMeshColors[i].x) * 255.0f);
        FloatToInt(&g, (1.0f - g_aWalkMeshColors[i].y) * 255.0f);
        FloatToInt(&b, (1.0f - g_aWalkMeshColors[i].z) * 255.0f);
        p[i] = ((((b | 0xffffff00) << 8) | g) << 8) | r;
    }
    return s_pColors;
}

typedef void (__stdcall* PFNPROGRAM)(unsigned int, unsigned int);
extern int g_bARBVertexProgram;         // 0x0078E5EC
extern PFNPROGRAM g_pfnBindProgramARB;  // 0x007BB788
extern PFNPROGRAM g_pfnBindProgramNV;   // 0x007BB834

// FUNCTION: SWKOTOR 0x0046F550
void BindVertexProgram(unsigned int id)
{
    if (g_bARBVertexProgram)
        g_pfnBindProgramARB(0x8620, id);
    else
        g_pfnBindProgramNV(0x8620, id);
}

// FUNCTION: SWKOTOR 0x0046F580
int ClipProjectionCaps(Vector* a, Vector* b, Plane* p)
{
    if (b->x * p->n.x + b->y * p->n.y + b->z * p->n.z + p->d > g_fZeroEpsilon) {
        if (p->d > g_fZeroEpsilon)
            return 0;
        *b = PlaneIntersect(p->n, p->d, *a, *b);
    }
    if (a->x * p->n.x + a->y * p->n.y + a->z * p->n.z + p->d > g_fZeroEpsilon)
        *a = PlaneIntersect(p->n, p->d, *a, *b);
    return 1;
}

// FUNCTION: SWKOTOR 0x0046F650
void ClipProjection(Vector* a, Vector* b, Plane* p)
{
    if (a->x * p->n.x + a->y * p->n.y + a->z * p->n.z + p->d > g_fZeroEpsilon)
        *a = PlaneIntersect(p->n, p->d, *b, *a);
}
