// LoadExtensions (0x00436490): reads the GL / WGL extension strings,
// sets the capability bits, fetches every extension entry point and picks
// the ATI or NV combiner path. Name not recorded (FUN_00436490); behavioural
// reference: src/units/0788/0f67f8bd6c-extension_loader.cpp.
typedef unsigned int size_t;

extern "C" {
__declspec(dllimport) void* __stdcall wglGetProcAddress(const char* szName);
__declspec(dllimport) void* __stdcall wglGetCurrentDC();
__declspec(dllimport) const char* __stdcall glGetString(unsigned int name);
__declspec(dllimport) void __stdcall glGetFloatv(unsigned int pname, float* params);
char* __cdecl strstr(const char* s, const char* sub);
size_t __cdecl strlen(const char* s);
char* __cdecl strcpy(char* d, const char* s);
char* __cdecl strcat(char* d, const char* s);
}
#pragma intrinsic(strlen, strcpy, strcat)

void* __cdecl operator new[](size_t n);

#define GL_RENDERER 0x1f01
#define GL_EXTENSIONS 0x1f03
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84ff
#define GL_VERTEX_PROGRAM_ARB 0x8620

typedef const char*(__stdcall* PFNWGLGETEXTENSIONSSTRINGARB)(void* hDC);
extern PFNWGLGETEXTENSIONSSTRINGARB wglGetExtensionsStringARB;  // 0x007bb7ec

