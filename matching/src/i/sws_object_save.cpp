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

// FUNCTION: SWKOTOR 0x004CC7E0
void CSWSObject::SaveActionQueue(CResGFF* pRes, CResStruct* pStruct)
{
    CResList cList;
    pRes->AddList(&cList, pStruct, "ActionList");
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    while (pos) {
        CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
        CResStruct cAction;
        pRes->AddListElement(&cAction, &cList, 0);
        pRes->WriteFieldDWORD(&cAction, pNode->m_nActionId, "ActionId");
        pRes->WriteFieldWORD(&cAction, pNode->m_nGroupActionId, "GroupActionId");
        pRes->WriteFieldWORD(&cAction, pNode->m_nNumParams, "NumParams");
        if (pNode->m_nNumParams > 0) {
            CResList cParams;
            pRes->AddList(&cParams, &cAction, "Paramaters");
            for (unsigned int i = 0; i < pNode->m_nNumParams; i++) {
                CResStruct cParam;
                pRes->AddListElement(&cParam, &cParams, 1);
                pRes->WriteFieldDWORD(&cParam, pNode->m_nParamType[i], "Type");
                switch (pNode->m_nParamType[i]) {
                case 1:
                    pRes->WriteFieldINT(&cParam, (int)pNode->m_pParam[i], "Value");
                    break;
                case 2:
                    pRes->WriteFieldFLOAT(&cParam, *(float*)&pNode->m_pParam[i], "Value");
                    break;
                case 4:
                    pRes->WriteFieldCExoString(&cParam, *(CExoString*)pNode->m_pParam[i], "Value");
                    break;
                case 3:
                    pRes->WriteFieldDWORD(&cParam, (unsigned long)pNode->m_pParam[i], "Value");
                    break;
                case 5: {
                    CResStruct cScript;
                    pRes->AddStructToStruct(&cScript, &cParam, "Value", 2);
                    g_pVirtualMachine->SaveScriptSituation(pNode->m_pParam[i], pRes, &cScript);
                    break;
                }
                }
            }
        }
        m_lQueuedActions.GetNext(pos);
    }
}
