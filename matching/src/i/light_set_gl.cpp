// SetLightGL (aurlightmanager.o, 0x004A2A00); name and signature from the
// Mac debug map.
#include "i/vector_math.h"

__declspec(dllimport) void __stdcall glLightfv(unsigned int light, unsigned int pname, const float* params);

#include "shared/globals.h"

// NONMATCH: cl hoists GL_LIGHT0 + nIndex above the bAmbient branch; the
// original computes it in each branch after the first FUN_004aaab0 call.
// FUNCTION: SWKOTOR 0x004A2A00
void SetLightGL(int nIndex, const Vector& vPosition, const Vector& vColor, const float& fRadius,
                const int& bAmbient)
{
    if (bAmbient) {
        glLightfv(nIndex + 0x4001, g_nGLAmbient, (float*)FUN_004aaab0(vColor, 1.0f));
        glLightfv(nIndex + 0x4001, g_nGLDiffuse, (float*)FUN_004aaab0(Vector(0.0f, 0.0f, 0.0f), 1.0f));
        glLightfv(nIndex + 0x4001, g_nGLSpecular, (float*)FUN_004aaab0(Vector(0.0f, 0.0f, 0.0f), 1.0f));
    } else {
        glLightfv(nIndex + 0x4001, g_nGLAmbient, (float*)FUN_004aaab0(Vector(0.0f, 0.0f, 0.0f), 1.0f));
        glLightfv(nIndex + 0x4001, g_nGLDiffuse, (float*)FUN_004aaab0(vColor, 1.0f));
        glLightfv(nIndex + 0x4001, g_nGLSpecular, (float*)FUN_004aaab0(vColor, 1.0f));
    }
    glLightfv(nIndex + 0x4001, g_nGLPosition, (float*)FUN_004aaab0(vPosition, 1.0f));
    float fR = fRadius > 0.1f ? fRadius : 0.1f;
    float fAtten = 9.0f / (fR * fR * 2.0f);
    glLightfv(nIndex + 0x4001, g_nGLQuadraticAttenuation, &fAtten);
}
