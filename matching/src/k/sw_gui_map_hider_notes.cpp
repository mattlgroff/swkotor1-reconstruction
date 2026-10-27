// CSWGuiMapHider map-note list (swguiingamemap.o): the hider keeps the ids of
// the current area's waypoints that carry a map note and walks them for the
// in-game map's note arrows. Names follow the playable reconstruction
// (InitializeMapNotes, GetNextMapNote, GetPrevMapNote) and are descriptive.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
};

class CExoLocString {
public:
    int GetString(unsigned long nLanguage, CExoString* pString, unsigned char nGender) const;  // 0x005EA130
    char pad_0x0[0x8];
};

class Vector {
public:
    float x;
    float y;
    float z;
};

typedef void* CExoLinkedListPosition;

class CExoLinkedListNode {
public:
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* GetPrev(CExoLinkedListPosition& pos);     // 0x005E9BD0
    void* AddHead(void* pObject);                   // 0x005E9D70
    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListPosition GetHeadPos() { return m_pList->m_pHead; }
    CExoLinkedListPosition GetTailPos() { return m_pList->m_pTail; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pList->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pList->GetNext(pos); }
    T* GetPrev(CExoLinkedListPosition& pos) { return (T*)m_pList->GetPrev(pos); }
    void AddHead(T* pObject) { m_pList->AddHead(pObject); }
    CExoLinkedListInternal* m_pList;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CSWSWaypoint {
public:
    char pad_0x0[0x90];
    Vector m_vPosition;                             // 0x90
    char pad_0x9c[0x228 - 0x9c];
    int m_bHasMapNote;                              // 0x228
    int m_bMapNoteEnabled;                          // 0x22c
    CExoLocString m_sMapNote;                       // 0x230
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54();
    virtual CSWSWaypoint* AsSWSWaypoint();          // 0x58
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);  // 0x004D8230
};

class CSWSAreaMap {
public:
    int IsWorldPointExplored(Vector vWorld);        // 0x00579210
};

class CSWSArea {
public:
    char pad_0x0[0x190];
    CExoArrayList<unsigned long> m_aGameObjects;    // 0x190
};

class CSWSModule {
public:
    CSWSArea* GetArea();                            // 0x004C30F0
    char pad_0x0[0x218];
    CSWSAreaMap* m_pAreaMap;                        // 0x218
};

class CServerExoApp {
public:
    CSWSModule* GetModule();                        // 0x004AE6B0
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
    CGameObjectArray* GetObjectArray();             // 0x004AED70
};

class CClientExoApp {
public:
    unsigned long GetClientLanguage();              // 0x005ED4C0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

struct CSWGuiMapNote {
    unsigned long m_oidNote;
};

class CSWGuiMapHider {
public:
    CExoString InitializeMapNotes(int bFlag);
    CExoString GetNextMapNote();
    CExoString GetPrevMapNote();

