// SetupATIFrameBuffer: build the eight ATI_fragment_shader programs the
// framebuffer effects bind. Names of the shaders not used elsewhere in
// this folder are not recorded; they follow what each program does.
// Behavioural reference: src/units/0533/c3261110ed-ati_setup.cpp.
extern "C" {
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
}

#define GL_TEXTURE0_ARB 0x84c0
#define GL_TEXTURE1_ARB 0x84c1
#define GL_TEXTURE2_ARB 0x84c2
#define GL_TEXTURE3_ARB 0x84c3
#define GL_PRIMARY_COLOR_ARB 0x8577
#define GL_FRAGMENT_SHADER_ATI 0x8920
#define GL_REG_0_ATI 0x8921
#define GL_REG_1_ATI 0x8922
#define GL_REG_2_ATI 0x8923
#define GL_REG_3_ATI 0x8924
#define GL_REG_4_ATI 0x8925
#define GL_CON_0_ATI 0x8941
#define GL_CON_1_ATI 0x8942
#define GL_CON_2_ATI 0x8943
#define GL_CON_3_ATI 0x8944
#define GL_CON_4_ATI 0x8945
#define GL_CON_5_ATI 0x8946
#define GL_MOV_ATI 0x8961
#define GL_ADD_ATI 0x8963
#define GL_MUL_ATI 0x8964
#define GL_DOT3_ATI 0x8966
#define GL_MAD_ATI 0x8968
#define GL_SWIZZLE_STR_ATI 0x8976
#define GL_ALPHA 0x1906

typedef void(__stdcall* PFNGLSAMPLEMAPATI)(unsigned int dst, unsigned int interp,
                                           unsigned int swizzle);
typedef PFNGLSAMPLEMAPATI PFNGLPASSTEXCOORDATI;

#include "shared/gl_ext.h"

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x00427490
void SetupATIFrameBuffer()
{
    glEnable(GL_FRAGMENT_SHADER_ATI);
    g_nColorRampShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nColorRampShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_1_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_2_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_1_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_3_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_2_ATI, 0, 0);
    glPassTexCoordATI(GL_REG_0_ATI, GL_REG_0_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_1_ATI, GL_REG_1_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_2_ATI, GL_REG_2_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_3_ATI, GL_REG_3_ATI, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_1_ATI, 0, 0, GL_REG_2_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_REG_3_ATI, 0, 0);
    glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, GL_PRIMARY_COLOR_ARB, 0, 0);
    glEndFragmentShaderATI();

    g_nDistortionShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nDistortionShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glPassTexCoordATI(GL_REG_1_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
    glPassTexCoordATI(GL_REG_2_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_4_ATI, 1, 0, GL_REG_0_ATI, 0, 9, GL_REG_1_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_4_ATI, 2, 0, GL_REG_0_ATI, 0, 9, GL_REG_2_ATI, 0, 0);
    glPassTexCoordATI(GL_REG_0_ATI, GL_REG_0_ATI, GL_SWIZZLE_STR_ATI);
    glPassTexCoordATI(GL_REG_1_ATI, GL_REG_1_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_2_ATI, GL_REG_4_ATI, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, 0, GL_REG_2_ATI, 0, 0);
    glEndFragmentShaderATI();

    g_nFilmNoiseShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nFilmNoiseShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_REG_1_ATI, 0, 0);
    glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, 0, 0, 2);
    glEndFragmentShaderATI();

    g_nAccumBlurShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nAccumBlurShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_1_ATI, 0, 0, GL_CON_1_ATI, 0, 0, GL_REG_2_ATI, 0, 0);
    glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, GL_CON_0_ATI, 0, 0);
    glEndFragmentShaderATI();

    g_nColorMatrixShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nColorMatrixShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_1_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_2_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_1_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_1_ATI, 0, 0, GL_REG_1_ATI, 0, 0, GL_CON_3_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_1_ATI, 0, 0, GL_REG_2_ATI, 0, 0, GL_CON_4_ATI, 0, 0, GL_REG_1_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_2_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_2_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_2_ATI, 0, 0, GL_CON_5_ATI, 0, 0, GL_REG_1_ATI, 0, 0);
    glEndFragmentShaderATI();

    g_nColorRampAlphaShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nColorRampAlphaShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_1_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_2_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_1_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_3_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_2_ATI, 0, 0);
    glPassTexCoordATI(GL_REG_0_ATI, GL_REG_0_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_1_ATI, GL_REG_1_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_2_ATI, GL_REG_2_ATI, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_3_ATI, GL_REG_3_ATI, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_1_ATI, 0, 0, GL_REG_2_ATI, 0, 0);
    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_REG_3_ATI, 0, 0);
    glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, 0, 0, 2);
    glEndFragmentShaderATI();

    g_nCopyShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nCopyShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, 0, GL_REG_0_ATI, 0, 0);
    glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, 0, 0, 2);
    glEndFragmentShaderATI();

    g_nConvolutionShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nConvolutionShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_3_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_1_ATI, 0, 0, GL_REG_1_ATI, 0, 0, GL_CON_1_ATI, 0, 0, GL_REG_0_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_2_ATI, 0, 0, GL_REG_2_ATI, 0, 0, GL_CON_2_ATI, 0, 0, GL_REG_1_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_3_ATI, 0, 0, GL_CON_3_ATI, 0, 0, GL_REG_2_ATI, 0, 0);
    glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, 0, GL_PRIMARY_COLOR_ARB, GL_ALPHA, 0);
    glEndFragmentShaderATI();
    glDisable(GL_FRAGMENT_SHADER_ATI);
}
