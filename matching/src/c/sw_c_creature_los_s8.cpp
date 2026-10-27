// CSWCCreature line-of-sight test, worker C s8 (nwccreature.o;
// translation-unit-local layout).

struct Vector {
    float x, y, z;
};

class CGob;

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);                           // 0x005E03C0 family
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
};

class CGameObject;

// Shared scene hit-test record (static in the original).
class CHitInfo {
public:
    void ClearOutput();                             // 0x00456A60

    char pad_0x0[0x8];
    CExoArrayList<CGob*> m_lstIgnore;               // 0x08
    char pad_0x14[0x38 - 0x14];
    CGameObject* m_pHitObject;                      // 0x38 (descriptive)
    char pad_0x3c[0x5c - 0x3c];
    unsigned char m_bIgnoreList;                    // 0x5c (descriptive)
};

extern CHitInfo g_cHitInfo;                         // 0x007B93A8

class Scene;
int SceneHitCheck(Scene* pScene, Vector vFrom, Vector vTo, int b, void* pHit);   // 0x0060F1E0

class CSWSDoor {
public:
    char pad_0x0[0x2cc];
    unsigned char m_bOpen;                          // 0x2cc (descriptive)
};

class CSWCArea {
public:
    char pad_0x0[0x184];
    Scene* m_pScene;                                // 0x184
};

class CSWCObject;
class CSWCDoor;
class CSWCPlaceable;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CGameObject* GetGameObject();           // 0x70 (descriptive)
};

class CSWCObject : public CGameObject {
public:
    virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
    virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13C();
    virtual float GetEyeHeight();                   // 0x140 (descriptive)

    char pad_0x4[0x20 - 0x4];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
};

class CSWCDoor : public CSWCObject {
public:
    CSWSDoor* GetServerDoor();                      // 0x00683EE0
};

class CSWCPlaceable : public CSWCObject {
};

class CSWCCreature : public CSWCObject {
public:
    int CanSeeObject(CSWCObject* pTarget);
};

// Descriptive name: scene line-of-sight test from the creature's eyes
// (+1.5) to the target's eye height. Closed doors block; open doors and
// placeables hit on the way are ignored and the test repeated.
// NONMATCH (119 diff insns): MSVC rotates the hit-test loop (duplicating
// the ClearOutput/SceneHitCheck head at the bottom; for/while/do/goto forms
// all give the same), the original enters an aligned loop top with a jmp.
// Also this/ignore-gob registers (ebx/edi) and the two Vector slots swap.
// FUNCTION: SWKOTOR 0x00617AD0
int CSWCCreature::CanSeeObject(CSWCObject* pTarget)
{
    if (!pTarget) {
        return 0;
    }
    Vector vTo;
    Vector vFrom = m_vPosition;
    vFrom.z += 1.5f;
    vTo = pTarget->m_vPosition;
    vTo.z += pTarget->GetEyeHeight();
    g_cHitInfo.ClearOutput();
    g_cHitInfo.m_bIgnoreList = 1;
    g_cHitInfo.m_lstIgnore.num = 0;
    g_cHitInfo.m_lstIgnore.Add(FindPart(0xff, 1));
    CGob* pGob = FindPart(0xfe, 1);
    if (pGob) {
        g_cHitInfo.m_lstIgnore.Add(pGob);
    }
    g_cHitInfo.m_lstIgnore.Add(pTarget->FindPart(0xff, 1));
    pGob = pTarget->FindPart(0xfe, 1);
    if (pGob) {
        g_cHitInfo.m_lstIgnore.Add(pGob);
    }
    int nResult;
    for (;;) {
        g_cHitInfo.ClearOutput();
        nResult = SceneHitCheck(m_pArea->m_pScene, vFrom, vTo, 0, &g_cHitInfo);
        if (nResult != 2) {
            break;
        }
        CGameObject* pObject = g_cHitInfo.m_pHitObject->GetGameObject();
        if (!pObject) {
            nResult = 1;
            break;
        }
        CSWCObject* pBlocker;
        if (pObject->AsSWCDoor()) {
            CSWSDoor* pServerDoor = pObject->AsSWCDoor()->GetServerDoor();
            if (pServerDoor && !pServerDoor->m_bOpen) {
                nResult = 1;
                break;
            }
            pBlocker = pObject->AsSWCDoor();
        } else {
            if (!pObject->AsSWCPlaceable()) {
                break;
            }
            pBlocker = pObject->AsSWCPlaceable();
        }
        g_cHitInfo.m_lstIgnore.Add(pBlocker->FindPart(0xff, 1));
    }
    return nResult != 1;
}
