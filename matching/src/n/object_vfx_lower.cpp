class ObjectAnimation {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20(); virtual void V24(); virtual void V28();
    virtual unsigned int GetValue();             // +0x2c
};

class ObjectWithAnimation {
public:
    char pad_0x00[0x68];
    ObjectAnimation* m_pAnimation;

    unsigned int GetVfxValue();
};

// FUNCTION: SWKOTOR 0x0063D460
unsigned int ObjectWithAnimation::GetVfxValue()
{
    if (!m_pAnimation)
        return 0;
    return m_pAnimation->GetValue();
}
