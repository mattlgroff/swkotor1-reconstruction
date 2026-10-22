// model text-field parse helpers (0x00463510 .. 0x00463940).

#define H_ALLOCATE_OUT_OF_LINE
#include "h/texctl.h"

extern "C" int __cdecl sprintf(char*, const char*, ...);

char* AurResGetNextLine();              // 0x0044BFA0
void* AurResGetData(int nSize);         // 0x0044BFD0
extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int);
void ParseGenericKey(char* s, AurGenericKey* pKey);     // 0x00463B30

// Face record of the ASCII model reader (0x14 bytes).
struct MaxFace_PostProcess {
    int* pVerts;            // +0x00 int[3]
    int* pTVerts;           // +0x04 int[12]
    int nSmoothGroup;       // +0x08
    int nMaterial;          // +0x0c
    int* pNormals;          // +0x10 int[3]
};

struct SkinVertexWeight {
    char* szBones[4];       // +0x00
    float fWeights[4];      // +0x10
    int nBone0;             // +0x20
    int nBone1;             // +0x24
    char pad_0x28[8];
    int nNumBones;          // +0x30
};


// FUNCTION: SWKOTOR 0x00463510
char* SkipWhitespace(char* s)
{
    if (!s)
        return s;
    while (*s == ' ' || *s == '\t')
        s++;
    return s;
}

static char g_szWord[256];

// FUNCTION: SWKOTOR 0x00463530
char* GetWord(char* s)
{
    int i = 0;
    while (s && *s && *s != ' ' && *s != '\t' && *s != '\n' && *s != '\r') {
        g_szWord[i] = *s;
        i++;
        s++;
    }
    g_szWord[i] = 0;
    return g_szWord;
}

// FUNCTION: SWKOTOR 0x00463570
void ParseVector(char* s, float* pOut)
{
    sscanf(s, "%f %f %f", pOut, pOut + 1, pOut + 2);
}

// FUNCTION: SWKOTOR 0x00463590
void ParseString(char* s, char* pOut)
{
    int n = sscanf(s, "%s", pOut);
    if (n == 0)
        strcpy(pOut, (char*)n);
}

// FUNCTION: SWKOTOR 0x004635C0
void ParseFloat(char* s, float* pOut)
{
    sscanf(s, "%f", pOut);
}

// FUNCTION: SWKOTOR 0x004635E0
void ParseInt(char* s, int* pOut)
{
    if (sscanf(s, "%d", pOut) == 0) {
        char szBuf[64];
        szBuf[0] = 0;
        sscanf(s, "%s", szBuf);
        if (!_stricmp("TRUE", szBuf))
            *pOut = 1;
        if (!_stricmp("FALSE", szBuf))
            *pOut = 0;
    }
}

// FUNCTION: SWKOTOR 0x00463650
void ParseShort(char* s, short* pOut)
{
    int n;
    sscanf(s, "%d", &n);
    *pOut = (short)n;
}

// FUNCTION: SWKOTOR 0x00463680
void ParseBool(char* s, unsigned char* pOut)
{
    char szBuf[8];
    szBuf[0] = 0;
    sscanf(s, "%s", szBuf);
    if (!_stricmp("true", szBuf))
        *pOut = 1;
    if (!_stricmp("false", szBuf))
        *pOut = 0;
    if (!_stricmp("1", szBuf))
        *pOut = 1;
    if (!_stricmp("0", szBuf))
        *pOut = 0;
}

// FUNCTION: SWKOTOR 0x00463710
void ParseMaxFace(char* s, MaxFace_PostProcess* pFace)
{
    if (pFace->pVerts)
        delete[] pFace->pVerts;
    if (pFace->pNormals)
        delete[] pFace->pNormals;
    if (pFace->pTVerts)
        delete[] pFace->pTVerts;
    pFace->pTVerts = new int[12];
    pFace->pVerts = new int[3];
    pFace->pNormals = new int[3];
    sscanf(s, "%d %d %d %d %d %d %d %d",
           &pFace->pVerts[0], &pFace->pVerts[1], &pFace->pVerts[2], &pFace->nSmoothGroup,
           &pFace->pTVerts[0], &pFace->pTVerts[1], &pFace->pTVerts[2], &pFace->nMaterial);
}

