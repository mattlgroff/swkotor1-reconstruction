// CSWSTrigger geometry loader (round s8, 0x0058D060).

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator+(const Vector &v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    float x, y, z;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int GetListCount(CResList *pList);
    int GetListElement(CResStruct *pOut, CResList *pList, unsigned long nIndex);
    unsigned long GetStructID(CResStruct *pStruct);
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);
};

class CSWSTrigger {
public:
    void LoadGeometry(CResGFF *pRes, CResList *pList);

    char pad_0x0[0x90];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0x1e8];
    int m_nVertices;                 // +0x284
    Vector *m_pvVertices;            // +0x288
    char pad_0x28c[0x8];
    int m_nOutlineVertices;          // +0x294
    int *m_pnOutlineVertices;        // +0x298
};

// FUNCTION: SWKOTOR 0x0058D060
void CSWSTrigger::LoadGeometry(CResGFF *pRes, CResList *pList)
{
    CResStruct cElement;
    int bSuccess;
    m_nVertices = pRes->GetListCount(pList);
    m_nOutlineVertices = m_nVertices;
    if (m_pvVertices) {
        delete[] m_pvVertices;
        m_pvVertices = 0;
    }
    if (m_pnOutlineVertices) {
        delete[] m_pnOutlineVertices;
        m_pnOutlineVertices = 0;
    }
    m_pvVertices = new Vector[m_nVertices];
    m_pnOutlineVertices = new int[m_nVertices];
    for (int i = 0; i < m_nVertices; i++) {
        if (pRes->GetListElement(&cElement, pList, i) && pRes->GetStructID(&cElement) == 3) {
            Vector vPoint;
            vPoint.x = pRes->ReadFieldFLOAT(&cElement, "PointX", bSuccess, 0.0f);
            vPoint.y = pRes->ReadFieldFLOAT(&cElement, "PointY", bSuccess, 0.0f);
            vPoint.z = pRes->ReadFieldFLOAT(&cElement, "PointZ", bSuccess, 0.0f);
            m_pvVertices[i] = vPoint + m_vPosition;
            m_pnOutlineVertices[i] = i;
        }
    }
}
