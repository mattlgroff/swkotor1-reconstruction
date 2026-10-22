// AurPixelShadersAvailable 0x0046EDA0 (own TU so the capability
// queries it uses stay calls).

#include "h/glcaps.h"

bool aurATIFragmentShadersBumpMapAvailable();   // 0x0045FB90
extern int g_n5DC;                  // 0x0078E5DC

// FUNCTION: SWKOTOR 0x0046EDA0
int AurPixelShadersAvailable()
{
    if (g_n5DC == -1) {
        if (aurATIFragmentShadersBumpMapAvailable()) {
            if (AurGLCaps560()) {
                g_n5DC = 1;
                return 1;
            }
            glGetIntegerv(g_eMaxTextureUnits, &g_n5DC);
            if (g_n5DC >= 8) {
                g_n5DC = 1;
                return 1;
            }
        }
        g_n5DC = 0;
        g_n5DC = AurGLCaps560();
    }
    return g_n5DC;
}
