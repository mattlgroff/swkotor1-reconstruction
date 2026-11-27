struct VertexProgramData {
    char pad_0x0[0x200c];
    unsigned int program_id;
    int allowed_lights;
    int generation;
    int enabled;
};

#include "shared/globals.h"

#include "shared/gl_ext.h"

// FUNCTION: SWKOTOR 0x004A2470
void __cdecl ResetVertexPrograms_004a2470()
{
    int i = 0;
    for (; i < g_vertex_program_size_008305d0; ++i) {
        glDeleteProgramsARB(1, &g_vertex_program_data_008305cc[i]->program_id);
        g_vertex_program_data_008305cc[i]->program_id = 0;
        g_vertex_program_data_008305cc[i]->enabled = 0;
    }
}
