// Save-game GFF readers of swguisaveload.o: the status summary and the PC
// autosave module-start parameters.
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006C8490
void CStatusSummary::LoadFromGFF(CResGFF* pGFF, CResStruct& rStruct)
{
    int bSuccess;
    m_bDisplaysPending = pGFF->ReadFieldBYTE(&rStruct, "DISPLAYSPENDING", bSuccess, m_bDisplaysPending);
    m_bItemReceived = pGFF->ReadFieldBYTE(&rStruct, "ITEMRECEIVED", bSuccess, m_bItemReceived);
    m_bItemLost = pGFF->ReadFieldBYTE(&rStruct, "ITEMLOST", bSuccess, m_bItemLost);
    m_bJournal = pGFF->ReadFieldBYTE(&rStruct, "JOURNAL", bSuccess, m_bJournal);
    m_nLightShift = pGFF->ReadFieldBYTE(&rStruct, "LIGHTSHIFT", bSuccess, m_nLightShift);
    m_nDarkShift = pGFF->ReadFieldBYTE(&rStruct, "DARKSHIFT", bSuccess, m_nDarkShift);
    m_nCredits = pGFF->ReadFieldINT(&rStruct, "CREDITS", bSuccess, m_nCredits);
    m_bCreditsNet = pGFF->ReadFieldBYTE(&rStruct, "CREDITSNET", bSuccess, m_bCreditsNet);
    m_nXP = pGFF->ReadFieldINT(&rStruct, "XP", bSuccess, m_nXP);
    m_nStealthXP = pGFF->ReadFieldINT(&rStruct, "STEALTHXP", bSuccess, m_nStealthXP);
    m_bSoundPending = pGFF->ReadFieldBYTE(&rStruct, "SOUNDPENDING", bSuccess, m_bSoundPending);
    m_bLevelUpSound = pGFF->ReadFieldBYTE(&rStruct, "LEVELUPSOUND", bSuccess, m_bLevelUpSound);
    m_bNewQuestSound = pGFF->ReadFieldBYTE(&rStruct, "NEWQUESTSOUND", bSuccess, m_bNewQuestSound);
    m_bCompleteSound = pGFF->ReadFieldBYTE(&rStruct, "COMPLETESOUND", bSuccess, m_bCompleteSound);
    m_nSuppressed = pGFF->ReadFieldINT(&rStruct, "SUPPRESSED", bSuccess, m_nSuppressed);
}

// FUNCTION: SWKOTOR 0x006C9DE0
void KOTOR_AUTOSAVE_PARAMS::LoadFromGFF(CResGFF* pGFF, CResStruct& rStruct)
{
    int bSuccess;
    m_sLoadMusic = pGFF->ReadFieldCExoString(&rStruct, "LOADMUSIC", bSuccess, m_sLoadMusic);
    m_sStartWaypoint = pGFF->ReadFieldCExoString(&rStruct, "STARTWAYPOINT", bSuccess, m_sStartWaypoint);
    m_sMovie1 = pGFF->ReadFieldCExoString(&rStruct, "MOVIE1", bSuccess, m_sMovie1);
    m_sMovie2 = pGFF->ReadFieldCExoString(&rStruct, "MOVIE2", bSuccess, m_sMovie2);
    m_sMovie3 = pGFF->ReadFieldCExoString(&rStruct, "MOVIE3", bSuccess, m_sMovie3);
    m_sMovie4 = pGFF->ReadFieldCExoString(&rStruct, "MOVIE4", bSuccess, m_sMovie4);
    m_sMovie5 = pGFF->ReadFieldCExoString(&rStruct, "MOVIE5", bSuccess, m_sMovie5);
    m_sMovie6 = pGFF->ReadFieldCExoString(&rStruct, "MOVIE6", bSuccess, m_sMovie6);
    unsigned long nYear = pGFF->ReadFieldDWORD(&rStruct, "TIME_YEAR", bSuccess, 0);
    unsigned char nMonth = pGFF->ReadFieldBYTE(&rStruct, "TIME_MONTH", bSuccess, 0);
    unsigned char nDay = pGFF->ReadFieldBYTE(&rStruct, "TIME_DAY", bSuccess, 0);
    unsigned char nHour = pGFF->ReadFieldBYTE(&rStruct, "TIME_HOUR", bSuccess, 0);
    unsigned short nMinute = pGFF->ReadFieldWORD(&rStruct, "TIME_MINUTE", bSuccess, 0);
    unsigned short nSecond = pGFF->ReadFieldWORD(&rStruct, "TIME_SECOND", bSuccess, 0);
    unsigned short nMillisecond = pGFF->ReadFieldWORD(&rStruct, "TIME_MILLISECOND", bSuccess, 0);
    m_nYear = nYear;
    m_nMonth = nMonth;
    m_nDay = nDay;
    m_nHour = nHour;
    m_nMinute = nMinute;
    m_nSecond = nSecond;
    m_nMillisecond = nMillisecond;
    m_nPauseDay = pGFF->ReadFieldDWORD(&rStruct, "TIME_PAUSEDAY", bSuccess, 0);
    m_nPauseTime = pGFF->ReadFieldDWORD(&rStruct, "TIME_PAUSETIME", bSuccess, 0);
    CResStruct cStatus;
    if (pGFF->GetStructFromStruct(&cStatus, &rStruct, "STATUSSUMMARY")) {
        m_cStatusSummary.LoadFromGFF(pGFF, cStatus);
    }
}
