// Screen shots: SaveScreenShot renders the camera view into a scaled
// TGA (0x00420f20); AurSnapshot writes the current frame buffer
// (0x004211a0).
typedef unsigned int size_t;

extern "C" int __cdecl strcmp(const char* a, const char* b);
extern "C" char* __cdecl strcpy(char* d, const char* s);
extern "C" char* __cdecl strcat(char* d, const char* s);
extern "C" size_t __cdecl strlen(const char* s);
extern "C" void* __cdecl memset(void* d, int c, size_t n);
extern "C" void* __cdecl memcpy(void* d, const void* s, size_t n);
#pragma intrinsic(memcpy, strcmp, strcpy, strcat, strlen, memset)
extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" int __cdecl sprintf(char* d, const char* fmt, ...);

void* __cdecl operator new(size_t n);
void __cdecl operator delete(void* p);

__declspec(dllimport) void __stdcall glGetIntegerv(unsigned int pname, int* params);
__declspec(dllimport) void __stdcall glClear(unsigned int mask);
__declspec(dllimport) void __stdcall glLoadIdentity();
__declspec(dllimport) void __stdcall glReadPixels(int x, int y, int w, int h,
                                                  unsigned int format, unsigned int type,
                                                  void* pixels);

extern const unsigned int g_nSnapshotFormat;    // 0x0073f370 (GL_RGB)
extern int g_nSnapshotCount;                    // 0x007a47b4

void __cdecl ImageFlipY(unsigned char* p, int w, int h, int bpp);           // 0x0045d990
void __cdecl ImageWriteTGA(char* szFile, int w, int h, int bpp, unsigned char* p);  // 0x0045f2a0
unsigned char* __cdecl ImageScale(unsigned char* p, int w, int h, int bpp,
                                  int nNewW, int nNewH);                   // 0x0045dad0
void __cdecl SaveScreenShot(char* szName, int nSize, int bSquare, int bPad);

extern int g_nScreenShotCount;                  // 0x007a47b0
extern int g_bRenderingScreenShot;              // 0x007b9350

class Gob;
Gob* __cdecl GobListFind(char* szName);         // 0x00440460

class CameraView {
public:
    virtual void v00();
    virtual void v04();
    virtual void Render();                      // +0x08
};

class Gob {
public:
    virtual ~Gob();
};

// A camera gob: the render interface comes first, the Gob base at +4.
class CameraGob : public CameraView, public Gob {
};

// FUNCTION: SWKOTOR 0x00420f20
// NONMATCH (57): same code with esi / ebx swapped for the width and the
// scaled image (ours keeps the padded image in ebx, 16 bytes shorter); the
// else arm really reads bSquare into the height and leaves nOutW unset.
void __cdecl SaveScreenShot(char* szName, int nSize, int bSquare, int bPad)
{
    int nOutW;
    int nOutH;
    char szFile[64];
    if (szName && strcmp(szName, "NULL")) {
        strcpy(szFile, szName);
        int nLen = strlen(szFile);
        if (nLen <= 3 || _stricmp(&szFile[nLen - 4], ".tga"))
            strcat(szFile, ".tga");
    } else {
        sprintf(szFile, "snap%d.tga", ++g_nScreenShotCount);
    }
    int aViewport[4];
    glGetIntegerv(0xba2, aViewport);
    int nWidth = aViewport[2];
    int nHeight = aViewport[3];
    if (nWidth % 4)
        nWidth = nWidth - nWidth % 4 + 4;
    if (nWidth > nHeight) {
        if ((unsigned char)bSquare) {
            nOutW = nSize;
            nOutH = nSize;
        } else {
            nOutW = nSize;
            nOutH = (int)((double)nHeight * nSize / nWidth);
        }
    } else {
        nOutH = bSquare;
    }
    Gob* pGob = GobListFind("camera");
    CameraGob* pCamera = pGob ? static_cast<CameraGob*>(pGob) : 0;
    glClear(0x4500);
    glLoadIdentity();
    g_bRenderingScreenShot = 1;
    pCamera->Render();
    g_bRenderingScreenShot = 0;
    unsigned char* pPixels = (unsigned char*)operator new(nHeight * nWidth * 3);
    memset(pPixels, 0, nHeight * nWidth * 3);
    glReadPixels(0, 0, nWidth, nHeight, g_nSnapshotFormat, 0x1401, pPixels);
    ImageFlipY(pPixels, nWidth, nHeight, 3);
    unsigned char* pScaled = ImageScale(pPixels, nWidth, nHeight, 3, nOutW, nOutH);
    if (!(unsigned char)bSquare && (unsigned char)bPad) {
        int nSide = nOutW > nOutH ? nOutW : nOutH;
        unsigned char* pPadded = (unsigned char*)operator new(nSide * nSide * 3);
        memset(pPadded, 0x7f, nSide * nSide * 3);
        memcpy(pPadded, pScaled, nOutH * nOutW * 3);
        ImageWriteTGA(szFile, nSide, nSide, 3, pPadded);
        operator delete(pPadded);
    } else {
        ImageWriteTGA(szFile, nOutW, nOutH, 3, pScaled);
    }
    operator delete(pScaled);
    operator delete(pPixels);
}

// FUNCTION: SWKOTOR 0x004211a0
void AurSnapshot(char* szName, unsigned char bIncludeGUI)
{
    char szFile[64];
    if (szName && strcmp(szName, "NULL")) {
        strcpy(szFile, szName);
        int nLen = strlen(szFile);
        if (nLen <= 3 || _stricmp(&szFile[nLen - 4], ".tga"))
            strcat(szFile, ".tga");
    } else {
        sprintf(szFile, "snap%d.tga", ++g_nSnapshotCount);
    }
    int aViewport[4];
    glGetIntegerv(0xba2, aViewport);
    int nWidth = aViewport[2];
    int nHeight = aViewport[3];
    if (nWidth % 4)
        nWidth = nWidth - nWidth % 4 + 4;
    if (bIncludeGUI) {
        unsigned char* pPixels = (unsigned char*)operator new(nHeight * nWidth * 3);
        memset(pPixels, 0, nHeight * nWidth * 3);
        glReadPixels(0, 0, nWidth, nHeight, g_nSnapshotFormat, 0x1401, pPixels);
        ImageFlipY(pPixels, nWidth, nHeight, 3);
        ImageWriteTGA(szFile, nWidth, nHeight, 3, pPixels);
        operator delete(pPixels);
        return;
    }
    SaveScreenShot(szName, nWidth, 0, 0);
}
