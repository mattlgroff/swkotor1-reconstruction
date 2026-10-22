// OpenGL capability queries (0x0045F560-0x0045FD20). Each query
// tests the extension mask 0x007BB85C against the masks of the extensions it
// needs and most cache the answer in a global initialised to -1.
#ifndef MATCHING_H_GLCAPS_H
#define MATCHING_H_GLCAPS_H

extern "C" unsigned int __cdecl strlen(const char*);
extern "C" unsigned int __cdecl strcspn(const char*, const char*);
extern "C" int __cdecl strncmp(const char*, const char*, unsigned int);

extern "C" __declspec(dllimport) void __stdcall glGetIntegerv(unsigned int, int*);

extern int g_nGLExtensions;         // 0x007BB85C
extern int g_bGLExtension860;       // 0x007BB860
extern unsigned char g_bNoVBO;      // 0x007B927D
extern int g_nShaderLevel;          // 0x0078E500
extern int g_bGL4FC;                // 0x0078E4FC
extern int g_bARBVertexProgram;     // 0x0078E5EC
extern unsigned int g_eMaxTextureUnits; // 0x0073F26C

// Extension masks.
extern int g_nExt480, g_nExt484, g_nExt48C, g_nExt490, g_nExt49C, g_nExt4A0,
    g_nExt4A8, g_nExt4B8, g_nExt4BC, g_nExt4C8, g_nExt4CC, g_nExt4D0,
    g_nExt4D4, g_nExt4D8, g_nExt4DC, g_nExt4E0, g_nExt4E4, g_nExt4F0,
    g_nExt4F4;

// Cached answers (-1 until first asked).
extern int g_n504, g_n508, g_n50C, g_n510, g_n514, g_n518, g_n51C, g_n520,
    g_n524, g_n528, g_n52C, g_n530, g_n534, g_n538, g_n53C, g_n540,
    g_n544, g_n548, g_nStencilBits, g_nStencilClear;

inline int HAS_EXT(int m) { return (g_nGLExtensions & m) == m; }

int AurGLCaps560();                         // 0x0045F560
bool AurGLCaps580();                        // 0x0045F580
bool AurGLCaps5A0();                        // 0x0045F5A0
bool AurGLCaps5E0();                        // 0x0045F5E0
bool AurGLCaps640();                        // 0x0045F640
int AurGLCaps680();                         // 0x0045F680
int AurGLCaps710();                         // 0x0045F710
int AurGLCaps740();                         // 0x0045F740
int AurVertexProgramsAvailable();           // 0x0045F770
int AurGLCaps7B0();                         // 0x0045F7B0
int AurGLCaps7E0();                         // 0x0045F7E0
int AurGLCaps820();                         // 0x0045F820
int AurATIFrameBufferModificationsAvailable();  // 0x0045F860
int AurGLCaps8B0();                         // 0x0045F8B0
int AurGLCaps8E0();                         // 0x0045F8E0
int AurGLCaps910();                         // 0x0045F910
int AurGLCaps950();                         // 0x0045F950
bool CheckExtension(char* szExtension, char* szList);  // 0x0045F980
int AurGLCaps9F0();                         // 0x0045F9F0
bool AurGLCapsA20();                        // 0x0045FA20
bool AurCheckFrameBufferModificationsAvailable();   // 0x0045FAB0
int AurBumpMappingAvailable();              // 0x0045FB10
bool aurATIFragmentShadersBumpMapAvailable();   // 0x0045FB90
bool AurNV2xLevelShadersAvailable();        // 0x0045FBD0
int AurOnePassEnvMapAvailable();            // 0x0045FBF0
int AurVertexBufferObjectARBAvailable();    // 0x0045FC90
int AurGetAvailableStencilBits();           // 0x0045FD00
int AurGetStencilClearValue();              // 0x0045FD20

#endif
