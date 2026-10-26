// Native CSWRules shutdown: release rule tables and their element arrays.
#include "b/exo_string.h"

class CTwoDimArrays {
public:
    ~CTwoDimArrays();                                       // 0x005BFCC0
    void UnLoad2DArrays();                                  // 0x005C5070
};
class CSWBaseItemArray { public: ~CSWBaseItemArray(); };    // 0x005B4730
class CSWSpellArray { public: ~CSWSpellArray(); };          // 0x0059B9E0
class CSWFeat {
public:
    ~CSWFeat();                                             // 0x004015A0, empty
    char pad_0x0[0x48];
};
class CSWClass {
public:
    ~CSWClass();                                            // 0x005BCBB0
    char pad_0x0[0x1a0];
};
class CSWRace {
public:
    ~CSWRace();                                             // 0x005C78D0
    char pad_0x0[0x34];
};
class CSWSkill {
public:
    char pad_0x0[0x10];
    CExoString m_sLabel;
    char pad_0x18[8];
};

class CSWRules {
public:
    virtual ~CSWRules();
    char pad_0x4[0x30];
    CSWBaseItemArray* m_pBaseItemArray;                      // 0x34
    char pad_0x38[0x8c - 0x38];
    CSWSpellArray* m_pSpellArray;                            // 0x8c
    CSWFeat* m_pFeats;                                       // 0x90
    char* m_p94;
    char* m_p98;
    char* m_p9C;
    char* m_pA0;
    char pad_0xa4[8];
    CSWClass* m_pClasses;                                    // 0xac
    CSWRace* m_pRaces;                                       // 0xb0
    CSWSkill* m_pSkills;                                     // 0xb4
    CTwoDimArrays* m_p2DArrays;                              // 0xb8
};

// FUNCTION: SWKOTOR 0x00551440
CSWRules::~CSWRules()
{
    if (m_p2DArrays) {
        m_p2DArrays->UnLoad2DArrays();
        delete m_p2DArrays;
        m_p2DArrays = 0;
    }
    if (m_pBaseItemArray) { delete m_pBaseItemArray; m_pBaseItemArray = 0; }
    if (m_pSpellArray) { delete m_pSpellArray; m_pSpellArray = 0; }
    if (m_pFeats) { delete[] m_pFeats; m_pFeats = 0; }
    if (m_pA0) { delete[] m_pA0; m_pA0 = 0; }
    if (m_pClasses) { delete[] m_pClasses; m_pClasses = 0; }
    if (m_pRaces) { delete[] m_pRaces; m_pRaces = 0; }
    if (m_pSkills) { delete[] m_pSkills; m_pSkills = 0; }
    if (m_p94) { delete[] m_p94; m_p94 = 0; }
    if (m_p98) { delete[] m_p98; m_p98 = 0; }
    if (m_p9C) { delete[] m_p9C; m_p9C = 0; }
}
