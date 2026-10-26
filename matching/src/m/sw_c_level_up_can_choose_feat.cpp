// CSWCLevelUpStats::CanChooseFeat (nwccreaturestats.o): whether the feat can
// still be picked with the remaining normal and bonus feat slots.

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void SetSize(int n)
    {
        if (n) {
            array_size = n;
            element = new T[n];
        }
        num = n;
    }
};

#include <stddef.h>

class CSWFeat;

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCCreatureStats {
public:
    unsigned short GetNumFeats() { return (unsigned short)m_lstFeats.num; }
    unsigned short GetFeat(unsigned short nIndex)
    {
        if (nIndex >= m_lstFeats.num) {
            return 0xffff;
        }
        return m_lstFeats.element[nIndex];
    }

    char pad_0x0[0xb0];
    CExoArrayList<unsigned short> m_lstFeats;       // 0xb0
};

class CSWCLevelUpStats : public CSWCCreatureStats {
public:
    void SetNormalBonusFlags(unsigned short nFeat, int* pbNormal, int* pbBonus);   // 0x00649110
    int FeatRequirementsMet(unsigned short nFeat, CExoArrayList<unsigned short>* pList);  // 0x0064A2E0
    int CanChooseFeat(unsigned short nFeat, unsigned char nNormal, unsigned char nBonus,
                      CExoArrayList<unsigned short>* pList);
};

// FUNCTION: SWKOTOR 0x0064A4C0
int CSWCLevelUpStats::CanChooseFeat(unsigned short nFeat, unsigned char nNormal,
                                    unsigned char nBonus, CExoArrayList<unsigned short>* pList)
{
    int bNormal = 0;
    int bBonus = 0;
    CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
    if (pFeat == NULL) {
        return 0;
    }
    if (nNormal == 0 && nBonus == 0) {
        return 0;
    }
    unsigned short i;
    int nNumFeats = m_lstFeats.num;
    for (i = 0; i < (unsigned short)nNumFeats; i++) {
        if (nFeat == GetFeat(i)) {
            return 0;
        }
    }
    for (i = 0; i < pList->num; i++) {
        if (nFeat == pList->element[i]) {
            return 0;
        }
    }
    if (!FeatRequirementsMet(nFeat, pList)) {
        return 0;
    }
    SetNormalBonusFlags(nFeat, &bNormal, &bBonus);
    if (!bNormal && !bBonus) {
        return 0;
    }

    int bTaken = 0;
    unsigned char nNormalLeft = nNormal;
    unsigned char nBonusLeft = nBonus;
    CExoArrayList<int> lstCounted;
    lstCounted.SetSize(pList->num);
    for (int j = 0; j < pList->num; j++) {
        lstCounted.element[j] = 0;
    }
    for (i = 0; i < pList->num; i++) {
        SetNormalBonusFlags(pList->element[i], &bNormal, &bBonus);
        if (bNormal) {
            if (!bBonus) {
                if (nNormalLeft == 0) {
                    return 0;
                }
                nNormalLeft--;
                lstCounted.element[i] = 1;
            }
        } else if (bBonus) {
            if (nBonusLeft != 0) {
                nBonusLeft--;
                lstCounted.element[i] = 1;
            }
        }
    }

    SetNormalBonusFlags(nFeat, &bNormal, &bBonus);
    if (bNormal) {
        if (!bBonus) {
            if (nNormalLeft == 0) {
                return 0;
            }
            nNormalLeft--;
            bTaken = 1;
        }
    } else if (bBonus) {
        if (nBonusLeft == 0) {
            return 0;
        }
        nBonusLeft--;
        bTaken = 1;
    }

    for (i = 0; i < pList->num; i++) {
        if (lstCounted.element[i] == 0) {
            if (nNormalLeft != 0) {
                nNormalLeft--;
            } else if (nBonusLeft != 0) {
                nBonusLeft--;
            } else {
                return 0;
            }
        }
    }
    if (!bTaken && nNormalLeft == 0 && nBonusLeft == 0) {
        return 0;
    }
    return 1;
}
