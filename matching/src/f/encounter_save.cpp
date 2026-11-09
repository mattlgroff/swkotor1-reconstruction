// CSWSEncounter::SaveEncounter (round s10, 0x00591350). Own minimal layout;
// member names follow encounter_load.cpp.

class CExoString {
public:
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    ~CExoLocString();
    void *m_pList;
    void *m_pPad;
};

class CResRef {
public:
    CResRef(const CExoString &s);
    char m_resRef[16];
};

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector &operator-=(const Vector &v);
    float x, y, z;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pElement, CResList *pList, unsigned long nType);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCExoLocString(CResStruct *pStruct, CExoLocString &sValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
};

class CEncounterSpawnPoint {
public:
    Vector m_vPosition;              // +0x0
    float m_fOrientation;            // +0xc
};

class CEncounterListEntry {
public:
    CResRef m_cResRef;               // +0x0
    float m_fCR;                     // +0x10
    float m_fPoints;                 // +0x14
    int m_bAlreadyUsed;              // +0x18
    int m_bTried;                    // +0x1c
    int m_bSingleSpawn;              // +0x20
};

class CSWSEncounter {
public:
    int SaveEncounter(CResGFF *pGFF, CResStruct *pStruct);

    char pad_0x0[0x18];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0x190];
    int m_nFactionId;                // +0x22c
    CExoLocString m_sLocName;        // +0x230
    int m_bActive;                   // +0x238
    int m_bReset;                    // +0x23c
    int m_nResetTime;                // +0x240
    int m_nSpawnOption;              // +0x244
    int m_nDifficultyValue;          // +0x248
    int m_nDifficultyIndex;          // +0x24c
    int m_nRecCreatures;             // +0x250
    int m_nMaxCreatures;             // +0x254
    CEncounterListEntry **m_ppSpawnList; // +0x258
    int m_nSpawnListCount;           // +0x25c
    int m_nSpawnListSize;            // +0x260
    int m_nNumSpawned;               // +0x264
    unsigned long m_nLastHeartbeatDay;  // +0x268
    unsigned long m_nLastHeartbeatTime; // +0x26c
    unsigned long m_nLastSpawnDay;   // +0x270
    unsigned long m_nLastSpawnTime;  // +0x274
    int m_bStarted;                  // +0x278
    int m_bExhausted;                // +0x27c
    int m_nActivateListSize;         // +0x280
    unsigned long *m_poidActivateList; // +0x284
    int m_nActivateListCount;        // +0x288
    int m_bTriggered;                // +0x28c
    Vector *m_pvVertices;            // +0x290
    int m_nVertices;                 // +0x294
    char pad_0x298[0x18];
    CEncounterListEntry *m_pCreatureList; // +0x2b0
    int m_nCreatureListCount;        // +0x2b4
    CEncounterSpawnPoint *m_pSpawnPoints; // +0x2b8
    int m_nSpawnPoints;              // +0x2bc
    int m_nSpawnLevel;               // +0x2c0
    int m_nMaxRespawns;              // +0x2c4
    int m_nNumRespawns;              // +0x2c8
    int m_n2CC;                      // +0x2cc
    float m_fMinCreaturePoints;      // +0x2d0
    float m_fSpawnPoolPoints;        // +0x2d4
    float m_fNextLevelPoints;        // +0x2d8
    float m_fSpawnPoolActive;        // +0x2dc
    unsigned long m_oidLastEntered;  // +0x2e0
    unsigned long m_oidLastLeft;     // +0x2e4
    CExoString m_sScripts[5];        // +0x2e8
    void *m_pFractionalCR;           // +0x310
    int m_nCustomScriptId;           // +0x314
    int m_bPlayerOnly;               // +0x318
};

