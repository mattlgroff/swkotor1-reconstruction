// Worker L matching headers: the save/load screen (swguisaveload.o,
// 0x006C8200-0x006CC680). CSWGuiSaveLoad (layout "saveload", vtable
// 0x00757650, 0x1160 bytes), its list entries CSWGuiSaveLoadEntry (vtable
// 0x00757740, 0x248 bytes), the save-name dialog CSWGuiSaveNamePanel
// (layout "savename", vtable 0x007576D0, 0x690 bytes) and its edit box
// CSWGuiSaveGameEditBox (vtable 0x007575B0).
#ifndef L_SW_GUI_SAVE_LOAD_H
#define L_SW_GUI_SAVE_LOAD_H

#define L_SW_GUI_SAVE_LOAD_FULL
#include "l/sw_gui_upgrade.h"

#ifdef L_SAVE_LOAD_DTORS
#define L_SAVE_VTABLE __declspec(novtable)
#else
#define L_SAVE_VTABLE
#endif

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    CResGFF(unsigned short nType, char* szFileType, const CResRef& cResRef);   // 0x00410630
    virtual ~CResGFF();
    int GetTopLevelStruct(CResStruct* pStruct);     // 0x00411240
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess,
                             const CResRef& cDefault);  // 0x00411E10
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);    // 0x00411A60
    unsigned short ReadFieldWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned short nDefault);  // 0x00411B40
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);   // 0x00411C20
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault);  // 0x00411C90
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault); // 0x00411EC0
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel);  // 0x00411A10
    char pad_0x4[0x9c];
};

// Feedback state saved with a game ("STATUSSUMMARY").
class CStatusSummary {
public:
    void LoadFromGFF(CResGFF* pGFF, CResStruct& rStruct);   // 0x006C8490

    unsigned long m_bDisplaysPending : 1;           // 0x00 bit 0
    unsigned long m_bSoundPending : 1;              //      bit 1
    unsigned long m_bItemReceived : 1;              //      bit 2
    unsigned long m_bItemLost : 1;                  //      bit 3
    unsigned long m_bJournal : 1;                   //      bit 4
    unsigned long m_bLevelUpSound : 1;              //      bit 5
    unsigned long m_bNewQuestSound : 1;             //      bit 6
    unsigned long m_bCompleteSound : 1;             //      bit 7
    unsigned long m_bCreditsNet : 1;                //      bit 8
    unsigned char m_nLightShift;                    // 0x04
    unsigned char m_nDarkShift;                     // 0x05
    int m_nCredits;                                 // 0x08
    int m_nXP;                                      // 0x0c
    int m_nStealthXP;                               // 0x10
    int m_nSuppressed;                              // 0x14
};

// Module-start parameters of the PC autosave.
struct KOTOR_AUTOSAVE_PARAMS {
    void LoadFromGFF(CResGFF* pGFF, CResStruct& rStruct);   // 0x006C9DE0

    CExoString m_sLoadMusic;                        // 0x00
    CExoString m_sStartWaypoint;                    // 0x08
    CExoString m_sMovie1;                           // 0x10
    CExoString m_sMovie2;                           // 0x18
    CExoString m_sMovie3;                           // 0x20
    CExoString m_sMovie4;                           // 0x28
    CExoString m_sMovie5;                           // 0x30
    CExoString m_sMovie6;                           // 0x38
    unsigned long m_nYear;                          // 0x40
    unsigned char m_nMonth;                         // 0x44
    unsigned char m_nDay;                           // 0x45
    unsigned char m_nHour;                          // 0x46
    unsigned short m_nMinute;                       // 0x48
    unsigned short m_nSecond;                       // 0x4a
    unsigned short m_nMillisecond;                  // 0x4c
    unsigned long m_nPauseDay;                      // 0x50
    unsigned long m_nPauseTime;                     // 0x54
    CStatusSummary m_cStatusSummary;                // 0x58
};

class CERFFile {
public:
    CERFFile();                                     // 0x005DD9C0
    ~CERFFile();                                    // 0x005DE3A0
    int ReadHeader(CExoString& sFile);              // 0x005DCE50
    int Read(CExoString& sFile);                    // 0x005DD3C0
    int ExportFilesFromERF(const CExoString& sFile, const CExoString& sDirectory, int nMax,
                           int (*pfnProgress)(int, int, int, int), int nBase,
                           int nRange);             // 0x005DD710
    char pad_0x0[0xd0];
};

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sAlias, unsigned short nType) const;   // 0x005E68A0
    const CExoString& GetAliasPath(const CExoString& sAlias, int nIndex) const;        // 0x005E6890
};

