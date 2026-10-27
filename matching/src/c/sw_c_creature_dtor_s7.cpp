// CSWCCreature destructor (nwccreature.o, worker C s7) with a
// translation-unit-local layout; the shared creature base is not changed.

void __cdecl operator delete(void* p);              // 0x006FA390

class CExoString {
public:
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    ~CExoArrayList() { delete[] element; element = 0; }
};

// Owned objects released through their virtual (deleting) destructor.
class COwnedObject {
public:
    virtual ~COwnedObject();
};

class CModel {
public:
    virtual ~CModel();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual void DetachFromScene();                 // 0x78 (descriptive)
};

class CPathRequest {
public:
    virtual ~CPathRequest();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void Notify(int n);                     // 0x4c
};

class CSWCSpellCastVisual {
public:
    ~CSWCSpellCastVisual();                         // 0x00695F20
};

class CSWCCreatureAppearance {
public:
    ~CSWCCreatureAppearance();                      // 0x004015A0 (empty)
};

struct CSWCCombatAttackData;
struct Vector;
struct CPerceptionRecord;

struct CSWCCreatureNameRecord {
    char pad_0x0[0x18];
    CExoString m_sName;                             // 0x18 (descriptive)
};

class CSWCCreature;

class CClientExoApp {
public:
    void ClearCachedCreatures(CSWCCreature* pCreature);   // 0x005EDF50
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned long g_oidInvalid;            // 0x00746644

class CSWCObject {
public:
    virtual ~CSWCObject();                          // 0x0063E800

    char pad_0x4[0x68 - 0x4];
    CModel* m_pModel;                               // 0x68
};

class CSWCCreature : public CSWCObject {
public:
    virtual ~CSWCCreature();

    void UnloadModel();                             // 0x004015A0 (empty)
    void ClearVisibilityList();                     // 0x006145D0

    char pad_0x6c[0xfc - 0x6c];
    COwnedObject* m_apSoundSources[12];             // 0xfc (descriptive)
    COwnedObject* m_p12C;                           // 0x12c
    COwnedObject* m_p130;                           // 0x130
    COwnedObject* m_p134;                           // 0x134
    char pad_0x138[0x15c - 0x138];
    COwnedObject* m_p15C;                           // 0x15c
    CPathRequest* m_apPathRequests[4];              // 0x160 (descriptive)
    CPathRequest* m_pPathRequest;                   // 0x170
    char pad_0x174[0x17c - 0x174];
    COwnedObject* m_pLoopingSound;                  // 0x17c
    char pad_0x180[0x1bc - 0x180];
    Vector* m_pWayPoints;                           // 0x1bc
    char pad_0x1c0[0x21c - 0x1c0];
    CSWCCreatureAppearance* m_pAppearance;          // 0x21c
    char pad_0x220[0x2b8 - 0x220];
    CSWCCombatAttackData* m_pAttackData;            // 0x2b8 (descriptive)
    CSWCCombatAttackData* m_pDefendData;            // 0x2bc (descriptive)
    char pad_0x2c0[0x2f0 - 0x2c0];
    CSWCSpellCastVisual* m_pCastVisual;             // 0x2f0
    char pad_0x2f4[0x2f8 - 0x2f4];
    COwnedObject* m_pStats;                         // 0x2f8
    char pad_0x2fc[0x30c - 0x2fc];
    CSWCCreatureNameRecord* m_pNameRecord;          // 0x30c (descriptive)
    char pad_0x310[0x360 - 0x310];
    CExoArrayList<CPerceptionRecord*> m_lstPerception;  // 0x360
    COwnedObject* m_pSoundSet;                      // 0x36c
    char pad_0x370[0x3a0 - 0x370];
    CExoArrayList<unsigned long>* m_pVisibleList;   // 0x3a0 (descriptive)
    char pad_0x3a4[0x3c8 - 0x3a4];
    unsigned long m_oidProjectile;                  // 0x3c8
    char pad_0x3cc[0x430 - 0x3cc];
    COwnedObject* m_p430;                           // 0x430
};

// FUNCTION: SWKOTOR 0x006170D0
CSWCCreature::~CSWCCreature()
{
    delete m_p430;
    delete m_pVisibleList;
    if (m_pSoundSet) {
        delete m_pSoundSet;
        m_pSoundSet = 0;
    }
    if (m_pStats) {
        delete m_pStats;
        m_pStats = 0;
    }
    if (m_p15C) {
        delete m_p15C;
        m_p15C = 0;
    }
    if (m_pLoopingSound) {
        delete m_pLoopingSound;
        m_pLoopingSound = 0;
    }
    delete m_pCastVisual;
    delete m_pAppearance;
    if (m_pWayPoints) {
        delete[] m_pWayPoints;
    }
    UnloadModel();
    if (m_pModel) {
        m_pModel->DetachFromScene();
        delete m_pModel;
        m_pModel = 0;
    }
    if (m_pAttackData) {
        delete m_pAttackData;
    }
    if (m_pDefendData) {
        delete m_pDefendData;
    }
    int i;
    for (i = 0; i < 12; i++) {
        delete m_apSoundSources[i];
        m_apSoundSources[i] = 0;
    }
    delete m_p12C;
    for (i = 0; i < 4; i++) {
        if (m_apPathRequests[i]) {
            m_apPathRequests[i]->Notify(0);
            delete m_apPathRequests[i];
            m_apPathRequests[i] = 0;
        }
    }
    if (m_pPathRequest) {
        m_pPathRequest->Notify(0);
        delete m_pPathRequest;
    }
    delete m_p130;
    delete m_p134;
    if (m_pNameRecord) {
        delete m_pNameRecord;
        m_pNameRecord = 0;
    }
    m_oidProjectile = g_oidInvalid;
    ClearVisibilityList();
    g_pAppManager->m_pClientExoApp->ClearCachedCreatures(this);
}
