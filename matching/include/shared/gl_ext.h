// gl_ext.h -- the OpenGL and WGL extension entry points (CODE_RULES.md, R2)
//
// One declaration for each pointer the renderer loads with wglGetProcAddress
// (g/ext_loader.cpp), typed as glext.h / wglext.h type them; device contexts
// and pbuffers are void*, as the units hold them.
#ifndef SHARED_GL_EXT_H
#define SHARED_GL_EXT_H

typedef void* (__stdcall* PFNWGLCREATEPBUFFERARBPROC)(void* hdc, int iFormat, int iWidth, int iHeight, const int* piAttribs);
typedef void (__stdcall* PFNGLBUFFERDATAARBPROC)(unsigned int target, int size, const void* data, unsigned int usage);
typedef void (__stdcall* PFNGLDELETEFRAGMENTSHADERATIPROC)(unsigned int id);
typedef void (__stdcall* PFNGLENDFRAGMENTSHADERATIPROC)();
typedef int (__stdcall* PFNWGLRELEASEPBUFFERDCARBPROC)(void* hPbuffer, void* hDC);
typedef int (__stdcall* PFNWGLDESTROYPBUFFERARBPROC)(void* hPbuffer);
typedef void (__stdcall* PFNGLGETBUFFERPARAMETERIVARBPROC)(unsigned int target, unsigned int pname, int* params);
typedef void (__stdcall* PFNGLGENBUFFERSARBPROC)(int n, unsigned int* buffers);
typedef void (__stdcall* PFNGLSETFRAGMENTSHADERCONSTANTATIPROC)(unsigned int dst, const float* value);
typedef void (__stdcall* PFNGLALPHAFRAGMENTOP2ATIPROC)(unsigned int op, unsigned int dst, unsigned int dstMod, unsigned int arg1, unsigned int arg1Rep, unsigned int arg1Mod, unsigned int arg2, unsigned int arg2Rep, unsigned int arg2Mod);
typedef void (__stdcall* PFNGLSAMPLEMAPATIPROC)(unsigned int dst, unsigned int interp, unsigned int swizzle);
typedef void (__stdcall* PFNGLACTIVESTENCILFACEEXTPROC)(unsigned int face);
typedef void* (__stdcall* PFNGLMAPBUFFERARBPROC)(unsigned int target, unsigned int access);
typedef void (__stdcall* PFNGLALPHAFRAGMENTOP3ATIPROC)(unsigned int op, unsigned int dst, unsigned int dstMod, unsigned int arg1, unsigned int arg1Rep, unsigned int arg1Mod, unsigned int arg2, unsigned int arg2Rep, unsigned int arg2Mod, unsigned int arg3, unsigned int arg3Rep, unsigned int arg3Mod);
typedef void (__stdcall* PFNGLCLIENTACTIVETEXTUREARBPROC)(unsigned int tex);
typedef void* (__stdcall* PFNWGLGETPBUFFERDCARBPROC)(void* hPbuffer);
typedef void (__stdcall* PFNGLBINDFRAGMENTSHADERATIPROC)(unsigned int id);
typedef int (__stdcall* PFNWGLRELEASETEXIMAGEARBPROC)(void* hPbuffer, int iBuffer);
typedef void (__stdcall* PFNGLCOLORFRAGMENTOP1ATIPROC)(unsigned int op, unsigned int dst, unsigned int dstMask, unsigned int dstMod, unsigned int arg1, unsigned int arg1Rep, unsigned int arg1Mod);
typedef unsigned char (__stdcall* PFNGLUNMAPBUFFERARBPROC)(unsigned int target);
typedef void (__stdcall* PFNGLMULTITEXCOORD2FARBPROC)(unsigned int tex, float s, float t);
typedef void (__stdcall* PFNGLCOMBINERSTAGEPARAMETERFVNVPROC)(unsigned int stage, unsigned int pname, const float* params);
typedef void (__stdcall* PFNGLALPHAFRAGMENTOP1ATIPROC)(unsigned int op, unsigned int dst, unsigned int dstMod, unsigned int arg1, unsigned int arg1Rep, unsigned int arg1Mod);
typedef void (__stdcall* PFNGLDELETEPROGRAMSARBPROC)(int n, const unsigned int* programs);
typedef int (__stdcall* PFNWGLCHOOSEPIXELFORMATARBPROC)(void* hdc, const int* piAttribs, const float* pfAttribs, unsigned int nMax, int* piFormats, unsigned int* pnFormats);
typedef unsigned int (__stdcall* PFNGLGENFRAGMENTSHADERSATIPROC)(unsigned int range);
typedef void (__stdcall* PFNGLBUFFERSUBDATAARBPROC)(unsigned int target, int offset, int size, const void* data);
typedef void (__stdcall* PFNGLCOMBINERPARAMETERFVNVPROC)(unsigned int pname, const float* params);
typedef void (__stdcall* PFNGLBINDBUFFERARBPROC)(unsigned int target, unsigned int buffer);
typedef int (__stdcall* PFNWGLBINDTEXIMAGEARBPROC)(void* hPbuffer, int iBuffer);
typedef void (__stdcall* PFNGLDELETEBUFFERSARBPROC)(int n, const unsigned int* buffers);
typedef const char* (__stdcall* PFNWGLGETEXTENSIONSSTRINGARBPROC)(void* hDC);
typedef void (__stdcall* PFNGLBEGINFRAGMENTSHADERATIPROC)();
typedef void (__stdcall* PFNGLCOLORFRAGMENTOP3ATIPROC)(unsigned int op, unsigned int dst, unsigned int dstMask, unsigned int dstMod, unsigned int arg1, unsigned int arg1Rep, unsigned int arg1Mod, unsigned int arg2, unsigned int arg2Rep, unsigned int arg2Mod, unsigned int arg3, unsigned int arg3Rep, unsigned int arg3Mod);
typedef void (__stdcall* PFNGLPASSTEXCOORDATIPROC)(unsigned int, unsigned int, unsigned int);
typedef void (__stdcall* PFNGLACTIVETEXTUREARBPROC)(unsigned int tex);
typedef int (__stdcall* PFNWGLMAKECONTEXTCURRENTARBPROC)(void* hDrawDC, void* hReadDC, void* hglrc);
typedef void (__stdcall* PFNGLCOLORFRAGMENTOP2ATIPROC)(unsigned int op, unsigned int dst, unsigned int dstMask, unsigned int dstMod, unsigned int arg1, unsigned int arg1Rep, unsigned int arg1Mod, unsigned int arg2, unsigned int arg2Rep, unsigned int arg2Mod);

