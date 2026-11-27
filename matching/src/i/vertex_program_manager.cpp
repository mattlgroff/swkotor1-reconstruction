struct VertexProgramData {
    char pad_0x0[0x200c];
    unsigned int program_id;
    int allowed_lights;
    int generation;
    int enabled;
};

#include "shared/globals.h"
typedef void (__stdcall *GlDeleteProgramsARB)(int, const unsigned int*);
extern GlDeleteProgramsARB gl_delete_programs_arb_007bb724;
extern GlDeleteProgramsARB gl_delete_programs_nv_007bb6f4;
typedef unsigned char (__stdcall *GlIsProgramARB)(unsigned int);
extern GlIsProgramARB gl_is_program_arb_007bb5b8;
extern GlIsProgramARB gl_is_program_nv_007bb5ac;

// FUNCTION: SWKOTOR 0x004A2470
void __cdecl ResetVertexPrograms_004a2470()
{
    int i = 0;
    for (; i < g_vertex_program_size_008305d0; ++i) {
        gl_delete_programs_arb_007bb724(1, &g_vertex_program_data_008305cc[i]->program_id);
        g_vertex_program_data_008305cc[i]->program_id = 0;
        g_vertex_program_data_008305cc[i]->enabled = 0;
    }
}
