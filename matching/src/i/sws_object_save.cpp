// CSWSObject effect / listen-data savers (own file so /Ob2 does
// not inline them into SaveObjectState).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CC9D0
void CSWSObject::SaveEffectList(CResGFF* pRes, CResStruct* pStruct)
{
    if (m_appliedEffects.num > 0) {
        CResList cList;
        pRes->AddList(&cList, pStruct, "EffectList");
        for (int i = 0; i < m_appliedEffects.num; i++) {
            CResStruct cStruct;
            pRes->AddListElement(&cStruct, &cList, 2);
            m_appliedEffects[i]->SaveGameEffect(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x004CCA50
void CSWSObject::SaveListenData(CResGFF* pRes, CResStruct* pStruct)
{
    if (m_lstListenExpressions.num > 0) {
        CResList cList;
        pRes->AddList(&cList, pStruct, "ExpressionList");
        for (int i = 0; i < m_lstListenExpressions.num; i++) {
            CResStruct cStruct;
            pRes->AddListElement(&cStruct, &cList, 5);
            pRes->WriteFieldINT(&cStruct, m_lstListenExpressions[i]->m_nId, "ExpressionId");
            pRes->WriteFieldCExoString(&cStruct, m_lstListenExpressions[i]->m_sExpression,
                                       "ExpressionString");
        }
    }
}
