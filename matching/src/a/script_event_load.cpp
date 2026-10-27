// Worker A round s7: CScriptEvent::LoadEvent (0x004D79B0).
#include "a/script_event.h"

// FUNCTION: SWKOTOR 0x004D79B0
int CScriptEvent::LoadEvent(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    CResList cList;
    CResStruct cElement;
    unsigned long i;
    m_nType = pRes->ReadFieldWORD(pStruct, "EventType", bSuccess, 0);
    if (pRes->GetList(&cList, pStruct, "IntList")) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            m_lstIntegers.Add(pRes->ReadFieldINT(&cElement, "Parameter", bSuccess, 0));
        }
    }
    if (pRes->GetList(&cList, pStruct, "FloatList")) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            m_lstFloats.Add(pRes->ReadFieldFLOAT(&cElement, "Parameter", bSuccess, 0.0f));
        }
    }
    if (pRes->GetList(&cList, pStruct, "StringList")) {
        CExoString sValue;
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            {
                CExoString sDefault("");
                sValue = pRes->ReadFieldCExoString(&cElement, "Parameter", bSuccess, sDefault);
            }
            m_lstStrings.Add(sValue);
        }
    }
    if (pRes->GetList(&cList, pStruct, "ObjectList")) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            m_lstObjects.Add(pRes->ReadFieldDWORD(&cElement, "Parameter", bSuccess, 0));
        }
    }
    return 1;
}
