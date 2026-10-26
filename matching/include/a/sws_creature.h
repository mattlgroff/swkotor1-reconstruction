// CSWSCreature medium leaves (0x004EAE30-0x005140E5). Self-contained
// declarations; only the members these functions use are placed.
#ifndef A_SWS_CREATURE_H
#define A_SWS_CREATURE_H

#include <stddef.h>

class CSWSCreature;
class CGameObject;
class CItemRepository;
class CSWSItem;
class CSWSArea;
struct CResStruct;

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator==(const char* s) const;            // 0x005E5290

    char* m_sString;
    unsigned long m_nBufferLength;
};


struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }

    float x, y, z;
};

class CResGFF {
public:
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault); // 0x00411C90
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);           // 0x00411C20
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault); // 0x00411D00
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);     // 0x00411EC0
    // nDefault as int: the BOOL members are pushed as whole dwords.
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                int nDefault);                      // 0x00411A60
    Vector ReadFieldVector(CResStruct* pStruct, char* szLabel, int& bSuccess,
                           const Vector& vDefault);                 // 0x004122A0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldVector(CResStruct* pStruct, const Vector& vValue, char* szLabel); // 0x00412D30
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);    // 0x00413430
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);        // 0x00413660
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);                  // 0x00413350
};

class C2DAs {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearance;                            // 0x0c
    char pad_0x10[0xc4 - 0x10];
    C2DA* m_pRanges;                                // 0xc4 (placeholder name)
    char pad_0xc8[0xd4 - 0xc8];
    C2DA* m_pBodyBag;                               // 0xd4

    C2DA* GetAppearance() { return m_pAppearance; }
    C2DA* GetRanges() { return m_pRanges; }
    C2DA* GetBodyBag() { return m_pBodyBag; }
};

class CSWBaseItem;
class CSWClass;

struct CSWFeat {
    int m_nSuccessor;                               // 0x00 (placeholder name)
};
class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nItem);            // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x4];
    float m_fRangePersonal;                         // 0x04 (placeholder names 0x04-0x18)
    float m_fRangeTouch;                            // 0x08
    float m_fRangeShort;                            // 0x0c
    float m_fRangeMedium;                           // 0x10
    float m_fRangeLong;                             // 0x14
    float m_fRangeW;                                // 0x18
    char pad_0x1c[0x34 - 0x1c];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34
    char pad_0x38[0xac - 0x38];
    CSWClass* m_pClasses;                           // 0xac
    char pad_0xb0[0xb8 - 0xb0];
    C2DAs* m_p2DArrays;                             // 0xb8

    C2DAs* Get2DArrays() { return m_p2DArrays; }
    CSWBaseItem* GetBaseItem(int nItem) { return m_pBaseItemArray->GetBaseItem(nItem); }
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00
};
extern CSWRules* g_pRules;
extern int g_nAppearanceColHitRadius;               // 0x007A2118 (cached 2DA column, placeholder names)
extern int g_nAppearanceColPerSpace;                // 0x007A20D0
extern int g_nAppearanceColCrePerSpace;             // 0x007A20CC
extern int g_nAppearanceColWalkDist;                // 0x007A2108
extern int g_nAppearanceColHeight;                  // 0x007A2104                          // 0x007A3A28

template <class T> class CExoArrayList {
public:
    void SetSize(int nSize);
    void Allocate(int nSize);                       // out of line (sws_creature2.cpp)
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void Insert(T t, int nPos);

    void DelIndex(int i) { num--; for (; i < num; i++) element[i] = element[i+1]; }

    T* element;
    int num;
    int array_size;
};


class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const CResRef& r);            // 0x00406120
    char m_resRef[16];
};

class CExoLinkedListInternal {
public:
    void* GetAtPos(void* pos);                      // 0x005E9B60
    void* GetNext(void*& pos);                      // 0x005E9B90
    void* Remove(void* pos);                        // 0x005E9C10
    void* m_pHead;
};

class CVirtualMachine {
public:
    int RunScript(CExoString* psFile, unsigned long oidSelf, int bOid);   // 0x005D0FC0
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

#pragma pack(push, 4)
class CGameEffect {
public:
    int GetInteger(int nIndex);                     // 0x00503690

    unsigned __int64 m_nID;                         // 0x00
    unsigned short m_nType;                         // 0x08
};
#pragma pack(pop)

extern const int EFFECT_TYPE_AC_INCREASE;           // 0x0074673C
extern const int EFFECT_TYPE_AC_DECREASE;           // 0x00746740
extern const int EFFECT_TYPE_0746738;               // 0x00746738 (47, placeholder name)
extern const int EFFECT_TYPE_0746778;               // 0x00746778 (63, placeholder name)
extern const int EFFECT_TYPE_0746828;               // 0x00746828 (108, placeholder name)
extern const int EFFECT_TYPE_074682C;               // 0x0074682C (109, placeholder name)
extern const float g_fRate125;                      // 0x0073F400 (0.125f)
extern const float g_fRate15;                       // 0x0073F414 (1.5f)
extern const float g_fThousand;                     // 0x0073D6FC (1000.0f)
extern const float g_fHundred;                      // 0x00741838 (100.0f)
extern const float g_fZero;                         // 0x0073D700 (0.0f)
extern const int AI_ACTION_RESULT_005140D0;         // 0x00745DC4 (placeholder name)
extern const int AI_ACTION_RESULT_005140E0;         // 0x00745DC0 (placeholder name)
extern const int DAMAGE_FLAG_00;                    // 0x00746850
extern const int EVENT_AI_SOON;                     // 0x007441C4 (placeholder name)
extern const int EVENT_AI_LATER;                    // 0x007441E0 (placeholder name)

class CSWSAction {
public:
    int m_nType;                                    // 0x00
};

class CSWSFaction;

class CFactionManager {
public:
    int IsValidFactionId(int nFaction);             // 0x0052B280
    int GetNPCReputation(int nFaction1, int nFaction2); // 0x0052B420
    void SetNPCReputation(int nFaction1, int nFaction2, int nReputation); // 0x0052B3D0
    CSWSFaction* GetFaction(int nFaction);          // 0x0052B3B0
};

class CServerExoAppInternal {
public:
    char pad_0x0[0x10054];
    CFactionManager* m_pFactionManager;             // 0x10054
};

class CSWSObject;

class CServerAIMaster {
public:
    int SetAILevel(CSWSObject* pObject, int nLevel);    // 0x004B08A0
    void AddEventAbsoluteTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                              unsigned long oidObject, unsigned long nEvent, void* pData);  // 0x004AFDB0
    void AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                           unsigned long oidObject, unsigned long nEvent, void* pData);  // 0x004B08D0
};

