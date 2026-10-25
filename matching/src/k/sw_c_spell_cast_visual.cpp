// Spell-cast model slots recovered from the canonical client binary.
#include <string.h>

#include "k/sw_c_anim_base.h"

class CSWCSpellCastVisual {
public:
    CAurObject* m_pVisual[6];
    char pad_0x18[0x18];

    CSWCSpellCastVisual();
    ~CSWCSpellCastVisual();
    void UnloadConjureVisual();
    void UnloadCastVisual();
};

// FUNCTION: SWKOTOR 0x00695CE0
CSWCSpellCastVisual::CSWCSpellCastVisual()
{
    m_pVisual[0] = 0;
    m_pVisual[1] = 0;
    m_pVisual[2] = 0;
    m_pVisual[3] = 0;
    m_pVisual[4] = 0;
    m_pVisual[5] = 0;
    *(unsigned long*)(pad_0x18 + 0x00) = 0;
    *(unsigned long*)(pad_0x18 + 0x04) = 0;
    *(unsigned long*)(pad_0x18 + 0x08) = 0;
    *(unsigned long*)(pad_0x18 + 0x0c) = 0;
    *(unsigned long*)(pad_0x18 + 0x10) = 0;
    *(unsigned long*)(pad_0x18 + 0x14) = 0;
}

// FUNCTION: SWKOTOR 0x00695D10
void CSWCSpellCastVisual::UnloadConjureVisual()
{
    if (m_pVisual[0] != 0) {
        m_pVisual[0]->AttachToObject(0, 0, 0);
        if (m_pVisual[0] != 0) delete m_pVisual[0];
        m_pVisual[0] = 0;
    }
    if (m_pVisual[1] != 0) {
        m_pVisual[1]->AttachToObject(0, 0, 0);
        if (m_pVisual[1] != 0) delete m_pVisual[1];
        m_pVisual[1] = 0;
    }
    if (m_pVisual[2] != 0) {
        m_pVisual[2]->AttachToObject(0, 0, 0);
        if (m_pVisual[2] != 0) delete m_pVisual[2];
        m_pVisual[2] = 0;
    }
}

// FUNCTION: SWKOTOR 0x00695D90
void CSWCSpellCastVisual::UnloadCastVisual()
{
    if (m_pVisual[3] != 0) {
        m_pVisual[3]->AttachToObject(0, 0, 0);
        if (m_pVisual[3] != 0) delete m_pVisual[3];
        m_pVisual[3] = 0;
    }
    if (m_pVisual[4] != 0) {
        m_pVisual[4]->AttachToObject(0, 0, 0);
        if (m_pVisual[4] != 0) delete m_pVisual[4];
        m_pVisual[4] = 0;
    }
    if (m_pVisual[5] != 0) {
        m_pVisual[5]->AttachToObject(0, 0, 0);
        if (m_pVisual[5] != 0) delete m_pVisual[5];
        m_pVisual[5] = 0;
    }
}

// FUNCTION: SWKOTOR 0x00695F20
CSWCSpellCastVisual::~CSWCSpellCastVisual()
{
    UnloadConjureVisual();
    UnloadCastVisual();
}
