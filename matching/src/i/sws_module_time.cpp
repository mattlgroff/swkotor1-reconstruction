// CSWSModule::UpdateTime (kept apart from AIUpdate so /Ob2 does
// not inline it).
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C6210
void CSWSModule::UpdateTime(unsigned long nDay, unsigned long nTime, unsigned long nDelta)
{
    unsigned long nYear;
    unsigned long nMonth;
    unsigned long nCalendarDay;
    unsigned long nHour;
    unsigned long nMinute;
    unsigned long nSecond;
    unsigned long nMillisecond;
    GetServerExoApp()->GetWorldTimer()->FUN_004add50(nDay, &nYear, &nMonth, &nCalendarDay);
    GetServerExoApp()->GetWorldTimer()->FUN_004add90(nTime, &nHour, &nMinute, &nSecond,
                                                     &nMillisecond);
    unsigned char nPhase;
    if (m_nDawnHour == m_nDuskHour) {
        nPhase = 1;
    } else if (nHour == m_nDawnHour) {
        nPhase = 3;
    } else if (nHour == m_nDuskHour) {
        nPhase = 4;
    } else if (m_nDawnHour < m_nDuskHour) {
        if (nHour < m_nDawnHour || nHour > m_nDuskHour) {
            nPhase = 2;
        } else {
            nPhase = 1;
        }
    } else {
        if (nHour >= m_nDuskHour && nHour <= m_nDawnHour) {
            nPhase = 2;
        } else {
            nPhase = 1;
        }
    }
    if (m_nPhase == 3 || m_nPhase == 4) {
        m_nTransitionTime += nDelta;
    } else {
        m_nTransitionTime = 0;
    }
    unsigned char nFlags = 0;
    if (m_nPhase != nPhase) {
        nFlags = 1;
        m_nPhase = nPhase;
    }
    if (m_nHour != nHour) {
        nFlags |= 2;
        m_nHour = nHour;
    }
    if (m_nDay != nCalendarDay) {
        nFlags |= 4;
        m_nDay = nCalendarDay;
    }
    if (m_nMonth != nMonth) {
        nFlags |= 8;
        m_nMonth = nMonth;
    }
    if (m_nYear != nYear) {
        nFlags |= 0x10;
        m_nYear = nYear;
    }
    if (nFlags) {
        CExoLinkedList<CSWSPlayer>* pList = GetServerExoApp()->GetPlayerList();
        CExoLinkedListPosition pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = pList->GetAtPos(pos);
            if (pPlayer) {
                GetServerExoApp()->GetSWSMessage()->SendServerToPlayerModuleUpdate_Time(
                    pPlayer, nFlags, nYear, nMonth, nCalendarDay, nHour, m_nPhase,
                    m_nTransitionTime);
            }
            pList->GetNext(pos);
        }
    }
}
