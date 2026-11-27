// combiners.h -- the texture-combiner entry points (CODE_RULES.md, R2)
//
// LoadExtensions (g/ext_loader.cpp) points these at the ATI fragment-shader
// or the NV register-combiner functions; the renderers call through them.
#ifndef SHARED_COMBINERS_H
#define SHARED_COMBINERS_H

#include "shared/vector.h"

class CAurTexture;

typedef void (__cdecl* PFNENABLECOMBINERS)(int nMode, CAurTexture* pTex0, CAurTexture* pTex1,
                                         CAurTexture* pTex2, CAurTexture* pTex3);
typedef void (__cdecl* PFNSETCOMBINERCONSTANT)(const Vector& vColor, float fAlpha);

extern PFNSETCOMBINERCONSTANT g_pfnSetCombinerConstant1;    // 0x007A47DC
extern PFNENABLECOMBINERS g_pfnEnableCombiners;             // 0x007A47E0
extern PFNSETCOMBINERCONSTANT g_pfnSetCombinerConstant0;    // 0x007A6820

#endif
