// Pilot group 3: CSWSMessage::UpdateLastUpdateVisibilityList.

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    T &operator[](int i) { return element[i]; }

    void Allocate(int s)
    {
        T *old = element;
        array_size = s;
        element = new T[s];
        for (int i = 0; i < num; i++)
            element[i] = old[i];
        if (old)
            delete[] old;
    }

    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }

    void DelIndex(int i)
    {
        num--;
        while (i < num) {
            element[i] = element[i + 1];
            i++;
        }
    }
};

class CSWSObjectVisibility {
public:
    CSWSObjectVisibility()
    {
        m_oidObject = 0x7f000000;
        m_bSeen = 0;
        m_bHeard = 0;
        m_bWasSeen = 0;
        m_bWasHeard = 0;
        m_bVisible = 0;
    }
    ~CSWSObjectVisibility() {}
    unsigned long m_oidObject;
    unsigned char m_bSeen : 1;
    unsigned char m_bHeard : 1;
    unsigned char m_bWasSeen : 1;
    unsigned char m_bWasHeard : 1;
    unsigned char m_bVisible : 1;
};

class CSWSCreature {
public:
    char pad_0x0[0x920];
    CExoArrayList<CSWSObjectVisibility *> m_lstVisibility; // +0x920
};

class CSWSPlayerLastUpdateObject {
public:
    char pad_0x0[0x158];
    CExoArrayList<CSWSObjectVisibility *> m_lstVisibility; // +0x158
};

class CSWSMessage {
public:
    void UpdateLastUpdateVisibilityList(CSWSCreature *pCreature, CSWSPlayerLastUpdateObject *pLUO);
};

// FUNCTION: SWKOTOR 0x005707A0
void CSWSMessage::UpdateLastUpdateVisibilityList(CSWSCreature *pCreature, CSWSPlayerLastUpdateObject *pLUO)
{
    if (pLUO->m_lstVisibility.num > 0) {
        for (int i = pLUO->m_lstVisibility.num - 1; i >= 0; i--) {
            CSWSObjectVisibility *pOld = pLUO->m_lstVisibility[i];
            pLUO->m_lstVisibility.DelIndex(i);
            delete pOld;
        }
    }

    for (int j = 0; j < pCreature->m_lstVisibility.num; j++) {
        CSWSObjectVisibility *pSrc = pCreature->m_lstVisibility[j];
        CSWSObjectVisibility *pNew = new CSWSObjectVisibility;
        pNew->m_oidObject = pSrc->m_oidObject;
        pNew->m_bHeard = pSrc->m_bHeard;
        pNew->m_bSeen = pSrc->m_bSeen;
        pNew->m_bVisible = pSrc->m_bVisible;
        pLUO->m_lstVisibility.Add(pNew);
    }
}