class CSWSPlayerStore {
public:
    unsigned long m_aSlotItems[18];
};
class CSWSPlayerStoreGUI {
public:
    CSWSPlayerStore* m_pStore;
};

class CSWSPlayer {
public:
    char pad_0x0[0x4];
    unsigned long m_nPlayerID;                      // 0x04
    char pad_0x8[0x54 - 0x8];
    CSWSPlayerStoreGUI* m_pStoreGUI;                 // 0x54

    unsigned long* m_pControlled;                   // 0x58 (placeholder name)

    CSWSCreature* GetGameObject();                  // 0x00561080
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                             // 0x004D69F0
    ~CSWCCMessageData();                            // 0x004D6A40
    void SetInteger(int nIndex, int nValue);        // 0x004D76B0
    void SetObjectID(int nIndex, unsigned long oid);    // 0x004D7780
    char pad_0x0[0x34];
};

class CSWSMessage {
public:
    int SendServerToPlayerVoiceChat_Play(CSWSPlayer* pPlayer, unsigned long oid,
                                         unsigned long nArg);    // 0x00569EC0
    int SendServerToPlayerCamera_ChangeLocation(CSWSPlayer* pPlayer, unsigned char nFlags, float x, float y, float z, unsigned char nMode); // 0x0056C610
    int SendServerToPlayerInventory_EquipCancel(unsigned long oidPlayer, unsigned long oidItem, int nSlot); // 0x0056FBD0
    int SendServerToPlayerInventory_UnequipCancel(unsigned long oidPlayer, unsigned long oidItem); // 0x0056FD80
    int SendServerToPlayerCCMessage(unsigned long nPlayerID, unsigned char nMinor,
                                    CSWCCMessageData* pData, void* pAttackData);   // 0x00568570
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);  // 0x004D8230
};

extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
class CSWPartyTable {
public:
    CItemRepository* GetItemRepository();           // 0x00563340
    int GetIsPartyMember(unsigned long oid);        // 0x00563790 (placeholder name)
    char pad_0x0[0xf0];
    int m_nF0;                                      // 0xf0 (placeholder name)
    char pad_0xf4[0x120 - 0xf4];
    int m_nPazaakCards[1];                          // 0x120

    void AddPazaakCards(int nIndex, int nCount) { m_nPazaakCards[nIndex] += nCount; }
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* day, unsigned long* time); // 0x004ADE40
    int SubtractWorldTimes(unsigned long day1, unsigned long time1, unsigned long day2, unsigned long time2, unsigned long* day, unsigned long* time); // 0x004AE460
};

class CSWSEncounter {
public:
    void RemoveFromActiveCreatureCount(float fCR, int bArg); // 0x00594310 (descriptive reference name, RET 8)
};
class CSWSModule;
class CServerExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);                  // 0x004AE750
    CSWSEncounter* GetEncounterByGameObjectID(unsigned long oid); // 0x004AE7F0
    CSWSItem* GetItemByGameObjectID(unsigned long oid);             // 0x004AE760
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x004AE770
    unsigned long GetPlayerCreatureID();                            // 0x004AEA40
    CSWSPlayer* GetClientObjectByObjectId(unsigned long oid);       // 0x004AE910
    CWorldTimer* GetWorldTimer();                    // 0x004AEDE0
    void* GetModule();                                              // 0x004AEC90
    CGameObjectArray* GetObjectArray();                             // 0x004AED70
    CSWSModule* GetModule6B0();                                     // 0x004AE6B0 (placeholder name)
    CSWSMessage* GetSWSMessage();                                   // 0x004AEDB0
    CServerAIMaster* GetServerAIMaster();                           // 0x004AED80
    CSWPartyTable* GetPartyTable();                                 // 0x004AEE70

    char pad_0x0[0x4];
    CServerExoAppInternal* m_pInternal;             // 0x04
};

class CSWPartyMember {
public:
    char pad_0x0[0x4];
    unsigned long m_oid;                            // 0x04
};

struct CSWPartyMovementRecord {
    unsigned long m_nState;
    char pad_4[0x88-4];
};

class CSWParty {
public:
    CSWPartyMember* GetMember(int n);               // 0x006346C0
    int IsPartyMember(unsigned long oid); // 0x006349C0
    int m_nMembers;                                 // 0x00
    char pad_4[0x28-4];
    CSWPartyMovementRecord m_aMovement[3];
};

class CClientExoApp {
public:
    CSWParty* GetSWParty();                         // 0x005ED8B0
    void* GetInGameGui(int n);                      // 0x005ED690
    unsigned long ServerToClientObjectId(unsigned long oid);    // 0x005EDA50
    class CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);  // 0x005ED5B0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x4
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }
inline CClientExoApp* GetClientExoApp() { return g_pAppManager->m_pClientExoApp; }

class CGuiInGame {
public:
    int GetIsInMenu();                              // 0x0062B4D0 (placeholder name)
};

class CSWBaseItem {
public:
    unsigned short GetRequiredFeat(unsigned char nIndex); // 0x005B30C0
    char pad_0x0[0x4];
    unsigned long m_nEquipableSlots;                // 0x04
    unsigned char m_nWeaponType;                    // 0x08
    unsigned char m_nWeaponWield;                   // 0x09
    char pad_0xa[0x24 - 0xa];
    unsigned short m_nStackSize;                    // 0x24 (placeholder name)
    char pad_0x26[0x80 - 0x26];
    unsigned char m_nRequiredFeatCount;             // 0x80
    char pad_0x81[0x8a - 0x81];
    unsigned char m_nArmorType;                     // 0x8a (placeholder name)
};

