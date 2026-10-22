// Pilot group 3: CFactionManager::SaveReputations.

struct CResStruct {
    unsigned long m_nStructIndex;
};
struct CResList;

class CResGFF {
public:
    int AddListElement(CResStruct *pStruct, CResList *pList, unsigned long nType);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
};

class CFactionManager {
public:
    int GetNPCReputation(int nFactionId1, int nFactionId2);
    int SaveReputations(CResGFF *pRes, CResList *pList);

    char pad_0x0[0x1c];
    int m_nNumFactions; // +0x1c
};

// FUNCTION: SWKOTOR 0x0052B830
int CFactionManager::SaveReputations(CResGFF *pRes, CResList *pList)
{
    CResStruct cStruct;
    int nCount = 0;
    for (int i = 0; i <= m_nNumFactions; i++) {
        for (int j = 0; j <= m_nNumFactions; j++) {
            int nRep = GetNPCReputation(j, i);
            if (nRep != 100) {
                pRes->AddListElement(&cStruct, pList, nCount);
                pRes->WriteFieldDWORD(&cStruct, i, "FactionID1");
                pRes->WriteFieldDWORD(&cStruct, j, "FactionID2");
                pRes->WriteFieldDWORD(&cStruct, nRep, "FactionRep");
                nCount++;
            }
        }
    }
    return 1;
}
