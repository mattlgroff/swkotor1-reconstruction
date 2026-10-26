// CSWSArea creature head scan (0x00508820). Unnamed in the project records;
// the name describes the contract: every creature in the area whose
// appearance NORMALHEAD equals nHead gets its head-cloned flag set.
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x00508820
void CSWSArea::FlagCreaturesWithHead(int nHead)
{
    int nValue;
    for (int i = 0; i < m_aGameObjects.num; i++) {
        unsigned long oidObject = m_aGameObjects.element[i];
        CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(oidObject);
        if (pCreature == NULL || pCreature->m_aEquippedAppearance == NULL) {
            continue;
        }
        if (pCreature->m_bDisguised) {
            CExoString sColumn("NORMALHEAD");
            g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(
                pCreature->GetDisguiseAppearance(), sColumn, &nValue);
        } else {
            CExoString sColumn("NORMALHEAD");
            g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(
                pCreature->m_nAppearanceType, sColumn, &nValue);
        }
        if (nValue == nHead) {
            pCreature->m_bHeadCloned = 1;
        }
    }
}
