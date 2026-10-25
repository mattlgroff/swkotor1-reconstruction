typedef void (__stdcall *GlProgramStringARB)(unsigned int, unsigned int, int, const char*);
typedef void (__stdcall *GlLoadProgramNV)(unsigned int, unsigned int, int, const unsigned char*);

extern int g_programs_arb_0078e5ec;
extern GlProgramStringARB gl_program_string_arb_007bb580;
extern GlLoadProgramNV gl_load_program_nv_007bb5e4;

// FUNCTION: SWKOTOR 0x004A23C0
void __cdecl LoadVertexProgram_004a23c0(unsigned int program, unsigned int length,
                                        const char* text, unsigned int* parameters)
{
    (void)parameters;
    if (g_programs_arb_0078e5ec != 0) {
        gl_program_string_arb_007bb580(0x8620, 0x8875, (int)length, text);
    } else {
        gl_load_program_nv_007bb5e4(0x8620, program, (int)length,
                                   (const unsigned char*)text);
    }
}
