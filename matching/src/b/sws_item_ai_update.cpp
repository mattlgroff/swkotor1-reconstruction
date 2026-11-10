// CSWSItem::AIUpdate (round s13). Not in functions.csv: the row for
// 0x0055A510 spans it. Slot 0x70 of the CSWSItem vftable for CSWSObject
// (0x00748CC0), so `this` is the CSWSObject base at +0x10. Recharges the
// per-day uses of activatable properties once enough game time has passed.
#include "b/sws_item.h"
#include "b/server_exo_app.h"

// FUNCTION: SWKOTOR 0x0055CB60
void CSWSItem::AIUpdate()
{
    if (!HasProperty(10, 0)) {
        return;
    }
    unsigned long nDay;
    unsigned long nTime;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    for (int i = 0; i < m_nActiveProperties; i++) {
        CSWItemProperty* pProperty = GetActiveProperty(i);
        if (pProperty == NULL || pProperty->m_nPropertyName != 10) {
            continue;
        }
        int nCost = pProperty->m_nCostValue;
        if (nCost != 14 && nCost != 15 && nCost != 16 && nCost != 17 && nCost != 18) {
            continue;
        }
        if (pProperty->m_bUseable) {
            continue;
        }
        unsigned long nDiffDay;
        unsigned long nDiffTime;
        g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
            nDay, nTime, pProperty->m_nLastUsedDay, pProperty->m_nLastUsedTime, &nDiffDay,
            &nDiffTime);
        if ((nCost == 14 && nDiffTime >= 60000) || (nCost == 15 && nDiffTime >= 120000) ||
            (nCost == 16 && nDiffTime >= 180000) || (nCost == 17 && nDiffTime >= 240000) ||
            (nCost == 18 && nDiffTime >= 300000)) {
            pProperty->m_nUseable = 1;
        }
    }
}
