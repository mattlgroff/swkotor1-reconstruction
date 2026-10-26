// Minimal client message handler interfaces, recovered from canonical x86.
#ifndef M_SW_C_MESSAGE_HANDLERS_H
#define M_SW_C_MESSAGE_HANDLERS_H

struct Vector;
class CResRef { public: CResRef(); char m_ref[16]; };
class CExoString {
public:
    char* m_sString; unsigned long m_nBufferLength;
    CExoString();
    ~CExoString();
    CExoString& operator=(const CExoString&);
    char* CStr() const;
    int GetLength() const;
};
class CSWCAreaObject {
public:
    virtual ~CSWCAreaObject();
    virtual void Unknown04();
    virtual void Unknown08();
    virtual void Unknown0C();
    virtual void Unknown10();
    virtual void Unknown14();
    virtual void Unknown18();
    virtual void Unknown1C();
    virtual void Unknown20();
    virtual void Unknown24();
    virtual void Unknown28();
    virtual void Unknown2C();
    virtual void Unknown30();
    virtual void Unknown34();
    virtual void Unknown38();
    virtual void Unknown3C();
    virtual void Unknown40();
    virtual void Unknown44();
    virtual void Unknown48();
    virtual void Unknown4C();
    virtual void Unknown50();
    virtual void Unknown54();
    virtual void Unknown58();
    virtual void Unknown5C();
    virtual void Unknown60();
    virtual void Unknown64();
    virtual void Unknown68();
    virtual void Unknown6C();
    virtual void Unknown70();
    virtual void Unknown74();
    virtual void Unknown78();
    virtual void Unknown7C();
    virtual void Unknown80();
    virtual void Unknown84();
    virtual void Unknown88();
    virtual void SetPosition(const Vector&);
    char pad_004[0xfc-4];
    int m_bEnabled;
    CExoString m_sNote;
    unsigned long m_nReference;
    int m_bReference;
    CSWCAreaObject();
};

struct Vector { float x, y, z; };
class CSWCAmbientSound {
public:
    virtual void PlayMusic(int);
    virtual void SetMusicDelay(int);
    virtual void SetMusicDayTrack(int);
    virtual void SetMusicNightTrack(int);
    virtual void PlayBattleMusic(int);
    virtual void SetBattleMusicTrack(int);
    virtual void PlayAmbientSound(int);
    virtual void SetAmbientDayTrack(int);
    virtual void SetAmbientNightTrack(int);
    void SetAmbientDayVolume(int);
    void SetAmbientNightVolume(int);
};
class CSWCArea {
public:
    char pad_000[0x1dc];
    CSWCAmbientSound* m_pAmbientSound;
    CSWCAreaObject* FindObject(unsigned long, unsigned long); // 0x00605AC0
    void AddObject(CSWCAreaObject*); // 0x00606D90
};
class CSWCModule {
public:
    char pad_000[0x48];
    CSWCArea* m_pArea;
};
class CSWCSoundObject {
public:
    void Play();
    void Stop();
    void SetPositionElevated(const Vector&); // established name, 0x0066ADC0
};
class CExoLocString {
public:
    CExoLocString(); CExoLocString(const CExoLocString&); ~CExoLocString();
    int AddString(int, const CExoString&, unsigned char); // established signature, 0x005EA340
    void* m_pList; void* m_pPad;
};
struct SJournalEntry {
    CExoLocString m_sName;           // 0x00
    CExoLocString m_sText;           // 0x08
    unsigned long m_nCalendarDay;    // 0x10
    unsigned long m_nTimeOfDay;      // 0x14
    CExoString m_sPlotID;            // 0x18
    int m_nState;                    // 0x20
    int m_nPriority;                 // 0x24
    unsigned short m_nPictureIndex;  // 0x28
    unsigned long m_bQuestCompleted : 1; // 0x2C bit 0
    unsigned long m_bQuestDisplayed : 1; // 0x2C bit 1
    unsigned long m_bUpdated : 1;        // 0x2C bit 2
    unsigned long m_nPlanetID;       // 0x30
    int m_n34;                       // 0x34
    unsigned long m_nQuestXP;        // 0x38

    SJournalEntry();
    SJournalEntry(const SJournalEntry &e);
    ~SJournalEntry();
    SJournalEntry &operator=(const SJournalEntry &e);
    void Update(SJournalEntry e, unsigned long nMask);
};


