// CSWSArea weather broadcast (0x00508050).
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x00508050
void CSWSArea::SetCurrentWeather(unsigned char nWeather, int bStarted)
{
    if (nWeather == 0xff) {
        if (m_nWeatherTarget == m_nCurrentWeather) {
            return;
        }
        if (m_nAreaFlags & 1) {
            m_nCurrentWeather = 0xff;
            if (m_nWeatherTarget == 1) {
                nWeather = 1;
                bStarted = 0;
            } else if (m_nWeatherTarget == 2) {
                nWeather = 2;
                bStarted = 0;
            }
        } else if (m_nWeatherTarget == 1) {
            if (m_nCurrentWeather == 2) {
                nWeather = m_nCurrentWeather;
                bStarted = 1;
            } else {
                nWeather = 1;
                bStarted = 0;
            }
        } else if (m_nWeatherTarget == 2) {
            if (m_nCurrentWeather == 1) {
                nWeather = m_nCurrentWeather;
                bStarted = 1;
            } else {
                nWeather = 2;
                bStarted = 0;
            }
        }
    }

    CExoLinkedList<CSWSPlayer>* pList;
    pList = GetServerExoApp()->GetPlayerList();
    if (pList != NULL) {
        CExoLinkedListPosition pos = pList->GetHeadPos();
        CSWSPlayer* pPlayer = pList->GetAtPosInline(pList->GetHeadPos());
        while (pos) {
            if (pPlayer) {
                CSWSObject* pObject = pPlayer->GetGameObject();
                if (pObject && pObject->GetArea() &&
                    pObject->GetArea()->AsGameObject()->m_idSelf == m_idSelf) {
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerArea_Weather(
                        pPlayer, nWeather, bStarted);
                }
            }
            pPlayer = pList->GetNext(pos);
        }
    }
    m_nWeatherTarget = bStarted ? nWeather : (unsigned char)0;
}
