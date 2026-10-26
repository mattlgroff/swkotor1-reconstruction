// NV register-combiner constant setters stored in the renderer's
// constant table (0x00424940 / 0x00424960; both missing from
// functions.csv).
#include "g/fb_effects.h"

typedef void(__stdcall* PFNGLCOMBINERPARAMETERFVNV)(unsigned int pname, const float* params);
extern PFNGLCOMBINERPARAMETERFVNV glCombinerParameterfvNV;  // 0x007bb7a8

// FUNCTION: SWKOTOR 0x00424940
void SetCombinerConstant0(const Vector& vColor, float fAlpha)
{
    glCombinerParameterfvNV(0x852a, AurMakeColor(vColor, fAlpha));
}

// FUNCTION: SWKOTOR 0x00424960
void SetCombinerConstant1(const Vector& vColor, float fAlpha)
{
    glCombinerParameterfvNV(0x852b, AurMakeColor(vColor, fAlpha));
}
