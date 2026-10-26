// Framebuffer overlay passes (film noise, overbrighten, saturation),
// 0x00432780-0x00434cc0.
#include "g/fb_effects.h"

// FUNCTION: SWKOTOR 0x004340f0
void RenderOverlayOverbrighten(int nTexture)
{
    glBlendFunc(1, 0);
    FrameBufferModificationPixelShaders(0x512, nTexture, g_nBloomRamp0, g_nBloomRamp1, g_nBloomRamp2);
    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                     (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glVertex4f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f, 1.0f);
    }
    glEnd();
    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
}

// FUNCTION: SWKOTOR 0x00434240
void RenderOverlayOverbrightenATI(int nTexture)
{
    glBlendFunc(1, 0);
    FrameBufferModificationPixelShadersATI(0x512, nTexture, g_nBloomRamp0, g_nBloomRamp1, g_nBloomRamp2);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : fU, (i == 0 || i == 3) ? fV : 0.0f);
        glVertex4f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f, 1.0f);
    }
    glEnd();
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

// FUNCTION: SWKOTOR 0x00432780
void RenderOverlayFilmNoiseATI(unsigned int nTexture, unsigned char bStatic)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glBlendFunc(1, 0);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    g_nCurrentStage = 1;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    g_pFilmNoiseTexture->Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glEnable(GL_FRAGMENT_SHADER_ATI);
    glBindFragmentShaderATI(g_nFilmNoiseShaderATI);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (i == 0 || i == 1) ? 0.0f : fU,
                             (i == 0 || i == 3) ? fV : 0.0f);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (i == 0 || i == 1) ? (bStatic ? 0.0f : 7.5f) : 0.0f,
                             (i == 0 || i == 3) ? 0.0f : 7.5f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// FUNCTION: SWKOTOR 0x00432ad0
void RenderOverlayFilmNoise(unsigned int nTexture, unsigned char bStatic)
{
    glBlendFunc(1, 0);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, nTexture);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    g_nCurrentStage = 1;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    g_pFilmNoiseTexture->Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    FrameBufferModificationPixelShaders(0x32, 0, 0, 0, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                             (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (i == 0 || i == 1) ? (bStatic ? 0.0f : 7.5f) : 0.0f,
                             (i == 0 || i == 3) ? 0.0f : 7.5f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
    glFlush();
}

// FUNCTION: SWKOTOR 0x00432d70
void RenderForceDistortionATI(unsigned int nTexture)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, g_nScreenWidth, g_nScreenHeight);
    glEnable(GL_VERTEX_PROGRAM_ARB);
    g_pfnProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0x0f, 0.0f, 1.0f, 0.0f, 0.0f);
    g_pfnBindProgramARB(GL_VERTEX_PROGRAM_ARB, g_nDistortionProgram);
    g_pDistortionTexture->Activate();
    glEnable(GL_BLEND);
    glBlendFunc(1, 0);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    g_pDistortionTexture->Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_FRAGMENT_SHADER_ATI);
    glBindFragmentShaderATI(g_nDistortionShaderATI);
    glEnable(g_nGLStencilTest);
    glStencilFunc(GL_LESS, 0, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    float fV1 = fV * 0.1f;
    float fU1 = fU * 0.1f;
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (i == 0 || i == 1) ? 0.0f : 1.0f,
                             (i == 0 || i == 3) ? 0.0f : 1.0f);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, fU1, fV1);
        glActiveTextureARB(GL_TEXTURE2_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (i == 0 || i == 1) ? 0.0f : fU,
                             (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glDisable(g_nGLStencilTest);
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    glDisable(GL_VERTEX_PROGRAM_ARB);
    g_pfnBindProgramARB(GL_VERTEX_PROGRAM_ARB, 0);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glFlush();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// FUNCTION: SWKOTOR 0x004331d0
void RenderForceDistortion(unsigned int nTexture)
{
    glEnable(GL_VERTEX_PROGRAM_ARB);
    g_pfnProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0x0f, 0.0f, 1.0f, 0.0f, 0.0f);
    g_pfnBindProgramARB(GL_VERTEX_PROGRAM_ARB, g_nDistortionProgram);
    g_pDistortionTexture->Activate();
    glBlendFunc(1, 0);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    g_pDistortionTexture->Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    g_nCurrentStage = 1;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    g_nCurrentStage = 2;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, nTexture);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FrameBufferModificationPixelShaders(0x64, 0, 0, 0, 0);
    glEnable(g_nGLStencilTest);
    glStencilFunc(GL_LESS, 0, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (i == 0 || i == 1) ? 0.0f : 1.0f,
                             (i == 0 || i == 3) ? 0.0f : 1.0f);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, g_nScreenWidth * 0.1f, g_nScreenHeight * 0.1f);
        glActiveTextureARB(GL_TEXTURE2_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                             (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glDisable(g_nGLStencilTest);
    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
    glDisable(GL_VERTEX_PROGRAM_ARB);
    g_pfnBindProgramARB(GL_VERTEX_PROGRAM_ARB, 0);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glFlush();
}

// FUNCTION: SWKOTOR 0x00433970
void RenderAccumulationBlurATI(unsigned int nTexture)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, g_nScreenWidth, g_nScreenHeight);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(1, 0);
    Vector vBlend(g_fSpeedBlurRatio, g_fSpeedBlurRatio, g_fSpeedBlurRatio);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nBloomTexture);
    wglBindTexImageARB(g_hBloomPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_FRAGMENT_SHADER_ATI);
    glSetFragmentShaderConstantATI(GL_CON_0_ATI, AurMakeColor(vBlend, 1.0f));
    Vector vInverse;
    vInverse.x = 1.0f - vBlend.x;
    vInverse.y = 1.0f - vBlend.y;
    vInverse.z = 1.0f - vBlend.z;
    glSetFragmentShaderConstantATI(GL_CON_1_ATI, AurMakeColor(vInverse, 0.0f));
    glEnable(GL_FRAGMENT_SHADER_ATI);
    glBindFragmentShaderATI(g_nAccumBlurShaderATI);

    glDisable(GL_LIGHTING);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
                             (i == 0 || i == 1) ? 0.0f : 1.0f,
                             (i == 0 || i == 3) ? 1.0f : 0.0f);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (i == 0 || i == 1) ? 0.0f : fU,
                             (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f,
                   0.0f);
    }
    glEnd();

    glActiveTextureARB(GL_TEXTURE0_ARB);
    wglReleaseTexImageARB(g_hBloomPbuffer, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glDisable(GL_FRAGMENT_SHADER_ATI);
    glFlush();
}

