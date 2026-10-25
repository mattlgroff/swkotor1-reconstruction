// CSWCObject animation-state accessors.

class CAurAnimationBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34();
    virtual float GetAnimationLength(unsigned long nAnimation);  // +0x38
};

class CSWCObject {
public:
    unsigned short GetCurrentAnimation();
    CAurAnimationBase* GetAnimationBase();
    float GetAnimationLength(unsigned long nAnimation);

    char pad_0x0[0x48];
    unsigned short m_nCurrentAnimation;          // +0x48
    char pad_0x4a[0x1e];
    CAurAnimationBase* m_pAnimationBase;         // +0x68
};

// FUNCTION: SWKOTOR 0x0063C060
unsigned short CSWCObject::GetCurrentAnimation()
{
    return m_nCurrentAnimation;
}

// FUNCTION: SWKOTOR 0x0063C070
CAurAnimationBase* CSWCObject::GetAnimationBase()
{
    return m_pAnimationBase;
}

// FUNCTION: SWKOTOR 0x0063C0E0
float CSWCObject::GetAnimationLength(unsigned long nAnimation)
{
    if (m_pAnimationBase == 0)
        return *reinterpret_cast<const float*>(0x0073D764);
    return m_pAnimationBase->GetAnimationLength(nAnimation);
}
