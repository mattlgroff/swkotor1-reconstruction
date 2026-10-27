// CSWSEncounter loading from GFF / templates and spawn-pool helpers
// (0x00590410..0x00593FB0). Layout shared with encounter.cpp, extended here.

#include <math.h>
#include <stdlib.h>

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    char *CStr() const;
    void Format(const char *szFormat, ...);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    void *m_pList;
    void *m_pPad;
};

class CResRef {
public:
    CResRef();
    CResRef(const CExoString &s);
    CResRef operator=(const char *s);
    CResRef operator=(const CResRef &r);
    void CopyToString(CExoString &s) const;
    char *GetResRef();
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int s);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
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
    CResGFF(unsigned short nType, char *sFileType, const CResRef &cResRef);
    virtual ~CResGFF();
    void GetTopLevelStruct(CResStruct *pStruct);
    int GetListCount(CResList *pList);
    int GetListElement(CResStruct *pOut, CResList *pList, unsigned long nIndex);
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);
    CResRef ReadFieldCResRef(CResStruct *pStruct, char *szLabel, int &bSuccess, const CResRef &cDefault);
    char pad_0x4[0x90];
    int m_bLoaded;                   // +0x94
    char pad_0x98[0x8];
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString &sColumn, float *pValue);
    char pad_0x0[0x24];
    int m_nNumRows;                  // +0x24
};

class C2DArrays {
public:
    char pad_0x0[0x9c];
    C2DA *m_pFractionalCRTable;      // +0x9c
    char pad_0xa0[0x8];
    C2DA *m_pEncDifficultyTable;     // +0xa8
    C2DA *GetFractionalCRTable() { return m_pFractionalCRTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;

class CGameObject {
public:
    virtual ~CGameObject();
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSArea;

class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nObjectType, unsigned long oid, int bFlag);
    virtual ~CSWSObject();
    void SetPosition(const Vector &v, int a, int b, int c);
    CSWSArea *GetArea();
    unsigned char GetAIStateReputation(unsigned long oid);
    void UpdateActions(unsigned long nCalendarDay, unsigned long nTimeOfDay, unsigned __int64 nTimer);
    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0xc];
    unsigned long m_nLastUpdateDay;  // +0xa8
    unsigned long m_nLastUpdateTime; // +0xac
    char pad_0xb0[0x1c];
    unsigned long m_nUpdateDelta;    // +0xcc
    char pad_0xd0[0x158];
};

class CSWSCreatureStats {
public:
    unsigned char GetLevel(int bArg);
    char pad_0x0[0x6c];
    int m_bIsPC;                     // +0x6c
};

class CSWSCreature : public CSWSObject {
public:
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50();
    virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void v64(); virtual void v68(); virtual void v6c(); virtual void v70();
    virtual void v74(); virtual void v78(); virtual void v7c(); virtual void v80();
    virtual void v84(); virtual void v88(); virtual void v8c(); virtual void v90();
    virtual int GetDead();           // 0x94
    int GetIsPossessed();            // 0x0063E7F0 (shared zero leaf, placeholder name)
    char pad_0x228[0x7ac];
    int m_b9D4;                      // +0x9d4 (placeholder name)
    char pad_0x9d8[0x9c];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CSWSArea {
public:
    int GetFirstObjectIndiceByX(int *pIndex, float fX);
    char pad_0x0[0x190];
    unsigned long *m_poidObjectsByX; // +0x190
    int m_nObjectsByX;               // +0x194
};

class CSWPartyTable {
public:
    int GetIsNPCObjectMember(unsigned long oid);
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long *pDay, unsigned long *pTime);
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                           unsigned long nTime2, unsigned long *pDays, unsigned long *pTime);
};

class CServerAIMaster {
public:
    void AddObject(CGameObject *pObject, int bFlag);
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
    CSWPartyTable *GetPartyTable();
    CServerAIMaster *GetServerAIMaster();
    CWorldTimer *GetWorldTimer();
};

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers *m_pcExoTimers;       // +0x4
};

extern CExoBase *g_pExoBase;

class CScriptEvent {
public:
    CScriptEvent();
    ~CScriptEvent();
    int GetInteger(int nIndex);
    unsigned long GetObjectID(int nIndex);
    void SetObjectID(int nIndex, unsigned long oid);
    unsigned short m_nType;
    char pad_0x2[0x32];
};

class CVirtualMachine {
public:
    int RunScript(CExoString *psFileName, unsigned long oid, int bOid);
};

extern CVirtualMachine *g_pVirtualMachine;
extern unsigned long EVENT_SIGNAL_EVENT;

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