// FUNCTION: SWKOTOR 0x004343d0
void RenderSaturationAdjustmentATI(unsigned int nTexture)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(1, 0);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FrameBufferModificationPixelShadersATI(0x16, 0, 0, 0, 0);

    float fInv = 1.0f - g_fSaturation;
    float fR = 0.3086f * fInv;
    float fG = fInv * 0.6094f;
    float fB = fInv * 0.0820f;
    Vector v0, v1, v2;
    v0.x = g_fSaturation + fR;
    v0.y = fG;
    v0.z = fB;
    v1.x = fR;
    v1.y = fG + g_fSaturation;
    v1.z = fB;
    v2.x = fR;
    v2.y = fG;
    v2.z = fB + g_fSaturation;
    v0 = v0 * g_vSaturationTint.x;
    v1 = v1 * g_vSaturationTint.y;
    v2 = v2 * g_vSaturationTint.z;
    if (v0.x > 1.0f) v0.x = 1.0f;
    if (v0.y > 1.0f) v0.y = 1.0f;
    if (v0.z > 1.0f) v0.z = 1.0f;
    if (v1.x > 1.0f) v1.x = 1.0f;
    if (v1.y > 1.0f) v1.y = 1.0f;
    if (v1.z > 1.0f) v1.z = 1.0f;
    if (v2.x > 1.0f) v2.x = 1.0f;
    if (v2.y > 1.0f) v2.y = 1.0f;
    if (v2.x > 1.0f) v2.z = 1.0f;
    glSetFragmentShaderConstantATI(GL_CON_0_ATI, AurMakeColor(v0, 0.0f));
    glSetFragmentShaderConstantATI(GL_CON_1_ATI, AurMakeColor(v1, 0.0f));
    glSetFragmentShaderConstantATI(GL_CON_2_ATI, AurMakeColor(v2, 0.0f));

    glBegin(GL_QUADS);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    for (unsigned int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : fU, (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
