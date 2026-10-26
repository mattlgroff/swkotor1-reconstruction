// CGuiInGame::AppendToDialogBuffer(const CExoString&, unsigned long)
// (guiingame.o): resolves the speaker's name and forwards to the
// (text, name) overload. Standalone layout.
class CExoString {
public:
    char* m_sString; unsigned long m_nLength;
    CExoString(); // 0x005B3190
    ~CExoString(); // 0x005E5C20
    CExoString& operator=(const CExoString& s); // 0x005E5C50
    int operator!=(const char* s) const; // 0x005E5390
};
class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual CExoString GetName(); // +0x70
};
class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject(); // +0x0C
};
class CClientExoApp { public: CGameObject* GetGameObject(unsigned long oid); /* 0x005ED580 */ };
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager; // 0x007A39FC
class CGuiInGame {
public:
    void AppendToDialogBuffer(const CExoString& sText,const CExoString& sName); // 0x0062B680
    void AppendToDialogBuffer(const CExoString& sText,unsigned long oidSpeaker);
};

// FUNCTION: SWKOTOR 0x00630560
void CGuiInGame::AppendToDialogBuffer(const CExoString& sText,unsigned long oidSpeaker)
{
    if (sText != "") {
        CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidSpeaker);
        if (pObject) {
            CExoString sName;
            sName = pObject->AsSWCObject()->GetName();
            AppendToDialogBuffer(sText,sName);
        }
    }
}
