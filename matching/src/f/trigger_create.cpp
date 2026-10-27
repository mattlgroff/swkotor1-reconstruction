// CSWSTrigger::CreateNewGeometry (round s8, 0x0058D210).

class Vector {
public:
    Vector();
    float x, y, z;
};

class CSWSRoom;

class CSWSArea {
public:
    float ComputeHeight(const Vector &vPosition, int bWalkCheck, CSWSRoom **ppRoom);
};

class CSWSTrigger {
public:
    void CreateNewGeometry(float fSize, Vector *pUnused, CSWSArea *pArea);

    char pad_0x0[0x90];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0x1e8];
    int m_nVertices;                 // +0x284
    Vector *m_pvVertices;            // +0x288
    char pad_0x28c[0x8];
    int m_nOutlineVertices;          // +0x294
    int *m_pnOutlineVertices;        // +0x298
};

// FUNCTION: SWKOTOR 0x0058D210
void CSWSTrigger::CreateNewGeometry(float fSize, Vector *pUnused, CSWSArea *pArea)
{
    m_nOutlineVertices = 4;
    m_nVertices = 4;
    if (m_pvVertices) {
        delete[] m_pvVertices;
        m_pvVertices = 0;
    }
    if (m_pnOutlineVertices) {
        delete[] m_pnOutlineVertices;
        m_pnOutlineVertices = 0;
    }
    if (!pArea)
        return;
    m_pvVertices = new Vector[4];
    m_pnOutlineVertices = new int[4];

    m_pvVertices[0].x = m_vPosition.x - fSize;
    m_pvVertices[0].y = fSize + m_vPosition.y;
    m_pvVertices[0].z = pArea->ComputeHeight(m_pvVertices[0], 1, 0);
    if (m_pvVertices[0].z == 0.0f)
        m_pvVertices[0].z = m_vPosition.z;
    m_pnOutlineVertices[0] = 0;

    m_pvVertices[1].x = m_vPosition.x - fSize;
    m_pvVertices[1].y = m_vPosition.y - fSize;
    m_pvVertices[1].z = pArea->ComputeHeight(m_pvVertices[1], 1, 0);
    if (m_pvVertices[1].z == 0.0f)
        m_pvVertices[1].z = m_vPosition.z;
    m_pnOutlineVertices[1] = 1;

    m_pvVertices[2].x = fSize + m_vPosition.x;
    m_pvVertices[2].y = m_vPosition.y - fSize;
    m_pvVertices[2].z = pArea->ComputeHeight(m_pvVertices[2], 1, 0);
    if (m_pvVertices[2].z == 0.0f)
        m_pvVertices[2].z = m_vPosition.z;
    m_pnOutlineVertices[2] = 2;

    m_pvVertices[3].x = fSize + m_vPosition.x;
    m_pvVertices[3].y = fSize + m_vPosition.y;
    m_pvVertices[3].z = pArea->ComputeHeight(m_pvVertices[3], 1, 0);
    if (m_pvVertices[3].z == 0.0f)
        m_pvVertices[3].z = m_vPosition.z;
    m_pnOutlineVertices[3] = 3;
}
