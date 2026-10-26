// Bloom convolution overlay passes, 0x0042c6e0-0x0042d140.
#include "g/fb_effects.h"

// FUNCTION: SWKOTOR 0x0042cc60
void RenderOverlayConvolution(unsigned int nTexture)
{
    glBlendFunc(GL_SRC_ALPHA, 1);
    glEnable(g_nCapBlend);
    wglMakeCurrent(g_ahBloomDC[3], g_ahBloomContext[3]);
    glBlendFunc(GL_SRC_ALPHA, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 512.0, 0.0, 512.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, 0x200, 0x200);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(g_nCapLighting);
    FrameBufferModificationPixelShaders(1, nTexture, g_nBloomRamp0, g_nBloomRamp1, g_nBloomRamp2);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                     (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : 512.0f, (i == 0 || i == 3) ? 512.0f : 0.0f, 0.0f);
    }
    glEnd();
    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
    glFlush();
    MipmapFilter(g_ahBloomDC[2], g_ahBloomContext[2], 256.0f, 256.0f, g_ahBloomPbuffer[3],
                 g_anBloomTexture[3], 512.0f);
    MipmapFilter(g_ahBloomDC[1], g_ahBloomContext[1], 128.0f, 128.0f, g_ahBloomPbuffer[2],
                 g_anBloomTexture[2], 256.0f);
    MipmapFilter(g_ahBloomDC[0], g_ahBloomContext[0], 64.0f, 64.0f, g_ahBloomPbuffer[1],
                 g_anBloomTexture[1], 128.0f);
    BloomFilter(g_ahBloomDC[5], g_ahBloomContext[5], 256.0f, 256.0f, g_ahBloomPbuffer[g_nBloomSource],
                g_anBloomTexture[g_nBloomSource], g_fBloomSize, 0);
    wglMakeCurrent(g_hGLDC, g_hGLContext);
    glBlendFunc(GL_SRC_ALPHA, 1);
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
    glBindTexture(GL_TEXTURE_2D, g_anBloomTexture[g_nBloomLevel]);
    wglBindTexImageARB(g_ahBloomPbuffer[g_nBloomLevel], WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FrameBufferModificationPixelShaders(4, 0, 0, 0, 0);
    glBegin(GL_QUADS);
    for (int j = 0; j < 4; j++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((j == 0 || j == 1) ? 0.0f : 1.0f, (j == 0 || j == 3) ? 1.0f : 0.0f);
        glVertex3f((j == 0 || j == 1) ? 0.0f : (float)g_nScreenWidth,
                   (j == 0 || j == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(g_ahBloomPbuffer[g_nBloomLevel], WGL_FRONT_LEFT_ARB);
    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
}

// FUNCTION: SWKOTOR 0x0042c6e0
void RenderOverlayConvolutionATI(unsigned int nTexture)
{
    if (!g_bConvolutionKeep) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glBlendFunc(GL_SRC_ALPHA, 1);
    glEnable(g_nCapBlend);
    void* hRC = g_hGLContext;
    void* hDC = g_hGLDC;
    wglMakeContextCurrentARB(g_ahBloomDC[3], g_ahBloomDC[3], g_ahBloomContext[3]);
    glDrawBuffer(GL_FRONT);
    glBlendFunc(GL_SRC_ALPHA, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 512.0, 0.0, 512.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, 0x200, 0x200);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(g_nCapLighting);
    FrameBufferModificationPixelShadersATI(1, nTexture, g_nBloomRamp0, g_nBloomRamp1, g_nBloomRamp2);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (i == 0 || i == 1) ? 0.0f : fU,
                             (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : 512.0f, (i == 0 || i == 3) ? 512.0f : 0.0f, 0.0f);
    }
    glEnd();
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    glFlush();
    wglMakeContextCurrentARB(hDC, hDC, hRC);
    MipmapFilterATI(g_ahBloomDC[2], g_ahBloomContext[2], 256.0f, 256.0f, g_ahBloomPbuffer[3],
                    g_anBloomTexture[3], 512.0f);
    MipmapFilterATI(g_ahBloomDC[1], g_ahBloomContext[1], 128.0f, 128.0f, g_ahBloomPbuffer[2],
                    g_anBloomTexture[2], 256.0f);
    MipmapFilterATI(g_ahBloomDC[0], g_ahBloomContext[0], 64.0f, 64.0f, g_ahBloomPbuffer[1],
                    g_anBloomTexture[1], 128.0f);
    BloomFilterATI(g_ahBloomDC[5], g_ahBloomContext[5], 256.0f, 256.0f,
                   g_ahBloomPbuffer[g_nBloomSource], g_anBloomTexture[g_nBloomSource], g_fBloomSize, 0);
    wglMakeContextCurrentARB(hDC, hDC, hRC);
    glBlendFunc(GL_SRC_ALPHA, 1);
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
    glBindTexture(GL_TEXTURE_2D, g_anBloomTexture[g_nBloomLevel]);
    wglBindTexImageARB(g_ahBloomPbuffer[g_nBloomLevel], WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FrameBufferModificationPixelShadersATI(4, 0, 0, 0, 0);
    glDrawBuffer(GL_BACK);
    glBegin(GL_QUADS);
    for (int j = 0; j < 4; j++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((j == 0 || j == 1) ? 0.0f : 1.0f, (j == 0 || j == 3) ? 1.0f : 0.0f);
        glVertex3f((j == 0 || j == 1) ? 0.0f : (float)g_nScreenWidth,
                   (j == 0 || j == 3) ? (float)g_nScreenHeight : 0.0f, 0.0f);
    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(g_ahBloomPbuffer[g_nBloomLevel], WGL_FRONT_LEFT_ARB);
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
}
