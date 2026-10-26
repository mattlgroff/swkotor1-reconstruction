// Soft shadow framebuffer copies, 0x0042d140-0x0042d9a0.
#include "g/fb_effects.h"

// FUNCTION: SWKOTOR 0x0042d140
void CopySoftShadowsATI()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// FUNCTION: SWKOTOR 0x0042d1c0
void SetUpSoftShadows()
{
    if (AurGLCaps7E0()) {
        CopySoftShadowsATI();
        return;
    }
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nSoftShadowTexture);
    glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
    glFlush();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// FUNCTION: SWKOTOR 0x0042d9a0
void FinishSoftShadows()
{
    if (AurGLCaps7E0()) {
        FinishSoftShadowsATI();
        return;
    }
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
    glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, g_nScreenWidth, g_nScreenHeight);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
    glFlush();
    wglMakeCurrent(g_hSoftShadowDC2, g_hSoftShadowContext2);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, 0x200, 0x200);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    int i;
    for (i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                     (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    wglMakeCurrent(g_hGLDC, g_hGLContext);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nSoftShadowTexture);
    glDisable(GL_LIGHTING);
    glEnable(g_nCapBlend);
    glBlendFunc(1, 0);
    glDisable(GL_STENCIL_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, g_nScreenWidth, g_nScreenHeight);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                     (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
    glDepthMask(0);
    glPushAttrib(0x6040);
    wglMakeCurrent(g_hSoftShadowDC1, g_hSoftShadowContext1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glViewport(0, 0, 0x200, 0x200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    BloomFilter(g_hSoftShadowDC1, g_hSoftShadowContext1, 512.0f, 512.0f, g_hSoftShadowPbuffer2,
                g_nSoftShadowTexture2, 512.0f, 1);
    wglMakeCurrent(g_hGLDC, g_hGLContext);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture1);
    wglBindTexImageARB(g_hSoftShadowPbuffer1, WGL_FRONT_LEFT_ARB);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : 1.0f, (i == 0 || i == 3) ? 1.0f : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    wglReleaseTexImageARB(g_hSoftShadowPbuffer1, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(g_eGLDepthTest);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(1);
    glStencilMask(0xff);
}

// FUNCTION: SWKOTOR 0x0042d250
void FinishSoftShadowsATI()
{
    glDisable(g_eGLDepthTest);
    glEnable(g_nCapBlend);
    glDisable(GL_STENCIL_TEST);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_nPowerWidth, g_nPowerHeight);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glFlush();
    glClear(GL_COLOR_BUFFER_BIT);
    wglMakeContextCurrentARB(g_hSoftShadowDC1, g_hSoftShadowDC1, g_hSoftShadowContext1);
    glDrawBuffer(GL_FRONT);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glViewport(0, 0, 0x200, 0x200);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    int i;
    for (i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : fU, (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);
    glDrawBuffer(GL_BACK);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, g_nScreenWidth, g_nScreenHeight);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : fU, (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPushAttrib(0x6040);
    wglMakeContextCurrentARB(g_hSoftShadowDC2, g_hSoftShadowDC2, g_hSoftShadowContext2);
    glDrawBuffer(GL_FRONT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, 0x200, 0x200);
    glEnable(GL_BLEND);
    glBlendFunc(1, 0);
    BloomFilterATI(g_hSoftShadowDC2, g_hSoftShadowContext2, 512.0f, 512.0f, g_hSoftShadowPbuffer1,
                   g_nSoftShadowTexture1, 512.0f, 1);
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glFlush();
    wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);
    glDrawBuffer(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, g_nScreenWidth, 0.0, g_nScreenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, g_nScreenWidth, g_nScreenHeight);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture2);
    wglBindTexImageARB(g_hSoftShadowPbuffer2, WGL_FRONT_LEFT_ARB);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : 1.0f, (i == 0 || i == 3) ? 1.0f : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                   (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    wglReleaseTexImageARB(g_hSoftShadowPbuffer2, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(g_eGLDepthTest);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(1);
    glStencilMask(0xff);
    glDisable(GL_FRAGMENT_SHADER_ATI);
}
