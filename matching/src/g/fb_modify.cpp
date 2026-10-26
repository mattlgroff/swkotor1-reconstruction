// Framebuffer modification drivers, 0x00435bd0-0x00437a60.
#include "g/fb_effects.h"

// FUNCTION: SWKOTOR 0x00436070
void FrameBufferModificationsATI2(unsigned char nMask)
{
    if (!g_bSaturationAvailable || !g_bSaturation || !(nMask & 0x08))
        return;
    glDisable(g_eGLDepthTest);
    glDisable(g_nCapLighting);
    glDepthMask(0);
    glEnable(g_nCapBlend);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (g_bSaturationAvailable && g_bSaturation) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        RenderSaturationAdjustmentATI(g_nRootTexture);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(g_eGLDepthTest);
    glEnable(g_nCapAlphaTest);
    glEnable(g_nCapLighting);
    glDepthMask(1);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// FUNCTION: SWKOTOR 0x004361f0
void FrameBufferModifications2(unsigned char nMask)
{
    if (!g_bFrameBufferEffects || !g_bFrameBufferActive || !g_bFrameBufferEnabled)
        return;
    if (!AurGLCaps820()) {
        if (AurATIFrameBufferModificationsAvailable())
            FrameBufferModificationsATI2(nMask);
        return;
    }
    if (!g_bSaturationAvailable || !g_bSaturation || !(nMask & 0x08))
        return;
    glDisable(g_eGLDepthTest);
    glDisable(g_nCapLighting);
    glDepthMask(0);
    glEnable(g_nCapBlend);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (g_bSaturationAvailable && g_bSaturation) {
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        RenderSaturationAdjustment(g_nRootTexture);
    }
    _glBlendFunc(0, 1);
    glEnable(g_eGLDepthTest);
    glEnable(g_nCapAlphaTest);
    glEnable(g_nCapLighting);
    glDepthMask(1);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// FUNCTION: SWKOTOR 0x00435bd0
void FrameBufferModificationsATI(unsigned char nMask)
{
    glDisable(g_nCapAlphaTest);
    glDisable(g_eGLDepthTest);
    glDisable(g_nCapLighting);
    glDepthMask(0);
    glDisable(g_nCapBlend);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (g_bSpeedBlurAvailable && g_bSpeedBlur && (nMask & 0x10) && !g_bBloomRendered &&
        g_bAccumFrameRendered == 1) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();
        RenderAccumulationBlurATI(g_nRootTexture);
    }
    if (g_bSpeedBlurAvailable && g_bSpeedBlur && (nMask & 0x10)) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        g_nCurrentStage = 0;
        if (HasMultiTexture())
            glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
        RenderFrameToTextureATI(g_nRootTexture);
        g_bAccumFrameRendered = 1;
    } else {
        g_bAccumFrameRendered = 0;
    }
    if (g_bOverbrighten && (nMask & 0x01)) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();
        RenderOverlayOverbrightenATI(g_nRootTexture);
    }
    if (g_bConvolutionAvailable && (nMask & 0x02)) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();
        RenderOverlayConvolutionATI(g_nRootTexture);
    }
    if (g_bDistortionAvailable && (nMask & 0x20)) {
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        glFlush();
        RenderForceDistortionATI(g_nRootTexture);
    }
    if (g_bScanNoiseAvailable && g_bScanNoise && (nMask & 0x04) && g_pFilmNoiseTexture &&
        !g_pFilmNoiseTexture->IsNull()) {
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glFlush();
        RenderOverlayFilmNoiseATI(g_nRootTexture, 1);
    }
    if (g_bFilmNoiseAvailable && g_bFilmNoise && (nMask & 0x04) && g_pFilmNoiseTexture &&
        !g_pFilmNoiseTexture->IsNull()) {
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
        glFlush();
        RenderOverlayFilmNoiseATI(g_nRootTexture, 0);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(g_nCapAlphaTest);
    glEnable(g_eGLDepthTest);
    glEnable(g_nCapLighting);
    glDepthMask(1);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    g_bBloomRendered = 0;
}

// FUNCTION: SWKOTOR 0x00437520
void FrameBufferModifications(unsigned char nMask)
{
    glDisable(g_eGLFog);
    if (!g_bFrameBufferEffects || !g_bFrameBufferActive || !g_bFrameBufferEnabled)
        return;
    if (!AurGLCaps820()) {
        if (AurATIFrameBufferModificationsAvailable())
            FrameBufferModificationsATI(nMask);
        return;
    }
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(g_nCapAlphaTest);
    glDisable(g_eGLDepthTest);
    glDisable(g_nCapLighting);
    glDepthMask(0);
    glDisable(g_nCapBlend);
    if (g_bSpeedBlurAvailable && g_bSpeedBlur && (nMask & 0x10) && !g_bBloomRendered &&
        g_bFrameRendered == 1) {
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        glFlush();
        RenderAccumulationBlur(g_nRootTexture);
    }
    if (g_bSpeedBlurAvailable && g_bSpeedBlur && (nMask & 0x10)) {
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        glFlush();
        GLRender::ActivateTextureStage(0);
        RenderFrameToTexture(g_nRootTexture);
        g_bFrameRendered = 1;
    } else {
        g_bFrameRendered = 0;
    }
    if (g_bOverbrighten && (nMask & 0x01)) {
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
        glFlush();
        RenderOverlayOverbrighten(g_nRootTexture);
    }
    if (g_bConvolutionAvailable && (nMask & 0x02)) {
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
        glFlush();
        RenderOverlayConvolution(g_nRootTexture);
    }
    if (g_bDistortionAvailable && (nMask & 0x20)) {
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        RenderForceDistortion(g_nRootTexture);
    }
    if (g_bScanNoiseAvailable && g_bScanNoise && (nMask & 0x04) && g_pFilmNoiseTexture &&
        !g_pFilmNoiseTexture->IsNull()) {
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        RenderOverlayFilmNoise(g_nRootTexture, 1);
    }
    if (g_bFilmNoiseAvailable && g_bFilmNoise && (nMask & 0x04) && g_pFilmNoiseTexture &&
        !g_pFilmNoiseTexture->IsNull()) {
        glEnable(GL_TEXTURE_RECTANGLE_NV);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
        glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        RenderOverlayFilmNoise(g_nRootTexture, 0);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(g_nCapAlphaTest);
    glEnable(g_eGLDepthTest);
    glEnable(g_nCapLighting);
    glDepthMask(1);
    g_bBloomRendered = 0;
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
