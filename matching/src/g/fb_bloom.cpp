// Bloom pbuffer passes, 0x00433580-0x00433970.
#include "g/fb_effects.h"

// FUNCTION: SWKOTOR 0x00433580
void RenderFrameToTextureATI(unsigned int nTexture)
{
    void* hDC = g_hGLDC;
    void* hRC = g_hGLContext;
    wglMakeContextCurrentARB(g_hBloomDC, g_hBloomDC, g_hBloomContext);
    glDrawBuffer(GL_FRONT);
    glViewport(0, 0, 0x80, 0x80);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 128.0, 0.0, 128.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    FrameBufferModificationPixelShadersATI(4, 0, 0, 0, 0);
    glDisable(GL_DEPTH_TEST);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    glBlendFunc(1, 0);
    float fU = (float)g_nScreenWidth / g_nPowerWidth;
    float fV = (float)g_nScreenHeight / g_nPowerHeight;
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : fU, (i == 0 || i == 3) ? fV : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : 128.0f, (i == 0 || i == 3) ? 128.0f : 0.0f, 0.0f);
    }
    glEnd();
    glDisable(GL_FRAGMENT_SHADER_ATI);
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    glFlush();
    wglMakeContextCurrentARB(hDC, hDC, hRC);
    glDrawBuffer(GL_BACK);
}

// FUNCTION: SWKOTOR 0x004337a0
void RenderFrameToTexture(unsigned int nTexture)
{
    wglMakeCurrent(g_hBloomDC, g_hBloomContext);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 128.0, 0.0, 128.0, -1.0, 1.0);
    glViewport(0, 0, 0x80, 0x80);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    glBindTexture(GL_TEXTURE_RECTANGLE_NV, nTexture);
    FrameBufferModificationPixelShaders(4, 0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBlendFunc(1, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : (float)g_nScreenWidth,
                     (i == 0 || i == 3) ? (float)g_nScreenHeight : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : 128.0f, (i == 0 || i == 3) ? 128.0f : 0.0f, 0.0f);
    }
    glEnd();
    FrameBufferModificationPixelShaders(2, 0, 0, 0, 0);
    glFlush();
    wglMakeCurrent(g_hGLDC, g_hGLContext);
}

// FUNCTION: SWKOTOR 0x0042b370
void MipmapFilterATI(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                     unsigned int nTexture, float fSize)
{
    wglMakeContextCurrentARB(hDC, hDC, hRC);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, fWidth, 0.0, fHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int)fWidth, (int)fHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FrameBufferModificationPixelShadersATI(4, 0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (i == 0 || i == 1) ? 0.0f : 1.0f,
                             (i == 0 || i == 3) ? 1.0f : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : fWidth, (i == 0 || i == 3) ? fHeight : 0.0f, 0.0f);
    }
    glEnd();
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glFlush();
}

// FUNCTION: SWKOTOR 0x0042b5d0
void MipmapFilter(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                  unsigned int nTexture, float fSize)
{
    wglMakeCurrent(hDC, hRC);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, fWidth, 0.0, fHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int)fWidth, (int)fHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FrameBufferModificationPixelShaders(4, 0, 0, 0, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((i == 0 || i == 1) ? 0.0f : 1.0f, (i == 0 || i == 3) ? 1.0f : 0.0f);
        glVertex3f((i == 0 || i == 1) ? 0.0f : fWidth, (i == 0 || i == 3) ? fHeight : 0.0f, 0.0f);
    }
    glEnd();
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glFlush();
}

