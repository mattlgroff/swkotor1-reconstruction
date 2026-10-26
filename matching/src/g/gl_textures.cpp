// Generated GL textures: the specular look-up table (0x00423d80) and the
// normalization cube map (0x00425d80 / 0x00425ea0).
typedef unsigned int size_t;

extern "C" {
double __cdecl pow(double x, double y);
double __cdecl sqrt(double x);
__declspec(dllimport) unsigned char __stdcall glIsTexture(unsigned int tex);
__declspec(dllimport) void __stdcall glGenTextures(int n, unsigned int* tex);
__declspec(dllimport) void __stdcall glBindTexture(unsigned int target, unsigned int tex);
__declspec(dllimport) void __stdcall glTexParameteri(unsigned int target, unsigned int pname, int param);
int __stdcall gluBuild2DMipmaps(unsigned int target, int components, int w, int h,
                                unsigned int format, unsigned int type, const void* data);
}
#pragma intrinsic(pow, sqrt)

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

extern unsigned int g_nLookUpTexture;           // 0x007bb868
extern int g_nCurrentStage;                     // 0x007a6898
extern int g_aCurrentStageTID[6];               // 0x007a683c

// FUNCTION: SWKOTOR 0x00423d80
void MakeLookUpTexture(float fExponent)
{
    if (glIsTexture(g_nLookUpTexture))
        return;
    unsigned char* pData = new unsigned char[0x20000];
    unsigned char* p = pData;
    for (int i = 0; i < 256; i++) {
        unsigned char nHigh = (unsigned char)(pow(i * (1.0 / 255.0), fExponent) * 255.99);
        for (int j = 0; j < 256; j++) {
            float f = j / 255.0f;
            *p++ = (unsigned char)(f * 255.99f);
            *p++ = nHigh;
        }
    }
    glGenTextures(1, &g_nLookUpTexture);
    glBindTexture(0xde1, g_nLookUpTexture);
    g_aCurrentStageTID[g_nCurrentStage] = 0;
    gluBuild2DMipmaps(0xde1, 0x190a, 256, 256, 0x190a, 0x1401, pData);
    glTexParameteri(0xde1, 0x2802, 0x812f);
    glTexParameteri(0xde1, 0x2803, 0x812f);
    glTexParameteri(0xde1, 0x2800, 0x2601);
    glTexParameteri(0xde1, 0x2801, 0x2703);
    delete[] pData;
}
