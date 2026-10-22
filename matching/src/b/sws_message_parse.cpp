// CSWSMessage chat command parsing helpers (0x00528090-0x005281B0).
#include "b/sws_message.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x00528090
int CSWSMessage::ParseGetString(char* pText, unsigned long nLength, CExoString* pResult,
                                unsigned long nMaxLength)
{
    if (*pText == ' ') {
        if (!nMaxLength || (nLength > 1 && nLength - 1 <= nMaxLength)) {
            *pResult = CExoString(pText + 1, nLength - 1);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00528130
int CSWSMessage::ParseToken(char* szToken, char** ppText, unsigned long* pnLength, int bSeparator)
{
    unsigned long nTokenLength = strlen(szToken);
    if (nTokenLength <= *pnLength && !strncmp(*ppText, szToken, nTokenLength)) {
        if (bSeparator) {
            if ((*ppText)[nTokenLength] == '.') {
                *ppText += nTokenLength + 1;
                *pnLength -= nTokenLength + 1;
                return 1;
            }
        }
        else {
            *ppText += nTokenLength;
            *pnLength -= nTokenLength;
            return 1;
        }
    }
    return 0;
}