// NONMATCH (114): the stack slots of fInv and the unit-0 s coordinate are
// swapped, and the original keeps both arms of the unit-0 s expression
// whole (fsub/fmul in each) where cl cross-jumps the common fsub. The rest of
// the 5x5 tap loop lines up once those slots agree.
// FUNCTION: SWKOTOR 0x0042b810
void BloomFilterATI(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                    unsigned int nTexture, float fSize, int bScale)
{
    FrameBufferModificationPixelShadersATI(2, 0, 0, 0, 0);
    wglMakeContextCurrentARB(hDC, hDC, hRC);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, fWidth, 0.0, fHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int)fWidth, (int)fHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE3_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    if (bScale) {
        glEnable(GL_FRAGMENT_SHADER_ATI);
        glBindFragmentShaderATI(g_nBloomShaderATI);
    } else {
        FrameBufferModificationPixelShadersATI(8, 0, 0, 0, 0);
    }
    int nRow = 0;
    int nCol = 0;
    glEnable(GL_BLEND);
    float fInv = 1.0f / fSize;
    for (int k = 0; k < 25; k += 4) {
        for (int i = 0; i < 4; i++) {
            float c[4];
            if (bScale) {
                float f = g_fBloomScale * g_afBloomKernelATI[i + k];
                c[0] = f;
                c[1] = f;
                c[2] = f;
                c[3] = f;
            } else {
                c[0] = g_afBloomKernelATI[i + k];
                c[1] = g_afBloomKernelATI[i + k];
                c[2] = g_afBloomKernelATI[i + k];
                c[3] = g_afBloomKernelATI[i + k];
            }
            if (i == 0)
                glSetFragmentShaderConstantATI(GL_CON_0_ATI, c);
            else if (i == 1)
                glSetFragmentShaderConstantATI(GL_CON_1_ATI, c);
            else if (i == 2)
                glSetFragmentShaderConstantATI(GL_CON_2_ATI, c);
            else
                glSetFragmentShaderConstantATI(GL_CON_3_ATI, c);
        }
        glBegin(GL_QUADS);
        float fRow = (float)nRow;
        float fCol = (float)nCol;
        for (int v = 0; v < 4; v++) {
            glColor4f(1.0f, 1.0f, 1.0f, g_fBloomAlpha);
            glActiveTextureARB(GL_TEXTURE0_ARB);
            float fT = (((v == 0 || v == 3) ? fRow + fSize : fRow) - g_fBloomOffset) * fInv;
            float fS = (v == 0 || v == 1) ? (fCol - g_fBloomOffset) * fInv
                                          : (fCol + fSize - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, fS, fT);
            int nTapRow = (nCol + 1 > 4) ? nRow + 1 : nRow;
            int nTapCol = (nCol + 1) % 5;
            glActiveTextureARB(GL_TEXTURE1_ARB);
            fT = (((v == 0 || v == 3) ? nTapCol + fSize : nTapCol) - g_fBloomOffset) * fInv;
            fS = (((v == 0 || v == 1) ? nTapRow : nTapRow + fSize) - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, fS, fT);
            nTapRow = (nCol + 2 > 4) ? nRow + 1 : nRow;
            nTapCol = (nCol + 2) % 5;
            glActiveTextureARB(GL_TEXTURE2_ARB);
            fT = (((v == 0 || v == 3) ? nTapCol + fSize : nTapCol) - g_fBloomOffset) * fInv;
            fS = (((v == 0 || v == 1) ? nTapRow : nTapRow + fSize) - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE2_ARB, fS, fT);
            nTapRow = (nCol + 3 > 4) ? nRow + 1 : nRow;
            nTapCol = (nCol + 3) % 5;
            glActiveTextureARB(GL_TEXTURE3_ARB);
            fT = (((v == 0 || v == 3) ? nTapCol + fSize : nTapCol) - g_fBloomOffset) * fInv;
            fS = (((v == 0 || v == 1) ? nTapRow : nTapRow + fSize) - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE3_ARB, fS, fT);
            glActiveTextureARB(GL_TEXTURE0_ARB);
            float fY = (v == 0 || v == 3) ? fHeight : 0.0f;
            float fX = (v == 0 || v == 1) ? 0.0f : fWidth;
            glVertex3f(fX, fY, 0.0f);
        }
        glEnd();
        if (nCol + 4 > 4)
            nRow++;
        nCol = (nCol + 4) % 5;
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glDisable(GL_TEXTURE_2D);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTextureARB(GL_TEXTURE3_ARB);
    glDisable(GL_TEXTURE_2D);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glFlush();
}

