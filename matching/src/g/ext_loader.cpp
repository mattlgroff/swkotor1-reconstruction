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
#include "shared/gl_ext.h"

// GL / WGL extension entry points (function-pointer globals).
#include "shared/globals.h"

// Capability bits, one global mask per extension.


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


// FUNCTION: SWKOTOR 0x00436490
// NONMATCH (582, all a 4-byte shift after +0xe0a): the capability checks
// and the 201 entry points match. In both vendor branches cl hoists the
// first strlen setup (`mov eax, base; lea edx, [eax+1]`) above the branch;
// the original keeps it inside each arm, before the function-pointer
// stores. Tried inline helpers for the test and the texts, a flag with two
// ifs, a volatile pointer.
void LoadExtensions()
{
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
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
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
    glMultiTexCoord2iARB = wglGetProcAddress("glMultiTexCoord2iARB");
    glMultiTexCoord3fARB = wglGetProcAddress("glMultiTexCoord3fARB");
    glMultiTexCoord2fvARB = wglGetProcAddress("glMultiTexCoord2fvARB");
    glMultiTexCoord3fvARB = wglGetProcAddress("glMultiTexCoord3fvARB");
    glMultiTexCoord4fvARB = wglGetProcAddress("glMultiTexCoord4fvARB");
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
    glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
    glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
    glIsBufferARB = wglGetProcAddress("glIsBufferARB");
    glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
    glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
    glGetBufferSubDataARB = wglGetProcAddress("glGetBufferSubDataARB");
    glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
    glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
    glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
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
    glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC)wglGetProcAddress("glDeleteProgramsARB");
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
    glCombinerParameterfvNV = (PFNGLCOMBINERPARAMETERFVNVPROC)wglGetProcAddress("glCombinerParameterfvNV");
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
    glCombinerStageParameterfvNV = (PFNGLCOMBINERSTAGEPARAMETERFVNVPROC)wglGetProcAddress("glCombinerStageParameterfvNV");
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
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
    wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
    wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
    wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
    wglGetPixelFormatAttribivARB = wglGetProcAddress("wglGetPixelFormatAttribivARB");
    wglQueryPbufferARB = wglGetProcAddress("wglQueryPbufferARB");
    wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC)wglGetProcAddress("wglMakeContextCurrentARB");
    wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
    wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
    glGenFragmentShadersATI = (PFNGLGENFRAGMENTSHADERSATIPROC)wglGetProcAddress("glGenFragmentShadersATI");
    glBindFragmentShaderATI = (PFNGLBINDFRAGMENTSHADERATIPROC)wglGetProcAddress("glBindFragmentShaderATI");
    glDeleteFragmentShaderATI = (PFNGLDELETEFRAGMENTSHADERATIPROC)wglGetProcAddress("glDeleteFragmentShaderATI");
    glBeginFragmentShaderATI = (PFNGLBEGINFRAGMENTSHADERATIPROC)wglGetProcAddress("glBeginFragmentShaderATI");
    glEndFragmentShaderATI = (PFNGLENDFRAGMENTSHADERATIPROC)wglGetProcAddress("glEndFragmentShaderATI");
    glPassTexCoordATI = (PFNGLPASSTEXCOORDATIPROC)wglGetProcAddress("glPassTexCoordATI");
    glSampleMapATI = (PFNGLSAMPLEMAPATIPROC)wglGetProcAddress("glSampleMapATI");
    glColorFragmentOp1ATI = (PFNGLCOLORFRAGMENTOP1ATIPROC)wglGetProcAddress("glColorFragmentOp1ATI");
    glColorFragmentOp2ATI = (PFNGLCOLORFRAGMENTOP2ATIPROC)wglGetProcAddress("glColorFragmentOp2ATI");
    glColorFragmentOp3ATI = (PFNGLCOLORFRAGMENTOP3ATIPROC)wglGetProcAddress("glColorFragmentOp3ATI");
    glAlphaFragmentOp1ATI = (PFNGLALPHAFRAGMENTOP1ATIPROC)wglGetProcAddress("glAlphaFragmentOp1ATI");
    glAlphaFragmentOp2ATI = (PFNGLALPHAFRAGMENTOP2ATIPROC)wglGetProcAddress("glAlphaFragmentOp2ATI");
    glAlphaFragmentOp3ATI = (PFNGLALPHAFRAGMENTOP3ATIPROC)wglGetProcAddress("glAlphaFragmentOp3ATI");
    glSetFragmentShaderConstantATI = (PFNGLSETFRAGMENTSHADERCONSTANTATIPROC)wglGetProcAddress("glSetFragmentShaderConstantATI");
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
    glActiveStencilFaceEXT = (PFNGLACTIVESTENCILFACEEXTPROC)wglGetProcAddress("glActiveStencilFaceEXT");
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
