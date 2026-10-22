// CSWPartyTable small accessors (0x005636B0-0x005646D0). Names are guessed.
#include "b/party_table.h"

#define PARTY_INVALID_OBJECT_ID 0x7f000000

// FUNCTION: SWKOTOR 0x005636B0
int CSWPartyTable::GetNPCAvailable(int nNPC)
{
    if (nNPC >= 0 && nNPC < PARTY_NUM_NPCS) {
        return m_aNPCAvailable[nNPC];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005636D0
unsigned char CSWPartyTable::GetNumAvailableNPCs()
{
    unsigned char nCount = 0;
    for (unsigned char i = 0; i < PARTY_NUM_NPCS; i++) {
        if (m_aNPCAvailable[i]) {
            nCount++;
        }
    }
    return nCount;
}

// FUNCTION: SWKOTOR 0x005636F0
void CSWPartyTable::SetNPCObjectId(int nNPC, unsigned long oid)
{
    if (nNPC >= 0 && nNPC < PARTY_NUM_NPCS && m_aNPCAvailable[nNPC]) {
        m_aNPCObjectIds[nNPC] = oid;
    }
}

// FUNCTION: SWKOTOR 0x00563740
int CSWPartyTable::GetIsNPCObjectMember(unsigned long oid)
{
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        if (oid == m_aNPCObjectIds[i]) {
            for (int j = 0; j < m_nNumMembers; j++) {
                if (m_aMembers[j] == i) {
                    return 1;
                }
            }
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00563790
int CSWPartyTable::GetIsNPCObject(unsigned long oid)
{
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        if (oid == m_aNPCObjectIds[i]) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005637C0
unsigned char CSWPartyTable::GetNPCSelectable(int nNPC)
{
    if (nNPC >= 0 && nNPC < PARTY_NUM_NPCS && m_aNPCAvailable[nNPC]) {
        return m_aNPCSelectable[nNPC];
    }
    return 0xff;
}

// FUNCTION: SWKOTOR 0x005637F0
void CSWPartyTable::SetNPCSelectable(int nNPC, unsigned char nValue)
{
    if (nNPC >= 0 && nNPC < PARTY_NUM_NPCS && m_aNPCAvailable[nNPC]) {
        m_aNPCSelectable[nNPC] = nValue;
    }
}

// FUNCTION: SWKOTOR 0x005638A0
int CSWPartyTable::GetNPCIndexOfObject(unsigned long oid)
{
    if (oid == PARTY_INVALID_OBJECT_ID) {
        return -1;
    }
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        if (m_aNPCObjectIds[i] == oid) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x005639C0
void CSWPartyTable::ClearNPCObjectIds()
{
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        m_aNPCObjectIds[i] = PARTY_INVALID_OBJECT_ID;
    }
}

// FUNCTION: SWKOTOR 0x005639E0
void CSWPartyTable::ClearNPCObjectId(int nNPC)
{
    if (nNPC >= 0 && nNPC < PARTY_NUM_NPCS) {
        m_aNPCObjectIds[nNPC] = PARTY_INVALID_OBJECT_ID;
    }
}

// FUNCTION: SWKOTOR 0x00563A00
int CSWPartyTable::GetIsSelectedNPCObject(unsigned long oid)
{
    if (oid == PARTY_INVALID_OBJECT_ID) {
        return 0;
    }
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        if (oid == m_aNPCObjectIds[i]) {
            return m_nEC == i;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00563A40
void CSWPartyTable::SetSelectedNPCObject(unsigned long oid)
{
    m_nEC = -1;
    if (oid == PARTY_INVALID_OBJECT_ID) {
        return;
    }
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        if (oid == m_aNPCObjectIds[i]) {
            m_nEC = i;
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x00563AB0
void CSWPartyTable::SetPartyValues(int a, int b, int c)
{
    m_n100 = a;
    m_n104 = b;
    m_n108 = c;
}

// FUNCTION: SWKOTOR 0x00563AE0
void CSWPartyTable::SetTokenValue(unsigned int nIndex, int nValue)
{
    if (nIndex < 16) {
        m_aTokenValues[nIndex] = nValue;
    }
}

// FUNCTION: SWKOTOR 0x00563B00
int CSWPartyTable::GetTokenValue(unsigned int nIndex)
{
    if (nIndex < 16) {
        return m_aTokenValues[nIndex];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00563B20
void CSWPartyTable::SetToken(unsigned int nIndex, int nValue)
{
    if (nIndex < 16) {
        m_aTokens[nIndex] = nValue;
    }
}

// FUNCTION: SWKOTOR 0x00563B40
int CSWPartyTable::GetToken(unsigned int nIndex)
{
    if (nIndex < 16) {
        return m_aTokens[nIndex];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00563B60
void CSWPartyTable::SetLeaderToken(int nIndex)
{
    if (nIndex >= 0 && nIndex < 16 && m_aTokens[nIndex] && m_aTokenValues[nIndex]) {
        m_nLeaderNPC = nIndex;
    }
}

// FUNCTION: SWKOTOR 0x005646D0
int CSWPartyTable::RemoveAvailableNPC(int nNPC)
{
    if (!GetNPCAvailable(nNPC)) {
        return 0;
    }
    m_aNPCAvailable[nNPC] = 0;
    return 1;
}