// GL / WGL extension entry points (function-pointer globals).
extern void* glMultiTexCoord2fARB;                        // 0x007bb6e8
extern void* glMultiTexCoord2iARB;                        // 0x007bb604
extern void* glMultiTexCoord3fARB;                        // 0x007bb568
extern void* glMultiTexCoord2fvARB;                       // 0x007bb7f8
extern void* glMultiTexCoord3fvARB;                       // 0x007bb6ac
extern void* glMultiTexCoord4fvARB;                       // 0x007bb7e8
extern void* glActiveTextureARB;                          // 0x007bb80c
extern void* glClientActiveTextureARB;                    // 0x007bb6b0
extern void* glBindBufferARB;                             // 0x007bb7b4
extern void* glDeleteBuffersARB;                          // 0x007bb7bc
extern void* glGenBuffersARB;                             // 0x007bb5b4
extern void* glIsBufferARB;                               // 0x007bb728
extern void* glBufferDataARB;                             // 0x007bb544
extern void* glBufferSubDataARB;                          // 0x007bb75c
extern void* glGetBufferSubDataARB;                       // 0x007bb828
extern void* glMapBufferARB;                              // 0x007bb634
extern void* glUnmapBufferARB;                            // 0x007bb6e0
extern void* glGetBufferParameterivARB;                   // 0x007bb5a4
extern void* glGetBufferPointervARB;                      // 0x007bb778
extern void* glVertexAttrib1sARB;                         // 0x007bb65c
extern void* glVertexAttrib1fARB;                         // 0x007bb72c
extern void* glVertexAttrib1dARB;                         // 0x007bb5a8
extern void* glVertexAttrib2sARB;                         // 0x007bb6f0
extern void* glVertexAttrib2fARB;                         // 0x007bb798
extern void* glVertexAttrib2dARB;                         // 0x007bb60c
extern void* glVertexAttrib3sARB;                         // 0x007bb58c
extern void* glVertexAttrib3fARB;                         // 0x007bb5cc
extern void* glVertexAttrib3dARB;                         // 0x007bb7a4
extern void* glVertexAttrib4sARB;                         // 0x007bb600
extern void* glVertexAttrib4fARB;                         // 0x007bb668
extern void* glVertexAttrib4dARB;                         // 0x007bb7d4
extern void* glVertexAttrib4NubARB;                       // 0x007bb760
extern void* glVertexAttrib1svARB;                        // 0x007bb7d0
extern void* glVertexAttrib1fvARB;                        // 0x007bb740
extern void* glVertexAttrib1dvARB;                        // 0x007bb844
extern void* glVertexAttrib2svARB;                        // 0x007bb758
extern void* glVertexAttrib2fvARB;                        // 0x007bb574
extern void* glVertexAttrib2dvARB;                        // 0x007bb6c8
extern void* glVertexAttrib3svARB;                        // 0x007bb7cc
extern void* glVertexAttrib3fvARB;                        // 0x007bb5d4
extern void* glVertexAttrib3dvARB;                        // 0x007bb780
extern void* glVertexAttrib4bvARB;                        // 0x007bb82c
extern void* glVertexAttrib4svARB;                        // 0x007bb548
extern void* glVertexAttrib4ivARB;                        // 0x007bb564
extern void* glVertexAttrib4ubvARB;                       // 0x007bb540
extern void* glVertexAttrib4usvARB;                       // 0x007bb66c
extern void* glVertexAttrib4uivARB;                       // 0x007bb708
extern void* glVertexAttrib4fvARB;                        // 0x007bb794
extern void* glVertexAttrib4dvARB;                        // 0x007bb5c4
extern void* glVertexAttrib4NbvARB;                       // 0x007bb650
extern void* glVertexAttrib4NsvARB;                       // 0x007bb52c
extern void* glVertexAttrib4NivARB;                       // 0x007bb848
extern void* glVertexAttrib4NubvARB;                      // 0x007bb690
extern void* glVertexAttrib4NusvARB;                      // 0x007bb69c
extern void* glVertexAttrib4NuivARB;                      // 0x007bb748
extern void* glVertexAttribPointerARB;                    // 0x007bb7d8
extern void* glEnableVertexAttribArrayARB;                // 0x007bb79c
extern void* glDisableVertexAttribArrayARB;               // 0x007bb770
extern void* glProgramStringARB;                          // 0x007bb580
extern void* glBindProgramARB;                            // 0x007bb788
extern void* glDeleteProgramsARB;                         // 0x007bb724
extern void* glGenProgramsARB;                            // 0x007bb528
extern void* glProgramEnvParameter4dARB;                  // 0x007bb54c
extern void* glProgramEnvParameter4dvARB;                 // 0x007bb718
extern void* glProgramEnvParameter4fARB;                  // 0x007bb850
extern void* glProgramEnvParameter4fvARB;                 // 0x007bb6bc
extern void* glProgramLocalParameter4dARB;                // 0x007bb76c
extern void* glProgramLocalParameter4dvARB;               // 0x007bb588
extern void* glProgramLocalParameter4fARB;                // 0x007bb73c
extern void* glProgramLocalParameter4fvARB;               // 0x007bb784
extern void* glGetProgramEnvParameterdvARB;               // 0x007bb6a4
extern void* glGetProgramEnvParameterfvARB;               // 0x007bb56c
extern void* glGetProgramLocalParameterdvARB;             // 0x007bb618
extern void* glGetProgramLocalParameterfvARB;             // 0x007bb6e4
extern void* glGetProgramivARB;                           // 0x007bb670
extern void* glGetProgramStringARB;                       // 0x007bb808
extern void* glGetVertexAttribdvARB;                      // 0x007bb77c
extern void* glGetVertexAttribfvARB;                      // 0x007bb71c
extern void* glGetVertexAttribivARB;                      // 0x007bb684
extern void* glGetVertexAttribPointervARB;                // 0x007bb764
extern void* glIsProgramARB;                              // 0x007bb5b8
extern void* glCombinerParameterfvNV;                     // 0x007bb7a8
extern void* glCombinerParameterivNV;                     // 0x007bb81c
extern void* glCombinerParameterfNV;                      // 0x007bb678
extern void* glCombinerParameteriNV;                      // 0x007bb664
extern void* glCombinerInputNV;                           // 0x007bb738
extern void* glCombinerOutputNV;                          // 0x007bb59c
extern void* glFinalCombinerInputNV;                      // 0x007bb654
extern void* glGetCombinerInputParameterfvNV;             // 0x007bb53c
extern void* glGetCombinerInputParameterivNV;             // 0x007bb810
extern void* glGetCombinerOutputParameterfvNV;            // 0x007bb6a8
extern void* glGetCombinerOutputParameterivNV;            // 0x007bb84c
extern void* glGetFinalCombinerInputParameterfvNV;        // 0x007bb640
extern void* glGetFinalCombinerInputParameterivNV;        // 0x007bb6d8
extern void* glCombinerStageParameterfvNV;                // 0x007bb6f8
extern void* glFogCoordPointerEXT;                        // 0x007bb648
extern void* glFogCoordfEXT;                              // 0x007bb7e0
extern void* glLockArraysEXT;                             // 0x007bb6b4
extern void* glUnlockArraysEXT;                           // 0x007bb5bc
extern void* glSecondaryColor3fvEXT;                      // 0x007bb714
extern void* glGenProgramsNV;                             // 0x007bb7b0
extern void* glDeleteProgramsNV;                          // 0x007bb6f4
extern void* glLoadProgramNV;                             // 0x007bb5e4
extern void* glBindProgramNV;                             // 0x007bb834
extern void* glTrackMatrixNV;                             // 0x007bb744
extern void* glProgramParameter4fNV;                      // 0x007bb7ac
extern void* glExecuteProgramNV;                          // 0x007bb7f0
extern void* glGetProgramParameterfvNV;                   // 0x007bb55c
extern void* glVertexAttribPointerNV;                     // 0x007bb854
extern void* glIsProgramNV;                               // 0x007bb5ac
extern void* glVertexAttrib1fvNV;                         // 0x007bb734
extern void* glVertexAttrib2fvNV;                         // 0x007bb820
extern void* glVertexAttrib3fvNV;                         // 0x007bb768
extern void* glVertexAttrib4fvNV;                         // 0x007bb5fc
extern void* wglAllocateMemoryNV;                         // 0x007bb658
extern void* wglFreeMemoryNV;                             // 0x007bb5e0
extern void* glVertexArrayRangeNV;                        // 0x007bb7c0
extern void* glDeleteFencesNV;                            // 0x007bb638
extern void* glGenFencesNV;                               // 0x007bb57c
extern void* glIsFenceNV;                                 // 0x007bb660
extern void* glTestFenceNV;                               // 0x007bb5ec
extern void* glGetFenceivNV;                              // 0x007bb800
extern void* glFinishFenceNV;                             // 0x007bb584
extern void* glSetFenceNV;                                // 0x007bb560
extern void* glGetCompressedTexImageARB;                  // 0x007bb5f0
extern void* glCompressedTexImage2DARB;                   // 0x007bb774
extern void* glPNTrianglesiATI;                           // 0x007bb644
extern void* glPNTrianglesfATI;                           // 0x007bb614
extern void* wglChoosePixelFormatARB;                     // 0x007bb730
extern void* wglCreatePbufferARB;                         // 0x007bb530
extern void* wglGetPbufferDCARB;                          // 0x007bb6b8
extern void* wglReleasePbufferDCARB;                      // 0x007bb578
extern void* wglDestroyPbufferARB;                        // 0x007bb598
extern void* wglGetPixelFormatAttribivARB;                // 0x007bb63c
extern void* wglQueryPbufferARB;                          // 0x007bb7a0
extern void* wglMakeContextCurrentARB;                    // 0x007bb818
extern void* wglBindTexImageARB;                          // 0x007bb7b8
extern void* wglReleaseTexImageARB;                       // 0x007bb6d0
extern void* glGenFragmentShadersATI;                     // 0x007bb754
extern void* glBindFragmentShaderATI;                     // 0x007bb6cc
extern void* glDeleteFragmentShaderATI;                   // 0x007bb550
extern void* glBeginFragmentShaderATI;                    // 0x007bb7f4
extern void* glEndFragmentShaderATI;                      // 0x007bb554
extern void* glPassTexCoordATI;                           // 0x007bb804
extern void* glSampleMapATI;                              // 0x007bb620
extern void* glColorFragmentOp1ATI;                       // 0x007bb6d4
extern void* glColorFragmentOp2ATI;                       // 0x007bb840
extern void* glColorFragmentOp3ATI;                       // 0x007bb7fc
extern void* glAlphaFragmentOp1ATI;                       // 0x007bb710
extern void* glAlphaFragmentOp2ATI;                       // 0x007bb5e8
extern void* glAlphaFragmentOp3ATI;                       // 0x007bb680
extern void* glSetFragmentShaderConstantATI;              // 0x007bb5d8
extern void* glBeginVertexShaderEXT;                      // 0x007bb7c8
extern void* glEndVertexShaderEXT;                        // 0x007bb830
extern void* glBindVertexShaderEXT;                       // 0x007bb534
extern void* glGenVertexShadersEXT;                       // 0x007bb7c4
extern void* glDeleteVertexShadersEXT;                    // 0x007bb694
extern void* glShaderOp1EXT;                              // 0x007bb628
extern void* glShaderOp2EXT;                              // 0x007bb698
extern void* glShaderOp3EXT;                              // 0x007bb790
extern void* glSwizzleEXT;                                // 0x007bb610
extern void* glWriteMaskEXT;                              // 0x007bb630
extern void* glInsertComponentEXT;                        // 0x007bb720
extern void* glExtractComponentEXT;                       // 0x007bb67c
extern void* glGenSymbolsEXT;                             // 0x007bb6dc
extern void* glSetInvariantEXT;                           // 0x007bb61c
extern void* glSetLocalConstantEXT;                       // 0x007bb7e4
extern void* glVariantbvEXT;                              // 0x007bb6a0
extern void* glVariantsvEXT;                              // 0x007bb590
extern void* glVariantivEXT;                              // 0x007bb814
extern void* glVariantfvEXT;                              // 0x007bb5d0
extern void* glVariantdvEXT;                              // 0x007bb7dc
extern void* glVariantubEXT;                              // 0x007bb674
extern void* glVariantusEXT;                              // 0x007bb5c8
extern void* glVariantuiEXT;                              // 0x007bb6c0
extern void* glVariantPointerEXT;                         // 0x007bb70c
extern void* glEnableVariantClientStateEXT;               // 0x007bb838
extern void* glDisableVariantClientStateEXT;              // 0x007bb6ec
extern void* glBindLightParameterEXT;                     // 0x007bb6fc
extern void* glBindMaterialParameterEXT;                  // 0x007bb5f8
extern void* glBindTexGenParameterEXT;                    // 0x007bb64c
extern void* glBindTextureUnitParameterEXT;               // 0x007bb5dc
extern void* glBindParameterEXT;                          // 0x007bb6c4
extern void* glIsVariantEnabledEXT;                       // 0x007bb83c
extern void* glGetVariantBooleanvEXT;                     // 0x007bb62c
extern void* glGetVariantIntegervEXT;                     // 0x007bb558
extern void* glGetVariantFloatvEXT;                       // 0x007bb608
extern void* glGetVariantPointervEXT;                     // 0x007bb524
extern void* glGetInvariantBooleanvEXT;                   // 0x007bb704
extern void* glGetInvariantIntegervEXT;                   // 0x007bb5f4
extern void* glGetInvariantFloatvEXT;                     // 0x007bb5b0
extern void* glGetLocalConstantBooleanvEXT;               // 0x007bb700
extern void* glGetLocalConstantIntegervEXT;               // 0x007bb74c
extern void* glGetLocalConstantFloatvEXT;                 // 0x007bb5c0
extern void* glActiveStencilFaceEXT;                      // 0x007bb624
extern void* wglCreateBufferRegionARB;                    // 0x007bb688
extern void* wglDeleteBufferRegionARB;                    // 0x007bb78c
extern void* wglSaveBufferRegionARB;                      // 0x007bb594
extern void* wglRestoreBufferRegionARB;                   // 0x007bb824
extern void* wglSwapIntervalEXT;                          // 0x007bb538
extern void* wglGetSwapIntervalEXT;                       // 0x007bb570

