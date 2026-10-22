// CWorldTimer (0x004ADBA0-0x004AE570) and CObjectTableManager
// (0x004AE5D0-0x004AE650). Self-contained; see docs/batches/worker-i-notes.md.
#ifndef I_WORLD_TIMER_H
#define I_WORLD_TIMER_H

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    void* m_p0;
    CExoTimers* m_pcExoTimers;                      // +0x04
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

#pragma pack(push, 4)
class CWorldTimer {
public:
    CWorldTimer();                                                          // 0x004AE4C0
    void SetMinutesPerHour(unsigned char nMinutesPerHour);                  // 0x004ADBA0
    void SetSnapshotTime();                                                 // 0x004ADBD0
    unsigned __int64 GetSnapshotTime();                                     // 0x004ADC70
    unsigned __int64 GetSnapshotTimeDifference();                           // 0x004ADC80
    unsigned long ConvertFromCalendarDay(unsigned long nYear, unsigned long nMonth,
                                         unsigned long nDay);               // 0x004ADCA0
    unsigned long ConvertFromTimeOfDay(unsigned long nHour, unsigned long nMinute,
                                       unsigned long nSecond,
                                       unsigned long nMillisecond);         // 0x004ADCF0
    void ConvertToCalendarDay(unsigned long nCalendarDay, unsigned long* pYear,
                              unsigned long* pMonth, unsigned long* pDay);  // 0x004ADD50
    void ConvertToTimeOfDay(unsigned long nTimeOfDay, unsigned long* pHour,
                            unsigned long* pMinute, unsigned long* pSecond,
                            unsigned long* pMillisecond);                   // 0x004ADD90
    void SetWorldTime(unsigned long nCalendarDay, unsigned long nTimeOfDay,
                      int bUpdateSnapshot);                                 // 0x004ADDE0
    void GetWorldTime(unsigned long* pCalendarDay, unsigned long* pTimeOfDay); // 0x004ADE40
    void FUN_004adef0(const CWorldTimer* pOther);                           // 0x004ADEF0
    int AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDay,
                      unsigned long* pTime);                                // 0x004ADF50
    int CompareWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                          unsigned long nTime2);                            // 0x004ADFA0
    void PauseWorldTimer();                                                 // 0x004ADFF0
    void UnpauseWorldTimer();                                               // 0x004AE030
    unsigned long GetWorldTimeHour();                                       // 0x004AE090
    unsigned long GetWorldTimeMinute();                                     // 0x004AE0D0
    unsigned long GetWorldTimeSecond();                                     // 0x004AE110
    unsigned long GetWorldTimeMillisecond();                                // 0x004AE140
    void SetFixedSnapshotRate(int bFixed, int nRate);                       // 0x004AE170
    void SetSnapshotScale(float fScale);                                    // 0x004AE190
    unsigned long GetCalendarDayFromSeconds(float fSeconds);                // 0x004AE1B0
    unsigned long GetTimeOfDayFromSeconds(float fSeconds);                  // 0x004AE200
    void AdvanceToTime(int nHour, int nMinute, int nSecond, int nMillisecond); // 0x004AE260
    void SetWorldTime(unsigned long nYear, unsigned long nMonth, unsigned long nDay,
                      unsigned long nHour, unsigned long nMinute, unsigned long nSecond,
                      unsigned long nMillisecond, int bUpdateSnapshot);     // 0x004AE3E0
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                           unsigned long nTime2, unsigned long* pDay,
                           unsigned long* pTime);                           // 0x004AE460
    unsigned long GetTimeDifferenceFromWorldTime(unsigned long nDay,
                                                 unsigned long nTime);      // 0x004AE570

    int m_bFixedSnapshotRate;                       // +0x00
    int m_nFixedSnapshotRate;                       // +0x04 snapshots per second
    int m_nSnapshotScale;                           // +0x08 percent
    unsigned __int64 m_nSnapshotTime;               // +0x0c
    unsigned __int64 m_nLastSnapshotTime;           // +0x14
    unsigned __int64 m_nLastHighResTime;            // +0x1c
    int m_bPaused;                                  // +0x24
    unsigned long m_nPauseDay;                      // +0x28
    unsigned long m_nPauseTime;                     // +0x2c
    unsigned long m_nDayOffset;                     // +0x30
    unsigned long m_nTimeOffset;                    // +0x34
    unsigned char m_nMinutesPerHour;                // +0x38
    unsigned long m_nMsPerDay;                      // +0x3c
    unsigned long m_nSecondsPerDay;                 // +0x40
};
#pragma pack(pop)

struct CObjectTableEntry {
    ~CObjectTableEntry()
    {
        if (m_pData) {
            delete m_pData;
        }
    }
    char* m_pData;                                  // +0x00
};

class CObjectTableManager {
public:
    CObjectTableManager();                          // 0x004AE5D0
    ~CObjectTableManager();                         // 0x004AE640
    void ClearAll();                                // 0x004AE5F0
    int RemovePlayer(unsigned long nPlayer);        // 0x004AE650

    CObjectTableEntry* m_pTables[0x60];             // +0x000
    unsigned long m_nTables;                        // +0x180
};

#endif
