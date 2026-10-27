// CSWCMessage::ReadCExoLocStringClient (nwcmessage.o).

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    int m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef operator=(const CResRef& r);            // 0x00406120
    CResRef operator=(const char* s);               // 0x00406290
    char m_resRef[16];
};

// Talk table entry; the sound length is copied out as a raw dword.
struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    unsigned long nSoundLength;                     // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender);  // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CSWCMessage {
public:
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    CExoString ReadCExoString(int nBits);           // 0x004D67E0
    CExoString ReadCExoLocStringClient(CResRef& cSound, unsigned long* pLength);
};

// FUNCTION: SWKOTOR 0x00692660
// NONMATCH: constant-register choice only (the original caches 0 in ebx,
// cl here caches 1); control flow, EH states and calls agree.
CExoString CSWCMessage::ReadCExoLocStringClient(CResRef& cSound, unsigned long* pLength)
{
    CExoString sUnused;
    cSound = "";
    if (pLength) {
        *pLength = 0;
    }
    if (ReadBOOL()) {
        STR_RES strRes;
        unsigned char nGender = ReadBYTE(8);
        unsigned long nStrRef = ReadDWORD(32);
        if (!g_pTlkTable->Fetch(nStrRef, strRes, nGender)) {
            return CExoString("");
        }
        cSound = strRes.cSoundResRef;
        if (pLength) {
            *pLength = strRes.nSoundLength;
        }
        return strRes.sString;
    }
    return ReadCExoString(32);
}
