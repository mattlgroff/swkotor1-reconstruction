// Gob animation-event callbacks and a list-compare helper (worker c, s4).
// Callbacks use the cdecl (gob, event name, user data) signature of the
// other creature events (HitGroundEvent(CAurObject*, char*, void*) in the
// Mac build). These starts are missing from functions.csv; names are
// descriptive.

class CAurObject;

class CSWBehaviorAttachSound {
public:
    int PlaySoundA();                               // 0x00691000
};

class CSWCCreature {
public:
    void BlurWeapon(int bBlur);                     // 0x006116A0
    void PlayFootstep();                            // 0x0061A2D0
};

struct CAurPath {
    char pad_0x0[0x4];
    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
};

struct CNearestStruct {
    unsigned long m_oidObject;                      // 0x00
    float m_fDistance;                              // 0x04
};

// FUNCTION: SWKOTOR 0x00604B50
void RoomSoundEvent(CAurObject*, char*, CSWBehaviorAttachSound* pSound)
{
    if (pSound) {
        pSound->PlaySoundA();
    }
}

// FUNCTION: SWKOTOR 0x0060E1C0
void ClearPathEvent(CAurObject*, char*, CAurPath* pPath)
{
    pPath->m_n4 = 0;
    pPath->m_n8 = 0;
}

// FUNCTION: SWKOTOR 0x00616A00
void WeaponBlurOnEvent(CAurObject*, char*, CSWCCreature* pCreature)
{
    pCreature->BlurWeapon(1);
}

// FUNCTION: SWKOTOR 0x00616A10
void WeaponBlurOffEvent(CAurObject*, char*, CSWCCreature* pCreature)
{
    pCreature->BlurWeapon(0);
}

// FUNCTION: SWKOTOR 0x0061A970
void FootstepEvent(CAurObject*, char*, CSWCCreature* pCreature)
{
    pCreature->PlayFootstep();
}

// FUNCTION: SWKOTOR 0x0061FA90
int NearestObjectCompare(unsigned long oid, CNearestStruct sNearest)
{
    return oid == sNearest.m_oidObject;
}