    void* m_pVTable;                                // 0x00 (vtable 0x00754738)
    char pad_0x4[0x238 - 0x4];
    CExoLinkedList<CSWGuiMapNote> m_lMapNotes;      // 0x238
    CExoLinkedListPosition m_posSelectedNote;       // 0x23c
    char pad_0x240[0x244 - 0x240];
    int m_bMapNoteFlag;                             // 0x244 (descriptive)
};

// FUNCTION: SWKOTOR 0x00692C70
CExoString CSWGuiMapHider::InitializeMapNotes(int bFlag)
{
    m_bMapNoteFlag = bFlag;
    CExoString sNote("");
    CGameObjectArray* pObjects = g_pAppManager->m_pServerExoApp->GetObjectArray();
    CSWSArea* pArea = g_pAppManager->m_pServerExoApp->GetModule()->GetArea();
    for (int i = 0; i < pArea->m_aGameObjects.num; i++) {
        unsigned long oid = pArea->m_aGameObjects.element[i];
        CGameObject* pObject;
        if (pObjects->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
            CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
            if (pWaypoint && pWaypoint->m_bHasMapNote) {
                CSWGuiMapNote* pNote = new CSWGuiMapNote;
                pNote->m_oidNote = oid;
                m_lMapNotes.AddHead(pNote);
                if (m_posSelectedNote == 0 && pWaypoint->m_bMapNoteEnabled) {
                    CSWSAreaMap* pAreaMap = g_pAppManager->m_pServerExoApp->GetModule()->m_pAreaMap;
                    if (pAreaMap->IsWorldPointExplored(pWaypoint->m_vPosition)) {
                        m_posSelectedNote = m_lMapNotes.GetHeadPos();
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        pWaypoint->m_sMapNote.GetString(pClient->GetClientLanguage(), &sNote, 0);
                    }
                }
            }
        }
    }
    return sNote;
}

// FUNCTION: SWKOTOR 0x00692E80
CExoString CSWGuiMapHider::GetNextMapNote()
{
    CExoString sNote("");
    CExoLinkedListPosition pos = m_posSelectedNote;
    unsigned long oid = 0x7f000000;
    if (pos) {
        oid = m_lMapNotes.GetAtPos(pos)->m_oidNote;
    }
    CGameObject* pObject = g_pAppManager->m_pServerExoApp->GetGameObject(oid);
    if (pObject && pObject->AsSWSWaypoint()) {
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
        pWaypoint->m_sMapNote.GetString(pClient->GetClientLanguage(), &sNote, 0);
    }
    if (pos) {
        m_lMapNotes.GetNext(pos);
    }
    if (!pos) {
        pos = m_lMapNotes.GetHeadPos();
    }
    while (pos && m_posSelectedNote && pos != m_posSelectedNote) {
        pObject = g_pAppManager->m_pServerExoApp->GetGameObject(m_lMapNotes.GetAtPos(pos)->m_oidNote);
        if (pObject && pObject->AsSWSWaypoint()) {
            CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
            if (pWaypoint->m_bMapNoteEnabled) {
                CSWSAreaMap* pAreaMap = g_pAppManager->m_pServerExoApp->GetModule()->m_pAreaMap;
                if (pAreaMap->IsWorldPointExplored(pWaypoint->m_vPosition)) {
                    m_posSelectedNote = pos;
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    pWaypoint->m_sMapNote.GetString(pClient->GetClientLanguage(), &sNote, 0);
                    break;
                }
            }
        }
        m_lMapNotes.GetNext(pos);
        if (!pos) {
            pos = m_lMapNotes.GetHeadPos();
        }
    }
    return sNote;
}

// FUNCTION: SWKOTOR 0x00693090
CExoString CSWGuiMapHider::GetPrevMapNote()
{
    CExoString sNote("");
    CExoLinkedListPosition pos = m_posSelectedNote;
    unsigned long oid = 0x7f000000;
    if (pos) {
        oid = m_lMapNotes.GetAtPos(pos)->m_oidNote;
    }
    CGameObject* pObject = g_pAppManager->m_pServerExoApp->GetGameObject(oid);
    if (pObject && pObject->AsSWSWaypoint()) {
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
        pWaypoint->m_sMapNote.GetString(pClient->GetClientLanguage(), &sNote, 0);
    }
    if (pos) {
        m_lMapNotes.GetPrev(pos);
    }
    if (!pos) {
        pos = m_lMapNotes.GetTailPos();
    }
    while (pos && m_posSelectedNote && pos != m_posSelectedNote) {
        pObject = g_pAppManager->m_pServerExoApp->GetGameObject(m_lMapNotes.GetAtPos(pos)->m_oidNote);
        if (pObject && pObject->AsSWSWaypoint()) {
            CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
            if (pWaypoint->m_bMapNoteEnabled) {
                CSWSAreaMap* pAreaMap = g_pAppManager->m_pServerExoApp->GetModule()->m_pAreaMap;
                if (pAreaMap->IsWorldPointExplored(pWaypoint->m_vPosition)) {
                    m_posSelectedNote = pos;
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    pWaypoint->m_sMapNote.GetString(pClient->GetClientLanguage(), &sNote, 0);
                    break;
                }
            }
        }
        m_lMapNotes.GetPrev(pos);
        if (!pos) {
            pos = m_lMapNotes.GetTailPos();
        }
    }
    return sNote;
}
