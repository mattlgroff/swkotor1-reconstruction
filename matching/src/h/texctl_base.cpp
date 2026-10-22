// TextureController constructor and ParseFloatList (own TU: the
// subclass ctors call the base ctor out of line; SkipWhitespace stays a call).

#include "h/texctl.h"

// NONMATCH (16 insns): the original keeps an SEH frame with no state stores
// (the CExoArrayList members); ours drops the frame (same open problem as
// worker e LightningEmitter / PartTrigger ctors).
// FUNCTION: SWKOTOR 0x00462A70
TextureController::TextureController(AurTexture* pTexture)
{
    m_pDistortData = 0;
    m_nFrame = 0;
    m_bDistort = 0;
    m_pTexture = pTexture;
    m_nDistortAngle = 1;
    m_fDistortionAmplitude = 5.0f;
    m_fSpeed = 1.0f;
}

char* AurResGetNextLine();              // 0x0044BFA0
void* AurResGetData(int nSize);         // 0x0044BFD0
extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int);

// FUNCTION: SWKOTOR 0x004628C0
void ParseFloatList(char* s, CExoArrayList<float>* pList)
{
    int n = 0;
    char c;
    int nRead = sscanf(s, "%d %c", &n, &c);
    if (nRead > 0) {
        pList->SetSizeOutOfLine(n);
        if (nRead == 1) {
            for (int i = 0; i < pList->num; i++) {
                char* szLine = AurResGetNextLine();
                if (!szLine)
                    return;
                ParseFloat(szLine, &pList->element[i]);
            }
        } else {
            memcpy(pList->element, AurResGetData(n * 4), n * 4);
            *s = 0;
        }
    } else {
        char* szLine;
        for (szLine = AurResGetNextLine(); szLine; szLine = AurResGetNextLine()) {
            if (!_strnicmp(SkipWhitespace(szLine), "endlist", 7))
                break;
            if (pList->num == 0)
                pList->SetSizeOutOfLine(1);
            else if (pList->num == n)
                pList->AddOutOfLine(pList->element[n - 1]);
            ParseFloat(szLine, &pList->element[n]);
            n++;
        }
        SkipWhitespace(SkipWhitespace(szLine) + 7);
    }
}