class CSWCJournal {
public:
    void EnableSort(); // established symbols, 0x00675DB0
    void DisableSort(); // 0x00675DC0
    void CallFullUpdateComplete(); // 0x00676BA0
    void ClearAll(); // descriptive name, 0x00676B00
    void Add(SJournalEntry, unsigned short); // 0x00676610
};
// Existing sources use both journal names for a zero-offset interface.
// Keep the established symbols pending reconciliation of the class hierarchy.
class CSWCQuestJournal : public CSWCJournal {
public:
    void Remove(const CExoString&); // descriptive name, 0x006760F0
    void SetJournalPicture(const CExoString&, unsigned short); // 0x00676260
};
class CSWCWorldJournal {
public:
    void AddEntry(unsigned long, const CExoString&, const CExoString&, unsigned long, unsigned long); // 0x00677300
    void AddEntryStrRef(unsigned long, unsigned long, unsigned long, unsigned long); // 0x00677130
};
class CGuiInGame { public: void ClosePopUpPanel(); }; // descriptive name, folded empty 0x004015A0
class CClientExoApp {
public:
    CSWCQuestJournal* GetQuestJournal();
    unsigned long GetClientLanguage();
    void SetCharacterFile(unsigned long, const char*, unsigned long);
    CSWCWorldJournal* GetWorldJournal();
    CGuiInGame* GetGuiInGame();
    CSWCModule* GetModule(); // 0x005ED530
    CSWCSoundObject* GetSoundObjectByGameObjectID(unsigned long); // 0x005ED620
};
class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;
class CSWCMessage {
public:
    int MessageReadOverflow(int);
    int MessageReadUnderflow(int);
    int ReadBOOL();
    CExoString ReadCExoString(int);
    CExoString ReadCExoLocStringClient(CResRef&, unsigned long*);
    unsigned short ReadWORD(int);
    unsigned char* ReadVOIDPtr(unsigned long);
    unsigned char ReadBYTE(int);
    unsigned long ReadDWORD(int);
    float ReadFLOAT(float, int);
    unsigned long ReadOBJECTIDClient();
    int HandleServerToPlayerAmbientMusicPlay();
    int HandleServerToPlayerAmbientMusicSetDelay();
    int HandleServerToPlayerAmbientMusicChangeTrack();
    int HandleServerToPlayerAmbientBattleMusicPlay();
    int HandleServerToPlayerAmbientBattleMusicChange();
    int HandleServerToPlayerAmbientSoundLoopPlay();
    int HandleServerToPlayerAmbientSoundLoopChange();
    int HandleServerToPlayerAmbientSoundVolume();
    int HandleServerToPlayerAmbient(unsigned char);
    int HandleServerToPlayerSoundObject_Play();
    int HandleServerToPlayerSoundObject_Stop();
    int HandleServerToPlayerSoundObject_ChangePosition();
    int HandleServerToPlayerSound(unsigned char);
    int HandleServerToPlayerGuiQuickbar_SetButton(unsigned char, int);
    int HandleServerToPlayerGuiQuickbar(unsigned char);
    int HandleServerToPlayerMapPin(unsigned char);
    int HandleServerToPlayerMapPinEnabled();
    int HandleServerToPlayerMapPinReferenceNumber();
    int HandleServerToPlayerGuiInfoPopupMessage(unsigned char);
    int HandleServerToPlayerGuiInventory(unsigned char);
    int HandleServerToPlayerCombatRound(unsigned char);
    int HandleServerToPlayerJournal_Type7();
    int HandleServerToPlayerJournal_Type8();
    int HandleServerToPlayerJournal_Type1();
    int HandleServerToPlayerJournal_Type2();
    int HandleServerToPlayerPopUpGUIPanel(unsigned char);
    int HandleServerToPlayerModule_Type13();
    int HandleServerToPlayerJournal_Type6();
    int HandleServerToPlayerJournal_Type9();
    int HandleServerToPlayerCharacterDownload(unsigned char);
    int MoreDataToRead();
    int HandleServerToPlayerGameObjectUpdate(unsigned char);
    int HandleServerToPlayerUpdate_Add();
    int HandleServerToPlayerUpdate_Update();
    int HandleServerToPlayerUpdate_Delete();
    int HandleServerToPlayerUpdate_Appearance();
    int HandleServerToPlayerUpdate_PlayerInfo();
    int HandleServerToPlayerUpdate_GuiElements();
    int HandleServerToPlayerUpdate_LoadProgress(); // descriptive name for 0x0064DCF0
    int HandleServerToPlayerCreatureUpdate_ControlObject(unsigned long);
    int HandleServerToPlayerCreatureUpdate_VisEffect();
    int HandleServerToPlayerCreatureUpdate_StripEquippedItems();
    int HandleServerToPlayerItemUpdate_DestroyItem();
    int HandleServerToPlayerItemScript_Add();
    int HandleServerToPlayerSetCustomToken(unsigned char);
};
#endif