// NONMATCH (106): same two differences as BloomFilterATI (fInv / unit-0 s slot
// swap and the cross-jumped unit-0 s arms).
// FUNCTION: SWKOTOR 0x0042bf80
void BloomFilter(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                 unsigned int nTexture, float fSize, int bScale)
{
    wglMakeCurrent(hDC, hRC);
    glBlendFunc(GL_SRC_ALPHA, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, fWidth, 0.0, fHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int)fWidth, (int)fHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE3_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nTexture);
    wglBindTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_PER_STAGE_CONSTANTS_NV);
    if (bScale)
        FrameBufferModificationPixelShaders(0x256, 0, 0, 0, 0);
    else
        FrameBufferModificationPixelShaders(8, 0, 0, 0, 0);
    int nRow = 0;
    int nCol = 0;
    glEnable(GL_BLEND);
    float fInv = 1.0f / fSize;
    for (int k = 0; k < 25; k += 4) {
        for (int i = 0; i < 4; i++) {
            float c[4];
            if (bScale) {
                float f = g_fBloomScale * g_afBloomKernel[i + k];
                c[0] = f;
                c[1] = f;
                c[2] = f;
                c[3] = f;
            } else {
                c[0] = g_afBloomKernel[i + k];
                c[1] = g_afBloomKernel[i + k];
                c[2] = g_afBloomKernel[i + k];
                c[3] = g_afBloomKernel[i + k];
            }
            if (i == 0)
                glCombinerStageParameterfvNV(GL_COMBINER0_NV, GL_CONSTANT_COLOR0_NV, c);
            else if (i == 1)
                glCombinerStageParameterfvNV(GL_COMBINER0_NV, GL_CONSTANT_COLOR1_NV, c);
            else if (i == 2)
                glCombinerStageParameterfvNV(GL_COMBINER1_NV, GL_CONSTANT_COLOR0_NV, c);
            else
                glCombinerStageParameterfvNV(GL_COMBINER2_NV, GL_CONSTANT_COLOR0_NV, c);
        }
        glBegin(GL_QUADS);
        float fRow = (float)nRow;
        float fCol = (float)nCol;
        for (int v = 0; v < 4; v++) {
            glColor4f(1.0f, 1.0f, 1.0f, g_fBloomAlpha);
            glActiveTextureARB(GL_TEXTURE0_ARB);
            float fT = (((v == 0 || v == 3) ? fRow + fSize : fRow) - g_fBloomOffset) * fInv;
            float fS = (v == 0 || v == 1) ? (fCol - g_fBloomOffset) * fInv
                                          : (fCol + fSize - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, fS, fT);
            int nTapRow = (nCol + 1 > 4) ? nRow + 1 : nRow;
            int nTapCol = (nCol + 1) % 5;
            glActiveTextureARB(GL_TEXTURE1_ARB);
            fT = (((v == 0 || v == 3) ? nTapCol + fSize : nTapCol) - g_fBloomOffset) * fInv;
            fS = (((v == 0 || v == 1) ? nTapRow : nTapRow + fSize) - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, fS, fT);
            nTapRow = (nCol + 2 > 4) ? nRow + 1 : nRow;
            nTapCol = (nCol + 2) % 5;
            glActiveTextureARB(GL_TEXTURE2_ARB);
            fT = (((v == 0 || v == 3) ? nTapCol + fSize : nTapCol) - g_fBloomOffset) * fInv;
            fS = (((v == 0 || v == 1) ? nTapRow : nTapRow + fSize) - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE2_ARB, fS, fT);
            nTapRow = (nCol + 3 > 4) ? nRow + 1 : nRow;
            nTapCol = (nCol + 3) % 5;
            glActiveTextureARB(GL_TEXTURE3_ARB);
            fT = (((v == 0 || v == 3) ? nTapCol + fSize : nTapCol) - g_fBloomOffset) * fInv;
            fS = (((v == 0 || v == 1) ? nTapRow : nTapRow + fSize) - g_fBloomOffset) * fInv;
            glMultiTexCoord2fARB(GL_TEXTURE3_ARB, fS, fT);
            glActiveTextureARB(GL_TEXTURE0_ARB);
            float fY = (v == 0 || v == 3) ? fHeight : 0.0f;
            float fX = (v == 0 || v == 1) ? 0.0f : fWidth;
            glVertex3f(fX, fY, 0.0f);
        }
        glEnd();
        if (nCol + 4 > 4)
            nRow++;
        nCol = (nCol + 4) % 5;
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glActiveTextureARB(GL_TEXTURE3_ARB);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    wglReleaseTexImageARB(hPbuffer, WGL_FRONT_LEFT_ARB);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glFlush();
}
