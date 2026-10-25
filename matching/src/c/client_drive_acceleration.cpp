class DriveControl {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20(); virtual void V24(); virtual void V28();
    virtual void ResetAcceleration();              // +0x2c
};

class CClientExoAppInternal {
public:
    char pad_0x00[0x2a0];
    DriveControl* m_pDriveControl;

    void ResetDriveAcceleration();
};

// FUNCTION: SWKOTOR 0x005F1400
void CClientExoAppInternal::ResetDriveAcceleration()
{
    if (m_pDriveControl)
        m_pDriveControl->ResetAcceleration();
}
