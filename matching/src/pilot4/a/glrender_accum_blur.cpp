// Pilot 4 arm A: accumulation (speed) blur pass through NV register combiners.
#include "pilot4/a/p4a_glblur.h"

// FUNCTION: SWKOTOR 0x00433DA0
void RenderAccumulationBlur(unsigned int nTexture)
{
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
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FrameBufferModificationPixelShaders(0x128, 0, 0, 0, 0);
    glCombinerStageParameterfvNV(GL_COMBINER0_NV, GL_CONSTANT_COLOR0_NV, AurMakeColor(vBlend, 1.0f));
    Vector vInverse;
    vInverse.x = 1.0f - vBlend.x;
    vInverse.y = 1.0f - vBlend.y;
    vInverse.z = 1.0f - vBlend.z;
    glCombinerStageParameterfvNV(GL_COMBINER1_NV, GL_CONSTANT_COLOR1_NV, AurMakeColor(vInverse, 0.0f));

    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
                             (i == 0 || i == 1) ? 0.0f : 1.0f,
                             (i == 0 || i == 3) ? 1.0f : 0.0f);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,
                             (i == 0 || i == 1) ? 0.0f : (float)g_nWindowWidth,
                             (i == 0 || i == 3) ? (float)g_nWindowHeight : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nWindowWidth,
                   (i == 0 || i == 3) ? (float)g_nWindowHeight : 0.0f,
                   0.0f);
    }
    glEnd();

    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(g_hBloomPbuffer, WGL_FRONT_LEFT_ARB);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
    glActiveTextureARB(GL_TEXTURE0_ARB);
}
