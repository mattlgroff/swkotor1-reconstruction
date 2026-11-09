// Creature subarea updates during movement; unnamed layout members are descriptive.
struct Vector { float x, y, z; };
template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { delete[] element; }
    CExoArrayList& operator=(const CExoArrayList& other); // 0x0051B720 for unsigned long
    void Allocate(int nSize);                        // 0x005E03C0
    T& operator[](int i) { return element[i]; }
    void Add(T t) {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = t;
        num++;
    }
    void AddUnique(T t) {
        int nFound = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                nFound++;
            }
        }
        if (!nFound) {
            Add(t);
        }
    }
};
class CSWSArea;
class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24();
    virtual CSWSArea* AsSWSArea();                   // 0x28
};
class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject); // 0x004D8230
};
class CSWSAreaOfEffectObject {
public:
    void JumpToPoint(CSWSArea* pArea, const Vector& vPosition); // 0x00596360
};
class CServerExoApp {
public:
    CGameObjectArray* GetObjectArray();              // 0x004AED70
    CSWSAreaOfEffectObject* GetAreaOfEffectByGameObjectID(unsigned long oid); // 0x004AE7D0
};
extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern int g_bSubAreaTriggerOverride;               // 0x007A1B54 (descriptive name)
class CAppManager {
public:
    void* vptr;
    void* m_pClient;
    CServerExoApp* m_pServer;
    CServerExoApp* GetServer() { return m_pServer; }
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
class CSWSArea {
public:
    int IntersectingLineSegment(const Vector& from, const Vector& to,
                                CExoArrayList<unsigned long>* pAreas); // 0x004BF2C0
    int InSubAreas(const Vector& position, CExoArrayList<unsigned long>* pAreas); // 0x004BF470
};
class CSWSCreature {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    int HandleSubAreaStateChanges(CExoArrayList<unsigned long>* pNow,
                                  CExoArrayList<unsigned long>* pCrossed,
                                  int bTrigger, int bArg); // 0x00516020 (descriptive)
    int UpdateSubareasOnMoveTo(const Vector& from, const Vector& to, int bUseCrossed,
                               CExoArrayList<unsigned long>* pCrossed, int bTrigger);
    int UpdateSubareasOnJumpPosition(const Vector& vPosition, unsigned long oidArea, int bTrigger); // 0x0051B940
    char pad_0x0[0x324];
    CExoArrayList<unsigned long> m_lAreaOfEffects;  // +0x324 (descriptive name)
    CExoArrayList<unsigned long> m_lSubAreas;       // +0x330
};
// NONMATCH (7): list initialization/destruction stores and frame scheduling.
// FUNCTION: SWKOTOR 0x0051B7B0
int CSWSCreature::UpdateSubareasOnMoveTo(const Vector& from, const Vector& to, int bUseCrossed,
                                         CExoArrayList<unsigned long>* pCrossed, int bTrigger)
{
    CExoArrayList<unsigned long> now;
    int changed = 0;
    g_pAppManager->m_pServer->GetObjectArray();
    CSWSArea* pArea = GetArea();
    if (!pArea)
        return 0;
    int bHadAny = m_lSubAreas.num > 0;
    int bInAny = pArea->InSubAreas(to, &now);
    if (!bUseCrossed) {
        CExoArrayList<unsigned long> swept;
        pArea->IntersectingLineSegment(from, to, &swept);
        if (bHadAny || bInAny) {
            if (HandleSubAreaStateChanges(&now, &swept, bTrigger, 1) == 1) {
                m_lSubAreas = now;
                changed = 1;
            }
        }
    } else if (bHadAny || bInAny) {
        if (HandleSubAreaStateChanges(&now, pCrossed, bTrigger, 1) == 1) {
            m_lSubAreas = now;
            changed = 1;
        }
    }
    return changed;
}

// FUNCTION: SWKOTOR 0x0051B940
int CSWSCreature::UpdateSubareasOnJumpPosition(const Vector& vPosition, unsigned long oidArea, int bTrigger)
{
    CExoArrayList<unsigned long> now;
    CGameObjectArray* pArray = g_pAppManager->m_pServer->GetObjectArray();
    if (g_bSubAreaTriggerOverride)
        bTrigger = 1;
    CSWSArea* pArea;
    if (oidArea != OBJECT_INVALID) {
        CGameObject* pObject;
        pArray->GetGameObject(oidArea, &pObject);
        pArea = pObject->AsSWSArea();
    } else {
        pArea = 0;
    }
    int bHadAny = m_lSubAreas.num > 0;
    int changed = 0;
    int bInAny;
    if (pArea)
        bInAny = pArea->InSubAreas(vPosition, &now);
    else
        bInAny = 0;
    for (int i = 0; i < m_lAreaOfEffects.num; i++) {
        CSWSAreaOfEffectObject* pAoe =
            g_pAppManager->GetServer()->GetAreaOfEffectByGameObjectID(m_lAreaOfEffects[i]);
        if (pAoe) {
            pAoe->JumpToPoint(pArea, vPosition);
            now.AddUnique(m_lAreaOfEffects[i]);
        }
    }
    if (bHadAny || bInAny) {
        if (HandleSubAreaStateChanges(&now, 0, 0, bTrigger) == 1) {
            m_lSubAreas = now;
            changed = 1;
        }
    }
    return changed;
}