// Capability bits, one global mask per extension.
extern unsigned int g_nCap_GL_EXT_stencil_two_side;      // 0x0078e4e0
extern unsigned int g_nCap_GL_EXT_bgra;                  // 0x0078e480
extern unsigned int g_nCap_GL_ARB_multitexture;          // 0x0078e484
extern unsigned int g_nCap_GL_EXT_texture_env_combine;   // 0x0078e488
extern unsigned int g_nCap_GL_NV_register_combiners;     // 0x0078e48c
extern unsigned int g_nCap_GL_NV_register_combiners2;    // 0x0078e4dc
extern unsigned int g_nCap_GL_EXT_texture_cube_map;      // 0x0078e490
extern unsigned int g_nCap_GL_EXT_fog_coord;             // 0x0078e494
extern unsigned int g_nCap_GL_EXT_secondary_color;       // 0x0078e498
extern unsigned int g_nCap_GL_NV_vertex_program;         // 0x0078e49c
extern unsigned int g_nCap_GL_EXT_texture_filter_anisotropic; // 0x0078e4a0
extern unsigned int g_nCap_GL_NV_texture_shader;         // 0x0078e4a8
extern unsigned int g_nCap_GL_NV_vertex_array_range;     // 0x0078e4ac
extern unsigned int g_nCap_GL_NV_vertex_array_range2;    // 0x0078e4b0
extern unsigned int g_nCap_GL_NV_fence;                  // 0x0078e4b4
extern unsigned int g_nCap_GL_ARB_texture_compression;   // 0x0078e4b8
extern unsigned int g_nCap_GL_EXT_texture_compression_s3tc; // 0x0078e4bc
extern unsigned int g_nCap_GL_ATI_pn_triangles;          // 0x0078e4c0
extern unsigned int g_nCap_GL_EXT_compiled_vertex_array; // 0x0078e4c4
extern unsigned int g_nCap_WGL_ARB_render_texture;       // 0x0078e4c8
extern unsigned int g_nCap_GL_SGIS_generate_mipmap;      // 0x0078e4cc
extern unsigned int g_nCap_GL_ATI_fragment_shader;       // 0x0078e4d0
extern unsigned int g_nCap_GL_ARB_vertex_program;        // 0x0078e4d4
extern unsigned int g_nCap_GL_ARB_depth_texture;         // 0x0078e4f4
extern unsigned int g_nCap_GL_ARB_vertex_buffer_object;  // 0x0078e4d8
extern unsigned int g_nCap_WGL_NV_render_texture_rectangle; // 0x0078e4e4
extern unsigned int g_nCap_WGL_ARB_buffer_region;        // 0x0078e4e8
extern unsigned int g_nCap_WGL_EXT_swap_control;         // 0x0078e4f0