// FUNCTION: SWKOTOR 0x004637A0
void ParseSkinVertexWeight(char* s, SkinVertexWeight* pWeight)
{
    pWeight->szBones[0] = new char[32];
    pWeight->szBones[1] = new char[32];
    pWeight->szBones[2] = new char[32];
    pWeight->szBones[3] = new char[32];
    sprintf(pWeight->szBones[1], "ROOT");
    pWeight->fWeights[1] = 0;
    sprintf(pWeight->szBones[2], "ROOT");
    pWeight->fWeights[2] = 0;
    sprintf(pWeight->szBones[3], "ROOT");
    pWeight->fWeights[3] = 0;
    int n = sscanf(s, "%s %f %s %f %s %f %s %f",
                   pWeight->szBones[0], &pWeight->fWeights[0],
                   pWeight->szBones[1], &pWeight->fWeights[1],
                   pWeight->szBones[2], &pWeight->fWeights[2],
                   pWeight->szBones[3], &pWeight->fWeights[3]);
    pWeight->nBone1 = -1;
    pWeight->nBone0 = -1;
    pWeight->nNumBones = n / 2;
}

// FUNCTION: SWKOTOR 0x00463850
void ParseUIntList(char* s, unsigned int* pOut)
{
    int n = 0;
    sscanf(s, "%d", &n);
    for (int i = 0; i < n; i++) {
        char* szLine = AurResGetNextLine();
        sscanf(szLine, "%d", &pOut[i]);
    }
}

// FUNCTION: SWKOTOR 0x004638B0 SYMBOL: ?DelIndex@?$CExoArrayList@UAurGenericKey@@@@QAEXH@Z
template <> void CExoArrayList<AurGenericKey>::DelIndex(int i)
{
    num--;
    for (; i < num; i++)
        element[i] = element[i + 1];
}

// FUNCTION: SWKOTOR 0x004638F0
void ParseKeyString(char* s, AurKeyString* pKey)
{
    int n;
    sscanf(s, "%f%n", &pKey->fTime, &n);
    ParseString(SkipWhitespace(s + n), pKey->szValue);
}

inline void ParseValue(char* s, AurGenericKey* p) { ParseGenericKey(s, p); }
inline void ParseValue(char* s, int* p) { ParseInt(s, p); }

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
            if (!_strnicmp(SkipWhitespace(szLine), "endlist", 7))
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

template void ParseList(char* s, CExoArrayList<AurGenericKey>* pList);
template void ParseList(char* s, CExoArrayList<int>* pList);

// NONMATCH (27 insns): register allocation only (the original keeps s in ebx,
// the AurGenericKey list in edi / ebp); the int instance below matches.
// FUNCTION: SWKOTOR 0x00464440 SYMBOL: ?ParseList@@YAXPADPAV?$CExoArrayList@UAurGenericKey@@@@@Z
// FUNCTION: SWKOTOR 0x00464630 SYMBOL: ?ParseList@@YAXPADPAV?$CExoArrayList@H@@@Z

// FUNCTION: SWKOTOR 0x00464A40
void ParseUIntPtrList(char* s, CExoArrayList<unsigned int*>* pList)
{
    int nCount;
    int n = 0;
    sscanf(s, "%d", &n);
    pList->SetSizeOutOfLine(n);
    for (int i = 0; i < pList->num; i++) {
        char* szLine = AurResGetNextLine();
        sscanf(szLine, "%d", &nCount);
        pList->element[i] = new unsigned int[nCount];
        ParseUIntList(szLine, pList->element[i]);
    }
}

// FUNCTION: SWKOTOR 0x00464880
void ParseStringList(char* s, CExoArrayList<char*>* pList)
{
    int n = 0;
    if (sscanf(s, "%d", &n) > 0) {
        pList->SetSize(n);
        for (int i = 0; i < pList->num; i++) {
            pList->element[i] = new char[128];
            ParseString(AurResGetNextLine(), pList->element[i]);
        }
    } else {
        char* szLine;
        for (szLine = AurResGetNextLine(); szLine; szLine = AurResGetNextLine()) {
            if (!_strnicmp(SkipWhitespace(szLine), "endlist", 7))
                break;
            if (pList->num == 0)
                pList->SetSize(1);
            else if (pList->num == n)
                pList->Add(pList->element[n - 1]);
            pList->element[n] = new char[128];
            ParseString(szLine, pList->element[n]);
            n++;
        }
        SkipWhitespace(SkipWhitespace(szLine) + 7);
    }
}
