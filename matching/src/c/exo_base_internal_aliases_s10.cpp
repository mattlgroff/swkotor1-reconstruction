// CExoBaseInternal::LoadAliases (exobaseinternal.o, Mac name), worker C s10:
// registers the default directory aliases, each overridable from the ini
// file's [Alias] section.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoBaseInternal {
public:
    int m_nResTypes;                                // 0x00
    unsigned short* m_pResTypes;                    // 0x04
    CExoString* m_pResTypeExtensions;               // 0x08

    void AddAlias(const CExoString& sIniFile, const CExoString& sAlias, const CExoString& sDefault,
                  const CExoString& sDescription);  // 0x005E7760
    void LoadAliases(const CExoString& sIniFile);
};

// FUNCTION: SWKOTOR 0x005E7A90
void CExoBaseInternal::LoadAliases(const CExoString& sIniFile)
{
    CExoString sIniPath;
    sIniPath.Format(".\\%s", sIniFile.CStr());
    AddAlias(sIniPath, "HD0", ".\\", "d:\\");
    AddAlias(sIniPath, "CD0", "d:\\", "d:\\");
    AddAlias(sIniPath, "OVERRIDE", ".\\override", "d:\\override");
    AddAlias(sIniPath, "ERRORTEX", ".\\errortex", "d:\\errortex");
    AddAlias(sIniPath, "TEMP", ".\\temp", "d:\\temp");
    AddAlias(sIniPath, "MODULES", ".\\modules", "d:\\modules");
    AddAlias(sIniPath, "NWMFILES", ".\\nwm", "d:\\nwm");
    AddAlias(sIniPath, "LOGS", ".\\logs", "d:\\logs");
    AddAlias(sIniPath, "LOCALVAULT", ".\\localvault", "d:\\localvault");
    AddAlias(sIniPath, "DMVAULT", ".\\dmvault", "d:\\dmvault");
    AddAlias(sIniPath, "SERVERVAULT", ".\\servervault", "d:\\servervault");
    AddAlias(sIniPath, "SAVES", ".\\saves", "u:\\");
    AddAlias(sIniPath, "MUSIC", ".\\music", "d:\\music");
    AddAlias(sIniPath, "STREAMMUSIC", ".\\streammusic", "d:\\streammusic");
    AddAlias(sIniPath, "MOVIES", ".\\movies", "d:\\movies");
    AddAlias(sIniPath, "TEMPCLIENT", ".\\tempclient", "d:\\tempclient");
    AddAlias(sIniPath, "CURRENTGAME", ".\\currentgame", "d:\\currentgame");
    AddAlias(sIniPath, "HAK", ".\\hak", "d:\\hak");
    AddAlias(sIniPath, "TEXTUREPACKS", ".\\texturepacks", "d:\\texturepacks");
    AddAlias(sIniPath, "STREAMWAVES", ".\\streamwaves", "d:\\streamwaves");
    AddAlias(sIniPath, "DOWNLOADS", ".\\downloads", "t:\\");
    AddAlias(sIniPath, "OPTIONS", ".\\", "t:\\");
    AddAlias(sIniPath, "AMBIENT", ".\\ambient", "d:\\ambient");
    AddAlias(sIniPath, "PATCH", ".\\patch", "d:\\patch");
    AddAlias(sIniPath, "PORTRAITS", ".\\portraits", "d:\\portraits");
    AddAlias(sIniPath, "MUSIC", ".\\music", "d:\\music");
    AddAlias(sIniPath, "CURRENTGAME", ".\\currentgame", "d:\\currentgame");
    AddAlias(sIniPath, "GAMEINPROGRESS", ".\\gameinprogress", "z:\\gameinprogress");
    AddAlias(sIniPath, "FUTUREGAME", ".\\futuregame", "z:\\futuregame");
    AddAlias(sIniPath, "RIMS", ".\\rims", "d:\\rims");
    AddAlias(sIniPath, "RIMSXBOX", ".\\rimsxbox", "d:\\rimsxbox");
    AddAlias(sIniPath, "REBOOTDATA", ".\\rebootdata", "d:\\rebootdata");
    AddAlias(sIniPath, "LIPS", ".\\lips", "d:\\lips");
}