extern PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;  // 0x007BB530
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;  // 0x007BB544
extern PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI;  // 0x007BB550
extern PFNGLENDFRAGMENTSHADERATIPROC glEndFragmentShaderATI;  // 0x007BB554
extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;  // 0x007BB578
extern PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;  // 0x007BB598
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;  // 0x007BB5A4
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;  // 0x007BB5B4
extern PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI;  // 0x007BB5D8
extern PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI;  // 0x007BB5E8
extern PFNGLSAMPLEMAPATIPROC glSampleMapATI;  // 0x007BB620
extern PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;  // 0x007BB624
extern PFNGLMAPBUFFERARBPROC glMapBufferARB;  // 0x007BB634
extern PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI;  // 0x007BB680
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;  // 0x007BB6B0
extern PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;  // 0x007BB6B8
extern PFNGLBINDFRAGMENTSHADERATIPROC glBindFragmentShaderATI;  // 0x007BB6CC
extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;  // 0x007BB6D0
extern PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI;  // 0x007BB6D4
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;  // 0x007BB6E0
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;  // 0x007BB6E8
extern PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV;  // 0x007BB6F8
extern PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI;  // 0x007BB710
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;  // 0x007BB724
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;  // 0x007BB730
extern PFNGLGENFRAGMENTSHADERSATIPROC glGenFragmentShadersATI;  // 0x007BB754
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;  // 0x007BB75C
extern PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;  // 0x007BB7A8
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;  // 0x007BB7B4
extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;  // 0x007BB7B8
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;  // 0x007BB7BC
extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;  // 0x007BB7EC
extern PFNGLBEGINFRAGMENTSHADERATIPROC glBeginFragmentShaderATI;  // 0x007BB7F4
extern PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI;  // 0x007BB7FC
extern PFNGLPASSTEXCOORDATIPROC glPassTexCoordATI;  // 0x007BB804
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;  // 0x007BB80C
extern PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;  // 0x007BB818
extern PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI;  // 0x007BB840

#endif
