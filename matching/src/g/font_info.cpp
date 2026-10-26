// CAurFontInfo, the font TXI block of a texture (0x004221c0-0x00422210):
// constructor, the coordinate list reader and the field parser.
typedef unsigned int size_t;

extern "C" int __cdecl sscanf(const char* s, const char* fmt, ...);
extern "C" int __cdecl strncmp(const char* a, const char* b, size_t n);
extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" void* __cdecl memcpy(void* d, const void* s, size_t n);
#pragma intrinsic(memcpy)

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

class Vector {
public:
    float x, y, z;
};

// The sized constructor's dead `new` leaves the owner's empty-state SEH
// frame (see mdl_ctor.h).
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);                       // 0x00421310 (Vector)
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        } else {
            Allocate(n);
        }
        num = n;
    }
    __forceinline void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
};

char* __cdecl AurResGetNextLine();              // 0x0044bfa0
void* __cdecl AurResGetData(int nSize);         // 0x0044bfd0
char* __cdecl SkipWhitespace(char* s);          // 0x00463510
char* __cdecl GetWord(char* s);                 // 0x00463530
void __cdecl ParseVector(char* s, float* pOut); // 0x00463570
void __cdecl ParseFloat(char* s, float* pOut);  // 0x004635c0
void __cdecl ParseInt(char* s, int* pOut);      // 0x004635e0

inline void ParseValue(char* s, Vector* p) { ParseVector(s, &p->x); }

// Reads "<n>" followed by n lines, "<n> b" followed by n binary records, or
// lines up to "endlist".
template <class T>
void ParseList(char* s, CExoArrayList<T>* pList)
{
    int n = 0;
    char c;
    int nRead = sscanf(s, "%d %c", &n, &c);
    if (nRead > 0) {
        pList->SetSize(n);
        if (nRead == 1) {
            for (int i = 0; i < pList->num; i++) {
                char* szLine = AurResGetNextLine();
                if (!szLine)
                    return;
                ParseValue(szLine, &pList->element[i]);
            }
        } else {
            memcpy(pList->element, AurResGetData(n * sizeof(T)), n * sizeof(T));
            *s = 0;
        }
    } else {
        char* szLine;
        for (szLine = AurResGetNextLine(); szLine; szLine = AurResGetNextLine()) {
            if (!strncmp(SkipWhitespace(szLine), "endlist", 7))
                break;
            if (pList->num == 0)
                pList->SetSize(1);
            else if (pList->num == n)
                pList->Add(pList->element[n - 1]);
            ParseValue(szLine, &pList->element[n]);
            n++;
        }
        SkipWhitespace(SkipWhitespace(szLine) + 7);
    }
}

// FUNCTION: SWKOTOR 0x00421ff0 SYMBOL: ?ParseList@@YAXPADPAV?$CExoArrayList@VVector@@@@@Z
template void ParseList(char* s, CExoArrayList<Vector>* pList);

class CAurFontInfo {
public:
    int m_nNumChars;                            // +0x00
    float m_fFontHeight;                        // +0x04
    float m_fBaselineHeight;                    // +0x08
    float m_fTextureWidth;                      // +0x0c
    float m_fSpacingR;                          // +0x10
    float m_fSpacingB;                          // +0x14
    CExoArrayList<Vector> m_lUpperLeftCoords;   // +0x18
    CExoArrayList<Vector> m_lLowerRightCoords;  // +0x24

    CAurFontInfo();
    void ParseField(char* s);
};

// FUNCTION: SWKOTOR 0x004221c0
CAurFontInfo::CAurFontInfo()
{
    m_nNumChars = 0;
    m_fFontHeight = 0;
    m_fBaselineHeight = 0;
    m_fTextureWidth = 0;
    m_fSpacingR = 0;
    m_fSpacingB = 0;
}

// FUNCTION: SWKOTOR 0x00422210
void CAurFontInfo::ParseField(char* s)
{
    if (!_stricmp("numchars", GetWord(s)))
        ParseInt(s + 8, &m_nNumChars);
    if (!_stricmp("fontheight", GetWord(s)))
        ParseFloat(s + 10, &m_fFontHeight);
    if (!_stricmp("baselineheight", GetWord(s)))
        ParseFloat(s + 14, &m_fBaselineHeight);
    if (!_stricmp("texturewidth", GetWord(s)))
        ParseFloat(s + 12, &m_fTextureWidth);
    if (!_stricmp("spacingR", GetWord(s)))
        ParseFloat(s + 8, &m_fSpacingR);
    if (!_stricmp("spacingB", GetWord(s)))
        ParseFloat(s + 8, &m_fSpacingB);
    if (!_stricmp("upperleftcoords", GetWord(s)))
        ParseList(s + 15, &m_lUpperLeftCoords);
    if (!_stricmp("lowerrightcoords", GetWord(s)))
        ParseList(s + 16, &m_lLowerRightCoords);
}
