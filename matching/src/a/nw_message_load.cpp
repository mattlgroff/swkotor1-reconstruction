// Worker A round s7: CSWCCMessageData::LoadData (0x004D6B10). Uses the
// script-event declarations, whose CExoArrayList has the inline Allocate.
#include "a/script_event.h"

class CSWCCMessageData {
public:
    void SetInteger(int nIndex, int nValue);        // 0x004D76B0
    // Inline in this translation unit (out-of-line copies 0x004D7720 / 0x004D7780 / 0x004D6AE0).
    void SetFloat(int nIndex, float fValue)
    {
        if (nIndex >= m_lstFloats.num) {
            m_lstFloats.SetSize(nIndex + 1);
        }
        m_lstFloats.element[nIndex] = fValue;
    }
    void SetObjectID(int nIndex, unsigned long oidValue)
    {
        if (nIndex >= m_lstObjects.num) {
            m_lstObjects.SetSize(nIndex + 1);
        }
        m_lstObjects.element[nIndex] = oidValue;
    }
    void SetString(int nIndex, const CExoString& sValue)
    {
        if (nIndex >= m_lstStrings.num) {
            m_lstStrings.SetSize(nIndex + 1);
        }
        m_lstStrings.element[nIndex] = sValue;
    }
    int LoadData(CResGFF* pRes, CResStruct* pStruct);   // 0x004D6B10
    void CopyTo(CSWCCMessageData* pDest);           // 0x004D6EE0

    unsigned char m_nType;                      // 0x00
    CExoArrayList<int> m_lstIntegers;           // 0x04
    CExoArrayList<float> m_lstFloats;           // 0x10
    CExoArrayList<CExoString> m_lstStrings;     // 0x1c
    CExoArrayList<unsigned long> m_lstObjects;  // 0x28
};

// FUNCTION: SWKOTOR 0x004D6B10
int CSWCCMessageData::LoadData(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    CResList cList;
    CResStruct cElement;
    unsigned long i;
    m_nType = pRes->ReadFieldBYTE(pStruct, "Type", bSuccess, 0);
    if (pRes->GetList(&cList, pStruct, "IntList") == 1) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            SetInteger(i, pRes->ReadFieldINT(&cElement, "IntegerValue", bSuccess, 0));
        }
    }
    if (pRes->GetList(&cList, pStruct, "FloatList") == 1) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            SetFloat(i, pRes->ReadFieldFLOAT(&cElement, "FloatValue", bSuccess, 0.0f));
        }
    }
    if (pRes->GetList(&cList, pStruct, "ObjectIDList") == 1) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            SetObjectID(i, pRes->ReadFieldDWORD(&cElement, "ObjectValue", bSuccess, 0));
        }
    }
    if (pRes->GetList(&cList, pStruct, "StringList") == 1) {
        for (i = 0; i < pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            CExoString sDefault("");
            SetString(i, pRes->ReadFieldCExoString(&cElement, "StringValue", bSuccess, sDefault));
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D6EE0
void CSWCCMessageData::CopyTo(CSWCCMessageData* pDest)
{
    if (!pDest) {
        return;
    }
    pDest->m_lstIntegers.num = 0;
    pDest->m_lstFloats.num = 0;
    pDest->m_lstObjects.num = 0;
    pDest->m_lstStrings.num = 0;
    pDest->m_nType = 0;
    int i;
    for (i = 0; i < m_lstIntegers.num; i++) {
        pDest->SetInteger(i, m_lstIntegers.element[i]);
    }
    for (i = 0; i < m_lstFloats.num; i++) {
        pDest->SetFloat(i, m_lstFloats.element[i]);
    }
    for (i = 0; i < m_lstObjects.num; i++) {
        pDest->SetObjectID(i, m_lstObjects.element[i]);
    }
    for (i = 0; i < m_lstStrings.num; i++) {
        pDest->SetString(i, m_lstStrings.element[i]);
    }
    pDest->m_nType = m_nType;
}
