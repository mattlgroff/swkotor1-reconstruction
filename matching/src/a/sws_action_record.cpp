// Action record used by last-update object queues.
class Vector {
public:
    Vector() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector(float a, float b, float c) : x(a), y(b), z(c) {}
    float x, y, z;
};
class CSWSActionRecord {
public:
    CSWSActionRecord();
    void Reset() {
        m_nAction = 0xffff;
        m_nParam2 = 0xffff;
        m_nParam4 = 0xffff;
        m_vTarget = Vector(0.0f, 0.0f, 0.0f);
        m_oidTarget = 0x7f000000;
        m_nGroup = -1;
    }
    unsigned short m_nAction, m_nParam2, m_nParam4;
    int m_nGroup;
    Vector m_vTarget;
    unsigned long m_oidTarget;
};

// FUNCTION: SWKOTOR 0x004EADD0
CSWSActionRecord::CSWSActionRecord()
{
    Reset();
}
