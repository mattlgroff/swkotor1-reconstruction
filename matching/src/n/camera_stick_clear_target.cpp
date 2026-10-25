class CSWCameraOnAStick {
public:
    char pad_0x00[0x80];
    unsigned int m_oidTarget;

    void ClearTarget();
};

// FUNCTION: SWKOTOR 0x006383B0
void CSWCameraOnAStick::ClearTarget()
{
    m_oidTarget = 0x7f000000;
}