extern unsigned int g_nGLCaps;                  // 0x007bb85c
extern unsigned char g_bVBOWriteOnly;           // 0x0078e398
extern float g_fMaxAnisotropy;                  // 0x007a6858
extern int g_bVertexPrograms;                   // 0x0078e5ec
extern unsigned int g_eVertexProgramTarget;     // 0x0078d46c
extern char g_szVertexProgram0[];               // 0x0078f600
extern char g_szVertexProgram1[];               // 0x00791240
extern char g_szProgramSuffixATI[];             // 0x0078fe18
extern char g_szProgramSuffixNV[];              // 0x0078fd78
extern char* g_szProgramText0;                  // 0x0083028c
extern char* g_szProgramText1;                  // 0x00830290

typedef void (*CombinerFn)();
typedef void (*CombinerConstantFn)(float* pColor);
extern CombinerFn g_pfnEnableCombiners;         // 0x007a47e0
extern CombinerConstantFn g_pfnSetCombinerConstant0;    // 0x007a6820
extern CombinerConstantFn g_pfnSetCombinerConstant1;    // 0x007a47dc

void EnableCombinersATI();                      // 0x0047a2f0
void SetCombinerConstant0ATI(float* pColor);    // 0x0047a280
void SetCombinerConstant1ATI(float* pColor);    // 0x0047a2a0
void EnableCombiners();                         // 0x0042e600
void SetCombinerConstant0(float* pColor);       // 0x00424940
void SetCombinerConstant1(float* pColor);       // 0x00424960

bool __cdecl AurCheckExtension(char* szName, const char* szList);  // 0x0045f980
int AurHasAnisotropy();                         // 0x0045f8e0
void InitAttenuationTexture();                  // 0x004263c0
void InitializeATIFragmentShaders();            // 0x004794e0
int AurVertexBufferObjectARBAvailable();        // 0x0045fc90
void CreateRequestPoolManagerVBO();             // 0x00436400
void CreateRequestPoolManagerSys();             // 0x00436390

class RequestPoolManager {
public:
    virtual ~RequestPoolManager();
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28();
    virtual void RestoreBuffers();                                  // 0x2c
};

extern RequestPoolManager* g_pRequestPoolManager;  // 0x007a68a4

