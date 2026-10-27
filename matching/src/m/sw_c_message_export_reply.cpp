// CSWCMessage::HandleServerToPlayerExportReply (nwchandlemessage.o): writes
// an exported character to the local vault and reports the result.

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

struct STR_RES {
    CExoString m_sText;                             // 0x00
    CResRef m_cSound;                               // 0x08
    float m_fSoundLength;                           // 0x18
};

void* operator new(unsigned int nSize);
void operator delete(void* p);

class CExoFile {
public:
    CExoFile(const CExoString& sFile, int nType, const CExoString& sMode);   // 0x005E68D0
    ~CExoFile();                                    // 0x005E69E0
    unsigned long Write(const void* pBuffer, unsigned long nSize, unsigned long nCount);   // 0x005E69A0
    void* m_pInternal;
};

class CSWCCreature {
public:
    CExoString GetSaveCreatureFilename(const CExoString& sFirst, const CExoString& sLast);  // 0x0060C540
};

class CSWCCMessageData;

class CClientExoAppInternal {
public:
    char pad_0x0[0x260];
    int m_bExportPending;                           // 0x260 (descriptive)
};

class CClientExoApp {
public:
    void AddFeedbackMessage(int nType, CSWCCMessageData* pData, const CExoString& sMessage);   // 0x005ED360
    CSWCCreature* GetPlayerCreature();              // 0x005ED540

    void* m_p00;
    CClientExoAppInternal* m_pInternal;             // 0x04
    CClientExoAppInternal* GetInternal() { return m_pInternal; }
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCMessage {
public:
    unsigned char* ReadVOIDPtr(unsigned long nSize);    // 0x004D6200
    int MessageReadOverflow(int bError);            // 0x004D6230
    int MessageReadUnderflow(int bError);           // 0x004D6240
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    CExoString ReadCExoLocStringClient(CResRef& cRes, unsigned long* pStrRef);  // 0x00692660
    int HandleServerToPlayerExportReply();
};

// FUNCTION: SWKOTOR 0x006634A0
int CSWCMessage::HandleServerToPlayerExportReply()
{
    CExoFile* pFile = 0;
    CExoString sFile;
    STR_RES cStrRes;
    CExoString sUnused;
    CExoString sFirstName;
    CExoString sLastName;
    sFirstName = ReadCExoLocStringClient(CResRef(), 0);
    sLastName = ReadCExoLocStringClient(CResRef(), 0);
    unsigned long nSize = ReadDWORD(32);
    unsigned char* pData = ReadVOIDPtr(nSize);
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        sFile = "LOCALVAULT:";
        sFile = sFile + g_pAppManager->GetClientExoApp()->GetPlayerCreature()->GetSaveCreatureFilename(sFirstName,
                                                                                                    sLastName);
        pFile = new CExoFile(sFile, 0x7df, "wb");
    }
    if (pFile) {
        pFile->Write(pData, nSize, 1);
        g_pAppManager->GetClientExoApp()->AddFeedbackMessage(0xd8, 0, "");
    }
    else {
        g_pAppManager->GetClientExoApp()->AddFeedbackMessage(0xd9, 0, "");
    }
    delete pFile;
    if (g_pAppManager->GetClientExoApp()->GetInternal()->m_bExportPending == 1) {
        g_pAppManager->GetClientExoApp()->GetInternal()->m_bExportPending = 0;
    }
    return 1;
}