// Points table indexed by whole challenge ratings 0..50; every encounter
// constructor appends another 51 entries (original behaviour).
class CEncounterPointsTable {
public:
    float *element;
    int num;
    int array_size;
    void Allocate(int s)
    {
        float *pOld = element;
        array_size = s;
        element = new float[array_size];
        for (int i = 0; i < num; i++)
            element[i] = pOld[i];
        if (pOld)
            delete[] pOld;
    }
    void Add(float t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
};

extern CEncounterPointsTable g_lstEncounterCRPoints;

class CEncounterSpawnPoint {
public:
    CEncounterSpawnPoint();
    Vector m_vPosition;              // +0x0
    float m_fOrientation;            // +0xc
};

class CEncounterListEntry {
public:
    CEncounterListEntry()
    {
        m_cResRef = "";
        m_fCR = 0.0f;
        m_fPoints = 0.0f;
        m_bSingleSpawn = 0;
        m_bAlreadyUsed = 0;
        m_bTried = 0;
    }
    CEncounterListEntry &operator=(const CEncounterListEntry &o);
    ~CEncounterListEntry() {}
    CResRef m_cResRef;               // +0x0
    float m_fCR;                     // +0x10
    float m_fPoints;                 // +0x14
    int m_bAlreadyUsed;              // +0x18
    int m_bTried;                    // +0x1c
    int m_bSingleSpawn;              // +0x20
};

class CSWSEncounter : public CSWSObject {
public:
    CSWSEncounter(unsigned long oid);
    virtual ~CSWSEncounter();
    void LoadEncounterSpawnPoints(CResGFF *pRes, CResList &cList);
    int LoadEncounterGeometry(CResGFF *pRes, CResList &cList);
    void ReadEncounterScriptsFromGff(CResGFF *pRes, CResStruct *pStruct);
    void LoadFractionalCRData();
    float CalculatePointsFromCR(float fCR);
    void CalculateSpawnPool(float fCR);
    void TallyEnemyRadiusPoints();
    int InActivateArea(const Vector &vPosition);
    int GetInActivateAreaList(unsigned long oid);
    void AddToActivateAreaList(unsigned long oid);
    void RemoveFromActivateAreaList(unsigned long oid);
    void UpdateActivateAreaList(unsigned long oidEntered);
    int ReadEncounterFromGff(CResGFF *pRes, CResStruct *pStruct, int bUseGeometry, Vector *pPosition);
    int LoadFromTemplate(const CResRef &cResRef, int bUseGeometry, Vector *pPosition);
    void BuildInitialSpawnList(CEncounterListEntry *pList, int *pnCount);
    void PadOutCreatureCount(CEncounterListEntry *pList, int *pnCount);
    void BuildSpawnList();
    int LoadEncounter(CResGFF *pRes, CResStruct *pStruct);
    void SpawnIfAppropriate(unsigned long oid, float fDifficulty);
    void SpawnCreatureFromSpawnList();
    void AIUpdate();
    void RemoveFromActiveCreatureCount(float fCR, int bAddToNextLevel);
    void EventHandler(unsigned long nEventId, unsigned long oidCaller, void *pScript,
                      unsigned long nCalendarDay, unsigned long nTimeOfDay);
    void AddCreaturesToSpawnList(CEncounterListEntry *pList, int *pnCount, int nIndex,
                                 CEncounterListEntry cEntry);

