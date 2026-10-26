// The existing exact Clear method at 0050E270 establishes CSWSPathData's
// 0x74-byte vector/identifier layout. The playable CAvoidCreature alias is
// not used as a separate class owner.
class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    float x, y, z;
};
class CSWSPathData {
public:
    CSWSPathData();
    void Clear();                                   // 0x0050E270
    int m_nField00;
    Vector m_aPoints[6];                            // 0x04
    int m_nField4c, m_nField50;
    Vector m_v54;
    int m_nField60, m_nField64;
    Vector m_v68;
};
// FUNCTION: SWKOTOR 0x00514AB0
CSWSPathData::CSWSPathData()
{
    m_nField00 = 0;
    Clear();
}
