// OpenGL capability queries (0x0045F560-0x0045FD20). Each query
// tests the extension mask 0x007BB85C against the masks of the extensions it
// needs and most cache the answer in a global initialised to -1.
#ifndef MATCHING_H_GLCAPS_H
#define MATCHING_H_GLCAPS_H

extern "C" unsigned int __cdecl strlen(const char*);
extern "C" unsigned int __cdecl strcspn(const char*, const char*);
extern "C" int __cdecl strncmp(const char*, const char*, unsigned int);

extern "C" __declspec(dllimport) void __stdcall glGetIntegerv(unsigned int, int*);

#include "shared/globals.h"

// Extension masks.
extern int g_nExt480;
extern int g_nExt484;
extern int g_nExt48C;
extern int g_nExt490;
extern int g_nExt49C;
extern int g_nExt4A0;
extern int g_nExt4A8;
extern int g_nExt4B8;
extern int g_nExt4BC;
extern int g_nExt4C8;
extern int g_nExt4CC;
extern int g_nExt4D0;
extern int g_nExt4D4;
extern int g_nExt4D8;
extern int g_nExt4DC;
extern int g_nExt4E0;
extern int g_nExt4E4;
extern int g_nExt4F0;
extern int g_nExt4F4;

// Cached answers (-1 until first asked).
extern int g_n504;
extern int g_n508;
extern int g_n50C;
extern int g_n510;
extern int g_n514;
extern int g_n518;
extern int g_n51C;
extern int g_n520;
extern int g_n524;
extern int g_n528;
extern int g_n52C;
extern int g_n530;
extern int g_n534;
extern int g_n538;
extern int g_n53C;
extern int g_n540;
extern int g_n544;
extern int g_n548;
extern int g_nStencilBits;
extern int g_nStencilClear;

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
