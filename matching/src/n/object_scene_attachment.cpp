// CSWCObject scene attachment forwarding.

class CAurScene;

class CAurAnimationBase {
public:
    virtual void v00();
    virtual void AttachToScene(CAurScene* pScene);             // +0x04
};

class CSWCObject {
public:
    void AttachToScene(CAurScene* pScene);

    char pad_0x0[0x68];
    CAurAnimationBase* m_pAnimationBase;                     // +0x68
};

// FUNCTION: SWKOTOR 0x0063D820
void CSWCObject::AttachToScene(CAurScene* pScene)
{
    if (m_pAnimationBase != 0)
        m_pAnimationBase->AttachToScene(pScene);
}
