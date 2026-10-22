// CWorldTimer (0x004ADBA0-0x004AE570).
#include "i/world_timer.h"

// FUNCTION: SWKOTOR 0x004ADBA0
void CWorldTimer::SetMinutesPerHour(unsigned char nMinutesPerHour)
{
    m_nMinutesPerHour = nMinutesPerHour;
    if (nMinutesPerHour == 0) {
        m_nMinutesPerHour = 5;
    }
    m_nSecondsPerDay = m_nMinutesPerHour * 1440;
    m_nMsPerDay = m_nMinutesPerHour * 1440000;
}

// FUNCTION: SWKOTOR 0x004ADBD0
void CWorldTimer::SetSnapshotTime()
{
    unsigned __int64 nSnapshotTime = m_nSnapshotTime;
    m_nLastSnapshotTime = nSnapshotTime;
    if (m_bFixedSnapshotRate == 0) {
        unsigned __int64 nNow = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
        unsigned __int64 nDelta = nNow - m_nLastHighResTime;
        m_nSnapshotTime += (nDelta * m_nSnapshotScale) / 100;
        m_nLastHighResTime = nNow;
    } else {
        m_nSnapshotTime = nSnapshotTime +
                          (1000000 / m_nFixedSnapshotRate * m_nSnapshotScale) / 100;
    }
}

// FUNCTION: SWKOTOR 0x004ADC70
unsigned __int64 CWorldTimer::GetSnapshotTime()
{
    return m_nSnapshotTime;
}

// FUNCTION: SWKOTOR 0x004ADC80
unsigned __int64 CWorldTimer::GetSnapshotTimeDifference()
{
    if (m_bPaused) {
        return 0;
    }
    return m_nSnapshotTime - m_nLastSnapshotTime;
}