class CExoBase {
public:
    int GetDirectoryList(CExoArrayList<CExoString>* pList, const CExoString& sAlias,
                         unsigned short nType, int bSorted, int bUnique);    // 0x005E6640
    char pad_0x0[0xc];
    CExoAliasList* m_pcExoAliasList;                // 0x0c
};
extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CExoFile {
public:
    CExoFile(const CExoString& sFile, unsigned short nType, const CExoString& sMode);  // 0x005E68D0
    ~CExoFile();                                    // 0x005E69E0
    int Write(const char* sText);                   // 0x005E6A40
    int FileOpened();                               // 0x005E6A10
    unsigned long GetSize();                        // 0x005E6950
    int Flush();                                    // 0x005E6A20
    void* m_pInternal;
};

extern "C" int rename(const char* sOld, const char* sNew);

class CSWGuiEditText : public CSWGuiText {
public:
    CSWGuiEditText();                               // 0x004173D0
    void RemoveLastChar();                          // 0x004162E0
    void AddNewChar(const char* sChar);             // 0x004163A0
    void SetText(const CExoString& sText);          // 0x00416490
    short m_nMaxLength;                             // 0x70
    unsigned long m_bFlag0 : 1;                     // 0x74
    CExoString m_sText;                             // 0x78
};

// vtable 0x0073E9A0 family; ctor 0x0041C6B0, dtor 0x0041C740 (0x160 bytes).
class CSWGuiEditbox : public CSWGuiControl {
public:
    CSWGuiEditbox();                                // 0x0041C6B0
    ~CSWGuiEditbox();                               // 0x0041C740
    virtual void v98();
    virtual void HandleChar(int nChar);             // 0x9c
    void SetFocus();                                // 0x00418660
    char pad_0x5c[0x10];
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiEditText m_cText;                         // 0xe0
};

// vtable 0x007575B0; the out-of-line destructor is 0x006C9DD0.
class L_SAVE_VTABLE CSWGuiSaveGameEditBox : public CSWGuiEditbox {
public:
#ifdef L_SAVE_LOAD_DTORS
    virtual ~CSWGuiSaveGameEditBox() {}
#else
    CSWGuiSaveGameEditBox() {}
    virtual ~CSWGuiSaveGameEditBox();               // 0x006C9DD0
#endif
    void HandleChar(int nChar);                     // 0x9c (0x006C82F0)
};

class CSWGuiSaveLoad;

// vtable 0x007576D0; layout "savename".
class L_SAVE_VTABLE CSWGuiSaveNamePanel : public CSWGuiPanel {
public:
    CSWGuiSaveNamePanel(CSWGuiManager* pManager);   // 0x006CAE70
    virtual ~CSWGuiSaveNamePanel();                 // 0x006CC060
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006CB040)
    virtual void OnPanelAdded();                    // 0x48 (0x006C8410)
    virtual void OnPanelRemoved();                  // 0x4c (0x006C8460)

    void HandleOkButton(CSWGuiControl* pControl);   // 0x006C9D50
    void HandleCancelButton(CSWGuiControl* pControl);   // 0x006C8470 (descriptive)

    CSWGuiSaveLoad* m_pSaveLoad;                    // 0x64
    CSWGuiButton m_btnOk;                           // 0x68
    CSWGuiButton m_btnCancel;                       // 0x22c
    CSWGuiSaveGameEditBox m_ebName;                 // 0x3f0
    CSWGuiLabel m_lblTitle;                         // 0x550
};

// vtable 0x00757740; one saved game in LB_GAMES.
class L_SAVE_VTABLE CSWGuiSaveLoadEntry : public CSWGuiRadioButton {
public:
    CSWGuiSaveLoadEntry();                          // 0x006CB940
    virtual ~CSWGuiSaveLoadEntry();                 // 0x006CBA90

