// Scene::HitCheckGob, the nearest gob hit along a segment (0x0044fe00).
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class Vector {
public:
    float x, y, z;
};

class Model {
public:
    char pad00[0x50];
    unsigned short m_bNoHitCheck : 1;           // +0x50 bit 0
    unsigned short m_bb1 : 1;
    unsigned short m_bb2 : 1;
    unsigned short m_bb3 : 1;
    unsigned short m_bb4 : 1;
    unsigned short m_bb5 : 1;
    unsigned short m_bb6 : 1;
    unsigned short m_bb7 : 1;
    unsigned short m_bb8 : 1;
    unsigned short m_bb9 : 1;
    unsigned short m_bb10 : 1;

    unsigned char GetNoHitCheck() { return m_bNoHitCheck; }
    unsigned char GetB1() { return m_bb1; }
    unsigned char GetB2() { return m_bb2; }
    unsigned char GetB4() { return m_bb4; }
    unsigned char GetB5() { return m_bb5; }
    unsigned char GetB7() { return m_bb7; }
    unsigned char GetB10() { return m_bb10; }
};

class Gob;

class CHitInfo {
public:
    char pad00[0x08];
    CExoArrayList<Gob*> m_lIgnore;              // +0x08
    char pad14[0x5c - 0x14];
    unsigned char m_b5c;                        // +0x5c
    char pad5d[0x60 - 0x5d];
    unsigned char m_bAll;                       // +0x60
};

class Gob {
public:
    virtual void V000(); virtual void V004(); virtual void V008(); virtual void V00c();
    virtual void V010(); virtual void V014(); virtual void V018(); virtual void V01c();
    virtual void V020(); virtual void V024(); virtual void V028(); virtual void V02c();
    virtual void V030(); virtual void V034(); virtual void V038(); virtual void V03c();
    virtual void V040(); virtual void V044(); virtual void V048(); virtual void V04c();
    virtual void V050(); virtual void V054(); virtual void V058(); virtual void V05c();
    virtual void V060(); virtual void V064(); virtual void V068(); virtual void V06c();
    virtual void V070(); virtual void V074(); virtual void V078(); virtual void V07c();
    virtual void V080(); virtual void V084(); virtual void V088();
    virtual int HitCheck(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo);  // +0x8c

    char pad04[0x58 - 0x04];
    Model* m_pModel;                            // +0x58
    char pad5c[0x17c - 0x5c];
    unsigned char m_bHidden;                    // +0x17c
};

class Scene {
public:
    char pad00[0x54];
    CExoArrayList<Gob*> m_lGobs;                // +0x54

    int HitCheckGob(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo);
};

// FUNCTION: SWKOTOR 0x0044fe00
// NONMATCH (6): the original materialises the bit 7 and bit 10 tests with
// `and dl, 1` and a dead byte store into the pInfo argument slot; ours
// tests them in place (tried bool accessors and a function-scope flag).
int Scene::HitCheckGob(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo)
{
    int nResult = 0;
    unsigned char bFlag;
    for (int i = 0; i < m_lGobs.num; i++) {
        Gob* pGob = m_lGobs.element[i];
        Model* pModel = pGob->m_pModel;
        if (pModel) {
            if (pModel->GetNoHitCheck() && !pInfo->m_bAll)
                continue;
            if (pGob->m_bHidden)
                continue;
            if (pModel->GetB1())
                continue;
            bFlag = pModel->GetB7();
            if (bFlag)
                continue;
        }
        if (pInfo->m_b5c && pModel) {
            if (pModel->GetB2())
                continue;
            if (pModel->GetB4())
                continue;
            if (pModel->GetB5()) {
                bFlag = pModel->GetB10();
                if (!bFlag)
                    continue;
            }
        }
        int nIgnored = 0;
        for (int j = 0; j < pInfo->m_lIgnore.num; j++)
            if (pInfo->m_lIgnore.element[j] == pGob)
                nIgnored++;
        if (nIgnored)
            continue;
        int n = pGob->HitCheck(vStart, vEnd, &vEnd, pInfo);
        if (n)
            nResult = n;
    }
    if (nResult && pHit)
        *pHit = vEnd;
    return nResult;
}