class CResRef;

struct CSWItemProperty {
    unsigned short m_nPropertyName;                 // 0x00
    unsigned short m_nSubType;                      // 0x02
    char pad_0x4[0x11 - 0x4];
    unsigned char m_nActivation;                    // 0x11
};

class CSWSItem {
public:
    CSWSItem(unsigned long oid);                    // 0x005530A0
    virtual ~CSWSItem();                            // vtable slot 0
    int GetPropertyByTypeExists(unsigned short nType, unsigned short nSubtype); // 0x00555C90
    CSWItemProperty* GetPassiveProperty(int nIndex); // 0x00553990
    int MergeItem(CSWSItem* pOther);                // 0x00553F90
    CSWSItem* SplitItem(int nCount);                // 0x0055F280
    void AddToArea(CSWSArea* pArea, float x, float y, float z, int bArg);  // 0x00555720
    CSWBaseItem* GetBaseItem();                     // 0x005B4790
    int LoadFromTemplate(const CResRef& cTemplate); // 0x005608B0
    void CloseInventory(unsigned long oid, int bArg);   // 0x0055D800
    CSWItemProperty* GetActiveProperty(int nIndex);  // 0x00553960
    void DecrementUses(int nIndex, unsigned long oidPlayer); // 0x0055DE20
    unsigned long GetDamageFlags();                 // 0x00554050

    char pad_0x4[0xc - 0x4];
    int m_nBaseItem;                                // 0x0c
    char pad_0x10[0x14 - 0x10];
    unsigned long m_n14;                            // 0x14 (placeholder name)
    char pad_0x18[0x108 - 0x18];
    int m_n108;                                     // 0x108 (placeholder name)
    char pad_0x10c[0x248 - 0x10c];
    int m_nActiveProperties;                        // 0x248 (placeholder name)
    int m_nPassiveProperties;                       // 0x24c
    char pad_0x250[0x264 - 0x250];
    unsigned char m_nPazaakCard;                    // 0x264 (placeholder name)
    char pad_0x265[0x268 - 0x265];
    unsigned long m_oidPossessor;                   // 0x268 (placeholder name)
    CItemRepository* m_pRepository;                 // 0x26c (placeholder name)
    char pad_0x270[0x288 - 0x270];
    unsigned char m_n288;                           // 0x288 (placeholder name)
    char pad_0x289[0x28c - 0x289];
    int m_nStackSize;                               // 0x28c (placeholder name)
    char pad_0x290[0x294 - 0x290];
    unsigned long m_nPropertyActivationMask;        // 0x294
    char pad_0x298[0x29c - 0x298];
};

class CSWSInventory {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);   // 0x005A4C20
    int Fn5A4950(CSWSItem* pItem);                  // 0x005A4950 (placeholder name)
};

class CItemRepository {
public:
    unsigned long FindItemWithBaseItemType(unsigned char nType, int nNth);  // 0x00555E00
    int AddItem(CSWSItem** ppItem, int a2, int a3, int a4);     // 0x0055D330
    CSWSItem* ItemListGetItem(unsigned long nIndex);           // 0x005560B0

    char pad_0x0[0x10];
    unsigned long m_nItems;                         // 0x10 (placeholder name)
};

struct CSWSLevelStats {
    char pad_0x0[0x2b];
    unsigned char m_nHitDie;                        // 0x2b
};

class CSWClass {
public:
    char GetArmorBonus(int nLevel);       // 0x005BE770
    char pad_0x0[0x1a0];
};

struct CSWSLevelStats;
class CSWSCreatureStats {
public:
    CExoString GetFullName();                       // 0x005A4C60 (placeholder name)
    unsigned char GetLevel(int bArg);               // 0x005A5FD0
    float GetChallengeRating() { return m_fChallengeRating; }
    unsigned char GetSimpleAlignmentGoodEvil();      // 0x005A5110
    unsigned char GetFeatRemainingUses(unsigned short nFeat); // 0x005A6680
    int HasFeat(unsigned short nFeat);              // 0x005A6630
    int GetCanUseSkill(unsigned char nSkill);       // 0x005AF880
    struct CSWSLevelStats* GetLevelStats(unsigned char nLevel);  // 0x005A6170 (placeholder name)
    unsigned char GetClass(int nIndex);             // 0x005A4E90
    unsigned char GetClassLevel(int nIndex, int bArg);  // 0x005A5090
    char GetACNaturalBase();                        // 0x005A5A80
    char GetDEXMod(int bArg);                       // 0x005A5330
    char GetTotalACFeatMod();                       // 0x005A9F50
    int UnReadySpell(unsigned long nSpell);        // 0x005A55C0
    void DecrementSpellUses(unsigned long nSpell, int a, unsigned char nMeta);  // 0x005A6880 (placeholder name)
    int GetSaveVsDamage(CSWSCreature* pTarget, unsigned long a, int b, int c, int d, int e); // 0x005A9050 (placeholder name)
    void Dump6190();                                // 0x005A6190 (placeholder name)
    void Dump6310();                                // 0x005A6310 (placeholder name)
    void Dump63D0();                                // 0x005A63D0 (placeholder name)
    void Dump6550();                                // 0x005A6550 (placeholder name)
    void Dump6250();                                // 0x005A6250 (placeholder name)
    void Dump6490();                                // 0x005A6490 (placeholder name)

