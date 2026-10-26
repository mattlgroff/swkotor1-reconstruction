// Sound behavior leaves: member layout follows the 0x00691000/30 binary accesses.
#include "k/sw_c_ambient_sound.h"

struct Vector {
    float x, y, z;
};

class CAurPart;

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual Vector GetPosition();                      // +0x64
};

class CAurBehaviorAttach {
public:
    CAurBehaviorAttach(CAurObject* pGob, CAurObject* pObject, CAurPart* pPart); // 0x0049FB10
    virtual ~CAurBehaviorAttach();                     // 0x0049FCE0
    virtual void UpdateScenes();
    virtual int Control(float fDelta);
    virtual void Message(char* szMessage);
    virtual int GetType();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void UpdatePosition();
    char pad_0x4[0x10 - 0x4];
    CAurObject* m_pObject;                              // 0x10
    char pad_0x14[0x3c - 0x14];
};

class CSWBehaviorAttachSound : public CAurBehaviorAttach {
public:
    CResRef m_cSound;                                   // 0x3c
    CExoStreamingSoundSource* m_pStream;                // 0x4c

    CSWBehaviorAttachSound(CAurObject* pGob, CAurObject* pObject, CAurPart* pPart,
                           const CResRef& cSound);      // 0x00690EF0
    int PlaySoundA();
    virtual ~CSWBehaviorAttachSound();                  // 0x00691030
    virtual int Control(float fDelta);                  // 0x00690FC0
};

// FUNCTION: SWKOTOR 0x00690EF0
CSWBehaviorAttachSound::CSWBehaviorAttachSound(CAurObject* pGob, CAurObject* pObject,
                                               CAurPart* pPart, const CResRef& cSound)
    : CAurBehaviorAttach(pGob, pObject, pPart)
{
    m_cSound = cSound;
    m_pStream = new CExoStreamingSoundSource;
    m_pStream->SetLooping(1);
    m_pStream->Set3D(1);
    m_pStream->SetPriorityGroup(0x19);
    m_pStream->SetDistances(20.0f, 150.0f);
}

// Not in the function table (follows the constructor).
// FUNCTION: SWKOTOR 0x00690FC0
int CSWBehaviorAttachSound::Control(float fDelta)
{
    int result = CAurBehaviorAttach::Control(fDelta);
    Vector position = m_pObject->GetPosition();
    m_pStream->SetPosition(position);
    return result;
}

// FUNCTION: SWKOTOR 0x00691000
int CSWBehaviorAttachSound::PlaySoundA()
{
    int playing = m_pStream->IsPlaying();
    if (playing != 0) {
        return playing;
    }
    return m_pStream->Play(m_cSound, 4, playing, playing, playing);
}

// FUNCTION: SWKOTOR 0x00691030
CSWBehaviorAttachSound::~CSWBehaviorAttachSound()
{
    if (m_pStream != 0) {
        m_pStream->Stop();
        delete m_pStream;
        m_pStream = 0;
    }
}

// FUNCTION: SWKOTOR 0x006910B0 SYMBOL: ??_GCSWBehaviorAttachSound@@UAEPAXI@Z
// CSWBehaviorAttachSound::`scalar deleting destructor' (compiler generated).
