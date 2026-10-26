// CExoImeUI destructor (worker C s1). Layout from bucket j's exo_ime.h:
// three CExoString members at +0x110/+0x118/+0x120.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

class CExoImeUI {
public:
    ~CExoImeUI();

    unsigned long m_nFlags;                         // 0x00
    char pad_0x4[0x10c];
    CExoString m_sResult;                           // 0x110
    CExoString m_sComposition;                      // 0x118
    CExoString m_sCandidates;                       // 0x120
};

// FUNCTION: SWKOTOR 0x005F1020
CExoImeUI::~CExoImeUI()
{
}
