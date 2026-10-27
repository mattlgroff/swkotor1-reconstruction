// The debug command line shown with the frame info (g_cFrameInfoLine.m_szLine): cursor
// and history keys (0x00401870) and typed characters (0x00401f00). Names
// for the cursor, history and completion cells are descriptive.

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        if (element) {
            delete[] element;
        }
    }
    T& operator[](int i) { return element[i]; }
};

extern "C" unsigned int __cdecl strlen(const char* s);
extern "C" char* __cdecl strncpy(char* pDest, const char* pSource, unsigned int g_nCommandHistoryCount);
#pragma intrinsic(strlen)

// 0x007a3a40: the command line state (RunFrame reads m_bActive / m_szLine
// as g_bShowFrameInfo / g_szFrameInfo).
struct CFrameInfoLine {
    int m_bActive;                      // 0x00 (0x007a3a40)
    int m_nCursor;                      // 0x04
    int m_bExecute;                     // 0x08
    char m_szLine[0x200];               // 0x0c
};

extern CFrameInfoLine g_cFrameInfoLine; // 0x007a3a40
extern int g_nCommandHistoryIndex;      // 0x007b8fbc
extern char** g_ppCommandHistory;       // 0x007b8fcc
extern int g_nCommandHistoryCount;      // 0x007b8fd0
extern int g_bFrameInfoTextInput;       // 0x007a3cb4
extern int g_nCompletionOffset;         // 0x007a3cf0

class CExoInput {
public:
    void StopTextInput();               // 0x005df530
};

extern CExoInput* g_pExoInput;          // 0x007a39e4

char* __cdecl ConsoleComplete(char* sLine, CExoArrayList<char*>* pMatches); // 0x0044c040
char* __cdecl ConsoleExecute(char* sLine);                                  // 0x0044bc70
void __cdecl AurPostString(const char* s, int x, int y, float fLife);       // 0x0044d490
extern "C" char* __cdecl strcpy(char* pDest, const char* pSource);
#pragma intrinsic(strcpy)