    char pad_0x0[0x6c];
    int m_bIsPC;                                    // 0x6c (placeholder name)
    char pad_0x70[0x78 - 0x70];
    int m_nFactionId;                               // 0x78
    char pad_0x7c[0x84 - 0x7c];
    float m_fChallengeRating;                       // 0x84
    char pad_0x88;
    unsigned char m_nNumClasses;                    // 0x89 (placeholder name)
    char pad_0x8a[0xee - 0x8a];
    char m_nConMod;                                 // 0xee (placeholder name)
    char pad_0xef[0xf2 - 0xef];
    char m_nF2;                                     // 0xf2 (placeholder name)
    char pad_0xf3;
    char m_nF4;                                     // 0xf4 (placeholder name)
    char pad_0xf5;
    unsigned char m_nArmorType;                     // 0xf6 (placeholder name)
    char m_nF7;                                     // 0xf7 (placeholder names 0xf7-0x101)
    char m_nF8;
    char m_nF9;
    char m_nFA;
    char m_nFB;
    char m_nFC;
    char m_nFD;
    char m_nFE;
    char m_nFF;
    char m_n100;
    char m_n101;
    char pad_0x102[0x140 - 0x102];
    unsigned short m_n140;                          // 0x140 (effect-list start index, placeholder name)
    char pad_0x142[0x198 - 0x142];
    float m_f198;                                   // 0x198 (placeholder name)
    float m_f19C;                                   // 0x19c (placeholder name)
};

struct CExoLinkedListWrap {
    CExoLinkedListInternal* m_pInternal;
};

struct CSWSCombatRoundPending {
    char pad_0x0[0x10];
    unsigned char m_nType;                          // 0x10 (placeholder name)
    char pad_0x11[0x14 - 0x11];
    unsigned long m_oid;                            // 0x14 (placeholder name)
};

class CSWSCombatRound {
public:
    void* GetAttack(int nAttack);                   // 0x004D2B50 (placeholder name)
    CSWSItem* GetCurrentAttackWeapon(int nType);    // 0x004D4FF0

    int GetSpellActionPending();                    // 0x004D35C0

    char pad_0x0[0x934];
    int m_bRoundStarted;                            // 0x934
    char pad_0x938[0x940 - 0x938];
    int m_n940;                                     // 0x940 (placeholder name)
    char pad_0x944[0x96c - 0x944];
    unsigned char m_nCurrentAttack;                 // 0x96c
    char pad_0x96d[0x9b0 - 0x96d];
    CExoLinkedListWrap* m_pPending;                 // 0x9b0 (placeholder name)

    void Fn4D28F0(int a, unsigned long oid);        // 0x004D28F0 (placeholder name)
    void Fn4D2920(int a, int b);                    // 0x004D2920 (placeholder name)
    void IncrementTimer(int nTime);                 // 0x004D4C10
    void SetCurrentAction(int nAction);             // 0x004D2B40
    void AddAttackAction(unsigned long oidTarget, int a2, unsigned short nFeat, int nTime, int a5);  // 0x004D38B0
};

class CSWSAreaSound {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void Update(int nArg);                  // 0x10 (placeholder name)
};

class CSWSArea {
public:
    char pad_0x0[0x120];
    unsigned long m_idSelf;
    char pad_124[0x190-0x124];
    CExoArrayList<unsigned long> m_lPlayers;        // 0x190 (placeholder name)
    char pad_0x19c[0x208 - 0x19c];
    CSWSAreaSound* m_pSoundObject;                  // 0x208 (placeholder name)
};

class CScriptEvent {
public:
    CScriptEvent();                                 // 0x004D7540
    void SetInteger(int nIndex, int nValue);        // 0x004D76B0
    unsigned short m_nType;                         // 0x00
    char pad_0x2[0x34 - 0x2];
};

extern const unsigned long EVENT_0x7441DC;          // 0x007441DC (placeholder name)
extern const unsigned char OBJECT_TYPE_0746633;     // 0x00746633 (placeholder name)

Vector VectorSubtract(const Vector& a, const Vector& b);    // 0x00423BC0 (placeholder name)
Vector NormalizeVector(const Vector& v);            // 0x004AB180

class CSWSModule {
public:
    char pad_0x0[0x20];
    unsigned long m_idSelf;                         // 0x20 (placeholder name)
    char pad_0x24[0x104 - 0x24];
    unsigned long m_nFlags104;                      // 0x104 (placeholder name)
};

class CSWSAttackData {
public:
    char pad_0x0[0x3a];
    unsigned char m_nWeaponAttackType;              // 0x3a
};

struct CSWCCreature21C {
    char pad_0x0[0x60];
    float m_f60;                                    // 0x60 (placeholder name)
};

class CSWCCreature {
public:
    int GetPauseReadyAnimation();                   // 0x0060F020

    char pad_0x0[0x21c];
    CSWCCreature21C* m_p21C;                        // 0x21c (placeholder name)
};

class CGameObjectClient {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
};

class CSWSObjectF8 {
public:
    char pad_0x0[0xf8];
    int m_nF8;                                      // 0xf8 (placeholder name)
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void* AsSWSObject();                    // 0x10 (placeholder name)
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual void v34();
    virtual CSWSItem* AsSWSItem();                  // 0x38
    virtual void v3c(); virtual void v40(); virtual void v44();
    virtual class CSWSPlaceable* AsSWSPlaceable();  // 0x48

    CGameObject(unsigned char nObjectType, unsigned long oid);  // 0x004D7D60

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
    char pad_0x9[0x16 - 0x9];
    unsigned short m_nAIGroupId;                    // 0x16 (placeholder name)
};

// The ctor is declared throw(): SetInParty's new-expression has no EH frame.
struct CSWSCreaturePartyFollowInfo {
    CSWSCreaturePartyFollowInfo() throw();          // 0x004F79E0
    void Reset();                                   // 0x004F0020 (placeholder name)
    void Save(CResGFF* pGFF, CResStruct* pStruct);  // 0x004EAF70
    void Load(CResGFF* pGFF, CResStruct* pStruct);  // 0x004EB020

    unsigned long m_oidFollowObject;                // 0x00
    Vector m_vFollowLocation;                       // 0x04
    Vector m_vLastLeaderPos;                        // 0x10
    Vector m_vLastFollowerPos;                      // 0x1c
    float m_fMaxSpeed;                              // 0x28
    int m_bStickToPos;                              // 0x2c
    unsigned long m_nResult;                        // 0x30
    unsigned long m_nTimeElapsed;                   // 0x34
    int m_bInSafetyRange;                           // 0x38
};

