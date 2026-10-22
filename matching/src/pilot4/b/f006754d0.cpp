// Pilot 4 arm B: 0x006754D0, split "a.b c" into three strings.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    CExoString(const char *s, int nLength);     // 0x005E5B70
    CExoString(const CExoString &s);
    ~CExoString();                              // 0x005E5C20
    CExoString &operator=(const CExoString &s); // 0x005E5C50
    CExoString &operator=(const char *s);       // 0x005E5140
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CScriptParser {
public:
    int SplitName(unsigned char *pBuffer, unsigned long nLength, CExoString &sFirst,
                  CExoString &sSecond, CExoString &sThird);
};

// FUNCTION: SWKOTOR 0x006754D0
int CScriptParser::SplitName(unsigned char *pBuffer, unsigned long nLength,
                             CExoString &sFirst, CExoString &sSecond, CExoString &sThird)
{
    unsigned long i;
    int ch;
    for (i = 0; i < nLength; i++) {
        ch = pBuffer[i];
        if (ch == '.') {
            break;
        }
        if (ch == ' ') {
            sFirst = "";
            sSecond = "";
            sThird = "";
            return 0;
        }
    }
    sFirst = CExoString((char *)pBuffer, i);
    if (ch == '.') {
        i++;
        unsigned long nStart = i;
        for (; i < nLength; i++) {
            ch = pBuffer[i];
            if (ch == ' ') {
                break;
            }
            if (ch == '.') {
                sSecond = "";
                sThird = "";
                return 0;
            }
        }
        sSecond = CExoString((char *)pBuffer + nStart, i - nStart);
        if (ch == ' ') {
            i++;
            if (i >= nLength) {
                sThird = "";
                return 0;
            }
            sThird = CExoString((char *)pBuffer + i, nLength - i);
            return 3;
        }
        if (i > nStart) {
            sThird = "";
            return 2;
        }
        sThird = "";
        return 0;
    }
    if (i > 0) {
        sSecond = "";
        sThird = "";
        return 1;
    }
    sSecond = "";
    sThird = "";
    return 0;
}
