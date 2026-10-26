// Creature subarea updates during movement; unnamed layout members are descriptive.
struct Vector { float x, y, z; };
template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayList() : element(0), num(0), array_size(0) {}
    ~CExoArrayList() { delete[] element; }
    CExoArrayList& operator=(const CExoArrayList& other); // 0x0051B720 for unsigned long
};
class CGameObjectArray;
class CServerExoApp {
public:
    CGameObjectArray* GetObjectArray();              // 0x004AED70
};
class CAppManager {
public:
    void* vptr;
    void* m_pClient;
    CServerExoApp* m_pServer;
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
    char pad_0x0[0x330];
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
