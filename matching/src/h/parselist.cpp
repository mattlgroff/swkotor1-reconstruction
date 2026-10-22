// Parse<T> list readers of the ASCII model reader
// (0x00468A00-0x00468F60); same shape as ParseList in parse.cpp but compiled
// with SkipWhitespace expanded.

#define H_ALLOCATE_OUT_OF_LINE
#define H_ADD_OUT_OF_LINE
#include "h/texctl.h"

extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int);

char* AurResGetNextLine();              // 0x0044BFA0
void* AurResGetData(int nSize);         // 0x0044BFD0

struct MaxFace_PostProcess {
    int* pVerts;            // +0x00
    int* pTVerts;           // +0x04
    int nSmoothGroup;       // +0x08
    int nMaterial;          // +0x0c
    int* pNormals;          // +0x10
};

struct SkinVertexWeight {
    char* szBones[4];       // +0x00
    float fWeights[4];      // +0x10
    int nBone0;             // +0x20
    int nBone1;             // +0x24
    char pad_0x28[8];
    int nNumBones;          // +0x30
};

void ParseMaxFace(char* s, MaxFace_PostProcess* pFace);         // 0x00463710
void ParseSkinVertexWeight(char* s, SkinVertexWeight* pWeight); // 0x004637A0
void ParseQuaternion(char* s, Quaternion* pOut);                // 0x00463A30

inline char* SkipWhitespaceInline(char* s)
{
    if (!s)
        return s;
    while (*s == ' ' || *s == '\t')
        s++;
    return s;
}

inline void ParseValue(char* s, MaxFace_PostProcess* p) { ParseMaxFace(s, p); }
inline void ParseValue(char* s, SkinVertexWeight* p) { ParseSkinVertexWeight(s, p); }
inline void ParseValue(char* s, Quaternion* p) { ParseQuaternion(s, p); }
inline void ParseValue(char* s, unsigned int* p)
{
    int n;
    ParseInt(s, &n);
    *p = n;
}

// MaxFace lists call SetSize 0x00444F10 and Add 0x004647F0; the others expand
// both.
template <class T>
__forceinline void ListSetSize(CExoArrayList<T>* pList, int n) { pList->SetSize(n); }
__forceinline void ListSetSize(CExoArrayList<MaxFace_PostProcess>* pList, int n) { pList->SetSizeCall(n); }
template <class T>
__forceinline void ListAdd(CExoArrayList<T>* pList, T value) { pList->AddInline(value); }
__forceinline void ListAdd(CExoArrayList<MaxFace_PostProcess>* pList, MaxFace_PostProcess value)
{
    pList->AddCall(value);
}

template <class T>
void Parse(char* s, CExoArrayList<T>* pList)
{
    int n = 0;
    char c;
    int nRead = sscanf(s, "%d %c", &n, &c);
    if (nRead > 0) {
        ListSetSize(pList, n);
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
            if (!_strnicmp(SkipWhitespaceInline(szLine), "endlist", 7))
                break;
            if (pList->num == 0)
                pList->SetSize(1);
            else if (pList->num == n)
                ListAdd(pList, pList->element[n - 1]);
            ParseValue(szLine, &pList->element[n]);
            n++;
        }
        SkipWhitespaceInline(SkipWhitespaceInline(szLine) + 7);
    }
}

template void Parse(char* s, CExoArrayList<MaxFace_PostProcess>* pList);
template void Parse(char* s, CExoArrayList<unsigned int>* pList);
template void Parse(char* s, CExoArrayList<SkinVertexWeight>* pList);
template void Parse(char* s, CExoArrayList<Quaternion>* pList);

// FUNCTION: SWKOTOR 0x00468A00 SYMBOL: ?Parse@@YAXPADPAV?$CExoArrayList@UMaxFace_PostProcess@@@@@Z
// FUNCTION: SWKOTOR 0x00468B90 SYMBOL: ?Parse@@YAXPADPAV?$CExoArrayList@I@@@Z
// NONMATCH (27 insns): the original keeps s in ebx from the entry (as in
// ParseList<AurGenericKey>, parse.cpp); we reload it in the binary branch.
// FUNCTION: SWKOTOR 0x00468D70 SYMBOL: ?Parse@@YAXPADPAV?$CExoArrayList@USkinVertexWeight@@@@@Z
// FUNCTION: SWKOTOR 0x00468F60 SYMBOL: ?Parse@@YAXPADPAV?$CExoArrayList@VQuaternion@@@@@Z
