// Worker A round s9: small CSWSCreature virtual overrides (vtable 0x007470C8) that have no
// functions.csv row. Slot numbers are in a/sws_creature.h; unnamed ones keep placeholder names.
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004EB6B0
void CSWSCreature::Set350And388AndA8(unsigned long nA, unsigned long nB)
{
    m_n350 = nA;
    m_n354 = nB;
    m_n388 = nA;
    m_n38C = nB;
    m_nA8 = nA;
    m_nAC = nB;
}

// FUNCTION: SWKOTOR 0x004F8280
unsigned long CSWSCreature::GetLockOrientationToObject()
{
    return m_oid944;
}

// FUNCTION: SWKOTOR 0x004F8290
CResRef CSWSCreature::GetResRef44()
{
    if (!m_pStats) {
        return CResRef("");
    }
    return m_pStats->m_cResRef44;
}

// FUNCTION: SWKOTOR 0x004F82E0
int CSWSCreature::GetStats54()
{
    if (!m_pStats) {
        return 0;
    }
    return m_pStats->m_n54;
}

// FUNCTION: SWKOTOR 0x004F82F0
unsigned char CSWSCreature::GetStats64()
{
    if (!m_pStats) {
        return Fn4CB190();
    }
    return m_pStats->m_n64;
}

// FUNCTION: SWKOTOR 0x004F8310
CExoLocString& CSWSCreature::GetFirstName()
{
    if (!m_pStats) {
        return *(CExoLocString*)m_aLocName;
    }
    return *(CExoLocString*)m_pStats->m_aFirstName;
}

// FUNCTION: SWKOTOR 0x004F8330
CExoLocString& CSWSCreature::GetLastName()
{
    if (!m_pStats) {
        return *(CExoLocString*)m_aLocName;
    }
    return *(CExoLocString*)m_pStats->m_aLastName;
}

// FUNCTION: SWKOTOR 0x004F8350
int CSWSCreature::Get8F0()
{
    return m_n8F0;
}

// FUNCTION: SWKOTOR 0x004F8360
void CSWSCreature::Set8F0(int nValue)
{
    m_n8F0 = nValue;
}

// FUNCTION: SWKOTOR 0x004F8370
CResRef CSWSCreature::GetPortrait()
{
    if (m_pStats) {
        return m_pStats->m_cPortrait;
    }
    return CResRef("");
}

// FUNCTION: SWKOTOR 0x004F83C0
void CSWSCreature::SetPortrait(const CResRef& cPortrait)
{
    if (m_pStats) {
        m_pStats->SetPortrait(cPortrait);
    }
}
