struct VertexProgramArray {
    void* data;
    int size;
    int capacity;
    void Allocate(int count);
};

extern VertexProgramArray g_vertex_programs_008305cc;

// FUNCTION: SWKOTOR 0x004A2770
void __cdecl AddVertexProgramToManager_004a2770(void* program)
{
    if (g_vertex_programs_008305cc.size == g_vertex_programs_008305cc.capacity) {
        int capacity = g_vertex_programs_008305cc.capacity;
        if (capacity != 0)
            capacity += capacity;
        else
            capacity = 8;
        g_vertex_programs_008305cc.Allocate(capacity);
    }
    ((void**)g_vertex_programs_008305cc.data)[g_vertex_programs_008305cc.size] = program;
    ++g_vertex_programs_008305cc.size;
}