class CSWPartyMemberData {
public:
    void Reset();                                   // 0x004EAE80

    char pad_0x0[0x4];
    int m_n4;                                       // 0x04 (placeholder name)
    char pad_0x8[0xc - 0x8];
    Vector m_vC;                                    // 0x0c (placeholder name)
    Vector m_v18;                                   // 0x18 (placeholder name)
    char pad_0x24[0x28 - 0x24];
    Vector m_v28;                                   // 0x28 (placeholder name)
    int m_n34;                                      // 0x34 (placeholder name)
    char pad_0x38[0x44 - 0x38];
    int m_n44;                                      // 0x44 (placeholder name)
    int m_n48;                                      // 0x48 (placeholder name)
    Vector m_v4C;                                   // 0x4c (placeholder name)
    float m_f58;                                    // 0x58 (placeholder name)
    char pad_0x5c[0x68 - 0x5c];
    int m_n68;                                      // 0x68 (placeholder name)
    char pad_0x6c[0x80 - 0x6c];
    unsigned long m_oid80;                          // 0x80 (placeholder name)
    unsigned long m_n84;                            // 0x84 (placeholder name)
};

class CSWSPlaceable {
public:
    void CloseInventory(unsigned long oid, int bArg);   // 0x00587560
};

struct CSWSCreatureFollowInfo {
    void Fn5CEC90();                                // 0x005CEC90 (placeholder name)

    char pad_0x0[0x4];
    float m_fHitRadius;                             // 0x04 (placeholder names 0x04-0x18)
    float m_fPersonalSpace;                         // 0x08
    char pad_0xc[0x10 - 0xc];
    float m_fCreaturePersonalSpace;                 // 0x10
    float m_fHeight;                                // 0x14
    float m_fWalkDist;                              // 0x18
    char pad_0x1c[0x8c - 0x1c];
    short* m_p8C;                                   // 0x8c (placeholder name)
    char pad_0x90[0x9c - 0x90];
    short* m_p9C;                                   // 0x9c (placeholder name)
    char pad_0xa0[0xf8 - 0xa0];
    int m_nF8;                                      // 0xf8 (placeholder name)
    int m_nFC;                                      // 0xfc (placeholder name)
    unsigned char m_n100;                           // 0x100 (placeholder name)
    char pad_0x101[0x104 - 0x101];
    int m_n104;                                     // 0x104 (placeholder name)
    Vector m_vPosition;                             // 0x108
    Vector m_vOrientation;                          // 0x114
};

class CSWSSpellScriptData {
public:
    int SaveData(CResGFF* pGFF, CResStruct* pStruct);   // 0x004EE230
    int LoadData(CResGFF* pGFF, CResStruct* pStruct);   // 0x004EE2E0

    int m_nSpellId;                                 // 0x00
    unsigned long m_oidCaster;                      // 0x04
    unsigned long m_oidTarget;                      // 0x08
    unsigned long m_oidItem;                        // 0x0c
    Vector m_vTargetPosition;                       // 0x10
    CExoString m_sScript;                           // 0x1c
    unsigned long m_oidArea;                        // 0x24
};

struct CSWGuiActionData {
    unsigned long m_n[4];
};

class CSWGuiInterfaceAction {
public:
    CSWGuiInterfaceAction();                        // 0x004EAE30
    ~CSWGuiInterfaceAction();                       // 0x0059EA30
    CSWGuiInterfaceAction& operator=(const CSWGuiInterfaceAction& o);  // 0x004EFFA0
    void Reset();                                   // 0x0063C000 (placeholder name)

    CExoString m_sName;                             // 0x00
    unsigned long m_n08;                            // 0x08 (placeholder name)
    CSWGuiActionData m_data;                        // 0x0c (placeholder name)
    unsigned long m_n1C;                            // 0x1c (placeholder name)
    CResRef m_resIcon;                              // 0x20
    unsigned long m_b30 : 1;                        // 0x30 bit 0 (placeholder name)
    unsigned long m_n30 : 4;                        // 0x30 bits 1-4 (placeholder name)
    unsigned long m_n34;                            // 0x34 (placeholder name)
};

struct CSWSCreatureListEntry {
    unsigned long m_oid;                            // 0x00
    unsigned long m_nFlags;                         // 0x04
};

struct CSWSCreature93C {
    unsigned long m_n0;
    unsigned long m_n4;
};

class CSWSObjectActionNode {
public:
    ~CSWSObjectActionNode();                        // 0x004CC4D0

    unsigned long m_nActionId;                      // 0x00
    char pad_0x4[0x38 - 0x4];
    union { unsigned long m_n38; float m_f38; };      // 0x38 (placeholder name)
    union { unsigned long m_n3C; float m_f3C; };      // 0x3c (placeholder name)
    union { unsigned long m_n40; float m_f40; };      // 0x40

    union { unsigned long m_n44; float m_f44; };      // 0x44
    unsigned long m_n48;                            // 0x48

    unsigned long m_n4C;                            // 0x4c (placeholder name)
    union { unsigned long m_n50; float m_f50; };      // 0x50
    unsigned long m_n54;
    char pad_0x58[0x6c - 0x58];
    unsigned short m_nGroupId;                      // 0x6c
    unsigned short m_nParameterCount;               // 0x6e
};

class CSWSCreature : public CGameObject {
public:
    virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual int RunAction(CSWSObjectActionNode* pNode, int bArg);  // 0x74 (placeholder name)
    virtual void v78(); virtual void SetAnimation(int nAnimation);  // 0x7c (placeholder name)

    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual int GetDead();  // 0x94 (placeholder name)

    virtual short GetMaxHitPoints(int bArg);        // 0x98
    virtual short GetCurrentHitPoints(int bArg);    // 0x9c

