// Worker J matching headers: CExoImeUI (IMM32 composition / candidate UI).
#ifndef J_EXO_IME_H
#define J_EXO_IME_H

#include "j/exo_string.h"

typedef void* HWND;
typedef void* HIMC;

struct tagCANDIDATELIST {
    unsigned long dwSize;                           // 0x00
    unsigned long dwStyle;                          // 0x04
    unsigned long dwCount;                          // 0x08
    unsigned long dwSelection;                      // 0x0c
    unsigned long dwPageStart;                      // 0x10
    unsigned long dwPageSize;                       // 0x14
    unsigned long dwOffset[1];                      // 0x18
};

extern "C" {
// Called through the import-library thunks at 0x0070C144.. (plain call).
HIMC __stdcall ImmGetContext(HWND hWnd);
int __stdcall ImmReleaseContext(HWND hWnd, HIMC hImc);
long __stdcall ImmGetCompositionStringA(HIMC hImc, unsigned long dwIndex, void* lpBuf, unsigned long dwBufLen);
int __stdcall ImmGetOpenStatus(HIMC hImc);
unsigned long __stdcall ImmGetCandidateListA(HIMC hImc, unsigned long dwIndex, tagCANDIDATELIST* lpCandList, unsigned long dwBufLen);
__declspec(dllimport) int __stdcall lstrlenA(const char* s);     // IAT 0x0073D190
int sprintf(char* buf, const char* fmt, ...);
}

class CExoImeUI {
public:
    void HandleCompositionString(HWND hWnd, unsigned long nFlags);  // 0x005DF740
    void HandleResultString(HWND hWnd, int bClause);                // 0x005DF800
    void HandleImeStatusChange(HWND hWnd);                          // 0x005DF980
    CExoString GetCandidateString(tagCANDIDATELIST* pList);         // 0x005DF9D0
    void HandleImeMessage(HWND hWnd, unsigned long wParam, unsigned long lParam);  // 0x005DFB00
    void HandleCandidateList(HWND hWnd, unsigned long nMask);       // 0x005DFB40
    int HandleNotification(HWND hWnd, unsigned long nCode, unsigned long lParam);  // 0x005DFC40

    unsigned long m_nFlags;                         // 0x00 (bit 1: candidates shown)
    char pad_0x4[0x10c];
    CExoString m_sResult;                           // 0x110
    CExoString m_sComposition;                      // 0x118
    CExoString m_sCandidates;                       // 0x120
};

#endif