    char pad_0x228[0x4];
    int m_n22C;                      // +0x22c
    CExoLocString m_sLocName;        // +0x230
    int m_bActive;                   // +0x238
    int m_bReset;                    // +0x23c
    int m_nResetTime;                // +0x240
    int m_nSpawnOption;              // +0x244
    int m_nDifficultyValue;          // +0x248
    char pad_0x24c[0x4];
    int m_nRecCreatures;             // +0x250
    int m_nMaxCreatures;             // +0x254
    CExoArrayList<CEncounterListEntry *> m_aSpawnList; // +0x258
    int m_nNumSpawned;               // +0x264
    unsigned long m_nLastHeartbeatDay;  // +0x268
    unsigned long m_nLastHeartbeatTime; // +0x26c
    unsigned long m_nLastSpawnDay;   // +0x270
    unsigned long m_nLastSpawnTime;  // +0x274
    int m_bExhausted;                // +0x278
    char pad_0x27c[0x4];
    int m_nActivateListSize;         // +0x280
    unsigned long *m_poidActivateList; // +0x284
    int m_nActivateListCount;        // +0x288
    int m_bTriggered;                // +0x28c
    Vector *m_pvVertices;            // +0x290
    int m_nVertices;                 // +0x294
    int *m_pnPointIndices;           // +0x298
    int m_nPointIndices;             // +0x29c
    float m_fMinX;                   // +0x2a0
    float m_fMaxX;                   // +0x2a4
    float m_fMinY;                   // +0x2a8
    float m_fMaxY;                   // +0x2ac
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
    float m_f2DC;                    // +0x2dc
    unsigned long m_oidLastEntered;  // +0x2e0
    unsigned long m_oidLastLeft;     // +0x2e4
    CExoString m_sScripts[5];        // +0x2e8
    CExoArrayList<float> *m_pFractionalCR; // +0x310
    int m_nCustomScriptId;           // +0x314
    int m_bPlayerOnly;               // +0x318
};

// Defined here (not in encounter.cpp) so that /Ob2 inlines it into
// BuildInitialSpawnList as in the original translation unit.
// FUNCTION: SWKOTOR 0x00590250
CEncounterListEntry &CEncounterListEntry::operator=(const CEncounterListEntry &o)
{
    m_cResRef = o.m_cResRef;
    m_fCR = o.m_fCR;
    m_fPoints = o.m_fPoints;
    m_bAlreadyUsed = o.m_bAlreadyUsed;
    m_bTried = o.m_bTried;
    m_bSingleSpawn = o.m_bSingleSpawn;
    return *this;
}

// FUNCTION: SWKOTOR 0x00590410
void CSWSEncounter::LoadEncounterSpawnPoints(CResGFF *pRes, CResList &cList)
{
    int bSuccess;
    CResStruct cElement;
    m_nSpawnPoints = pRes->GetListCount(&cList);
    if (m_nSpawnPoints > 0) {
        if (m_pSpawnPoints)
            delete[] m_pSpawnPoints;
        m_pSpawnPoints = new CEncounterSpawnPoint[m_nSpawnPoints];
        for (int i = 0; i < m_nSpawnPoints; i++) {
            if (pRes->GetListElement(&cElement, &cList, i)) {
                m_pSpawnPoints[i].m_vPosition.x = pRes->ReadFieldFLOAT(&cElement, "X", bSuccess, 0.0f);
                m_pSpawnPoints[i].m_vPosition.y = pRes->ReadFieldFLOAT(&cElement, "Y", bSuccess, 0.0f);
                m_pSpawnPoints[i].m_vPosition.z = pRes->ReadFieldFLOAT(&cElement, "Z", bSuccess, 0.0f);
                m_pSpawnPoints[i].m_fOrientation = pRes->ReadFieldFLOAT(&cElement, "Orientation", bSuccess, 0.0f);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00590580
int CSWSEncounter::LoadEncounterGeometry(CResGFF *pRes, CResList &cList)
{
    CResStruct cElement;
    int bSuccess;
    m_nVertices = pRes->GetListCount(&cList);
    m_nPointIndices = m_nVertices;
    if (m_pvVertices)
        delete[] m_pvVertices;
    if (m_nVertices == 0) {
        CExoString sMessage;
        sMessage.Format("Problem loading encounter with tag '%s'.  It has geometry, but no vertices.  Skipping.",
                        m_sTag.CStr());
        return 0;
    }
    m_pvVertices = new Vector[m_nVertices];
    if (m_pnPointIndices)
        delete[] m_pnPointIndices;
    m_pnPointIndices = new int[m_nPointIndices];
    int bFirst = 1;
    for (int i = 0; i < m_nVertices; i++) {
        if (pRes->GetListElement(&cElement, &cList, i)) {
            m_pvVertices[i].x = pRes->ReadFieldFLOAT(&cElement, "X", bSuccess, 0.0f) + m_vPosition.x;
            m_pvVertices[i].y = pRes->ReadFieldFLOAT(&cElement, "Y", bSuccess, 0.0f) + m_vPosition.y;
            m_pvVertices[i].z = pRes->ReadFieldFLOAT(&cElement, "Z", bSuccess, 0.0f) + m_vPosition.z;
            m_pnPointIndices[i] = i;
            if (bFirst) {
                m_fMinX = m_pvVertices[i].x;
                m_fMaxX = m_pvVertices[i].x;
                m_fMinY = m_pvVertices[i].y;
                m_fMaxY = m_pvVertices[i].y;
                bFirst = 0;
            } else {
                if (m_pvVertices[i].x < m_fMinX)
                    m_fMinX = m_pvVertices[i].x;
                if (m_pvVertices[i].x > m_fMaxX)
                    m_fMaxX = m_pvVertices[i].x;
                if (m_pvVertices[i].y < m_fMinY)
                    m_fMinY = m_pvVertices[i].y;
                if (m_pvVertices[i].y > m_fMaxY)
                    m_fMaxY = m_pvVertices[i].y;
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00590820
void CSWSEncounter::ReadEncounterScriptsFromGff(CResGFF *pRes, CResStruct *pStruct)
{
    int bSuccess;
    m_sScripts[0] = pRes->ReadFieldCResRef(pStruct, "OnEntered", bSuccess, m_sScripts[0]).GetResRef();
    m_sScripts[1] = pRes->ReadFieldCResRef(pStruct, "OnExit", bSuccess, m_sScripts[1]).GetResRef();
    m_sScripts[2] = pRes->ReadFieldCResRef(pStruct, "OnHeartbeat", bSuccess, m_sScripts[2]).GetResRef();
    m_sScripts[3] = pRes->ReadFieldCResRef(pStruct, "OnExhausted", bSuccess, m_sScripts[3]).GetResRef();
    m_sScripts[4] = pRes->ReadFieldCResRef(pStruct, "OnUserDefined", bSuccess, m_sScripts[4]).GetResRef();
}

// FUNCTION: SWKOTOR 0x00593270
void CSWSEncounter::LoadFractionalCRData()
{
    C2DA *pTable = g_pRules->Get2DArrays()->GetFractionalCRTable();
    if (pTable) {
        m_pFractionalCR = new CExoArrayList<float>;
        float fValue;
        for (int i = 0; i < 5; i++) {
            pTable->GetFLOATEntry(i, "Min", &fValue);
            m_pFractionalCR->Add(fValue);
        }
    }
}

// FUNCTION: SWKOTOR 0x005919E0
float CSWSEncounter::CalculatePointsFromCR(float fCR)
{
    if (fCR > 0.0f && fCR < 1.0f) {
        if (fCR >= m_pFractionalCR->element[0])
            return 1.0f;
        if (fCR >= m_pFractionalCR->element[1])
            return pow(1.5, -1.0);
        if (fCR >= m_pFractionalCR->element[2])
            return pow(1.5, -2.0);
        if (fCR >= m_pFractionalCR->element[3])
            return pow(1.5, -3.0);
        return pow(1.5, -4.0);
    }
    if (fCR >= 0.0f && fCR <= 50.0f)
        return g_lstEncounterCRPoints.element[(int)fCR];
    return pow(1.5, fCR);
}

// FUNCTION: SWKOTOR 0x005931D0
void CSWSEncounter::CalculateSpawnPool(float fCR)
{
    if (fCR == -1.0f)
        TallyEnemyRadiusPoints();
    else
        m_fSpawnPoolPoints = CalculatePointsFromCR(fCR);
    if (m_fSpawnPoolPoints != 0.0f) {
        m_nSpawnLevel = (int)(log(m_fSpawnPoolPoints * 0.25) / log(1.5) + 0.5) + m_nDifficultyValue;
        m_fNextLevelPoints = CalculatePointsFromCR((float)(m_nSpawnLevel - 1)) + m_fNextLevelPoints;
    }
}

// NONMATCH (26): the second loop keeps the index in eax across the back edge and
// aligns the loop top; the original reloads [esp+10h] at the top, no alignment.
// FUNCTION: SWKOTOR 0x00590DA0
void CSWSEncounter::UpdateActivateAreaList(unsigned long oidEntered)
{
    CSWSArea *pArea = GetArea();
    if (!pArea)
        return;
    for (int i = 0; i < m_nActivateListCount; i++) {
        CSWSCreature *pCreature =
            g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_poidActivateList[i]);
        if (pCreature && !InActivateArea(pCreature->m_vPosition))
            RemoveFromActivateAreaList(m_poidActivateList[i]);
    }
    int nIndex;
    if (!pArea->GetFirstObjectIndiceByX(&nIndex, m_fMinX))
        return;
    for (; nIndex < pArea->m_nObjectsByX; nIndex++) {
        unsigned long oid = pArea->m_poidObjectsByX[nIndex];
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
        if (pCreature) {
            Vector vPosition = pCreature->m_vPosition;
            if (vPosition.x > m_fMaxX)
                return;
            if (pCreature->GetAIStateReputation(m_idSelf) == 2 && !pCreature->GetDead() &&
                !pCreature->GetIsPossessed()) {
                if (oid == oidEntered ||
                    (vPosition.y >= m_fMinY && vPosition.y <= m_fMaxY && InActivateArea(vPosition))) {
                    if (!GetInActivateAreaList(oid))
                        AddToActivateAreaList(oid);
                    if (m_bPlayerOnly && !pCreature->m_pStats->m_bIsPC) {
                        CSWPartyTable *pParty = g_pAppManager->GetServerExoApp()->GetPartyTable();
                        if (!pParty || pParty->GetIsNPCObjectMember(oid) != 1)
                            continue;
                    }
                    m_bTriggered = 1;
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00593A90
int CSWSEncounter::LoadFromTemplate(const CResRef &cResRef, int bUseGeometry, Vector *pPosition)
{
    CResGFF *pGFF = new CResGFF(0x7f8, "UTE ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        CExoString sMessage;
        CExoString sName;
        cResRef.CopyToString(sName);
        sMessage.Format("Encounter template %s doesn't exist.\n", sName.CStr());
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    if (bUseGeometry)
        ReadEncounterFromGff(pGFF, &cTop, bUseGeometry, pPosition);
    else
        ReadEncounterFromGff(pGFF, &cTop, 0, 0);
    ReadEncounterScriptsFromGff(pGFF, &cTop);
    delete pGFF;
    return 1;
}

// Name from the behavioural source: the spawn pool from the levels of the
// hostile creatures within 30m of the creature that entered.
// NONMATCH (26): same loop shape problem as UpdateActivateAreaList (index kept
// in a register across the back edge), plus entry-block register choice.
// FUNCTION: SWKOTOR 0x00591AF0
void CSWSEncounter::TallyEnemyRadiusPoints()
{
    CSWSArea *pArea = GetArea();
    if (!pArea)
        return;
    m_fSpawnPoolPoints = 0.0f;
    CSWSCreature *pEntered = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidLastEntered);
    if (!pEntered)
        return;
    Vector vEntered = pEntered->m_vPosition;
    int nIndex;
    if (!pArea->GetFirstObjectIndiceByX(&nIndex, vEntered.x - 30.0f))
        return;
    for (; nIndex < pArea->m_nObjectsByX; nIndex++) {
        CSWSCreature *pCreature =
            g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pArea->m_poidObjectsByX[nIndex]);
        if (pCreature) {
            Vector vPosition = pCreature->m_vPosition;
            if (vEntered.x + 30.0f < vPosition.x)
                return;
            if (pCreature->GetAIStateReputation(m_idSelf) == 2 && pCreature->GetDead() != 1 &&
                pCreature->GetIsPossessed() != 1 && vEntered.y - 30.0f <= vPosition.y &&
                vEntered.y + 30.0f >= vPosition.y) {
                int nLevel = pCreature->m_pStats->GetLevel(0);
                if (!pCreature->m_b9D4)
                    nLevel = nLevel / 2;
                m_fSpawnPoolPoints = CalculatePointsFromCR((float)nLevel) + m_fSpawnPoolPoints;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00592280
void CSWSEncounter::AddCreaturesToSpawnList(CEncounterListEntry *pList, int *pnCount, int nIndex,
                                            CEncounterListEntry cEntry)
{
    float fPoints = CalculatePointsFromCR(cEntry.m_fCR);
    if (fPoints == m_fMinCreaturePoints)
        return;
    if (*pnCount == 1) {
        *pnCount = 0;
    } else {
        int nRemoved = 0;
        for (int j = 0; j < *pnCount; j++) {
            if (j == nIndex)
                nRemoved++;
            else if (nRemoved)
                pList[j - nRemoved] = pList[j];
        }
        *pnCount -= nRemoved;
    }
    float fThreshold = fPoints - 0.0001f;
    for (int k = 0; k < m_nCreatureListCount; k++) {
        if (*pnCount > m_nRecCreatures - m_nNumSpawned)
            return;
        while (m_pCreatureList[k].m_fPoints <= fPoints && m_pCreatureList[k].m_fPoints <= fThreshold &&
               *pnCount < m_nMaxCreatures &&
               (!m_pCreatureList[k].m_bSingleSpawn || !m_pCreatureList[k].m_bAlreadyUsed)) {
            pList[*pnCount].m_cResRef = m_pCreatureList[k].m_cResRef;
            pList[*pnCount].m_fPoints = m_pCreatureList[k].m_fPoints;
            pList[*pnCount].m_fCR = m_pCreatureList[k].m_fCR;
            m_pCreatureList[k].m_bAlreadyUsed = 1;
            (*pnCount)++;
            fPoints -= m_pCreatureList[k].m_fPoints;
        }
    }
}

// FUNCTION: SWKOTOR 0x005935D0
void CSWSEncounter::BuildSpawnList()
{
    CEncounterListEntry *pList = new CEncounterListEntry[m_nMaxCreatures];
    int nCount = 0;
    BuildInitialSpawnList(pList, &nCount);
    if (nCount < m_nRecCreatures - m_nNumSpawned && m_fNextLevelPoints > m_fMinCreaturePoints) {
        int k;
        do {
            for (k = nCount - 1; k >= 0; k--) {
                if (nCount >= m_nRecCreatures - m_nNumSpawned)
                    break;
                AddCreaturesToSpawnList(pList, &nCount, k, pList[k]);
            }
        } while (k >= 0 && nCount < m_nRecCreatures - m_nNumSpawned);
    }
    if (nCount < m_nRecCreatures - m_nNumSpawned)
        PadOutCreatureCount(pList, &nCount);
    if (m_fNextLevelPoints < m_fMinCreaturePoints)
        m_fNextLevelPoints = 0.0f;
    if (nCount == 0)
        return;
    int nSpawn = nCount;
    for (int i = 0; i < nSpawn; i++) {
        CEncounterListEntry *pEntry = new CEncounterListEntry;
        pEntry->m_cResRef = pList[i].m_cResRef;
        pEntry->m_fCR = pList[i].m_fCR;
        m_aSpawnList.Add(pEntry);
    }
    delete[] pList;
}

// FUNCTION: SWKOTOR 0x00593830
int CSWSEncounter::LoadEncounter(CResGFF *pRes, CResStruct *pStruct)
{
    if (!ReadEncounterFromGff(pRes, pStruct, 0, 0))
        return 0;
    ReadEncounterScriptsFromGff(pRes, pStruct);
    CEncounterListEntry cTemp;
    for (int i = 0; i < m_nCreatureListCount; i++) {
        for (int j = 0; j < m_nCreatureListCount - 1; j++) {
            if (m_pCreatureList[j].m_fCR < m_pCreatureList[j + 1].m_fCR) {
                cTemp.m_fCR = m_pCreatureList[j].m_fCR;
                cTemp.m_cResRef = m_pCreatureList[j].m_cResRef;
                cTemp.m_fPoints = m_pCreatureList[j].m_fPoints;
                cTemp.m_bSingleSpawn = m_pCreatureList[j].m_bSingleSpawn;
                m_pCreatureList[j].m_fCR = m_pCreatureList[j + 1].m_fCR;
                m_pCreatureList[j].m_cResRef = m_pCreatureList[j + 1].m_cResRef;
                m_pCreatureList[j].m_fPoints = m_pCreatureList[j + 1].m_fPoints;
                m_pCreatureList[j].m_bSingleSpawn = m_pCreatureList[j + 1].m_bSingleSpawn;
                m_pCreatureList[j].m_bTried = 0;
                m_pCreatureList[j].m_bAlreadyUsed = 0;
                m_pCreatureList[j + 1].m_fCR = cTemp.m_fCR;
                m_pCreatureList[j + 1].m_cResRef = cTemp.m_cResRef;
                m_pCreatureList[j + 1].m_fPoints = cTemp.m_fPoints;
                m_pCreatureList[j + 1].m_bSingleSpawn = cTemp.m_bSingleSpawn;
                m_pCreatureList[j + 1].m_bTried = 0;
                m_pCreatureList[j + 1].m_bAlreadyUsed = 0;
            }
        }
    }
    CExoString sName;
    for (int k = 0; k < m_nCreatureListCount; k++) {
        m_pCreatureList[k].m_cResRef.CopyToString(sName);
        sName.CStr();
    }
    if (m_nCreatureListCount > 0)
        m_fMinCreaturePoints = m_pCreatureList[m_nCreatureListCount - 1].m_fPoints;
    return 1;
}

// FUNCTION: SWKOTOR 0x00590B10
void CSWSEncounter::PadOutCreatureCount(CEncounterListEntry *pList, int *pnCount)
{
    int nRemaining = m_nRecCreatures - m_nNumSpawned - *pnCount;
    float fPoints = 0.0f;
    while (nRemaining > 0) {
        int k = m_nCreatureListCount - 1;
        if (k < 0)
            return;
        for (;;) {
            {
                CExoString sName;
                m_pCreatureList[k].m_cResRef.CopyToString(sName);
                if (m_pCreatureList[k].m_fPoints > fPoints &&
                    (!m_pCreatureList[k].m_bSingleSpawn || !m_pCreatureList[k].m_bAlreadyUsed)) {
                    fPoints = m_pCreatureList[k].m_fPoints;
                    break;
                }
            }
            if (--k < 0)
                return;
        }
        int nFirst = 0;
        int nLast = 0;
        int bFirst = 1;
        int nSingles = 0;
        for (int i = 0; i < m_nCreatureListCount; i++) {
            if (m_pCreatureList[i].m_fPoints == fPoints) {
                if (bFirst == 1) {
                    nFirst = i;
                    bFirst = 0;
                }
                nLast = i;
                if (m_pCreatureList[i].m_bSingleSpawn == 1)
                    nSingles++;
                m_pCreatureList[i].m_bTried = 0;
            }
        }
        int nRange = nLast - nFirst + 1;
        int nTried = 0;
        do {
            int nIndex = rand() % nRange + nFirst;
            if (!m_pCreatureList[nIndex].m_bSingleSpawn || !m_pCreatureList[nIndex].m_bAlreadyUsed) {
                pList[*pnCount].m_cResRef = m_pCreatureList[nIndex].m_cResRef;
                pList[*pnCount].m_fPoints = m_pCreatureList[nIndex].m_fPoints;
                pList[*pnCount].m_fCR = m_pCreatureList[nIndex].m_fCR;
                (*pnCount)++;
                m_pCreatureList[nIndex].m_bAlreadyUsed = 1;
                nRemaining--;
            }
            if (!m_pCreatureList[nIndex].m_bTried) {
                nTried++;
                m_pCreatureList[nIndex].m_bTried = 1;
            }
            if (nRange == nSingles && nTried == nSingles)
                break;
        } while (nRemaining > 0);
    }
}

// NONMATCH (10, 4 real): register choice for the AddEventDeltaTime argument
// loads (EVENT_SIGNAL_EVENT in edx, the app pointer straight into ecx).
// FUNCTION: SWKOTOR 0x00593FB0
void CSWSEncounter::AIUpdate()
{
    SpawnCreatureFromSpawnList();
    unsigned __int64 nTimer = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    unsigned long nCalendarDay, nTimeOfDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    unsigned long nDayDiff, nTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastUpdateDay, m_nLastUpdateTime, &nDayDiff, &nTimeDiff);
    unsigned long nHBDayDiff, nHBTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastHeartbeatDay, m_nLastHeartbeatTime, &nHBDayDiff, &nHBTimeDiff);
    m_nLastUpdateDay = nCalendarDay;
    m_nLastUpdateTime = nTimeOfDay;
    m_nUpdateDelta = nTimeDiff;
    if (m_nSpawnOption == 1 && m_bReset == 1 && m_nLastSpawnDay && m_nLastSpawnTime) {
        unsigned long nResetDayDiff, nResetTimeDiff;
        g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
            nCalendarDay, nTimeOfDay, m_nLastSpawnDay, m_nLastSpawnTime, &nResetDayDiff, &nResetTimeDiff);
        if ((nResetTimeDiff >= (unsigned long)(m_nResetTime * 1000) || nResetDayDiff >= 1) &&
            (m_nNumRespawns < m_nMaxRespawns || m_nMaxRespawns == -1)) {
            m_nNumRespawns++;
            m_nLastSpawnDay = 0;
            m_nLastSpawnTime = 0;
            m_bActive = 1;
            SpawnIfAppropriate(0x7f000000, -1.0f);
            m_fNextLevelPoints = 0.0f;
            m_f2DC = 0.0f;
        }
    }
    if (!m_bActive)
        return;
    if (nHBTimeDiff >= 6000) {
        if (m_nLastHeartbeatTime) {
            CScriptEvent *pEvent = new CScriptEvent;
            pEvent->m_nType = 0;
            pEvent->SetObjectID(0, m_idSelf);
            g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, m_idSelf, m_idSelf, EVENT_SIGNAL_EVENT, pEvent);
        }
        m_nLastHeartbeatTime = nTimeOfDay;
        m_nLastHeartbeatDay = nCalendarDay;
    }
    UpdateActions(nCalendarDay, nTimeOfDay, nTimer);
}

// Function start missing from functions.csv (it follows AIUpdate's padding).
// FUNCTION: SWKOTOR 0x00594220
void CSWSEncounter::EventHandler(unsigned long nEventId, unsigned long oidCaller, void *pScript,
                                 unsigned long nCalendarDay, unsigned long nTimeOfDay)
{
    if (nEventId == EVENT_SIGNAL_EVENT) {
        CScriptEvent *pEvent = (CScriptEvent *)pScript;
        unsigned short nType = pEvent->m_nType;
        if (nType == 0xc) {
            m_oidLastEntered = pEvent->GetObjectID(0);
            SpawnIfAppropriate(pEvent->GetObjectID(0), -1.0f);
            g_pVirtualMachine->RunScript(&m_sScripts[0], m_idSelf, 1);
        } else if (nType == 0xd) {
            m_oidLastLeft = pEvent->GetObjectID(0);
            UpdateActivateAreaList(0x7f000000);
            g_pVirtualMachine->RunScript(&m_sScripts[1], m_idSelf, 1);
        } else if (nType == 0) {
            g_pVirtualMachine->RunScript(&m_sScripts[2], m_idSelf, 1);
        } else if (nType == 0x15) {
            g_pVirtualMachine->RunScript(&m_sScripts[3], m_idSelf, 1);
        } else if (nType == 0xb) {
            m_nCustomScriptId = pEvent->GetInteger(0);
            g_pVirtualMachine->RunScript(&m_sScripts[4], m_idSelf, 1);
        }
        delete pEvent;
    }
}

// FUNCTION: SWKOTOR 0x00593C70
CSWSEncounter::CSWSEncounter(unsigned long oid) : CSWSObject(0xd, oid, 0)
{
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->AddObject(this, 0);
    unsigned long nDay, nTime;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    m_nLastUpdateDay = nDay;
    m_nLastUpdateTime = nTime;
    m_nLastSpawnDay = 0;
    m_nLastSpawnTime = 0;
    m_nLastHeartbeatTime = 0;
    m_nLastHeartbeatDay = 0;
    m_nCreatureListCount = 0;
    m_pCreatureList = 0;
    m_nActivateListSize = 16;
    m_poidActivateList = new unsigned long[m_nActivateListSize];
    m_nActivateListCount = 0;
    m_bTriggered = 0;
    m_pvVertices = 0;
    m_pnPointIndices = 0;
    m_nVertices = 0;
    m_nPointIndices = 0;
    m_oidLastEntered = 0x7f000000;
    m_nSpawnLevel = 0;
    m_nSpawnPoints = 0;
    m_nNumRespawns = 0;
    m_nNumSpawned = 0;
    m_fNextLevelPoints = 0.0f;
    m_f2DC = 0.0f;
    m_n2CC = 0;
    m_fMinCreaturePoints = 0.0f;
    m_oidLastLeft = 0x7f000000;
    m_pSpawnPoints = 0;
    m_bExhausted = 0;
    for (int i = 0; i <= 50; i++)
        g_lstEncounterCRPoints.Add((float)pow(1.5, i));
    LoadFractionalCRData();
    m_bActive = 1;
    if (m_bExhausted != 1) {
        m_bTriggered = 0;
        UpdateActivateAreaList(0x7f000000);
        if (m_bTriggered == 1) {
            CalculateSpawnPool(-1.0f);
            if (m_fNextLevelPoints > 0.0f) {
                BuildSpawnList();
                m_bExhausted = 1;
            }
        }
    }
    m_nSpawnOption = 0;
    m_bReset = 0;
    m_nResetTime = 60;
    m_n22C = 1;
    m_nDifficultyValue = 0;
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
    m_nMaxRespawns = 0;
    m_bPlayerOnly = 0;
    m_nMaxCreatures = 8;
    m_nRecCreatures = 2;
}

// FUNCTION: SWKOTOR 0x00590090
int CSWSEncounter::InActivateArea(const Vector &vPosition)
{
    int nCrossings = 0;
    int nCount = m_nPointIndices;
    for (int i = 0; i < nCount; i++) {
        int nA = m_pnPointIndices[i];
        int nB = m_pnPointIndices[i != nCount - 1 ? i + 1 : 0];
        Vector *pB = &m_pvVertices[nB];
        Vector *pA = &m_pvVertices[nA];
        if (pB->x != pA->x) {
            float fAY = pA->y;
            float fX = vPosition.x;
            float fDY = pB->y - fAY;
            float fDX = pB->x - pA->x;
            float fY = fAY - (pA->x - vPosition.x) * (fDY / fDX);
            if (fY > vPosition.y) {
                if (fabs(fDY) > fabs(fDX)) {
                    if (pB->y > pA->y) {
                        if (fY >= pA->y && fY < pB->y)
                            nCrossings++;
                    } else if (pA->y > pB->y && fY > pB->y && fY <= pA->y) {
                        nCrossings++;
                    }
                } else {
                    if (pB->x > pA->x) {
                        if (fX >= pA->x && fX < pB->x)
                            nCrossings++;
                    } else if (pA->x > pB->x && fX > pB->x && fX <= pA->x) {
                        nCrossings++;
                    }
                }
            }
        }
        if (pB->x == vPosition.x && (pB->y > vPosition.y || pA->y >= vPosition.y))
            nCrossings++;
    }
    return nCrossings % 2 == 1;
}

// NONMATCH (14): i / fPoints stack slots swapped, and two register choices
// ((*pnCount)++ in ecx; group-count load before the inc).
// FUNCTION: SWKOTOR 0x00590F80
void CSWSEncounter::BuildInitialSpawnList(CEncounterListEntry *pList, int *pnCount)
{
    int i = 0;
    float fSlack = 0.1f * (m_fNextLevelPoints - m_f2DC);
    float fPoints = m_fNextLevelPoints - m_f2DC;
    CEncounterListEntry cEntry;
    if (m_nCreatureListCount <= 0)
        return;
    do {
        if (*pnCount >= m_nMaxCreatures - m_nNumSpawned)
            return;
        if (!(fPoints > 0.0f))
            return;
        cEntry = m_pCreatureList[i];
        if (cEntry.m_bSingleSpawn == 1 && cEntry.m_bAlreadyUsed == 0) {
            pList[*pnCount].m_cResRef = cEntry.m_cResRef;
            pList[*pnCount].m_fPoints = cEntry.m_fPoints;
            pList[*pnCount].m_fCR = cEntry.m_fCR;
            (*pnCount)++;
            cEntry.m_bAlreadyUsed = 1;
        } else if (cEntry.m_fCR != 0.0f) {
            if (cEntry.m_fPoints > fPoints + fSlack) {
            } else if (cEntry.m_fCR == m_pCreatureList[i + 1].m_fCR) {
                int nGroup = 1;
                int nSingles = 1;
                int nTried = 0;
                cEntry.m_bTried = 0;
                int nEnd = i + 1;
                for (; nEnd < m_nCreatureListCount; nEnd++) {
                    if (m_pCreatureList[nEnd].m_fCR != m_pCreatureList[i].m_fCR) {
                        nEnd--;
                        break;
                    }
                    m_pCreatureList[nEnd].m_bTried = 0;
                    if (m_pCreatureList[nEnd].m_bSingleSpawn)
                        nSingles++;
                    nGroup++;
                }
                int nIndex = -1;
                while (m_pCreatureList[nIndex].m_fPoints <= fPoints + fSlack) {
                    if (*pnCount >= m_nMaxCreatures - m_nNumSpawned)
                        break;
                    nIndex = rand() % nGroup + i;
                    if (!m_pCreatureList[nIndex].m_bSingleSpawn || !m_pCreatureList[nIndex].m_bAlreadyUsed) {
                        pList[*pnCount].m_cResRef = m_pCreatureList[nIndex].m_cResRef;
                        pList[*pnCount].m_fPoints = m_pCreatureList[nIndex].m_fPoints;
                        pList[*pnCount].m_fCR = m_pCreatureList[nIndex].m_fCR;
                        (*pnCount)++;
                        m_pCreatureList[nIndex].m_bAlreadyUsed = 1;
                        fPoints -= m_pCreatureList[nIndex].m_fPoints;
                    }
                    if (!m_pCreatureList[nIndex].m_bTried) {
                        m_pCreatureList[nIndex].m_bTried = 1;
                        nTried++;
                    }
                    if (nGroup == nSingles && nTried == nGroup)
                        break;
                }
                i = nEnd + 1;
                continue;
            } else {
                while (cEntry.m_fPoints <= fPoints + fSlack) {
                    if (*pnCount >= m_nMaxCreatures - m_nNumSpawned)
                        break;
                    if (cEntry.m_bSingleSpawn && cEntry.m_bAlreadyUsed)
                        break;
                    pList[*pnCount].m_cResRef = cEntry.m_cResRef;
                    pList[*pnCount].m_fPoints = cEntry.m_fPoints;
                    pList[*pnCount].m_fCR = cEntry.m_fCR;
                    (*pnCount)++;
                    fPoints -= cEntry.m_fPoints;
                    cEntry.m_bAlreadyUsed = 1;
                }
            }
        }
        i++;
    } while (i < m_nCreatureListCount);
}

// Name from the playable source (NWN: RemoveFromActiveCreatureCount); the flag
// returns the creature's points to the next-level pool instead of signalling
// OnExhausted.
// FUNCTION: SWKOTOR 0x00594310
void CSWSEncounter::RemoveFromActiveCreatureCount(float fCR, int bAddToNextLevel)
{
    if (m_nNumSpawned == 0)
        return;
    m_f2DC -= CalculatePointsFromCR(fCR);
    m_nNumSpawned--;
    if (m_f2DC < 0.0f)
        m_f2DC = 0.0f;
    if (bAddToNextLevel == 1) {
        m_fNextLevelPoints += CalculatePointsFromCR(fCR);
        return;
    }
    if (m_nNumSpawned != 0)
        return;
    CScriptEvent *pEvent = new CScriptEvent;
    pEvent->m_nType = 0x15;
    pEvent->SetObjectID(0, m_idSelf);
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
        0, 0, m_idSelf, m_idSelf, EVENT_SIGNAL_EVENT, pEvent);
    if (m_bReset == 1)
        g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&m_nLastSpawnDay,
                                                                        &m_nLastSpawnTime);
    for (int i = 0; i < m_nCreatureListCount; i++) {
        m_pCreatureList[i].m_bTried = 0;
        m_pCreatureList[i].m_bAlreadyUsed = 0;
    }
    m_bActive = 0;
    m_bExhausted = 0;
}
