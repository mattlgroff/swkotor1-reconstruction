// CExoImeUI, IMM32 composition / result / candidate handling.
// 0x005DF740-0x005DFCA8.
#include "j/exo_ime.h"

// FUNCTION: SWKOTOR 0x005DF740
void CExoImeUI::HandleCompositionString(HWND hWnd, unsigned long nFlags)
{
    HIMC hImc = ImmGetContext(hWnd);
    if (!hImc)
        return;
    unsigned long nLength = ImmGetCompositionStringA(hImc, 8, 0, 0);
    if (nLength <= 0) {
        ImmReleaseContext(hWnd, hImc);
        return;
    }
    char* pString = new char[nLength + 1];
    ImmGetCompositionStringA(hImc, 8, pString, nLength);
    pString[nLength] = 0;
    m_sComposition = pString;
    delete[] pString;
    if (nFlags & 0x10) {
        unsigned long nAttr = ImmGetCompositionStringA(hImc, 0x10, 0, 0);
        if (nAttr > 0) {
            char* pAttr = new char[nAttr + 1];
            ImmGetCompositionStringA(hImc, 0x10, pAttr, nAttr);
            pAttr[nAttr] = 0;
            delete[] pAttr;
        }
    }
    ImmReleaseContext(hWnd, hImc);
}

// FUNCTION: SWKOTOR 0x005DF800
void CExoImeUI::HandleResultString(HWND hWnd, int bClause)
{
    HIMC hImc = ImmGetContext(hWnd);
    if (!hImc)
        return;
    unsigned long nLength = ImmGetCompositionStringA(hImc, 0x800, 0, 0);
    if (nLength <= 0) {
        m_sResult = "";
        ImmReleaseContext(hWnd, hImc);
        return;
    }
    char* pString = new char[nLength + 1];
    ImmGetCompositionStringA(hImc, 0x800, pString, nLength);
    pString[nLength] = 0;
    m_sResult = pString;
    delete[] pString;
    if (bClause) {
        unsigned long nClause = ImmGetCompositionStringA(hImc, 0x1000, 0, 0);
        if (nClause > 0) {
            char* pClause = new char[nClause + 1];
            ImmGetCompositionStringA(hImc, 0x1000, pClause, nClause);
            pClause[nClause] = 0;
            {
                CExoString sClause(pClause);
                m_sResult = m_sResult + sClause;
            }
            delete[] pClause;
            m_sComposition = "";
            m_sCandidates = "";
        }
    }
    ImmReleaseContext(hWnd, hImc);
}

// FUNCTION: SWKOTOR 0x005DF980
void CExoImeUI::HandleImeStatusChange(HWND hWnd)
{
    HIMC hImc = ImmGetContext(hWnd);
    if (hImc) {
        if (!ImmGetOpenStatus(hImc)) {
            m_sCandidates = "";
            m_nFlags = 0;
        }
        ImmReleaseContext(hWnd, hImc);
    }
}

// FUNCTION: SWKOTOR 0x005DF9D0
CExoString CExoImeUI::GetCandidateString(tagCANDIDATELIST* pList)
{
    int nIndex = 1;
    CExoString sResult;
    unsigned long nPageSize = pList->dwPageSize;
    if (nPageSize == 0)
        nPageSize = 5;
    unsigned long nEnd = pList->dwPageStart + nPageSize;
    if (nEnd >= pList->dwCount)
        nEnd = pList->dwCount;
    for (unsigned long i = pList->dwPageStart; i < nEnd; i++) {
        char* pCandidate = (char*)pList + pList->dwOffset[i];
        char buf[52];
        buf[lstrlenA(pCandidate)] = 0;
        sprintf(buf, "%d:%s  ", nIndex, pCandidate);
        nIndex++;
        CExoString sItem(buf);
        sResult = sResult + sItem;
    }
    return sResult;
}

// FUNCTION: SWKOTOR 0x005DFB00
void CExoImeUI::HandleImeMessage(HWND hWnd, unsigned long wParam, unsigned long lParam)
{
    if (lParam & 0x800) {
        if (lParam & 0x1000)
            HandleResultString(hWnd, 1);
        else
            HandleResultString(hWnd, 0);
        return;
    }
    if (lParam & 8)
        HandleCompositionString(hWnd, lParam);
}

// FUNCTION: SWKOTOR 0x005DFB40
void CExoImeUI::HandleCandidateList(HWND hWnd, unsigned long nMask)
{
    m_nFlags &= ~2;
    m_sCandidates = "";
    HIMC hImc = ImmGetContext(hWnd);
    if (!hImc)
        return;
    unsigned long i;
    for (i = 0; i < 32; i++) {
        if (nMask & (1 << i))
            break;
    }
    if (i == 32) {
        ImmReleaseContext(hWnd, hImc);
        return;
    }
    nMask = ImmGetCandidateListA(hImc, i, 0, 0);
    if (nMask == 0) {
        ImmReleaseContext(hWnd, hImc);
        return;
    }
    tagCANDIDATELIST* pList = (tagCANDIDATELIST*)new char[nMask];
    ImmGetCandidateListA(hImc, i, pList, nMask);
    m_sCandidates = GetCandidateString(pList);
    delete[] (char*)pList;
    m_nFlags |= 2;
    ImmReleaseContext(hWnd, hImc);
}

// FUNCTION: SWKOTOR 0x005DFC40
int CExoImeUI::HandleNotification(HWND hWnd, unsigned long nCode, unsigned long lParam)
{
    switch (nCode) {
    case 3:
    case 5:
        HandleCandidateList(hWnd, lParam);
        return 1;
    case 4:
        m_sCandidates = "";
        m_nFlags &= ~2;
        return 1;
    case 8:
        HandleImeStatusChange(hWnd);
        return 1;
    }
    return 0;
}
