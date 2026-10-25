#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x005F0C60
int CClientExoAppInternal::IsValidResolution(int width, int height)
{
    if (width == 0x320) {
        if (height == 0x258) return 1;
    }
    if (width == 0x400) {
        if (height == 0x300) return 1;
    }
    if (width == 0x500) {
        if (height == 0x3c0 || height == 0x400) return 1;
    }
    if (width == 0x640) {
        if (height == 0x4b0) return 1;
    }
    return 0;
}