// FUNCTION: SWKOTOR 0x004ADCA0
unsigned long CWorldTimer::ConvertFromCalendarDay(unsigned long nYear, unsigned long nMonth,
                                                  unsigned long nDay)
{
    unsigned long nResult;
    if (nYear <= 32000) {
        nResult = nYear * 336;
    } else {
        nResult = 1340 * 336;
    }
    if (nMonth >= 1 && nMonth <= 12) {
        nResult += (nMonth - 1) * 28;
    } else {
        nResult += 140;
    }
    if (nDay >= 1 && nDay <= 28) {
        nResult += nDay - 1;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x004ADCF0
unsigned long CWorldTimer::ConvertFromTimeOfDay(unsigned long nHour, unsigned long nMinute,
                                                unsigned long nSecond,
                                                unsigned long nMillisecond)
{
    unsigned long nResult;
    if (nHour <= 23) {
        nResult = nHour * m_nMinutesPerHour * 60000;
    } else {
        nResult = m_nMinutesPerHour * 1380000;
    }
    if (nMinute <= 59) {
        nResult += nMinute * 60000;
    } else {
        nResult += 3540000;
    }
    if (nSecond <= 59) {
        nResult += nSecond * 1000;
    } else {
        nResult += 56000;
    }
    return nResult + nMillisecond;
}

// FUNCTION: SWKOTOR 0x004ADD50
void CWorldTimer::ConvertToCalendarDay(unsigned long nCalendarDay, unsigned long* pYear,
                                       unsigned long* pMonth, unsigned long* pDay)
{
    *pDay = nCalendarDay % 28 + 1;
    nCalendarDay /= 28;
    *pMonth = nCalendarDay % 12 + 1;
    nCalendarDay /= 12;
    *pYear = nCalendarDay;
    if (nCalendarDay > 32767) {
        *pYear = 1340;
    }
}

// FUNCTION: SWKOTOR 0x004ADD90
void CWorldTimer::ConvertToTimeOfDay(unsigned long nTimeOfDay, unsigned long* pHour,
                                     unsigned long* pMinute, unsigned long* pSecond,
                                     unsigned long* pMillisecond)
{
    *pMillisecond = nTimeOfDay % 1000;
    nTimeOfDay /= 1000;
    *pSecond = nTimeOfDay % 60;
    nTimeOfDay /= 60;
    *pMinute = nTimeOfDay % m_nMinutesPerHour;
    *pHour = nTimeOfDay / m_nMinutesPerHour;
}

// FUNCTION: SWKOTOR 0x004ADDE0 SYMBOL: ?SetWorldTime@CWorldTimer@@QAEXKKH@Z
void CWorldTimer::SetWorldTime(unsigned long nCalendarDay, unsigned long nTimeOfDay,
                               int bUpdateSnapshot)
{
    if (bUpdateSnapshot == 1) {
        SetSnapshotTime();
    }
    unsigned __int64 nTime = m_nSnapshotTime / 1000;
    unsigned __int64 nMs = nTime % m_nMsPerDay;
    unsigned __int64 nDays = nTime / m_nMsPerDay;
    m_nDayOffset = nCalendarDay - (unsigned long)nDays;
    m_nTimeOffset = nTimeOfDay - (unsigned long)nMs;
}

// FUNCTION: SWKOTOR 0x004ADE40
void CWorldTimer::GetWorldTime(unsigned long* pCalendarDay, unsigned long* pTimeOfDay)
{
    unsigned __int64 nSnapshotTime = m_nSnapshotTime;
    if (m_bPaused == 1) {
        *pCalendarDay = m_nPauseDay;
        *pTimeOfDay = m_nPauseTime;
    } else {
        unsigned __int64 nTime = nSnapshotTime / 1000;
        unsigned long nDays = (unsigned long)(nTime / m_nMsPerDay);
        unsigned long nMs = (unsigned long)(nTime % m_nMsPerDay);
        *pCalendarDay = nDays + m_nDayOffset;
        *pTimeOfDay = nMs + m_nTimeOffset;
    }
    while (*pTimeOfDay > 0xffffffff - m_nMsPerDay) {
        (*pCalendarDay)--;
        *pTimeOfDay += m_nMsPerDay;
    }
    while (*pTimeOfDay >= m_nMsPerDay) {
        (*pCalendarDay)++;
        *pTimeOfDay -= m_nMsPerDay;
    }
}

// FUNCTION: SWKOTOR 0x004ADEF0
void CWorldTimer::FUN_004adef0(const CWorldTimer* pOther)
{
    m_bFixedSnapshotRate = pOther->m_bFixedSnapshotRate;
    m_nFixedSnapshotRate = pOther->m_nFixedSnapshotRate;
    m_nSnapshotTime = pOther->m_nSnapshotTime;
    m_nLastSnapshotTime = pOther->m_nLastSnapshotTime;
    m_bPaused = pOther->m_bPaused;
    m_nPauseDay = pOther->m_nPauseDay;
    m_nPauseTime = pOther->m_nPauseTime;
    m_nDayOffset = pOther->m_nDayOffset;
    m_nTimeOffset = pOther->m_nTimeOffset;
    m_nMinutesPerHour = pOther->m_nMinutesPerHour;
    m_nMsPerDay = pOther->m_nMsPerDay;
    m_nSecondsPerDay = pOther->m_nSecondsPerDay;
    m_nSnapshotScale = 100;
}

// FUNCTION: SWKOTOR 0x004ADF50
int CWorldTimer::AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                               unsigned long nTime2, unsigned long* pDay, unsigned long* pTime)
{
    if (nTime1 >= m_nMsPerDay || nTime2 >= m_nMsPerDay) {
        return -2;
    }
    *pDay = nDay1 + nDay2;
    *pTime = nTime1 + nTime2;
    if (*pTime >= m_nMsPerDay) {
        (*pDay)++;
        *pTime -= m_nMsPerDay;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004ADFA0
int CWorldTimer::CompareWorldTimes(unsigned long nDay1, unsigned long nTime1,
                                   unsigned long nDay2, unsigned long nTime2)
{
    if (nTime1 >= m_nMsPerDay || nTime2 >= m_nMsPerDay) {
        return -2;
    }
    if (nDay1 == nDay2) {
        if (nTime1 > nTime2) {
            return 1;
        }
        if (nTime1 < nTime2) {
            return -1;
        }
        return 0;
    }
    if (nDay1 > nDay2) {
        return 1;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x004ADFF0
void CWorldTimer::PauseWorldTimer()
{
    if (m_bPaused == 0) {
        unsigned long nDay;
        unsigned long nTime;
        GetWorldTime(&nDay, &nTime);
        m_nPauseDay = nDay;
        m_nPauseTime = nTime;
        m_bPaused = 1;
    }
}

// FUNCTION: SWKOTOR 0x004AE030
void CWorldTimer::UnpauseWorldTimer()
{
    if (m_bPaused == 1) {
        unsigned __int64 nTime = m_nSnapshotTime / 1000;
        unsigned __int64 nMs = nTime % m_nMsPerDay;
        unsigned __int64 nDays = nTime / m_nMsPerDay;
        m_nDayOffset = m_nPauseDay - (unsigned long)nDays;
        m_bPaused = 0;
        m_nTimeOffset = m_nPauseTime - (unsigned long)nMs;
    }
}

// FUNCTION: SWKOTOR 0x004AE090
unsigned long CWorldTimer::GetWorldTimeHour()
{
    unsigned long nDay;
    unsigned long nTime;
    GetWorldTime(&nDay, &nTime);
    return nTime / 1000 / 60 / m_nMinutesPerHour;
}

// FUNCTION: SWKOTOR 0x004AE0D0
unsigned long CWorldTimer::GetWorldTimeMinute()
{
    unsigned long nDay;
    unsigned long nTime;
    GetWorldTime(&nDay, &nTime);
    return nTime / 1000 / 60 % m_nMinutesPerHour;
}

// FUNCTION: SWKOTOR 0x004AE110
unsigned long CWorldTimer::GetWorldTimeSecond()
{
    unsigned long nDay;
    unsigned long nTime;
    GetWorldTime(&nDay, &nTime);
    return nTime / 1000 % 60;
}

// FUNCTION: SWKOTOR 0x004AE140
unsigned long CWorldTimer::GetWorldTimeMillisecond()
{
    unsigned long nDay;
    unsigned long nTime;
    GetWorldTime(&nDay, &nTime);
    return nTime % 1000;
}

// FUNCTION: SWKOTOR 0x004AE170
void CWorldTimer::SetFixedSnapshotRate(int bFixed, int nRate)
{
    m_nSnapshotTime = 0;
    m_nLastSnapshotTime = 0;
    m_bFixedSnapshotRate = bFixed;
    m_nFixedSnapshotRate = nRate;
}

// FUNCTION: SWKOTOR 0x004AE190
void CWorldTimer::SetSnapshotScale(float fScale)
{
    m_nSnapshotScale = (int)(fScale * 100.0f);
}

// FUNCTION: SWKOTOR 0x004AE1B0
unsigned long CWorldTimer::GetCalendarDayFromSeconds(float fSeconds)
{
    float fSecondsPerDay = (float)m_nSecondsPerDay;
    if (fSeconds < fSecondsPerDay) {
        return 0;
    }
    unsigned long nDays = 0;
    float fRemaining = fSeconds;
    do {
        fRemaining -= fSecondsPerDay;
        nDays++;
    } while (fRemaining >= fSecondsPerDay);
    return nDays;
}

// FUNCTION: SWKOTOR 0x004AE200
unsigned long CWorldTimer::GetTimeOfDayFromSeconds(float fSeconds)
{
    if (fSeconds < 0.0f) {
        return 0;
    }
    float fSecondsPerDay = (float)m_nSecondsPerDay;
    float fRemaining;
    if (fSeconds < fSecondsPerDay) {
        fRemaining = fSeconds;
    } else {
        fRemaining = fSeconds;
        do {
            fRemaining -= fSecondsPerDay;
        } while (fRemaining >= fSecondsPerDay);
    }
    return (unsigned long)(fRemaining * 1000.0f);
}

// FUNCTION: SWKOTOR 0x004AE260
void CWorldTimer::AdvanceToTime(int nHour, int nMinute, int nSecond, int nMillisecond)
{
    if (nHour < 0 || nMinute < 0 || nSecond < 0 || nMillisecond < 0) {
        return;
    }
    nSecond += (unsigned long)nMillisecond / 1000;
    nMillisecond %= 1000;
    nMinute += (unsigned long)nSecond / 60;
    nSecond %= 60;
    nHour += (unsigned long)nMinute / 60;
    nMinute %= 60;
    unsigned long nDays = (unsigned long)nHour / 24;
    nHour %= 24;

    unsigned long nCurDay;
    unsigned long nCurTime;
    GetWorldTime(&nCurDay, &nCurTime);
    unsigned long nYear;
    unsigned long nMonth;
    unsigned long nDay;
    ConvertToCalendarDay(nCurDay, &nYear, &nMonth, &nDay);
    if (nDays == 0) {
        if (CompareWorldTimes(nCurDay, nCurTime, nCurDay,
                              ConvertFromTimeOfDay(nHour, nMinute, nSecond, nMillisecond)) == 1) {
            nDays = 1;
        }
    }
    if (nDays > 0) {
        nDay += nDays;
        nMonth += nDay / 29;
        nDay %= 29;
        nYear += nMonth / 13;
        nMonth %= 13;
        if (nYear > 32000) {
            return;
        }
    }
    SetWorldTime(ConvertFromCalendarDay(nYear, nMonth, nDay),
                 ConvertFromTimeOfDay(nHour, nMinute, nSecond, nMillisecond), 0);
}

// FUNCTION: SWKOTOR 0x004AE3E0 SYMBOL: ?SetWorldTime@CWorldTimer@@QAEXKKKKKKKH@Z
void CWorldTimer::SetWorldTime(unsigned long nYear, unsigned long nMonth, unsigned long nDay,
                               unsigned long nHour, unsigned long nMinute,
                               unsigned long nSecond, unsigned long nMillisecond,
                               int bUpdateSnapshot)
{
    unsigned long nCalendarDay = ConvertFromCalendarDay(nYear, nMonth, nDay);
    unsigned long nTimeOfDay = ConvertFromTimeOfDay(nHour, nMinute, nSecond, nMillisecond);
    SetWorldTime(nCalendarDay, nTimeOfDay, bUpdateSnapshot);
}

// FUNCTION: SWKOTOR 0x004AE460
int CWorldTimer::SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1,
                                    unsigned long nDay2, unsigned long nTime2,
                                    unsigned long* pDay, unsigned long* pTime)
{
    if (CompareWorldTimes(nDay1, nTime1, nDay2, nTime2) == -1) {
        return -2;
    }
    *pDay = nDay1 - nDay2;
    *pTime = nTime1 - nTime2;
    if (*pTime >= m_nMsPerDay) {
        (*pDay)--;
        *pTime += m_nMsPerDay;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004AE4C0
CWorldTimer::CWorldTimer()
{
    m_bFixedSnapshotRate = 0;
    m_nMinutesPerHour = 5;
    m_nSecondsPerDay = 7200;
    m_nMsPerDay = 7200000;
    m_nSnapshotScale = 100;
    m_nSnapshotTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    m_nLastHighResTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    m_nLastSnapshotTime = m_nSnapshotTime;
    unsigned __int64 nTime = m_nSnapshotTime / 1000;
    unsigned __int64 nMs = nTime % m_nMsPerDay;
    unsigned __int64 nDays = nTime / m_nMsPerDay;
    m_nDayOffset = 450380 - (unsigned long)nDays;
    m_bPaused = 0;
    m_nTimeOffset = m_nMinutesPerHour * 1380000 + 3596000 - (unsigned long)nMs;
}

// FUNCTION: SWKOTOR 0x004AE570
unsigned long CWorldTimer::GetTimeDifferenceFromWorldTime(unsigned long nDay,
                                                          unsigned long nTime)
{
    unsigned long nCurDay;
    unsigned long nCurTime;
    GetWorldTime(&nCurDay, &nCurTime);
    unsigned long nDiffDay;
    unsigned long nDiffTime;
    if (SubtractWorldTimes(nCurDay, nCurTime, nDay, nTime, &nDiffDay, &nDiffTime) != 0) {
        return 0;
    }
    return nDiffTime;
}
