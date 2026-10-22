// console text overlay (positions a string on the character grid),
// 0x0044ce50.
#include "g/gl_draw.h"

extern int g_bConsoleOverlay;                         // 0x0078e360
extern char g_bFontFlag;                              // 0x0078e35c
extern unsigned int g_nGLCap2;                        // 0x0078d45c
void AurBuildFont();                                  // 0x0044cb10
void AurDrawStringAt(char* sz, float x, float y);     // 0x0044cb90
void AurDrawFontStringAt(char* sz, float x, float y); // 0x0044cca0

// Same body as 0x0044cc40, inlined here.
static void AurInitFontInline()
{
    g_bFontFlag = 0;
    AurBuildFont();
    g_bFontInitialised = 1;
}

// FUNCTION: SWKOTOR 0x0044ce50
void AurDrawConsoleString(char* sz, int nCol, int nRow)
{
    if (g_bConsoleOverlay) {
        int aViewport[4];
        glGetIntegerv(0xba2, aViewport);
        if (aViewport[2] < 300 || aViewport[3] < 100)
            return;
        int nRows = aViewport[3] / 14;
        int nCols = aViewport[2] / 10;
        if (nRow >= nRows)
            nRow = nRows - 1;
        if (nRow < 0) {
            nRow += nRows;
            if (nRow < 0)
                nRow = 0;
        }
        if (nCol < 0)
            nCol = nCols - strlen(sz) + nCol + 1;
        if ((int)strlen(sz) + nCol > nCols)
            nCol = nCols - strlen(sz);
        if (nCol < 0)
            nCol = 0;
        nRow = nRows - nRow - 1;
        glDisable(g_nGLCap2);
        if (!g_bFontInitialised)
            AurInitFontInline();
        if (g_bFontFlag) {
            AurDrawFontStringAt(sz, (float)nCol / nCols * aViewport[2] - aViewport[2] / 2,
                                (float)nRow / nRows * aViewport[3] - aViewport[3] / 2);
        } else if (sz && *sz) {
            AurDrawStringAt(sz, (float)nCol / nCols, (float)nRow / nRows);
        }
        glEnable(g_nGLCap2);
    }
}