    CSWSArea* GetArea();                            // 0x004CB120
    void* GetClientObject();                        // 0x004CC2B0
    void SetGroupInterruptable(unsigned short nGroupId, int bInterruptable);  // 0x004CC790
    void AddAction(unsigned long nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2 = 0, void* p2 = 0,
                   int nType3 = 0, void* p3 = 0, int nType4 = 0, void* p4 = 0,
                   int nType5 = 0, void* p5 = 0, int nType6 = 0, void* p6 = 0,
                   int nType7 = 0, void* p7 = 0, int nType8 = 0, void* p8 = 0,
                   int nType9 = 0, void* p9 = 0, int nType10 = 0, void* p10 = 0,
                   int nType11 = 0, void* p11 = 0, int nType12 = 0, void* p12 = 0,
                   int nType13 = 0, void* p13 = 0);         // 0x004CEA20
    void AddActionToFront(unsigned long nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2 = 0, void* p2 = 0,
                   int nType3 = 0, void* p3 = 0, int nType4 = 0, void* p4 = 0,
                   int nType5 = 0, void* p5 = 0, int nType6 = 0, void* p6 = 0,
                   int nType7 = 0, void* p7 = 0, int nType8 = 0, void* p8 = 0,
                   int nType9 = 0, void* p9 = 0, int nType10 = 0, void* p10 = 0,
                   int nType11 = 0, void* p11 = 0, int nType12 = 0, void* p12 = 0,
                   int nType13 = 0, void* p13 = 0);         // 0x004CCB00
    int GetWeaponDamageBonus(CSWSCreature* pTarget, unsigned long a);  // 0x005B5640 (placeholder name)
    void UpdateAppearance();                        // 0x004F6D70 (placeholder name)