// FUNCTION: SWKOTOR 0x00591350
int CSWSEncounter::SaveEncounter(CResGFF *pGFF, CResStruct *pStruct)
{
    CResList cList;
    CResStruct cElement;
    Vector vPoint;
    int i;
    pGFF->WriteFieldBYTE(pStruct, m_bActive, "Active");
    pGFF->WriteFieldBYTE(pStruct, m_bReset, "Reset");
    pGFF->WriteFieldINT(pStruct, m_nResetTime, "ResetTime");
    pGFF->WriteFieldINT(pStruct, m_nMaxRespawns, "Respawns");
    pGFF->WriteFieldINT(pStruct, m_nSpawnOption, "SpawnOption");
    pGFF->WriteFieldINT(pStruct, m_nMaxCreatures, "MaxCreatures");
    pGFF->WriteFieldINT(pStruct, m_nRecCreatures, "RecCreatures");
    pGFF->WriteFieldBYTE(pStruct, m_bPlayerOnly, "PlayerOnly");
    pGFF->WriteFieldDWORD(pStruct, m_nFactionId, "Faction");
    pGFF->WriteFieldINT(pStruct, m_nDifficultyIndex, "DifficultyIndex");
    pGFF->WriteFieldINT(pStruct, m_nDifficultyValue, "Difficulty");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.x, "XPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.y, "YPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.z, "ZPosition");
    pGFF->WriteFieldCExoLocString(pStruct, m_sLocName, "LocalizedName");
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[0], "OnEntered");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[1], "OnExit");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[2], "OnHeartbeat");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[3], "OnExhausted");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[4], "OnUserDefined");
    if (m_nVertices > 0) {
        pGFF->AddList(&cList, pStruct, "Geometry");
        for (i = 0; i < m_nVertices; i++) {
            vPoint = m_pvVertices[i];
            vPoint -= m_vPosition;
            pGFF->AddListElement(&cElement, &cList, i);
            pGFF->WriteFieldFLOAT(&cElement, vPoint.x, "X");
            pGFF->WriteFieldFLOAT(&cElement, vPoint.y, "Y");
            pGFF->WriteFieldFLOAT(&cElement, vPoint.z, "Z");
        }
    }
    if (m_nCreatureListCount > 0) {
        pGFF->AddList(&cList, pStruct, "CreatureList");
        for (i = 0; i < m_nCreatureListCount; i++) {
            pGFF->AddListElement(&cElement, &cList, i);
            pGFF->WriteFieldCResRef(&cElement, m_pCreatureList[i].m_cResRef, "ResRef");
            pGFF->WriteFieldFLOAT(&cElement, m_pCreatureList[i].m_fCR, "CR");
            pGFF->WriteFieldBYTE(&cElement, m_pCreatureList[i].m_bSingleSpawn, "SingleSpawn");
        }
    }
    if (m_nSpawnPoints > 0) {
        pGFF->AddList(&cList, pStruct, "SpawnPointList");
        for (i = 0; i < m_nSpawnPoints; i++) {
            pGFF->AddListElement(&cElement, &cList, i);
            pGFF->WriteFieldFLOAT(&cElement, m_pSpawnPoints[i].m_vPosition.x, "X");
            pGFF->WriteFieldFLOAT(&cElement, m_pSpawnPoints[i].m_vPosition.y, "Y");
            pGFF->WriteFieldFLOAT(&cElement, m_pSpawnPoints[i].m_vPosition.z, "Z");
            pGFF->WriteFieldFLOAT(&cElement, m_pSpawnPoints[i].m_fOrientation, "Orientation");
        }
    }
    pGFF->WriteFieldINT(pStruct, m_nNumSpawned, "NumberSpawned");
    pGFF->WriteFieldDWORD(pStruct, m_nLastHeartbeatDay, "HeartbeatDay");
    pGFF->WriteFieldDWORD(pStruct, m_nLastHeartbeatTime, "HeartbeatTime");
    pGFF->WriteFieldDWORD(pStruct, m_nLastSpawnDay, "LastSpawnDay");
    pGFF->WriteFieldDWORD(pStruct, m_nLastSpawnTime, "LastSpawnTime");
    pGFF->WriteFieldBYTE(pStruct, m_bStarted, "Started");
    pGFF->WriteFieldBYTE(pStruct, m_bExhausted, "Exhausted");
    pGFF->WriteFieldINT(pStruct, m_nNumRespawns, "CurrentSpawns");
    pGFF->WriteFieldFLOAT(pStruct, m_fSpawnPoolActive, "SpawnPoolActive");
    pGFF->WriteFieldDWORD(pStruct, m_oidLastEntered, "LastEntered");
    pGFF->WriteFieldDWORD(pStruct, m_oidLastLeft, "LastLeft");
    pGFF->WriteFieldINT(pStruct, m_nCustomScriptId, "CustomScriptId");
    pGFF->WriteFieldINT(pStruct, m_nActivateListSize, "AreaListMaxSize");
    pGFF->WriteFieldFLOAT(pStruct, m_fSpawnPoolPoints, "AreaPoints");
    if (m_nActivateListCount > 0) {
        pGFF->WriteFieldINT(pStruct, m_nActivateListCount, "AreaListSize");
        pGFF->AddList(&cList, pStruct, "AreaList");
        for (i = 0; i < m_nActivateListCount; i++) {
            pGFF->AddListElement(&cElement, &cList, 3);
            pGFF->WriteFieldDWORD(&cElement, m_poidActivateList[i], "AreaObject");
        }
    }
    if (m_nSpawnListCount > 0) {
        pGFF->AddList(&cList, pStruct, "SpawnList");
        for (i = 0; i < m_nSpawnListCount; i++) {
            pGFF->AddListElement(&cElement, &cList, i);
            pGFF->WriteFieldCResRef(&cElement, m_ppSpawnList[i]->m_cResRef, "SpawnResRef");
            pGFF->WriteFieldFLOAT(&cElement, m_ppSpawnList[i]->m_fCR, "SpawnCR");
        }
    }
    return 1;
}
