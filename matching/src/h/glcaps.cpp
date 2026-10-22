// OpenGL capability queries (0x0045F560-0x0045FD20).

#include "h/glcaps.h"

// FUNCTION: SWKOTOR 0x0045F560
int AurGLCaps560()
{
    return g_bGL4FC & HAS_EXT(g_nExt4D0);
}

// FUNCTION: SWKOTOR 0x0045F580
bool AurGLCaps580()
{
    return HAS_EXT(g_nExt4F0);
}

// FUNCTION: SWKOTOR 0x0045F5A0
bool AurGLCaps5A0()
{
    if (g_n504 == -1)
        g_n504 = !AurGLCaps560();
    return g_n504 == 1 ? true : false;
}

// FUNCTION: SWKOTOR 0x0045F5E0
bool AurGLCaps5E0()
{
    if (g_n508 == -1) {
        if (AurGLCaps560() && !(g_nGLExtensions & g_nExt4F4))
            g_n508 = 0;
        else
            g_n508 = 1;
    }
    return g_n508 == 1 ? true : false;
}

// FUNCTION: SWKOTOR 0x0045F640
bool AurGLCaps640()
{
    if (g_n50C == -1)
        g_n50C = AurGLCaps560() != 0;
    return g_n50C == 1 ? true : false;
}

// FUNCTION: SWKOTOR 0x0045F680
int AurGLCaps680()
{
    if (g_n510 == -1) {
        g_n510 = HAS_EXT(g_nExt4A8 | g_nExt49C | g_nExt490 | g_nExt48C | g_nExt484 | g_nExt480);
        if (!g_n510)
            g_n510 = HAS_EXT(g_nExt490 | g_nExt484 | g_nExt480 | g_nExt4D4 | g_nExt4D0) &
                     g_bGL4FC & g_bARBVertexProgram;
    }
    return g_n510;
}

// FUNCTION: SWKOTOR 0x0045F710
int AurGLCaps710()
{
    if (g_n514 == -1)
        g_n514 = HAS_EXT(g_nExt484);
    return g_n514;
}

// FUNCTION: SWKOTOR 0x0045F740
int AurGLCaps740()
{
    if (g_n518 == -1)
        g_n518 = HAS_EXT(g_nExt49C);
    return g_n518;
}