    int AddDriveAction(unsigned short nGroupId, char* pVector, unsigned long b,
                       unsigned long c, unsigned long d, unsigned long e);  // 0x004EB510
    int AddPathfindingWaitActionToFront(unsigned short nGroupId);  // 0x004EB5A0
    void ComputeArmourClass(CSWSItem* pItem, int a, int b);       // 0x004ECE80
    int AddAppearActions();                         // 0x004ECFF0
    int AddDisappearActions(int bDestroy);          // 0x004ED060
    void UpdateExcitedStateTimer();                 // 0x004ED110
    unsigned long GetDamageFlags();                 // 0x004ED5E0
    int GetIsWeaponEffective(unsigned long oidTarget, unsigned long nFlags);   // 0x004EDB20
    int DecrementSpellReadyCount(unsigned long nSpell, unsigned char nMultiClass,
                                 unsigned char nMeta, int a, int bInstant); // 0x004EDDD0
    void DumpToLog();                               // 0x004EF640
    int CalculateSpellSaveDC(int nSpell);           // 0x004EF6E0
    CItemRepository* GetItemRepository(int bParty); // 0x004EF770
    int GetIsPartyMemberOr4CB810();                 // 0x004EF820 (placeholder name)
    int GetDeadTemp();                              // 0x004EF890
    int Fn4CB810();                                 // 0x004CB810 (placeholder name)
    void OnEndDialog(int bArg);                     // 0x004EF910
    void SendFeedbackMessage(unsigned short nMessage, CSWCCMessageData* pData);  // 0x004EDE10
    void AutoCloseGUIPanels(int bArg);              // 0x004EDED0
    void ResetFollowInfo();                         // 0x004EDF80 (placeholder name)
    void DisplayFloatyDataToSelf(int nValue);       // 0x004EF560
    void AddPazaakCard(CSWSItem* pItem);            // 0x004EFB40
    float GetSpotCheckDistance();                   // 0x004EFC70
    float GetListenCheckDistance();                 // 0x004EFD20
    void InstantUseItem(unsigned long oidItem);     // 0x004EFE30
    void SetInParty(int bInParty, int bUpdate);     // 0x004FDB20
    void UpdateAppearanceForEquippedItems();        // 0x004ED620
    unsigned char GetBodyBagAppearance();           // 0x004EF2E0
    int AddPickUpItemActions(unsigned long oidItem, unsigned long oidArea, unsigned long nArg); // 0x004EB620
    void UseItemProperty(unsigned short nSubType, Vector* pTarget, unsigned long oidSelf,
                         int a4, unsigned long oidItem, int a6, int a7, int a8, int a9); // 0x004CDF50 (placeholder name)
    int RemovePazaakCard(CSWSItem* pItem);          // 0x004EFC30
    int GetIsInjured();                             // 0x004EFF30 (placeholder name)
    int GetPauseReadyAnimation();                   // 0x004F0F90
    int GetCurrentlyFollowingLeader();              // 0x004F7750
    int GetTotalACEffectBonus();                    // 0x004F7780
    void SetDetectMode(unsigned char nMode);        // 0x0050EE30
    void SetStealthMode(unsigned char nMode, unsigned long nArg);  // 0x0050EE50
    CSWSFaction* GetFaction();                      // 0x00513FC0
    int GetCreatureReputation(unsigned long oidCreature, int nFaction); // 0x00513FF0
    void AdjustReputation(int nFaction, int nDelta);  // 0x00514070
    int FUN_005140d0(int nArg);                     // 0x005140D0
    int AIActionRest(int nArg);                     // 0x005140E0
    int RemoveEffectById(unsigned __int64 nID);     // 0x004D06C0 (CSWSObject)
    float GetMovementRate4F1BE0();                  // 0x004F1BE0 (placeholder name)
    int GetHasEffectOfType(int nType);              // 0x004F7880 (placeholder name)
    void RemoveFirstEffect108();                    // 0x004F77E0 (placeholder name)
    void RemoveFirstEffect109();                    // 0x004F7830 (placeholder name)
    void SetVector248(const Vector& v);             // 0x0050E250 (placeholder name)
    int GetIsInList920(unsigned long oid);          // 0x005179E0 (placeholder name)
    CSWSCreatureListEntry* GetFromList920(unsigned long oid);  // 0x00517A20 (placeholder name)
    unsigned long Get93CField4();                   // 0x005188C0 (placeholder name)
    float GetMovementRate4F1B20();                  // 0x004F1B20 (placeholder name)
    int GetHasActivePartyMember920();               // 0x004F7650 (placeholder name)
    int RemoveFirstActionOfType(unsigned long nAction);     // 0x004F76C0 (placeholder name)
    void RemoveEffects4F5F70();                     // 0x004F5F70 (placeholder name)
    int CountItems4F3770(int bFlag8, int bSkipEquipped, int bNo108);   // 0x004F3770 (placeholder name)
    void ClearActionsForType4F8770(int nType);      // 0x004F8770 (placeholder name)
    int AddAction38(unsigned long a1, unsigned long a2, unsigned long a3);  // 0x004F0900 (placeholder name)
    int GetNumActionGroups();                       // 0x004CC690
    void* GetPositionByGroupIndex(int nIndex);      // 0x004CC5C0
    unsigned short GetActionByGroupId(unsigned short nGroupId, CSWSObjectActionNode** ppNode);  // 0x004CC530
    void RemoveGroup(unsigned short nGroupId);      // 0x004CC6F0
    void RemoveTargetActions4F61D0(CGameObject* pTarget);  // 0x004F61D0 (placeholder name)
    int ResetCombatState4F1250(int nTime);          // 0x004F1250 (placeholder name)
    void SendVoiceChat4F2520(unsigned long nArg);   // 0x004F2520 (placeholder name)
    int GetIsPossessed();                           // 0x0063E7F0 (folded zero leaf)
    int Fn63E7F0();                                 // 0x0063E7F0 (placeholder name)
    int Fn4CAE60(unsigned long nArg);               // 0x004CAE60 (placeholder name)
    int IsDebilitated();                            // 0x005B4880
    void SetOrientation4CD170(const Vector& v);     // 0x004CD170 (placeholder name)
    void StopGuiTimingBar();                        // 0x004EF4D0
    void Fn510EF0(CSWSItem* pItem, int a, int b, int c);    // 0x00510EF0 (placeholder name)
    void Fn4F0420(CSWSItem* pItem, unsigned long nSlot, int a, int b, int c, int d, int e);  // 0x004F0420 (placeholder name)
    void SplitToRepository4F0A60(CSWSItem* pItem, int nCount);  // 0x004F0A60 (placeholder name)
    void MergeOrEquip4F0990(CSWSItem* pItem, CSWSItem* pOther);  // 0x004F0990 (placeholder name)
    void SetFacingTarget4F34A0(unsigned long oidTarget, int bForce);  // 0x004F34A0 (placeholder name)
    void Fn4F3690(unsigned short nMessage);         // 0x004F3690 (placeholder name)
    int UpdateTarget4F05D0(CSWSItem* pItem, int a2, int bSkipPending, unsigned long a4);  // 0x004F05D0 (placeholder name)
    short GetArmorClass();                          // 0x004ED1D0
    void UpdatePersonalSpace();                     // 0x004ED6E0
    int UseFeat(unsigned short nFeat, unsigned long oidTarget, int a3, int a4);  // 0x004ECEE0
    float ComputeSpellRange(CExoString sRange, unsigned long oidTarget);  // 0x004EB3A0
    int AddAction9(unsigned long oidItem, Vector* pV, unsigned long a3);    // 0x004F0840 (placeholder name)
    void Fn4F1460();                                // 0x004F1460 (placeholder name)
    unsigned char CanUnEquipWeapon(CSWSItem* pItem); // 0x00513E30
    int CheckProficiencies(CSWSItem* pItem, unsigned long nSlot); // 0x00510E30 (reference descriptive name)
    int CheckItemAlignmentRestrictions(CSWSItem* pItem); // 0x005145A0 (reference descriptive name)
    int CheckItemFeatRestrictions(CSWSItem* pItem);  // 0x00514650 (reference descriptive name)
    int CheckItemClassRestrictions(CSWSItem* pItem); // 0x00518210 (reference descriptive name)
    void ActivityManager(int nAction);              // 0x004F87D0
    int RunEquip(unsigned long oidItem, unsigned long nSlot, int nValue); // 0x00501DE0
    int RunUnequip(unsigned long oidItem, unsigned long oidRepository, int bUpdate, CSWSItem** ppItem, int nValue); // 0x005023A0
    int AIActionEncounterCreatureDestroySelf();      // 0x00510C20 (RET 0, no stack argument)
    unsigned char CheckWeaponSlot(CSWSItem* pItem, unsigned long* pSlot, int bFeedback, int bUpdate); // 0x00515860 (descriptive name)
    void SetVisibilityList(CExoArrayList<CSWSCreatureListEntry*>* pList); // 0x0051AFD0, descriptive name
    int AIActionCheckMoveToPoint(CSWSObjectActionNode* pNode); // 0x00510670
    int AIActionCheckMoveToPointRadius(CSWSObjectActionNode* pNode); // 0x005108A0
    int AIActionCheckMoveToFollowRadius(CSWSObjectActionNode* pNode); // 0x00510AB0
    int AddMoveToPointActionToFront(unsigned short nGroup, const Vector& vTarget, unsigned long oidArea,
        unsigned long oidTarget, int bRun, float fRange, float fTolerance, int, int, int, int, int, int,
        const Vector& vOffset, unsigned long, unsigned long, int); // 0x004F8A50
    int AIActionCheckForceFollowObject(CSWSObjectActionNode* pNode); // 0x005136B0
    int AIActionChangeFacingPoint(CSWSObjectActionNode* pNode); // 0x0050FC90
    int AIActionChangeFacingObject(CSWSObjectActionNode* pNode); // 0x0050FB60
    int AIActionEquipItem(CSWSObjectActionNode* pNode); // 0x00510FD0
    int AIActionUnequipItem(CSWSObjectActionNode* pNode); // 0x00513EC0
    int AIActionOrientCamera(CSWSObjectActionNode* pNode); // 0x005137C0 (descriptive reference name)
    int AIActionAppear(CSWSObjectActionNode* pNode);  // 0x005140F0
    int AIActionDisappear(CSWSObjectActionNode* pNode); // 0x005141B0
    int AIActionWaitForEndOfRound(CSWSObjectActionNode* pNode); // 0x00513F60
    int AIActionPartyFollowLeader(CSWSObjectActionNode* pNode);  // 0x00511130
    int HasAction(unsigned long nType);              // 0x005188E0
    CSWSObjectActionNode* GetAction(unsigned long nType); // 0x00518940
    void ClearVisibleList();                        // 0x005178F0
    void OnIdChanged(unsigned long oid);            // 0x00517960 (proven symbol name)
    void ClearAllActions(int bArg);                 // 0x004CCD80

