// VertexProgram::Enable (aurvertexprog.o, 0x004A24D0): rebuilds and loads
// the ARB/NV program text when the program source, lighting or the GL
// generation changed, otherwise only binds it.
#include <string.h>
#pragma intrinsic(strcpy, strcat, strlen)

class LightManager {
public:
    void SetupVertexProgramLightShaderConstants(int nLights, int bCondition);  // 0x004A50D0 (placeholder name)
};

class Scene {
public:
    char pad_0x0[0x80];
    LightManager* m_pLightManager;                  // +0x80
};

// The lighting state argument; only +0x40 is read here.
struct VertexProgramLighting {
    char pad_0x0[0x40];
    int m_n40;                                      // +0x40 (placeholder name)
};

extern char* g_pCurrentVertexProgramSource;             // 0x008305DC (placeholder name)
extern unsigned char g_nCurrentVertexProgramLighting;   // 0x008305D8 (placeholder name)
extern int g_bVertexProgramRefresh;                     // 0x007A193C
extern unsigned int g_vertex_program_current_generation;    // 0x00830540
extern int g_bUseARBVertexProgram;                      // 0x0078E5EC
extern short g_bHeadLight;                              // 0x0078E388
extern Scene* g_pActiveScene;                           // 0x007B92A0 (placeholder name)
extern unsigned char(__stdcall* g_pfnIsProgramARB)(unsigned int);          // 0x007BB5B8
extern unsigned char(__stdcall* g_pfnIsProgramNV)(unsigned int);           // 0x007BB5AC
extern void(__stdcall* g_pfnGenProgramsARB)(int, unsigned int*);           // 0x007BB528 (placeholder name)
extern void(__stdcall* g_pfnGenProgramsNV)(int, unsigned int*);            // 0x007BB7B0 (placeholder name)
extern void(__stdcall* g_pfnBindProgramARB)(unsigned int, unsigned int);   // 0x007BB788
extern void(__stdcall* g_pfnBindProgramNV)(unsigned int, unsigned int);    // 0x007BB834

bool AurGLCaps5E0();                                    // 0x0045F5E0
void BindVertexProgram(unsigned int nId);               // 0x0046F550
void ReportARBError(const char* szProgram);             // 0x0047A2C0
void LoadVertexProgram_004a23c0(unsigned int nId, unsigned int nLength, const char* szText,
                                unsigned int* pParameters);                 // 0x004A23C0
void SetupVertexProgramLights(char* szText, int nLights, int bCondition);  // 0x004A30E0 (placeholder name)

struct VertexProgram {
    unsigned int unknown_00;
    char* original_source;
    char program_text[0x2000];
    unsigned int* parameter_words;
    unsigned int program_id;
    int allowed_lights;
    int generation;
    int enabled;

    void Enable(VertexProgramLighting* pLighting, unsigned char bLighting);
};

// NONMATCH (96 insns): the original keeps bLighting in bl and `this` in
// ebp; cl merges our ARB/NV if/else calls into one `call eax`, where the
// original hoists only the shared pushes and keeps both `call [pfn]`.
// FUNCTION: SWKOTOR 0x004A24D0
void VertexProgram::Enable(VertexProgramLighting* pLighting, unsigned char bLighting)
{
    if (g_pCurrentVertexProgramSource == original_source && !g_bVertexProgramRefresh &&
        g_nCurrentVertexProgramLighting == bLighting && AurGLCaps5E0()) {
        BindVertexProgram(program_id);
        return;
    }
    g_pCurrentVertexProgramSource = original_source;
    g_nCurrentVertexProgramLighting = bLighting;
    g_bVertexProgramRefresh = 0;
    if (pLighting && bLighting) {
        g_pActiveScene->m_pLightManager->SetupVertexProgramLightShaderConstants(
            allowed_lights, pLighting->m_n40 == 0);
    }
    if (generation != g_vertex_program_current_generation) {
        unsigned char bExists =
            (g_bUseARBVertexProgram ? g_pfnIsProgramARB : g_pfnIsProgramNV)(program_id);
        if (!bExists || bLighting) {
            if (enabled && !g_bHeadLight) {
                BindVertexProgram(program_id);
                return;
            }
            generation = g_vertex_program_current_generation;
            strcpy(program_text, original_source);
            if (bLighting && allowed_lights > 0) {
                char* szLights = new char[0x1000];
                szLights[0] = 0;
                SetupVertexProgramLights(szLights, allowed_lights,
                                         pLighting ? pLighting->m_n40 == 0 : 0);
                strcat(program_text, szLights);
                delete[] szLights;
            }
            strcat(program_text, "END");
            if (!program_id) {
                (g_bUseARBVertexProgram ? g_pfnGenProgramsARB : g_pfnGenProgramsNV)(1, &program_id);
            }
            (g_bUseARBVertexProgram ? g_pfnBindProgramARB : g_pfnBindProgramNV)(0x8620, program_id);
            LoadVertexProgram_004a23c0(program_id, strlen(program_text), program_text,
                                       parameter_words);
            if (g_bUseARBVertexProgram) {
                ReportARBError(program_text);
            }
            if (pLighting && bLighting && g_pActiveScene) {
                g_pActiveScene->m_pLightManager->SetupVertexProgramLightShaderConstants(
                    allowed_lights, pLighting->m_n40 == 0);
            }
            enabled = 1;
            return;
        }
    }
    (g_bUseARBVertexProgram ? g_pfnBindProgramARB : g_pfnBindProgramNV)(0x8620, program_id);
}