    CResRef GetPlayerPortrait(int nIndex);          // 0x006C8200
    CExoString GetGameDirectory();                  // 0x006C8250
    void LoadData(const CExoString& sDirectory);    // 0x006C8E50
    void SetName(const CExoString& sName);          // 0x006C9780 (descriptive)

    union {
        unsigned long m_nEntryFlags;                // 0x1c4
        struct {
            unsigned long m_bSavedGame : 1;         //      bit 0 (existing save, descriptive)
            unsigned long m_bCorrupt : 1;           //      bit 1
            unsigned long m_bFlag2 : 1;
            unsigned long m_bRebootAutoSave : 1;    //      bit 3
            unsigned long m_bPCAutoSave : 1;        //      bit 4
            unsigned long m_bMissingContent : 1;    //      bit 5
            unsigned long m_bFlag6 : 1;
            unsigned long m_bCheatUsed : 1;         //      bit 7
        };
    };
    unsigned long m_nGameNumber;                    // 0x1c8
    unsigned long m_nTimePlayed;                    // 0x1cc (seconds)
    unsigned long m_nBlocksNeeded;                  // 0x1d0 (descriptive)
    unsigned char m_nGameplayHint;                  // 0x1d4
    unsigned char m_nStoryHint;                     // 0x1d5
    CExoString m_sSaveName;                         // 0x1d8
    CExoString m_sGameName;                         // 0x1e0
    CExoString m_sAreaName;                         // 0x1e8 ("planet - area")
    CExoString m_sLastModule;                       // 0x1f0
    CExoString m_s1f8;                              // 0x1f8
    CExoString m_sDisplayName;                      // 0x200
    CExoString m_sScreenShot;                       // 0x208
    CExoString m_sMissingContent;                   // 0x210 (descriptive)
    CResRef m_aPortraits[3];                        // 0x218
};

// vtable 0x00757650; layout "saveload".
class L_SAVE_VTABLE CSWGuiSaveLoad : public CSWGuiPanel {
public:
    CSWGuiSaveLoad(CSWGuiManager* pManager, int bSave, int bFromMainMenu);  // 0x006CC680
    virtual ~CSWGuiSaveLoad();                      // 0x006CA110
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006C86D0)

    void WriteGame(const CExoString& sName);        // 0x006C8790
    void DeleteGame(CSWGuiControl* pControl);       // 0x006C8900 (descriptive)
    void ShowGame(CSWGuiControl* pControl);         // 0x006C89D0
    void LoadPCAutoSave(const CExoString& sName);   // 0x006CA250
    void PromptDeleteGame(CSWGuiControl* pControl); // 0x006CAA90 (descriptive)
    void LoadGame(CSWGuiControl* pControl);         // 0x006CB0E0
    void PromptForSaveName(CSWGuiControl* pControl);    // 0x006CB820
    void HandleSaveButton(CSWGuiControl* pControl); // 0x006CBB60
    void VerifyLoadGame(CSWGuiControl* pControl);   // 0x006CC0E0
    void PopulateGameList();                        // 0x006CC160
    int ExtractSaveGame(int nGameNumber, const CExoString& sName);  // 0x006C9A90 (descriptive)
    int PrepareGameInProgress(int nGameNumber, const CExoString& sName, int nUnused);  // 0x006CAAF0 (descriptive)

    static int LoadBarCallback(int nCurrent, int nTotal, int nBase, int nRange);  // 0x006C8750 (descriptive)

    union {
        unsigned long m_nSaveFlags;                 // 0x64
        struct {
            unsigned long m_bSave : 1;              //      bit 0
            unsigned long m_bFromMainMenu : 1;      //      bit 1
        };
    };
    unsigned long m_nNextGameNumber;                // 0x68
    CExoString m_sTempDirectory;                    // 0x6c
    CSWGuiLabel m_lblPanelName;                     // 0x74
    CSWGuiLabel m_lblScreenShot;                    // 0x1b4
    CSWGuiLabel m_lblPlanetName;                    // 0x2f4
    CSWGuiLabel m_lblAreaName;                      // 0x434
    CSWGuiLabel m_aPartyMembers[3];                 // 0x574
    CSWGuiListBox m_lbGames;                        // 0x934
    CSWGuiButton m_btnSaveLoad;                     // 0xc14
    CSWGuiButton m_btnBack;                         // 0xdd8
    CSWGuiButton m_btnDelete;                       // 0xf9c
};

#endif