// FUNCTION: SWKOTOR 0x00401870
void __cdecl HandleFrameInfoKey(unsigned short nKey)
{
    if (nKey == 0x25) {
        if (g_cFrameInfoLine.m_bActive) {
            int nCursor = g_cFrameInfoLine.m_nCursor - 1;
            g_cFrameInfoLine.m_nCursor = nCursor <= 0 ? 0 : nCursor;
        }
    } else if (nKey == 0x27) {
        if (g_cFrameInfoLine.m_bActive) {
            g_cFrameInfoLine.m_nCursor++;
            if ((unsigned int)g_cFrameInfoLine.m_nCursor >= strlen(g_cFrameInfoLine.m_szLine)) {
                g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
            }
        }
    }
    if (!g_nCommandHistoryCount) {
        return;
    }
    if (nKey == 0x26) {
        if (g_cFrameInfoLine.m_bActive) {
            int nIndex = g_nCommandHistoryIndex - 1;
            g_nCommandHistoryIndex = nIndex < 0 ? 0 : nIndex;
            strncpy(g_cFrameInfoLine.m_szLine, g_ppCommandHistory[g_nCommandHistoryIndex], 0x50);
            g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
        }
    } else if (nKey == 0x27) {
        if (g_cFrameInfoLine.m_bActive && strlen(g_cFrameInfoLine.m_szLine) == 0) {
            if (g_nCommandHistoryIndex == g_nCommandHistoryCount) {
                g_nCommandHistoryIndex--;
            }
            strncpy(g_cFrameInfoLine.m_szLine, g_ppCommandHistory[g_nCommandHistoryIndex], 0x50);
            g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
        }
    } else if (nKey == 0x28) {
        if (g_cFrameInfoLine.m_bActive) {
            int nIndex = g_nCommandHistoryIndex + 1;
            if (g_nCommandHistoryCount < nIndex) {
                g_nCommandHistoryIndex = g_nCommandHistoryCount;
            } else {
                g_nCommandHistoryIndex = nIndex;
            }
            if (g_nCommandHistoryIndex == g_nCommandHistoryCount) {
                g_cFrameInfoLine.m_szLine[0] = 0;
                g_cFrameInfoLine.m_nCursor = 0;
            } else {
                strncpy(g_cFrameInfoLine.m_szLine, g_ppCommandHistory[g_nCommandHistoryIndex], 0x50);
                g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00401f00
void __cdecl HandleFrameInfoChar(char cChar)
{
    CExoArrayList<char*> lstMatches;
    if (cChar == 0x1b) {
        if (g_bFrameInfoTextInput) {
            g_pExoInput->StopTextInput();
        }
        g_cFrameInfoLine.m_bActive = 0;
        g_cFrameInfoLine.m_szLine[0] = 0;
        g_cFrameInfoLine.m_nCursor = 0;
        return;
    }
    if (cChar == '\n' || cChar == '\r') {
        if (g_bFrameInfoTextInput) {
            g_pExoInput->StopTextInput();
        }
        g_cFrameInfoLine.m_bActive = 0;
        AurPostString("* ", 0, 0, 5.0f);
        AurPostString(g_cFrameInfoLine.m_szLine, 2, 0, 5.0f);
        g_cFrameInfoLine.m_bExecute = 1;
        return;
    }
    if (cChar == 8) {
        unsigned int nLength = strlen(g_cFrameInfoLine.m_szLine);
        if (nLength == 0 || g_cFrameInfoLine.m_nCursor == 0) {
            return;
        }
        if ((unsigned int)g_cFrameInfoLine.m_nCursor >= strlen(g_cFrameInfoLine.m_szLine)) {
            g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
        }
        int nEnd = strlen(g_cFrameInfoLine.m_szLine) > 500 ? 500 : strlen(g_cFrameInfoLine.m_szLine);
        g_cFrameInfoLine.m_nCursor--;
        for (int i = g_cFrameInfoLine.m_nCursor; i < nEnd; i++) {
            g_cFrameInfoLine.m_szLine[i] = g_cFrameInfoLine.m_szLine[i + 1];
        }
        return;
    }
    if (cChar == 9) {
        strcpy(g_cFrameInfoLine.m_szLine, ConsoleComplete(g_cFrameInfoLine.m_szLine, &lstMatches));
        char* sResult = ConsoleExecute(g_cFrameInfoLine.m_szLine);
        AurPostString(sResult, 0, 1, 1.0f);
        if (lstMatches.num > 1) {
            float fLife = 4.0f;
            if (lstMatches.num <= 15) {
                fLife = 1.0f;
                if (lstMatches.num < 15) {
                    g_nCompletionOffset = 0;
                }
            }
            for (int i = 0; i < lstMatches.num && i < 15; i++) {
                AurPostString(lstMatches[(g_nCompletionOffset + i) % lstMatches.num], 4, i + 6, fLife);
            }
            g_nCompletionOffset += 5;
        }
        g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
        return;
    }
    int nEnd = strlen(g_cFrameInfoLine.m_szLine) > 500 ? 500 : strlen(g_cFrameInfoLine.m_szLine);
    if ((unsigned int)g_cFrameInfoLine.m_nCursor >= strlen(g_cFrameInfoLine.m_szLine)) {
        g_cFrameInfoLine.m_nCursor = strlen(g_cFrameInfoLine.m_szLine);
    }
    for (int i = nEnd; i > g_cFrameInfoLine.m_nCursor; i--) {
        g_cFrameInfoLine.m_szLine[i] = g_cFrameInfoLine.m_szLine[i - 1];
    }
    g_cFrameInfoLine.m_szLine[g_cFrameInfoLine.m_nCursor] = cChar;
    g_cFrameInfoLine.m_nCursor++;
    g_cFrameInfoLine.m_szLine[nEnd + 1] = 0;
}