// FUNCTION: SWKOTOR 0x0045F770
int AurVertexProgramsAvailable()
{
    int r = g_n51C;
    if (r == -1)
        g_n51C = r = HAS_EXT(g_nExt4D4);
    if ((r & g_bARBVertexProgram) && g_bARBVertexProgram)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0045F7B0
int AurGLCaps7B0()
{
    if (g_n520 == -1) {
        if (HAS_EXT(g_nExt4DC | g_nExt4E4))
            g_n520 = 1;
        else
            g_n520 = 0;
    }
    return g_n520;
}

// FUNCTION: SWKOTOR 0x0045F7E0
int AurGLCaps7E0()
{
    if (!g_bGLExtension860)
        return 0;
    if (g_n524 == -1) {
        if (HAS_EXT(g_nExt4C8 | g_nExt4D0))
            g_n524 = 1;
        else
            g_n524 = 0;
    }
    return g_n524;
}

// FUNCTION: SWKOTOR 0x0045F820
int AurGLCaps820()
{
    if (g_n528 == -1) {
        if (HAS_EXT(g_nExt4D4 | g_nExt4DC | g_nExt4C8))
            g_n528 = 1;
        else
            g_n528 = 0;
    }
    return g_n528;
}

// FUNCTION: SWKOTOR 0x0045F860
int AurATIFrameBufferModificationsAvailable()
{
    if (g_n52C == -1) {
        g_n52C = HAS_EXT(g_nExt4D4 | g_nExt4D0 | g_nExt4C8);
        if (!AurGLCaps5E0())
            g_n52C = 0;
    }
    return g_n52C;
}

// FUNCTION: SWKOTOR 0x0045F8B0
int AurGLCaps8B0()
{
    if (g_n530 == -1)
        g_n530 = HAS_EXT(g_nExt484 | g_nExt490);
    return g_n530;
}

// FUNCTION: SWKOTOR 0x0045F8E0
int AurGLCaps8E0()
{
    if (g_n534 == -1)
        g_n534 = HAS_EXT(g_nExt4A0);
    return g_n534;
}

// FUNCTION: SWKOTOR 0x0045F910
int AurGLCaps910()
{
    if (g_n538 == -1) {
        if (g_nGLExtensions & g_nExt4D0)
            g_n538 = 0;
        else
            g_n538 = (g_nExt4CC & g_nGLExtensions) == g_nExt4CC;
    }
    return g_n538;
}

// FUNCTION: SWKOTOR 0x0045F950
int AurGLCaps950()
{
    if (g_n53C == -1)
        g_n53C = HAS_EXT(g_nExt4BC | g_nExt4B8);
    return g_n53C;
}

// FUNCTION: SWKOTOR 0x0045F980
bool CheckExtension(char* szExtension, char* szList)
{
    char* p = szList;
    int nLength = strlen(szExtension);
    char* pEnd = p + strlen(p);
    while (p < pEnd) {
        int n = strcspn(p, " ");
        if (nLength == n && !strncmp(szExtension, p, n))
            return true;
        p += n + 1;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0045F9F0
int AurGLCaps9F0()
{
    if (g_n540 == -1)
        g_n540 = (g_nGLExtensions & g_nExt4E0) != 0;
    return g_n540;
}

// FUNCTION: SWKOTOR 0x0045FA20
bool AurGLCapsA20()
{
    return AurGLCaps7B0() || AurGLCaps7E0() ? true : false;
}

// FUNCTION: SWKOTOR 0x0045FAB0
bool AurCheckFrameBufferModificationsAvailable()
{
    return AurGLCaps820() || AurATIFrameBufferModificationsAvailable() ? true : false;
}

// FUNCTION: SWKOTOR 0x0045FB10
int AurBumpMappingAvailable()
{
    int r = HAS_EXT(g_nExt4D4 | g_nExt490 | g_nExt48C | g_nExt484 | g_nExt480);
    if (r)
        return r;
    if (g_n51C == -1)
        g_n51C = HAS_EXT(g_nExt4D4);
    if ((g_n51C & g_bARBVertexProgram) && AurGLCaps560())
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0045FB90
bool aurATIFragmentShadersBumpMapAvailable()
{
    if (g_nShaderLevel < 1)
        return false;
    if (AurGLCaps560() && g_nShaderLevel == 1)
        return false;
    return AurBumpMappingAvailable() ? true : false;
}

// FUNCTION: SWKOTOR 0x0045FBD0
bool AurNV2xLevelShadersAvailable()
{
    if (g_nShaderLevel < 2)
        return false;
    return AurGLCaps680() ? true : false;
}

// FUNCTION: SWKOTOR 0x0045FBF0
int AurOnePassEnvMapAvailable()
{
    if (g_n544 == -1) {
        if (aurATIFragmentShadersBumpMapAvailable()) {
            if (AurGLCaps560()) {
                g_n544 = 1;
            } else {
                glGetIntegerv(g_eMaxTextureUnits, &g_n544);
                g_n544 = g_n544 >= 2;
            }
        } else {
            g_n544 = 0;
        }
    }
    return g_n544;
}

// FUNCTION: SWKOTOR 0x0045FC90
int AurVertexBufferObjectARBAvailable()
{
    if (g_bNoVBO == 1)
        return 0;
    if (g_n548 == -1) {
        g_n548 = HAS_EXT(g_nExt4D8);
        if (!AurNV2xLevelShadersAvailable())
            g_n548 = 0;
        if (!AurGLCaps5E0())
            g_n548 = 0;
    }
    return g_n548;
}

// FUNCTION: SWKOTOR 0x0045FD00
int AurGetAvailableStencilBits()
{
    if (g_nStencilBits == -1)
        glGetIntegerv(0xd57, &g_nStencilBits);
    return g_nStencilBits;
}

// FUNCTION: SWKOTOR 0x0045FD20
int AurGetStencilClearValue()
{
    if (!AurGetAvailableStencilBits())
        return 0;
    if (g_nStencilClear == -1)
        g_nStencilClear = (1 << (g_nStencilBits - 1)) - 1;
    return g_nStencilClear;
}

extern int g_nExt4EC;               // 0x0078E4EC
extern int g_n5D8;                  // 0x0078E5D8

// FUNCTION: SWKOTOR 0x0046ED70
int AurGLCaps5D8()
{
    if (g_n5D8 == -1)
        g_n5D8 = HAS_EXT(g_nExt4EC);
    return g_n5D8;
}
