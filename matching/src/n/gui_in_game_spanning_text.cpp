// CGuiInGame::DialogNodeSpanningTextParse (guiingame.o): splits a dialog
// node text at "##" markers into up to eight pages (+0xB58), returning the
// pages one by one on later calls. Standalone layout.
#include <string.h>
class CExoString {
public:
    char* m_sString; unsigned long m_nLength;
    CExoString(const char* s); // 0x005E5A90
    CExoString(const CExoString& s); // 0x005E5B00
    ~CExoString(); // 0x005E5C20
    CExoString& operator=(const CExoString& s); // 0x005E5C50
    char* CStr() const; // 0x005E5670
    int GetLength() const; // 0x005E5790
    int Find(const CExoString& s,int nPosition) const; // 0x005E5EB0
};
class CGuiInGame {
public:
    char pad_0x0[0xb50];
    int m_nSpanningPending; // +0xB50
    int m_nSpanningCount; // +0xB54
    CExoString m_asSpanning[8]; // +0xB58
    CExoString DialogNodeSpanningTextParse(const CExoString& sText);
};

// FUNCTION: SWKOTOR 0x0062ADD0
CExoString CGuiInGame::DialogNodeSpanningTextParse(const CExoString& sText)
{
    if (m_nSpanningPending > 0)
        return m_asSpanning[m_nSpanningCount - m_nSpanningPending--];
    bool bFound;
    {
        CExoString sMarker("##");
        bFound = sText.Find(sMarker,0) != -1;
    }
    if (bFound) {
        m_nSpanningCount = 0;
        int bOverflow = 0;
        char* p = new char[sText.GetLength() + 1];
        strcpy(p,sText.CStr());
        p[sText.GetLength()] = 0;
        char* pBuffer = p;
        int i = 0;
        while (p[i]) {
            if ((p[i] == '#' && p[i + 1] == '#') || p[i + 1] == 0) {
                if (i == 0 && p[1] != 0) {
                    bOverflow = 1;
                    break;
                }
                int nLength = p[i + 1] == 0 ? i + 1 : i - 1;
                char* pPiece = new char[nLength + 1];
                strncpy(pPiece,p,nLength);
                pPiece[nLength] = 0;
                CExoString sPiece(pPiece);
                if (pPiece) delete[] pPiece;
                if (m_nSpanningCount == 8) {
                    bOverflow = 1;
                    break;
                }
                m_asSpanning[m_nSpanningCount++] = sPiece;
                if (p[i + 1] != 0) {
                    if (p[i + 2] == 0) break;
                    p = p + i + 3;
                    i = 0;
                } else {
                    i++;
                }
            } else {
                i++;
            }
        }
        delete[] pBuffer;
        if (!bOverflow) {
            // Same pop as the pending path above, starting from the full count.
            m_nSpanningPending = m_nSpanningCount;
            return m_asSpanning[m_nSpanningCount - m_nSpanningPending--];
        }
    }
    return sText;
}
