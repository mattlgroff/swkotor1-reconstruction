// WindManager noise field, timer and point-source list (aurwindmanager.o).
// Names follow the playable build (fillNoiseField / updateTimer).
#include "i/wind_manager_life.h"
#include <stdlib.h>
#include <string.h>

// FUNCTION: SWKOTOR 0x004A6640
WindManager::WindManager()
{
    m_vGlobalWind = m_vWindStep = Vector();
    m_fMinimumDeviation = 0.0f;
    m_fMaximumDeviation = 0.0f;
    m_fTimer = 0.0f;
    m_fPeriod = 2.0f;
    FillNoiseField(m_afNoise[0]);
    FillNoiseField(m_afNoise[1]);
    m_pCurrentNoise = m_afNoise[0];
    m_pNextNoise = m_afNoise[1];
}

// FUNCTION: SWKOTOR 0x004A57C0
void WindManager::FillNoiseField(float* pField)
{
    memset(pField, 0, 0x100 * sizeof(float));
    for (int nStep = 0x10; nStep >= 1; nStep /= 2) {
        for (int y0 = 0, y1 = nStep; y1 <= 0x10; y0 = y1, y1 += nStep) {
            for (int x0 = 0, x1 = nStep; x1 <= 0x10; x0 = x1, x1 += nStep) {
                float fValue = (rand() % (500 / (0x10 / nStep))) * 0.001f;
                for (int j = y0; j < y1; j++) {
                    for (int i = x0; i < x1; i++) {
                        pField[j * 0x10 + i] += fValue;
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004A58C0 SYMBOL: ??0PointSourceWind@@QAE@XZ
// FUNCTION: SWKOTOR 0x004A5F50 SYMBOL: ??1?$CExoArrayList@VPointSourceWind@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x004A5F80 SYMBOL: ?Allocate@?$CExoArrayList@VPointSourceWind@@@@QAEXH@Z
template CExoArrayList<PointSourceWind>::~CExoArrayList();
template void CExoArrayList<PointSourceWind>::Allocate(int);

// NONMATCH (5 insns): num - 1 lands in edi instead of esi.
// FUNCTION: SWKOTOR 0x004A5E30
void WindManager::UpdateTimer(float fDelta)
{
    m_fTimer += fDelta;
    if (m_fTimer > m_fPeriod) {
        float* pNoise = m_pCurrentNoise;
        m_pCurrentNoise = m_pNextNoise;
        m_pNextNoise = pNoise;
        FillNoiseField(pNoise);
        m_fTimer = 0.0f;
    }
    m_fBlend = m_fTimer / m_fPeriod;
    m_vWindStep = fDelta * m_vGlobalWind;
    for (int i = 0; i < m_lPointSourceWinds.num; i++) {
        m_lPointSourceWinds.element[i].m_fTimeLeft -= fDelta;
        if (m_lPointSourceWinds.element[i].m_fTimeLeft < 0.0f) {
            m_lPointSourceWinds.num--;
            for (int j = i; j < m_lPointSourceWinds.num; j++) {
                m_lPointSourceWinds.element[j] = m_lPointSourceWinds.element[j + 1];
            }
            i--;
        }
    }
}

// FUNCTION: SWKOTOR 0x004A6570 SYMBOL: ??1?$SafePointerList@VWindManager@@@@QAE@XZ

// FUNCTION: SWKOTOR 0x004A6710
WindManager::~WindManager()
{
}
