// CSWSObject effect / listen-data savers (own file so /Ob2 does
// not inline them into SaveObjectState).
#include <string.h>
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

// FUNCTION: SWKOTOR 0x004CECB0
void CSWSObject::LoadActionQueue(CResGFF* pRes, CResStruct* pStruct)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "ActionList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cAction;
            if (pRes->GetListElement(&cAction, &cList, i)) {
                int bSuccess;
                void* pParams[13];
                int nParamTypes[13];
                int nAllocated[13];
                memset(pParams, 0, sizeof(pParams));
                memset(nParamTypes, 0, sizeof(nParamTypes));
                unsigned long nActionId = pRes->ReadFieldDWORD(&cAction, "ActionId", bSuccess, 0);
                unsigned short nGroupId = pRes->ReadFieldWORD(&cAction, "GroupActionId", bSuccess, 0);
                unsigned short nNumParams = pRes->ReadFieldWORD(&cAction, "NumParams", bSuccess, 0);
                if (nNumParams > 0) {
                    CResList cParams;
                    pRes->GetList(&cParams, &cAction, "Paramaters");
                    for (unsigned int j = 0; j < nNumParams; j++) {
                        CResStruct cParam;
                        pRes->GetListElement(&cParam, &cParams, j);
                        nParamTypes[j] = pRes->ReadFieldDWORD(&cParam, "Type", bSuccess, 0);
                        switch (nParamTypes[j]) {
                        case 1: {
                            int* pInt = new int;
                            *pInt = pRes->ReadFieldINT(&cParam, "Value", bSuccess, 0);
                            pParams[j] = pInt;
                            nAllocated[j] = 1;
                            break;
                        }
                        case 2: {
                            float* pFloat = new float;
                            *pFloat = pRes->ReadFieldFLOAT(&cParam, "Value", bSuccess, 0);
                            pParams[j] = pFloat;
                            nAllocated[j] = 2;
                            break;
                        }
                        case 4: {
                            CExoString* pString = new CExoString;
                            *pString = pRes->ReadFieldCExoString(&cParam, "Value", bSuccess, "");
                            pParams[j] = pString;
                            nAllocated[j] = 4;
                            break;
                        }
                        case 3: {
                            unsigned long* pDword = new unsigned long;
                            *pDword = pRes->ReadFieldDWORD(&cParam, "Value", bSuccess, 0);
                            pParams[j] = pDword;
                            nAllocated[j] = 3;
                            break;
                        }
                        case 5: {
                            CResStruct cScript;
                            void* pScript;
                            pRes->GetStructFromStruct(&cScript, &cParam, "Value");
                            g_pVirtualMachine->LoadScriptSituation(&pScript, pRes, &cScript);
                            pParams[j] = pScript;
                            nAllocated[j] = 5;
                            break;
                        }
                        }
                    }
                }
                AddAction(nActionId, nGroupId,
                          nParamTypes[0], pParams[0], nParamTypes[1], pParams[1],
                          nParamTypes[2], pParams[2], nParamTypes[3], pParams[3],
                          nParamTypes[4], pParams[4], nParamTypes[5], pParams[5],
                          nParamTypes[6], pParams[6], nParamTypes[7], pParams[7],
                          nParamTypes[8], pParams[8], nParamTypes[9], pParams[9],
                          nParamTypes[10], pParams[10], nParamTypes[11], pParams[11],
                          nParamTypes[12], pParams[12]);
                for (unsigned int k = 0; k < nNumParams; k++) {
                    switch (nAllocated[k]) {
                    case 1:
                        delete (int*)pParams[k];
                        break;
                    case 2:
                        delete (float*)pParams[k];
                        break;
                    case 4:
                        delete (CExoString*)pParams[k];
                        break;
                    case 3:
                        delete (unsigned long*)pParams[k];
                        break;
                    }
                }
            }
        }
    }
}
