// KOTOR_AUTOSAVE_PARAMS and CStatusSummary (0x004B1400-0x004B28E0),
// CServerInfo's destructor and the small leaves around them.
#include "i/autosave_params.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x004B1330
CVirtualMachineCmdImplementer::~CVirtualMachineCmdImplementer()
{
}

// FUNCTION: SWKOTOR 0x004B1360
CServerInfo::~CServerInfo()
{
}

// FUNCTION: SWKOTOR 0x004B1400
int KOTOR_AUTOSAVE_PARAMS::Reset()
{
    m_sStartWaypoint = "";
    m_sLoadMusic = "";
    m_sStartWaypoint = "";
    m_sMovie1 = "";
    m_sMovie2 = "";
    m_sMovie3 = "";
    m_sMovie4 = "";
    m_sMovie5 = "";
    m_sMovie6 = "";
    m_nYear = 0;
    m_nMonth = 0;
    m_nDay = 0;
    m_nHour = 0;
    m_nMinute = 0;
    m_nSecond = 0;
    m_nMillisecond = 0;
    m_nPauseDay = 0;
    m_nPauseTime = 0;
    memset(&m_cStatusSummary, 0, sizeof(m_cStatusSummary));
    return 0;
}

// FUNCTION: SWKOTOR 0x004B2230
KOTOR_AUTOSAVE_PARAMS::~KOTOR_AUTOSAVE_PARAMS()
{
}

// FUNCTION: SWKOTOR 0x004B25D0 SYMBOL: ??0?$CExoLinkedList@VCSWSClient@@@@QAE@XZ
template class CExoLinkedList<CSWSClient>;

// FUNCTION: SWKOTOR 0x004B2640 SYMBOL: ?count@?$CExoArrayList@VCExoString@@@@QAEHVCExoString@@@Z
template <>
int CExoArrayList<CExoString>::count(CExoString t)
{
    int n = 0;
    for (int i = 0; i < m_nSize; i++) {
        if (m_pData[i] == t) {
            n++;
        }
    }
    return n;
}

// FUNCTION: SWKOTOR 0x004B26C0
int CStatusSummary::SaveToGFF(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)(m_nFlags & 1), "DISPLAYSPENDING");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 2) & 1), "ITEMRECEIVED");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 3) & 1), "ITEMLOST");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 4) & 1), "JOURNAL");
    pGFF->WriteFieldBYTE(pStruct, m_nLightShift, "LIGHTSHIFT");
    pGFF->WriteFieldBYTE(pStruct, m_nDarkShift, "DARKSHIFT");
    pGFF->WriteFieldINT(pStruct, m_nCredits, "CREDITS");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 8) & 1), "CREDITSNET");
    pGFF->WriteFieldINT(pStruct, m_nXP, "XP");
    pGFF->WriteFieldINT(pStruct, m_nStealthXP, "STEALTHXP");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 1) & 1), "SOUNDPENDING");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 5) & 1), "LEVELUPSOUND");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 6) & 1), "NEWQUESTSOUND");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)((m_nFlags >> 7) & 1), "COMPLETESOUND");
    return pGFF->WriteFieldINT(pStruct, m_nSuppressed, "SUPPRESSED");
}

// FUNCTION: SWKOTOR 0x004B2840
KOTOR_AUTOSAVE_PARAMS::KOTOR_AUTOSAVE_PARAMS()
{
    Reset();
}

// FUNCTION: SWKOTOR 0x004B28E0
int KOTOR_AUTOSAVE_PARAMS::SaveToGFF(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldCExoString(pStruct, m_sLoadMusic, "LOADMUSIC");
    pGFF->WriteFieldCExoString(pStruct, m_sStartWaypoint, "STARTWAYPOINT");
    pGFF->WriteFieldCExoString(pStruct, m_sMovie1, "MOVIE1");
    pGFF->WriteFieldCExoString(pStruct, m_sMovie2, "MOVIE2");
    pGFF->WriteFieldCExoString(pStruct, m_sMovie3, "MOVIE3");
    pGFF->WriteFieldCExoString(pStruct, m_sMovie4, "MOVIE4");
    pGFF->WriteFieldCExoString(pStruct, m_sMovie5, "MOVIE5");
    pGFF->WriteFieldCExoString(pStruct, m_sMovie6, "MOVIE6");
    pGFF->WriteFieldDWORD(pStruct, m_nYear, "TIME_YEAR");
    pGFF->WriteFieldBYTE(pStruct, m_nMonth, "TIME_MONTH");
    pGFF->WriteFieldBYTE(pStruct, m_nDay, "TIME_DAY");
    pGFF->WriteFieldBYTE(pStruct, m_nHour, "TIME_HOUR");
    pGFF->WriteFieldWORD(pStruct, m_nMinute, "TIME_MINUTE");
    pGFF->WriteFieldWORD(pStruct, m_nSecond, "TIME_SECOND");
    pGFF->WriteFieldWORD(pStruct, m_nMillisecond, "TIME_MILLISECOND");
    pGFF->WriteFieldDWORD(pStruct, m_nPauseDay, "TIME_PAUSEDAY");
    pGFF->WriteFieldDWORD(pStruct, m_nPauseTime, "TIME_PAUSETIME");
    CResStruct cSummary;
    pGFF->AddStructToStruct(&cSummary, pStruct, "STATUSSUMMARY", 0);
    return m_cStatusSummary.SaveToGFF(pGFF, &cSummary);
}

// FUNCTION: SWKOTOR 0x004B13E0
void CUnknown004b13e0::FUN_004b13e0(unsigned long nValue)
{
    m_n00 = 1;
    m_n04 = nValue;
    m_n10 = 1;
    m_n38 = 0;
}

// FUNCTION: SWKOTOR 0x004B14B0
CUnknown004b14b0::CUnknown004b14b0()
{
    m_n04 = 0;
    m_n08 = 0;
    m_n0c = 0;
    m_n10 = 0;
    m_n14 = 0;
    m_n18 = 0;
    m_n1c = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_n28 = 0;
    m_n2c = 0;
    m_n30 = 0;
    m_n34 = 0;
    m_n38 = 0;
    m_n3c = 0;
    m_n00 = 0;
    m_n44 = 0;
    m_oid40 = 0x7f000000;
}
