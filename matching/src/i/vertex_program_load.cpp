

#include "shared/globals.h"
#include "shared/gl_ext.h"

// FUNCTION: SWKOTOR 0x004A23C0
void __cdecl LoadVertexProgram_004a23c0(unsigned int program, unsigned int length,
                                        const char* text, unsigned int* parameters)
{
    (void)parameters;
    if (g_programs_arb_0078e5ec != 0) {
        glProgramStringARB(0x8620, 0x8875, (int)length, text);
    } else {
        glLoadProgramNV(0x8620, program, (int)length,
                                   (const unsigned char*)text);
    }
}