// FUNCTION: SWKOTOR 0x00436490
// NONMATCH (582, all a 4-byte shift after +0xe0a): the capability checks
// and the 201 entry points match. In both vendor branches cl hoists the
// first strlen setup (`mov eax, base; lea edx, [eax+1]`) above the branch;
// the original keeps it inside each arm, before the function-pointer
// stores. Tried inline helpers for the test and the texts, a flag with two
// ifs, a volatile pointer.
void LoadExtensions()
{
    wglGetExtensionsStringARB =
        (PFNWGLGETEXTENSIONSSTRINGARB)wglGetProcAddress("wglGetExtensionsStringARB");
    const char* szGL = glGetString(GL_EXTENSIONS);
    const char* szWGL = 0;
    if (wglGetExtensionsStringARB)
        szWGL = wglGetExtensionsStringARB(wglGetCurrentDC());
    if (AurCheckExtension("GL_EXT_stencil_two_side", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_stencil_two_side;
    if (AurCheckExtension("GL_EXT_bgra", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_bgra;
    if (AurCheckExtension("GL_ARB_multitexture", szGL))
        g_nGLCaps |= g_nCap_GL_ARB_multitexture;
    if (AurCheckExtension("GL_EXT_texture_env_combine", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_texture_env_combine;
    if (AurCheckExtension("GL_NV_register_combiners", szGL))
        g_nGLCaps |= g_nCap_GL_NV_register_combiners;
    if (AurCheckExtension("GL_NV_register_combiners2", szGL))
        g_nGLCaps |= g_nCap_GL_NV_register_combiners2;
    if (AurCheckExtension("GL_EXT_texture_cube_map", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_texture_cube_map;
    if (AurCheckExtension("GL_EXT_fog_coord", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_fog_coord;
    if (AurCheckExtension("GL_EXT_secondary_color", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_secondary_color;
    if (AurCheckExtension("GL_NV_vertex_program", szGL))
        g_nGLCaps |= g_nCap_GL_NV_vertex_program;
    if (AurCheckExtension("GL_EXT_texture_filter_anisotropic", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_texture_filter_anisotropic;
    if (AurCheckExtension("GL_NV_texture_shader", szGL))
        g_nGLCaps |= g_nCap_GL_NV_texture_shader;
    if (AurCheckExtension("GL_NV_vertex_array_range", szGL))
        g_nGLCaps |= g_nCap_GL_NV_vertex_array_range;
    if (AurCheckExtension("GL_NV_vertex_array_range2", szGL))
        g_nGLCaps |= g_nCap_GL_NV_vertex_array_range2;
    if (AurCheckExtension("GL_NV_fence", szGL))
        g_nGLCaps |= g_nCap_GL_NV_fence;
    if (AurCheckExtension("GL_ARB_texture_compression", szGL))
        g_nGLCaps |= g_nCap_GL_ARB_texture_compression;
    if (AurCheckExtension("GL_EXT_texture_compression_s3tc", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_texture_compression_s3tc;
    if (AurCheckExtension("GL_ATI_pn_triangles", szGL))
        g_nGLCaps |= g_nCap_GL_ATI_pn_triangles;
    if (AurCheckExtension("GL_EXT_compiled_vertex_array", szGL))
        g_nGLCaps |= g_nCap_GL_EXT_compiled_vertex_array;
    if (szWGL && AurCheckExtension("WGL_ARB_render_texture", szWGL))
        g_nGLCaps |= g_nCap_WGL_ARB_render_texture;
    if (AurCheckExtension("GL_SGIS_generate_mipmap", szGL))
        g_nGLCaps |= g_nCap_GL_SGIS_generate_mipmap;
    if (AurCheckExtension("GL_ATI_fragment_shader", szGL))
        g_nGLCaps |= g_nCap_GL_ATI_fragment_shader;
    if (AurCheckExtension("GL_ARB_vertex_program", szGL))
        g_nGLCaps |= g_nCap_GL_ARB_vertex_program;
    if (AurCheckExtension("GL_ARB_depth_texture", szGL))
        g_nGLCaps |= g_nCap_GL_ARB_depth_texture;
    if (AurCheckExtension("GL_ARB_vertex_buffer_object", szGL))
        g_nGLCaps |= g_nCap_GL_ARB_vertex_buffer_object;
    if (szWGL && AurCheckExtension("WGL_NV_render_texture_rectangle", szWGL))
        g_nGLCaps |= g_nCap_WGL_NV_render_texture_rectangle;
    if (szWGL && AurCheckExtension("WGL_ARB_buffer_region", szWGL))
        g_nGLCaps |= g_nCap_WGL_ARB_buffer_region;
    if (szWGL && AurCheckExtension("WGL_EXT_swap_control", szWGL))
        g_nGLCaps |= g_nCap_WGL_EXT_swap_control;
    glMultiTexCoord2fARB = wglGetProcAddress("glMultiTexCoord2fARB");
    glMultiTexCoord2iARB = wglGetProcAddress("glMultiTexCoord2iARB");
    glMultiTexCoord3fARB = wglGetProcAddress("glMultiTexCoord3fARB");
    glMultiTexCoord2fvARB = wglGetProcAddress("glMultiTexCoord2fvARB");
    glMultiTexCoord3fvARB = wglGetProcAddress("glMultiTexCoord3fvARB");
    glMultiTexCoord4fvARB = wglGetProcAddress("glMultiTexCoord4fvARB");
    glActiveTextureARB = wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB = wglGetProcAddress("glClientActiveTextureARB");
    glBindBufferARB = wglGetProcAddress("glBindBufferARB");
    glDeleteBuffersARB = wglGetProcAddress("glDeleteBuffersARB");
    glGenBuffersARB = wglGetProcAddress("glGenBuffersARB");
    glIsBufferARB = wglGetProcAddress("glIsBufferARB");
    glBufferDataARB = wglGetProcAddress("glBufferDataARB");
    glBufferSubDataARB = wglGetProcAddress("glBufferSubDataARB");
    glGetBufferSubDataARB = wglGetProcAddress("glGetBufferSubDataARB");
    glMapBufferARB = wglGetProcAddress("glMapBufferARB");
    glUnmapBufferARB = wglGetProcAddress("glUnmapBufferARB");
    glGetBufferParameterivARB = wglGetProcAddress("glGetBufferParameterivARB");
    glGetBufferPointervARB = wglGetProcAddress("glGetBufferPointervARB");
    glVertexAttrib1sARB = wglGetProcAddress("glVertexAttrib1sARB");
    glVertexAttrib1fARB = wglGetProcAddress("glVertexAttrib1fARB");
    glVertexAttrib1dARB = wglGetProcAddress("glVertexAttrib1dARB");
    glVertexAttrib2sARB = wglGetProcAddress("glVertexAttrib2sARB");
    glVertexAttrib2fARB = wglGetProcAddress("glVertexAttrib2fARB");
    glVertexAttrib2dARB = wglGetProcAddress("glVertexAttrib2dARB");
    glVertexAttrib3sARB = wglGetProcAddress("glVertexAttrib3sARB");
    glVertexAttrib3fARB = wglGetProcAddress("glVertexAttrib3fARB");
    glVertexAttrib3dARB = wglGetProcAddress("glVertexAttrib3dARB");
    glVertexAttrib4sARB = wglGetProcAddress("glVertexAttrib4sARB");
    glVertexAttrib4fARB = wglGetProcAddress("glVertexAttrib4fARB");
    glVertexAttrib4dARB = wglGetProcAddress("glVertexAttrib4dARB");
    glVertexAttrib4NubARB = wglGetProcAddress("glVertexAttrib4NubARB");
    glVertexAttrib1svARB = wglGetProcAddress("glVertexAttrib1svARB");
    glVertexAttrib1fvARB = wglGetProcAddress("glVertexAttrib1fvARB");
    glVertexAttrib1dvARB = wglGetProcAddress("glVertexAttrib1dvARB");
    glVertexAttrib2svARB = wglGetProcAddress("glVertexAttrib2svARB");
    glVertexAttrib2fvARB = wglGetProcAddress("glVertexAttrib2fvARB");
    glVertexAttrib2dvARB = wglGetProcAddress("glVertexAttrib2dvARB");
    glVertexAttrib3svARB = wglGetProcAddress("glVertexAttrib3svARB");
    glVertexAttrib3fvARB = wglGetProcAddress("glVertexAttrib3fvARB");
    glVertexAttrib3dvARB = wglGetProcAddress("glVertexAttrib3dvARB");
    glVertexAttrib4bvARB = wglGetProcAddress("glVertexAttrib4bvARB");
    glVertexAttrib4svARB = wglGetProcAddress("glVertexAttrib4svARB");
    glVertexAttrib4ivARB = wglGetProcAddress("glVertexAttrib4ivARB");
    glVertexAttrib4ubvARB = wglGetProcAddress("glVertexAttrib4ubvARB");
    glVertexAttrib4usvARB = wglGetProcAddress("glVertexAttrib4usvARB");
    glVertexAttrib4uivARB = wglGetProcAddress("glVertexAttrib4uivARB");
    glVertexAttrib4fvARB = wglGetProcAddress("glVertexAttrib4fvARB");
    glVertexAttrib4dvARB = wglGetProcAddress("glVertexAttrib4dvARB");
    glVertexAttrib4NbvARB = wglGetProcAddress("glVertexAttrib4NbvARB");
    glVertexAttrib4NsvARB = wglGetProcAddress("glVertexAttrib4NsvARB");
    glVertexAttrib4NivARB = wglGetProcAddress("glVertexAttrib4NivARB");
    glVertexAttrib4NubvARB = wglGetProcAddress("glVertexAttrib4NubvARB");
    glVertexAttrib4NusvARB = wglGetProcAddress("glVertexAttrib4NusvARB");
    glVertexAttrib4NuivARB = wglGetProcAddress("glVertexAttrib4NuivARB");
    glVertexAttribPointerARB = wglGetProcAddress("glVertexAttribPointerARB");
    glEnableVertexAttribArrayARB = wglGetProcAddress("glEnableVertexAttribArrayARB");
    glDisableVertexAttribArrayARB = wglGetProcAddress("glDisableVertexAttribArrayARB");
    glProgramStringARB = wglGetProcAddress("glProgramStringARB");
    glBindProgramARB = wglGetProcAddress("glBindProgramARB");
    glDeleteProgramsARB = wglGetProcAddress("glDeleteProgramsARB");
    glGenProgramsARB = wglGetProcAddress("glGenProgramsARB");
    glProgramEnvParameter4dARB = wglGetProcAddress("glProgramEnvParameter4dARB");
    glProgramEnvParameter4dvARB = wglGetProcAddress("glProgramEnvParameter4dvARB");
    glProgramEnvParameter4fARB = wglGetProcAddress("glProgramEnvParameter4fARB");
    glProgramEnvParameter4fvARB = wglGetProcAddress("glProgramEnvParameter4fvARB");
    glProgramLocalParameter4dARB = wglGetProcAddress("glProgramLocalParameter4dARB");
    glProgramLocalParameter4dvARB = wglGetProcAddress("glProgramLocalParameter4dvARB");
    glProgramLocalParameter4fARB = wglGetProcAddress("glProgramLocalParameter4fARB");
    glProgramLocalParameter4fvARB = wglGetProcAddress("glProgramLocalParameter4fvARB");
    glGetProgramEnvParameterdvARB = wglGetProcAddress("glGetProgramEnvParameterdvARB");
    glGetProgramEnvParameterfvARB = wglGetProcAddress("glGetProgramEnvParameterfvARB");
    glGetProgramLocalParameterdvARB = wglGetProcAddress("glGetProgramLocalParameterdvARB");
    glGetProgramLocalParameterfvARB = wglGetProcAddress("glGetProgramLocalParameterfvARB");
    glGetProgramivARB = wglGetProcAddress("glGetProgramivARB");
    glGetProgramStringARB = wglGetProcAddress("glGetProgramStringARB");
    glGetVertexAttribdvARB = wglGetProcAddress("glGetVertexAttribdvARB");
    glGetVertexAttribfvARB = wglGetProcAddress("glGetVertexAttribfvARB");
    glGetVertexAttribivARB = wglGetProcAddress("glGetVertexAttribivARB");
    glGetVertexAttribPointervARB = wglGetProcAddress("glGetVertexAttribPointervARB");
    glIsProgramARB = wglGetProcAddress("glIsProgramARB");
    glCombinerParameterfvNV = wglGetProcAddress("glCombinerParameterfvNV");
    glCombinerParameterivNV = wglGetProcAddress("glCombinerParameterivNV");
    glCombinerParameterfNV = wglGetProcAddress("glCombinerParameterfNV");
    glCombinerParameteriNV = wglGetProcAddress("glCombinerParameteriNV");
    glCombinerInputNV = wglGetProcAddress("glCombinerInputNV");
    glCombinerOutputNV = wglGetProcAddress("glCombinerOutputNV");
    glFinalCombinerInputNV = wglGetProcAddress("glFinalCombinerInputNV");
    glGetCombinerInputParameterfvNV = wglGetProcAddress("glGetCombinerInputParameterfvNV");
    glGetCombinerInputParameterivNV = wglGetProcAddress("glGetCombinerInputParameterivNV");
    glGetCombinerOutputParameterfvNV = wglGetProcAddress("glGetCombinerOutputParameterfvNV");
    glGetCombinerOutputParameterivNV = wglGetProcAddress("glGetCombinerOutputParameterivNV");
    glGetFinalCombinerInputParameterfvNV = wglGetProcAddress("glGetFinalCombinerInputParameterfvNV");
    glGetFinalCombinerInputParameterivNV = wglGetProcAddress("glGetFinalCombinerInputParameterivNV");
    glCombinerStageParameterfvNV = wglGetProcAddress("glCombinerStageParameterfvNV");
    glFogCoordPointerEXT = wglGetProcAddress("glFogCoordPointerEXT");
    glFogCoordfEXT = wglGetProcAddress("glFogCoordfEXT");
    glLockArraysEXT = wglGetProcAddress("glLockArraysEXT");
    glUnlockArraysEXT = wglGetProcAddress("glUnlockArraysEXT");
    glSecondaryColor3fvEXT = wglGetProcAddress("glSecondaryColor3fvEXT");
    glGenProgramsNV = wglGetProcAddress("glGenProgramsNV");
    glDeleteProgramsNV = wglGetProcAddress("glDeleteProgramsNV");
    glLoadProgramNV = wglGetProcAddress("glLoadProgramNV");
    glBindProgramNV = wglGetProcAddress("glBindProgramNV");
    glTrackMatrixNV = wglGetProcAddress("glTrackMatrixNV");
    glProgramParameter4fNV = wglGetProcAddress("glProgramParameter4fNV");
    glExecuteProgramNV = wglGetProcAddress("glExecuteProgramNV");
    glGetProgramParameterfvNV = wglGetProcAddress("glGetProgramParameterfvNV");
    glVertexAttribPointerNV = wglGetProcAddress("glVertexAttribPointerNV");
    glIsProgramNV = wglGetProcAddress("glIsProgramNV");
    glVertexAttrib1fvNV = wglGetProcAddress("glVertexAttrib1fvNV");
    glVertexAttrib2fvNV = wglGetProcAddress("glVertexAttrib2fvNV");
    glVertexAttrib3fvNV = wglGetProcAddress("glVertexAttrib3fvNV");
    glVertexAttrib4fvNV = wglGetProcAddress("glVertexAttrib4fvNV");
    wglAllocateMemoryNV = wglGetProcAddress("wglAllocateMemoryNV");
    wglFreeMemoryNV = wglGetProcAddress("wglFreeMemoryNV");
    glVertexArrayRangeNV = wglGetProcAddress("glVertexArrayRangeNV");
    glDeleteFencesNV = wglGetProcAddress("glDeleteFencesNV");
    glGenFencesNV = wglGetProcAddress("glGenFencesNV");
    glIsFenceNV = wglGetProcAddress("glIsFenceNV");
    glTestFenceNV = wglGetProcAddress("glTestFenceNV");
    glGetFenceivNV = wglGetProcAddress("glGetFenceivNV");
    glFinishFenceNV = wglGetProcAddress("glFinishFenceNV");
    glSetFenceNV = wglGetProcAddress("glSetFenceNV");
    glGetCompressedTexImageARB = wglGetProcAddress("glGetCompressedTexImageARB");
    glCompressedTexImage2DARB = wglGetProcAddress("glCompressedTexImage2DARB");
    glPNTrianglesiATI = wglGetProcAddress("glPNTrianglesiATI");
    glPNTrianglesfATI = wglGetProcAddress("glPNTrianglesfATI");
    wglChoosePixelFormatARB = wglGetProcAddress("wglChoosePixelFormatARB");
    wglCreatePbufferARB = wglGetProcAddress("wglCreatePbufferARB");
    wglGetPbufferDCARB = wglGetProcAddress("wglGetPbufferDCARB");
    wglReleasePbufferDCARB = wglGetProcAddress("wglReleasePbufferDCARB");
    wglDestroyPbufferARB = wglGetProcAddress("wglDestroyPbufferARB");
    wglGetPixelFormatAttribivARB = wglGetProcAddress("wglGetPixelFormatAttribivARB");
    wglQueryPbufferARB = wglGetProcAddress("wglQueryPbufferARB");
    wglMakeContextCurrentARB = wglGetProcAddress("wglMakeContextCurrentARB");
    wglBindTexImageARB = wglGetProcAddress("wglBindTexImageARB");
    wglReleaseTexImageARB = wglGetProcAddress("wglReleaseTexImageARB");
    glGenFragmentShadersATI = wglGetProcAddress("glGenFragmentShadersATI");
    glBindFragmentShaderATI = wglGetProcAddress("glBindFragmentShaderATI");
    glDeleteFragmentShaderATI = wglGetProcAddress("glDeleteFragmentShaderATI");
    glBeginFragmentShaderATI = wglGetProcAddress("glBeginFragmentShaderATI");
    glEndFragmentShaderATI = wglGetProcAddress("glEndFragmentShaderATI");
    glPassTexCoordATI = wglGetProcAddress("glPassTexCoordATI");
    glSampleMapATI = wglGetProcAddress("glSampleMapATI");
    glColorFragmentOp1ATI = wglGetProcAddress("glColorFragmentOp1ATI");
    glColorFragmentOp2ATI = wglGetProcAddress("glColorFragmentOp2ATI");
    glColorFragmentOp3ATI = wglGetProcAddress("glColorFragmentOp3ATI");
    glAlphaFragmentOp1ATI = wglGetProcAddress("glAlphaFragmentOp1ATI");
    glAlphaFragmentOp2ATI = wglGetProcAddress("glAlphaFragmentOp2ATI");
    glAlphaFragmentOp3ATI = wglGetProcAddress("glAlphaFragmentOp3ATI");
    glSetFragmentShaderConstantATI = wglGetProcAddress("glSetFragmentShaderConstantATI");
    glBeginVertexShaderEXT = wglGetProcAddress("glBeginVertexShaderEXT");
    glEndVertexShaderEXT = wglGetProcAddress("glEndVertexShaderEXT");
    glBindVertexShaderEXT = wglGetProcAddress("glBindVertexShaderEXT");
    glGenVertexShadersEXT = wglGetProcAddress("glGenVertexShadersEXT");
    glDeleteVertexShadersEXT = wglGetProcAddress("glDeleteVertexShadersEXT");
    glShaderOp1EXT = wglGetProcAddress("glShaderOp1EXT");
    glShaderOp2EXT = wglGetProcAddress("glShaderOp2EXT");
    glShaderOp3EXT = wglGetProcAddress("glShaderOp3EXT");
    glSwizzleEXT = wglGetProcAddress("glSwizzleEXT");
    glWriteMaskEXT = wglGetProcAddress("glWriteMaskEXT");
    glInsertComponentEXT = wglGetProcAddress("glInsertComponentEXT");
    glExtractComponentEXT = wglGetProcAddress("glExtractComponentEXT");
    glGenSymbolsEXT = wglGetProcAddress("glGenSymbolsEXT");
    glSetInvariantEXT = wglGetProcAddress("glSetInvariantEXT");
    glSetLocalConstantEXT = wglGetProcAddress("glSetLocalConstantEXT");
    glVariantbvEXT = wglGetProcAddress("glVariantbvEXT");
    glVariantsvEXT = wglGetProcAddress("glVariantsvEXT");
    glVariantivEXT = wglGetProcAddress("glVariantivEXT");
    glVariantfvEXT = wglGetProcAddress("glVariantfvEXT");
    glVariantdvEXT = wglGetProcAddress("glVariantdvEXT");
    glVariantubEXT = wglGetProcAddress("glVariantubEXT");
    glVariantusEXT = wglGetProcAddress("glVariantusEXT");
    glVariantuiEXT = wglGetProcAddress("glVariantuiEXT");
    glVariantPointerEXT = wglGetProcAddress("glVariantPointerEXT");
    glEnableVariantClientStateEXT = wglGetProcAddress("glEnableVariantClientStateEXT");
    glDisableVariantClientStateEXT = wglGetProcAddress("glDisableVariantClientStateEXT");
    glBindLightParameterEXT = wglGetProcAddress("glBindLightParameterEXT");
    glBindMaterialParameterEXT = wglGetProcAddress("glBindMaterialParameterEXT");
    glBindTexGenParameterEXT = wglGetProcAddress("glBindTexGenParameterEXT");
    glBindTextureUnitParameterEXT = wglGetProcAddress("glBindTextureUnitParameterEXT");
    glBindParameterEXT = wglGetProcAddress("glBindParameterEXT");
    glIsVariantEnabledEXT = wglGetProcAddress("glIsVariantEnabledEXT");
    glGetVariantBooleanvEXT = wglGetProcAddress("glGetVariantBooleanvEXT");
    glGetVariantIntegervEXT = wglGetProcAddress("glGetVariantIntegervEXT");
    glGetVariantFloatvEXT = wglGetProcAddress("glGetVariantFloatvEXT");
    glGetVariantPointervEXT = wglGetProcAddress("glGetVariantPointervEXT");
    glGetInvariantBooleanvEXT = wglGetProcAddress("glGetInvariantBooleanvEXT");
    glGetInvariantIntegervEXT = wglGetProcAddress("glGetInvariantIntegervEXT");
    glGetInvariantFloatvEXT = wglGetProcAddress("glGetInvariantFloatvEXT");
    glGetLocalConstantBooleanvEXT = wglGetProcAddress("glGetLocalConstantBooleanvEXT");
    glGetLocalConstantIntegervEXT = wglGetProcAddress("glGetLocalConstantIntegervEXT");
    glGetLocalConstantFloatvEXT = wglGetProcAddress("glGetLocalConstantFloatvEXT");
    glActiveStencilFaceEXT = wglGetProcAddress("glActiveStencilFaceEXT");
    wglCreateBufferRegionARB = wglGetProcAddress("wglCreateBufferRegionARB");
    wglDeleteBufferRegionARB = wglGetProcAddress("wglDeleteBufferRegionARB");
    wglSaveBufferRegionARB = wglGetProcAddress("wglSaveBufferRegionARB");
    wglRestoreBufferRegionARB = wglGetProcAddress("wglRestoreBufferRegionARB");
    wglSwapIntervalEXT = wglGetProcAddress("wglSwapIntervalEXT");
    wglGetSwapIntervalEXT = wglGetProcAddress("wglGetSwapIntervalEXT");
    if (g_nGLCaps & g_nCap_GL_NV_vertex_program) {
        const char* szRenderer = glGetString(GL_RENDERER);
        if (strstr(szRenderer, "GeForce3"))
            g_bVBOWriteOnly = 0;
    }
    if (AurHasAnisotropy())
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_fMaxAnisotropy);
    InitAttenuationTexture();
    if ((g_nGLCaps & g_nCap_GL_ATI_fragment_shader) == g_nCap_GL_ATI_fragment_shader) {
        g_pfnEnableCombiners = EnableCombinersATI;
        g_pfnSetCombinerConstant0 = SetCombinerConstant0ATI;
        g_pfnSetCombinerConstant1 = SetCombinerConstant1ATI;
        g_szProgramText0 =
            new char[strlen(g_szVertexProgram0) + strlen(g_szProgramSuffixATI) + 1];
        g_szProgramText1 =
            new char[strlen(g_szVertexProgram1) + strlen(g_szProgramSuffixATI) + 1];
        strcpy(g_szProgramText0, g_szVertexProgram0);
        strcat(g_szProgramText0, g_szProgramSuffixATI);
        strcpy(g_szProgramText1, g_szVertexProgram1);
        strcat(g_szProgramText1, g_szProgramSuffixATI);
        InitializeATIFragmentShaders();
    } else {
        g_pfnEnableCombiners = EnableCombiners;
        g_pfnSetCombinerConstant0 = SetCombinerConstant0;
        g_pfnSetCombinerConstant1 = SetCombinerConstant1;
        g_szProgramText0 =
            new char[strlen(g_szVertexProgram0) + strlen(g_szProgramSuffixNV) + 1];
        g_szProgramText1 =
            new char[strlen(g_szVertexProgram1) + strlen(g_szProgramSuffixNV) + 1];
        strcpy(g_szProgramText0, g_szVertexProgram0);
        strcat(g_szProgramText0, g_szProgramSuffixNV);
        strcpy(g_szProgramText1, g_szVertexProgram1);
        strcat(g_szProgramText1, g_szProgramSuffixNV);
    }
    if (g_bVertexPrograms)
        g_eVertexProgramTarget = GL_VERTEX_PROGRAM_ARB;
    if (AurVertexBufferObjectARBAvailable())
        CreateRequestPoolManagerVBO();
    else
        CreateRequestPoolManagerSys();
    g_pRequestPoolManager->RestoreBuffers();
}
