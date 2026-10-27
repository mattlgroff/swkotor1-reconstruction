// CClientExoAppInternal selection cycling, worker C s9
// (clientexoappinternal.o; translation-unit-local layout).

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void RemoveIndex(int i);                        // out of line (0x0048DDE0)
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
};

// 12-byte selectable-object entry (descriptive).
struct CSelectableObject {
    unsigned long m_oidObject;                      // 0x00
    unsigned char m_nCanSee;                        // 0x04 (0xff: not computed yet)
    char pad_0x5[3];
    unsigned long m_nUnknown8;                      // 0x08
};

typedef int (*SelectableCompareFn)(unsigned long nKey, CSelectableObject cEntry);

int NearestObjectCompare(unsigned long nKey, CSelectableObject cEntry);  // 0x0061FA90

class CSelectableObjectList : public CExoArrayList<CSelectableObject> {
public:
    int Find(unsigned long nKey, SelectableCompareFn pCompare);   // 0x005F4FB0
};

class CSWCObject;
class CSWCCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c

    unsigned long m_idSelf;                         // 0x04
};

class CSWCObject : public CGameObject {
public:
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134();
    virtual int IsHostile();                        // 0x138 (descriptive)
};

class CSWCCreature : public CSWCObject {
public:
    unsigned char CanSeeObject(CSWCObject* pTarget);    // 0x00617AD0
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CSWParty {
public:
    CSWCCreature* GetPC();                          // 0x00635460
};

class CGuiInGame {
public:
    int TutorialReasonWillShowWindow(int nReason);  // 0x0062F420
    int SetTutorialReason(unsigned char nReason);   // 0x0062B0D0
};

class CClientExoAppInternal {
public:
    CSWCObject* SelectNearestObject(int bBackward, unsigned char nMode);
    void SetSelectedObject(CSWCObject* pObject, int a, int b, int c);   // 0x005F9C60 (descriptive)

    char pad_0x0[0x14];
    CGameObjectArray* m_pGameObjectArray;           // 0x14
    char pad_0x18[0x40 - 0x18];
    CGuiInGame* m_pGuiInGame;                       // 0x40
    char pad_0x44[0x270 - 0x44];
    CSWParty* m_pParty;                             // 0x270
    char pad_0x274[0x2a8 - 0x274];
    CSelectableObjectList m_lstSelectable;          // 0x2a8
    unsigned long m_oidSelected;                    // 0x2b4
    char pad_0x2b8[0x3ac - 0x2b8];
    unsigned long m_oidTutorial3ac;                 // 0x3ac
    unsigned long m_oidTutorial3b0;                 // 0x3b0
    int m_nTutorial3b4;                             // 0x3b4
    unsigned char m_nTutorialReason;                // 0x3b8
};

// NONMATCH (~105 insns): control flow and block layout now follow the
// original; the differences are a callee-saved register permutation (the
// original keeps the list in ebx, the entry offset in edi and the loop index
// in ebp, spilled) and a zero constant our compiler keeps in edx for the
// second loop's tests and initialisations. The out-of-line RemoveIndex
// (0x0048DDE0) in the scan loop and the inline DelIndex in the selection loop
// are both in the original.
// FUNCTION: SWKOTOR 0x005FB050
CSWCObject* CClientExoAppInternal::SelectNearestObject(int bBackward, unsigned char nMode)
{
    if (m_pGuiInGame->TutorialReasonWillShowWindow(8) && m_pGuiInGame->SetTutorialReason(8)) {
        m_oidTutorial3ac = 0x7f000000;
        m_oidTutorial3b0 = 0x7f000000;
        m_nTutorial3b4 = 0;
        m_nTutorialReason = 8;
    }
    int nCount = m_lstSelectable.num;
    if (nCount == 0) {
        return 0;
    }
    CSWCObject* pObject;
    int i;
    if (nMode) {
        for (i = nCount - 1; i >= 0; i--) {
            pObject = 0;
            if (m_pGameObjectArray->GetGameObject(m_lstSelectable.element[i].m_oidObject,
                                                  (CGameObject**)&pObject) == GAME_OBJECT_VALID &&
                pObject) {
                pObject = pObject->AsSWCObject();
            } else {
                pObject = 0;
            }
            if (pObject) {
                CSelectableObject& cEntry = m_lstSelectable.element[i];
                if (cEntry.m_nCanSee == 0xff) {
                    cEntry.m_nCanSee = m_pParty->GetPC()->CanSeeObject(pObject);
                }
                if (m_lstSelectable.element[i].m_nCanSee) {
                    if (pObject->AsSWCCreature() && pObject->IsHostile()) {
                        goto scanned;
                    }
                } else {
                    m_lstSelectable.RemoveIndex(i);
                }
            } else {
                m_lstSelectable.RemoveIndex(i);
            }
        }
        nMode = 0;
    }
scanned:
    if (m_oidSelected != 0x7f000000) {
        i = m_lstSelectable.Find(m_oidSelected, NearestObjectCompare);
        if (i >= 0 && i < m_lstSelectable.num) {
            if (bBackward) {
                if (i) {
                    i--;
                } else {
                    i = m_lstSelectable.num - 1;
                }
            } else if (i < m_lstSelectable.num - 1) {
                i++;
            } else {
                i = 0;
            }
        } else if (bBackward) {
            i = m_lstSelectable.num - 1;
        } else {
            i = 0;
        }
    } else if (bBackward) {
        i = m_lstSelectable.num - 1;
    } else {
        i = 0;
    }

    int nRemaining = m_lstSelectable.num;
    int bFound = 0;
    while (!bFound && m_lstSelectable.num && nRemaining) {
        pObject = 0;
        CGameObject* pGameObject = 0;
        int bRemove = 1;
        if (m_pGameObjectArray->GetGameObject(m_lstSelectable.element[i].m_oidObject, &pGameObject) ==
                GAME_OBJECT_VALID &&
            pGameObject && (pObject = pGameObject->AsSWCObject()) != 0) {
            bRemove = 0;
            if (nMode == 0 || (nMode == 1 && !pObject->IsHostile()) ||
                (nMode == 2 && pObject->AsSWCCreature() && pObject->IsHostile())) {
                CSelectableObject& cEntry = m_lstSelectable.element[i];
                if (cEntry.m_nCanSee == 0xff) {
                    cEntry.m_nCanSee = m_pParty->GetPC()->CanSeeObject(pObject);
                }
                if (m_lstSelectable.element[i].m_nCanSee) {
                    bFound = 1;
                } else {
                    bRemove = 1;
                }
            }
        }
        if (bRemove) {
            m_lstSelectable.DelIndex(i);
        } else if (!bBackward) {
            i++;
        }
        if (bBackward) {
            i--;
            if (i < 0 || i >= m_lstSelectable.num) {
                i = m_lstSelectable.num - 1;
            }
        } else if (i >= m_lstSelectable.num) {
            i = 0;
        }
        nRemaining--;
    }
    if (bFound) {
        m_oidSelected = pObject->m_idSelf;
        SetSelectedObject(pObject, 1, 1, 1);
    } else if (m_lstSelectable.num == 0) {
        m_oidSelected = 0x7f000000;
        SetSelectedObject(0, 1, 0, 1);
    }
    return pObject;
}