    char pad_0x18[0x78 - 0x18];
    int m_nAILevel;                                 // 0x78
    char pad_0x7c[0x8c - 0x7c];
    unsigned long m_oidArea;                        // 0x8c
    Vector m_vPosition;                             // 0x90
    Vector m_vOrientation;                          // 0x9c
    char pad_0xa8[0xbc - 0xa8];
    unsigned long m_nActionStartDay;                // 0xbc
    unsigned long m_nActionStartTime;               // 0xc0
    char pad_0xc4[0xcc - 0xc4];
    unsigned long m_nTimeDelta;                     // 0xcc (placeholder name)
    char pad_0xd0[0xd4 - 0xd0];
    unsigned long m_nD4;                            // 0xd4 (placeholder name)
    float m_fAnimationSpeed;                        // 0xd8
    char pad_0xdc[0xe0 - 0xdc];
    short m_nE0;                                    // 0xe0 (placeholder name)
    char pad_0xe2[0xe8 - 0xe2];
    int m_bE8;                                      // 0xe8 (placeholder name)
    char pad_0xec[0xfc - 0xec];
    CExoLinkedListInternal* m_pActions;             // 0xfc
    char pad_0x100[0x124 - 0x100];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    char pad_0x130[0x248 - 0x130];
    Vector m_v248;                                  // 0x248 (placeholder name)
    char pad_0x254[0x298 - 0x254];
    CExoString m_sOnEndDialog;                      // 0x298
    char pad_0x2a0[0x340 - 0x2a0];
    CSWSCreatureFollowInfo* m_pFollowInfo;          // 0x340 (placeholder name)
    char pad_0x344[0x384 - 0x344];
    unsigned long m_nExcitedTimer;                  // 0x384 (placeholder name)
    char pad_0x388[0x4c0 - 0x388];
    CSWSCreaturePartyFollowInfo* m_pPartyFollowInfo;    // 0x4c0
    char pad_0x4c4[0x4d0 - 0x4c4];
    unsigned char m_nDetectMode;                    // 0x4d0
    unsigned char m_nStealthMode;                   // 0x4d1
    char pad_0x4d2[0x4dc - 0x4d2];
    int m_n4DC;                                     // 0x4dc (placeholder name)
    int m_b4E0;                                     // 0x4e0 (placeholder name)
    char pad_0x4e4[0x8e0 - 0x4e4];
    unsigned short m_nModeFeat;                     // 0x8e0 (placeholder name)
    char pad_0x8e2[0x8e8 - 0x8e2];
    int m_b8E8;                                     // 0x8e8 (placeholder name)
    char pad_0x8ec[0x914 - 0x8ec];
    float m_fSpotDistance;                          // 0x914 (placeholder name)
    float m_fListenDistance;                        // 0x918 (placeholder name)
    char pad_0x91c[0x920 - 0x91c];
    CExoArrayList<CSWSCreatureListEntry*> m_l920;   // 0x920 (placeholder name)
    char pad_0x92c[0x93c - 0x92c];
    CSWSCreature93C* m_p93C;                        // 0x93c (placeholder name)
    char pad_0x940[0x944 - 0x940];
    unsigned long m_oid944;                         // 0x944 (placeholder name)
    char pad_0x948[0x960 - 0x948];
    int m_b960;                                     // 0x960 (placeholder name)
    char pad_0x964[0x9c8 - 0x964];
    CSWSCombatRound* m_pCombatRound;                // 0x9c8
    char pad_0x9cc[0x9d4 - 0x9cc];
    int m_b9D4;                                     // 0x9d4 (placeholder name)
    char pad_0x9d8[0x9da - 0x9d8];
    unsigned char m_nBodyBag;                       // 0x9da
    char pad_0x9db[0x9f0 - 0x9db];
    unsigned char m_nPartyFollowFlags;              // 0x9f0
    char pad_0x9f1;
    unsigned char m_n9F2;                           // 0x9f2 (placeholder name)
    char pad_0x9f3[0x9fc - 0x9f3];
    unsigned long m_n9FC;                           // 0x9fc (placeholder name)
    unsigned long m_nA00;                           // 0xa00 (placeholder name)
    char pad_0xa04[0xa08 - 0xa04];
    float m_fA08;                                   // 0xa08 (placeholder name)
    char pad_0xa0c[0xa24 - 0xa0c];
    unsigned long m_oidEncounter;                   // 0xa24
    int m_bEncounterDestroyed;                      // 0xa28

    CSWSInventory* m_pInventory;                    // 0xa2c
    CItemRepository* m_pItemRepository;             // 0xa30
    char pad_0xa34[0xa50 - 0xa34];
    unsigned long m_aEquippedAppearance[4];         // 0xa50 (placeholder name)
    unsigned short m_nAppearanceType;               // 0xa60
    char pad_0xa62[0xa74 - 0xa62];
    CSWSCreatureStats* m_pStats;                    // 0xa74
    char pad_0xa78[0xa88 - 0xa78];
    int m_bA88;                                     // 0xa88 (placeholder name)
    int m_nAIState;                                 // 0xa8c
    char pad_0xa90[0xab0 - 0xa90];
    unsigned long m_nAB0;                           // 0xab0 (placeholder name)
    char pad_0xab4[0xabc - 0xab4];
    unsigned long m_nStealthArg;                    // 0xabc (placeholder name)
    char pad_0xac0[0xac4 - 0xac0];
    int m_bAC4;                                     // 0xac4 (placeholder name)
};

#endif
