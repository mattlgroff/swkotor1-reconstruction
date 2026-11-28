// CSWTlkTable derives the displayed gender from the object's creature data.

class CSWCObject;
class CSWCCreature;
class CSWSCreature;

#include "shared/game_object.h"

class CSWCCreatureAppearance {
public:
    char pad_0x0[0x31];
    unsigned char m_nGender;                          // +0x31
};

class CSWCCreature : public CGameObject {
public:
    char pad_0x0C[0x2F8 - 0x0C];
    CSWCCreatureAppearance* m_pAppearance;            // +0x2F8
};

class CSWTlkTable {
public:
    char pad_0x0[0x58];
    CGameObject* m_pObject;                           // +0x58

    unsigned char GetObjectGender();
};

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x00674B40 SYMBOL: ?GetObjectGender@CSWTlkTable@@QAEEXZ
unsigned char CSWTlkTable::GetObjectGender()
{
    if (m_pObject != 0 && m_pObject->AsSWCObject() != 0) {
        if (m_pObject->AsSWCCreature() != 0) {
            if (m_pObject->AsSWCCreature()->m_pAppearance != 0) {
                return m_pObject->AsSWCCreature()->m_pAppearance->m_nGender;
            }
        }
    }
    return g_nDefaultObjectGender;
}
