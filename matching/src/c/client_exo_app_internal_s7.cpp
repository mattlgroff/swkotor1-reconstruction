// CClientExoAppInternal::GetObjectName (clientexoappinternal.o, worker C s7)
// with a translation-unit-local layout.

#include "c/exo_base.h"

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

// Inline constructor; MSVC 7.0 expands it once in GetObjectName and emits the
// second use out of line (0x00414DC0, debug map: clientexoappinternal.o).
struct STR_RES {
    CExoString m_sString;                           // 0x00
    CResRef m_resSound;                             // 0x08
    char pad_0x18[0x1c - 0x18];
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strres, unsigned char nGender);   // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern const unsigned char g_nGender;               // 0x0073ECD0
extern const unsigned char GAME_OBJECT_ARRAY_SUCCESS;   // 0x0074666C
extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char OBJECT_TYPE_TRIGGER;     // 0x00746636
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern int g_nPlaceablesCol_StrRef;                 // 0x007A2258

class C2DA {
public:
    int GetINTEntry(int nRow, int nColumn, int* pValue);   // 0x00413510
};

class CTwoDimArrays {
public:
    char pad_0x0[0x2c];
    C2DA* m_pPlaceables;                            // 0x2c
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCCreatureStats {
public:
    CExoString GetFullName();                       // 0x00647CE0
};

class CSWCCreature;
class CSWCDoor;
class CSWCItem;
class CSWCTrigger;
class CSWCPlaceable;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30();
    virtual CSWCItem* AsSWCItem();                  // 0x34
    virtual void v38();
    virtual CSWCTrigger* AsSWCTrigger();            // 0x3c
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCObject : public CGameObject {
public:
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CExoString GetName();                   // 0x70 (descriptive)
};

class CSWCCreature : public CSWCObject {
public:
    char pad_0xc[0x2f8 - 0xc];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

class CSWCPlaceable : public CSWCObject {
public:
    char pad_0xc[0x10c - 0xc];
    unsigned char m_nAppearance;                    // 0x10c
};

class CSWCDoor : public CSWCObject {
};

class CSWCTrigger : public CSWCObject {
};

class CSWCItem {
public:
    CExoString GetDisplayName(int b);               // 0x00646E70 (descriptive)
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

class CClientPlayer {
public:
    char pad_0x0[0x2c];
    unsigned long m_oidPlayer;                      // 0x2c
    CExoString m_sPlayerName;                       // 0x30
};

class CClientExoAppInternal {
public:
    int GetObjectName(unsigned long oid, CExoString* pName);

    char pad_0x0[0x14];
    CGameObjectArray* m_pGameObjectArray;           // 0x14
    char pad_0x18[0x1c - 0x18];
    CExoLinkedList<CClientPlayer> m_lstPlayerList;  // 0x1c
};

// NONMATCH (117 diff insns): block layout. The original keeps the player-list
// fallback directly after the lookup test and the type dispatch after it (our
// compiler moves the early-return list block to the end, as in the
// GetSoundSetStrres/GetPlayerByPlayerID ties); its frame is also 0xc larger.
// FUNCTION: SWKOTOR 0x005F6640
int CClientExoAppInternal::GetObjectName(unsigned long oid, CExoString* pName)
{
    STR_RES strres;
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_ARRAY_SUCCESS) {
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
                *pName = pObject->AsSWCCreature()->m_pStats->GetFullName();
                return 1;
            }
            if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                int bNoName = pObject->AsSWCPlaceable()->GetName() == "";
                if (bNoName) {
                    int nStrRef;
                    g_pRules->m_p2DArrays->m_pPlaceables->GetINTEntry(pObject->AsSWCPlaceable()->m_nAppearance,
                                                                      g_nPlaceablesCol_StrRef, &nStrRef);
                    g_pTlkTable->Fetch(nStrRef, strres, g_nGender);
                    *pName = strres.m_sString;
                } else {
                    *pName = pObject->AsSWCPlaceable()->GetName();
                }
                return 1;
            }
            if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                *pName = pObject->AsSWCDoor()->GetName();
                return 1;
            }
            if (pObject->m_nObjectType == OBJECT_TYPE_ITEM) {
                *pName = pObject->AsSWCItem()->GetDisplayName(1);
                return 1;
            }
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                *pName = pObject->AsSWCTrigger()->GetName();
                if (*pName == "") {
                    STR_RES strresDefault;
                    g_pTlkTable->Fetch(1421, strresDefault, g_nGender);
                    *pName = strresDefault.m_sString;
                }
                return 1;
            }
            *pName = "";
            return 0;
        }
    }
    CExoLinkedListPosition pos = m_lstPlayerList.GetHeadPos();
    while (pos) {
        CClientPlayer* pPlayer = (CClientPlayer*)m_lstPlayerList.m_pcExoLinkedListInternal->GetAtPos(pos);
        if (pPlayer && pPlayer->m_oidPlayer == oid) {
            *pName = pPlayer->m_sPlayerName;
            return 1;
        }
        m_lstPlayerList.m_pcExoLinkedListInternal->GetNext(pos);
    }
    *pName = "";
    return 0;
}
