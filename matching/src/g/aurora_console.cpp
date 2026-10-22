// Aurora console strings, debug font and scene lookup,
// 0x0044cb10-0x0044fdb0.
extern "C" {
__declspec(dllimport) void __stdcall glLoadIdentity();
__declspec(dllimport) void __stdcall glDeleteLists(unsigned int list, int range);
__declspec(dllimport) void __stdcall glPixelStorei(unsigned int pname, int param);
__declspec(dllimport) unsigned int __stdcall glGenLists(int range);
__declspec(dllimport) void __stdcall glNewList(unsigned int list, unsigned int mode);
__declspec(dllimport) void __stdcall glBitmap(int w, int h, float xorig, float yorig,
                                              float xmove, float ymove,
                                              const unsigned char* bitmap);
__declspec(dllimport) void __stdcall glEndList();
int __cdecl sprintf(char* d, const char* fmt, ...);
int __cdecl _stricmp(const char* a, const char* b);
int __cdecl strncmp(const char* a, const char* b, unsigned int n);
unsigned int __cdecl strlen(const char* s);
}
#pragma intrinsic(strlen)

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
    void Remove(T x)
    {
        int i;
        for (i = num - 1; i >= 0; i--)
            if (element[i] == x)
                break;
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
};

struct AurConsoleVar {
    char m_szName[1];
};

class CAurPostString {
public:
    CAurPostString(const char* sz, int nX, int nY, float fLife); // 0x0044d320
    void Render();                                       // 0x0044d010
    char pad[0x434];
};

class CAurFont {
public:
    virtual ~CAurFont();
};

class CAurObject {
public:
    char pad_0[0x38];
    char m_szName[1];
};

class Scene {
public:
    CAurObject* GetObjectA(const char* szName);
    char pad_0[0x10];
    char m_szName[0x44];
    CExoArrayList<CAurObject*> m_aObjects; // +0x54
};

extern CExoArrayList<Scene*> g_aScenes;               // 0x007b9398
extern CExoArrayList<CAurPostString*> g_aPostStrings; // 0x007b90fc
extern CExoArrayList<AurConsoleVar*> g_aConsoleVars;  // 0x007b9108
extern char g_szConsoleResult[];                      // 0x007b9118
extern char g_bConsoleEcho;                           // 0x007b8fc0
extern char g_bFontFlag;                              // 0x0078e35c
extern int g_bFontInitialised;                        // 0x007b90f0
extern unsigned int g_nFontListBase;                  // 0x007a47c0
extern CAurFont* g_pFont;                             // 0x007b90ec
extern unsigned char g_aFontBitmaps[];                // 0x0078d498

const char* AurConsoleVarValue(AurConsoleVar* p); // 0x0044c1f0
void AurFlushPostStrings();                       // 0x0046ee00

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0044cb10
void AurBuildFont()
{
    glPixelStorei(0xcf5, 1);
    g_nFontListBase = glGenLists(0x80);
    unsigned int c = 0x20;
    unsigned char* pBits = g_aFontBitmaps;
    int n = 0x5f;
    do {
        glNewList(g_nFontListBase + c, 0x1300);
        glBitmap(8, 13, 0.0f, 2.0f, 10.0f, 0.0f, pBits);
        glEndList();
        c++;
        pBits += 13;
    } while (--n);
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0044cc40
void AurInitFont()
{
    g_bFontFlag = 0;
    AurBuildFont();
    g_bFontInitialised = 1;
}

// FUNCTION: SWKOTOR 0x0044cc60
void AurFreeFont()
{
    if (g_bFontInitialised) {
        g_bFontInitialised = 0;
        glDeleteLists(g_nFontListBase, 0x80);
        if (g_pFont)
            delete g_pFont;
        g_pFont = 0;
    }
}

// FUNCTION: SWKOTOR 0x0044d1e0
void AurFreePostStrings()
{
    int i = g_aPostStrings.num;
    while (i) {
        CAurPostString* p = g_aPostStrings[i - 1];
        i--;
        if (p) {
            g_aPostStrings.Remove(p);
            delete p;
        }
    }
}

// FUNCTION: SWKOTOR 0x0044d250
char* AurDeleteConsoleVars(const char* szPrefix)
{
    int nCount = 0;
    int i = g_aConsoleVars.num;
    while (i) {
        AurConsoleVar* p = g_aConsoleVars[i - 1];
        i--;
        if (szPrefix && *szPrefix
            && strncmp(p->m_szName, szPrefix, strlen(szPrefix)) != 0)
            continue;
        if (p) {
            g_aConsoleVars.Remove(p);
            delete p;
        }
        nCount++;
    }
    sprintf(g_szConsoleResult, "deleted %d trace commands", nCount);
    return g_szConsoleResult;
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0044d490
void AurPostString(const char* sz, int nX, int nY, float fLife)
{
    new CAurPostString(sz, nX, nY, fLife);
}

// FUNCTION: SWKOTOR 0x0044d620
void AurPostConsoleVars()
{
    char szLine[0x200];
    char bEcho = g_bConsoleEcho;
    g_bConsoleEcho = 0;
    for (int i = 0; i < g_aConsoleVars.num; i++) {
        sprintf(szLine, "%s: %s", g_aConsoleVars[i]->m_szName,
                AurConsoleVarValue(g_aConsoleVars[i]));
        AurPostString(szLine, 0, -1 - i, 0.0f);
    }
    g_bConsoleEcho = bEcho;
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0044d6a0
void AurRenderStrings()
{
    AurPostConsoleVars();
    glLoadIdentity();
    int i = g_aPostStrings.num;
    while (i) {
        CAurPostString* p = g_aPostStrings[i - 1];
        i--;
        p->Render();
    }
    AurFlushPostStrings();
}

// FUNCTION: SWKOTOR 0x0044fd60
Scene* FindScene(const char* szName)
{
    for (int i = 0; i < g_aScenes.num; i++) {
        if (!_stricmp(szName, g_aScenes[i]->m_szName))
            return g_aScenes[i];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0044fdb0
CAurObject* Scene::GetObjectA(const char* szName)
{
    for (int i = 0; i < m_aObjects.num; i++) {
        if (!_stricmp(szName, m_aObjects[i]->m_szName))
            return m_aObjects[i];
    }
    return 0;
}
