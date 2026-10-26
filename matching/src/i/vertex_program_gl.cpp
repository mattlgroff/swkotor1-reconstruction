// Vertex-program GL helpers (aurvertexprog.o): matrix tracking setup
// (0x004A2400) and program teardown (0x004A2830). Descriptive names.
struct VertexProgram {
    unsigned int unknown_00;
    char* original_source;
    char program_text[0x2000];
    unsigned int* parameter_words;
    unsigned int program_id;                            // +0x200c
    int allowed_lights;
    int generation;
    int enabled;

    ~VertexProgram();
};

struct VertexProgramArray {
    VertexProgram** data;
    int size;
    int capacity;

    void Remove(VertexProgram* p);                      // 0x00445080
};

extern VertexProgramArray g_vertex_programs_008305cc;
extern int g_bARBVertexProgram;                         // 0x0078E5EC

extern unsigned char (__stdcall* g_pfnIsProgramARB)(unsigned int id);          // 0x007BB5B8
extern unsigned char (__stdcall* g_pfnIsProgramNV)(unsigned int id);           // 0x007BB5AC
extern void (__stdcall* g_pfnDeleteProgramsARB)(int n, const unsigned int* p); // 0x007BB724
extern void (__stdcall* g_pfnDeleteProgramsNV)(int n, const unsigned int* p);  // 0x007BB6F4
extern void (__stdcall* g_pfnTrackMatrixNV)(unsigned int target, unsigned int address,
                                            unsigned int matrix, unsigned int transform);  // 0x007BB744
extern void (__stdcall* g_pfnBindProgramARB)(unsigned int target, unsigned int id);       // 0x007BB788
extern void (__stdcall* g_pfnBindProgramNV)(unsigned int target, unsigned int id);        // 0x007BB834

extern unsigned int g_nTrackModelViewProjection;        // 0x0073F218
extern unsigned int g_nTrackModelView;                  // 0x0073F21C
extern unsigned int g_nTrackIdentity;                   // 0x0073F224

inline unsigned char AurIsProgram(unsigned int nId)
{
    if (g_bARBVertexProgram)
        return g_pfnIsProgramARB(nId);
    return g_pfnIsProgramNV(nId);
}

// FUNCTION: SWKOTOR 0x004A2400
void AurVertexProgramTrackMatrices()
{
    if (!g_bARBVertexProgram) {
        g_pfnTrackMatrixNV(0x8620, 0, g_nTrackModelViewProjection, g_nTrackIdentity);
        if (!g_bARBVertexProgram) {
            g_pfnTrackMatrixNV(0x8620, 8, g_nTrackModelView, g_nTrackIdentity);
        }
    }
    if (g_bARBVertexProgram) {
        g_pfnBindProgramARB(0x8620, 0);
    } else {
        g_pfnBindProgramNV(0x8620, 0);
    }
}

// FUNCTION: SWKOTOR 0x004A2830
VertexProgram::~VertexProgram()
{
    if (AurIsProgram(program_id)) {
        if (g_bARBVertexProgram) {
            g_pfnDeleteProgramsARB(1, &program_id);
        } else {
            g_pfnDeleteProgramsNV(1, &program_id);
        }
    }
    g_vertex_programs_008305cc.Remove(this);
}
