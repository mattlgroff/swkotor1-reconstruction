// Sound behavior leaves: member layout follows the 0x00691000/30 binary accesses.
#include "k/sw_c_ambient_sound.h"

class CAurBehaviorAttach {
public:
    virtual ~CAurBehaviorAttach();                     // 0x0049FCE0
    virtual void UpdateScenes();
    virtual int Control(float fDelta);
    virtual void Message(char* szMessage);
    virtual int GetType();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void UpdatePosition();
    char pad_0x4[0x38];
};

class CSWBehaviorAttachSound : public CAurBehaviorAttach {
public:
    CResRef m_cSound;                                   // 0x3c
    CExoStreamingSoundSource* m_pStream;                // 0x4c

    int PlaySoundA();
    virtual ~CSWBehaviorAttachSound();                  // 0x00691030
};

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
