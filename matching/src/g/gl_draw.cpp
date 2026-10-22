// immediate / client-array GL draw helpers, 0x004318f0-0x00432780.
#include "g/gl_draw.h"

// FUNCTION: SWKOTOR 0x00431bb0
void DrawPoolVerticesPCT(int nMode, int nId, int nCount)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0x18, 0);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0x18, (void*)0xc);
        glTexCoordPointer(2, GL_FLOAT, 0x18, (void*)0x10);
        glDrawArrays(g_aPrimitiveModes[nMode], 0, nCount);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glDisableClientState(GL_COLOR_ARRAY);
    } else {
        glBegin(g_aPrimitiveModes[nMode]);
        VertexPCT* p = (VertexPCT*)g_pRequestPoolManager->GetBuffer(nId);
        for (int i = 0; i < nCount; i++) {
            glColor4ubv(p[i].c);
            glTexCoord2f(p[i].s, p[i].t);
            glVertex3fv(p[i].v);
        }
        glEnd();
    }
}

// FUNCTION: SWKOTOR 0x00431cc0
void DrawQuadsPCT(VertexPCT* p, int nCount)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0x18, p->c);
        glTexCoordPointer(2, GL_FLOAT, 0x18, &p->s);
        glVertexPointer(3, GL_FLOAT, 0x18, p);
        glDrawArrays(GL_QUADS, 0, nCount);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    } else {
        glBegin(GL_QUADS);
        for (int i = 0; i < nCount; i++) {
            glColor4ubv(p[i].c);
            glTexCoord2f(p[i].s, p[i].t);
            glVertex3fv(p[i].v);
        }
        glEnd();
    }
}

// FUNCTION: SWKOTOR 0x00431db0
void DrawPoolQuadsPCT(int nId, int nCount)
{
    if (HasVertexBuffers()) {
        int nBuffer = g_pRequestPoolManager->GetBuffer(nId);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nBuffer);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0x18, (void*)0xc);
        glTexCoordPointer(2, GL_FLOAT, 0x18, (void*)0x10);
        glVertexPointer(3, GL_FLOAT, 0x18, 0);
        glDrawArrays(GL_QUADS, 0, nCount);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    } else {
        glBegin(GL_QUADS);
        VertexPCT* p = (VertexPCT*)g_pRequestPoolManager->GetBuffer(nId);
        for (int i = 0; i < nCount; i++) {
            glColor4ubv(p[i].c);
            glTexCoord2f(p[i].s, p[i].t);
            glVertex3fv(p[i].v);
        }
        glEnd();
    }
}

// FUNCTION: SWKOTOR 0x00431eb0
void DrawQuadsPT(VertexPT* p, int nCount)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0x14, &p->s);
        glVertexPointer(3, GL_FLOAT, 0x14, p);
        glDrawArrays(GL_QUADS, 0, nCount);
    } else {
        glBegin(GL_QUADS);
        for (int i = 0; i < nCount; i++) {
            glTexCoord2f(p[i].s, p[i].t);
            glVertex3fv(p[i].v);
        }
        glEnd();
    }
}

// FUNCTION: SWKOTOR 0x00431f70
void DrawVerticesPC(int nMode, VertexPC* p, int nCount)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0x10, p->c);
        glVertexPointer(3, GL_FLOAT, 0x10, p);
        glDrawArrays(g_aPrimitiveModes[nMode], 0, nCount);
    } else {
        glBegin(g_aPrimitiveModes[nMode]);
        for (int i = 0; i < nCount; i++) {
            glColor4ubv(p[i].c);
            glVertex3fv(p[i].v);
        }
        glEnd();
    }
}

// FUNCTION: SWKOTOR 0x00432040
void AurDrawString(char* sz)
{
    if (!g_bFontInitialised)
        AurInitFont();
    glPushAttrib(0x20000);
    glListBase(g_nFontListBase);
    glCallLists(strlen(sz), GL_UNSIGNED_BYTE, sz);
    glPopAttrib();
}

// FUNCTION: SWKOTOR 0x00432720
void RenderAllSquares()
{
    glDisable(0xb50);
    glDisable(0xde1);
    glDisable(0xb71);
    for (int i = 0; i < g_aRenderSquares.num; i++)
        RenderSquares(g_aRenderSquares.element[i]);
    glEnable(0xb71);
    glEnable(0xb50);
}

// FUNCTION: SWKOTOR 0x0044cb90
void AurDrawStringAt(char* sz, float x, float y)
{
    glPushAttrib(0x40140);
    glDisable(0xde1);
    glDisable(0xb50);
    glDisable(0xb71);
    glMatrixMode(0x1701);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glRasterPos2f(x, y);
    AurDrawString(sz);
    glRasterPos2f(0.0f, 0.0f);
    glPopMatrix();
    glMatrixMode(0x1700);
    glPopAttrib();
}

// FUNCTION: SWKOTOR 0x0044cca0
void AurDrawFontStringAt(char* sz, float x, float y)
{
    glEnable(g_nGLCap4);
    glBlendFunc(g_aGLBlendFactors[0], g_aGLBlendFactors[1]);
    if (!sz || !*sz)
        return;
    int aViewport[4];
    glGetIntegerv(0xba2, aViewport);
    glPushAttrib(0x2140);
    glDisable(g_nGLCap1);
    glEnable(0xb57);
    glMatrixMode(g_aGLMatrixModes[0]);
    glPushMatrix();
    glLoadIdentity();
    _glOrtho(-(aViewport[2] / 2) * 0.01f, (aViewport[2] / 2) * 0.01f,
             -(aViewport[3] / 2) * 0.01f, (aViewport[3] / 2) * 0.01f, 0.0, 10.0);
    glTranslatef(x * 0.01f, y * 0.01f, 0.0f);
    g_aFonts.element[g_nCurrentFont]->m_pTexture->Bind();
    g_aFonts.element[g_nCurrentFont]->Draw(sz, strlen(sz));
    glPopMatrix();
    glMatrixMode(g_aGLMatrixModes[2]);
    glEnable(g_nGLCap1);
    glDisable(0xb57);
    glDisable(g_nGLCap4);
    glPopAttrib();
}
