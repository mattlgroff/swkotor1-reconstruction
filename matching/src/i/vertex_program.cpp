extern unsigned int g_vertex_program_current_generation;

// FUNCTION: SWKOTOR 0x004A23B0
void AurVertexProgramMakeCurrent()
{
    ++g_vertex_program_current_generation;
}
