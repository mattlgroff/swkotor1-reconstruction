// CScriptEvent (0x004D7270-0x004D79A2; behavioural reference:
// src/batches/b004d42_combat_round_lifecycle.cpp, b004ed_creature_misc.cpp).
#include "a/script_event.h"

// FUNCTION: SWKOTOR 0x004D7270
int CScriptEvent::operator==(const CScriptEvent& e)
{
    int i;
    if (m_lstIntegers.num != e.m_lstIntegers.num) {
        return 0;
    }
    for (i = 0; i < m_lstIntegers.num; i++) {
        if (m_lstIntegers.element[i] != e.m_lstIntegers.element[i]) {
            return 0;
        }
    }
    if (m_lstFloats.num != e.m_lstFloats.num) {
        return 0;
    }
    for (i = 0; i < m_lstFloats.num; i++) {
        if (m_lstFloats.element[i] != e.m_lstFloats.element[i]) {
            return 0;
        }
    }
    if (m_lstObjects.num != e.m_lstObjects.num) {
        return 0;
    }
    for (i = 0; i < m_lstObjects.num; i++) {
        if (m_lstObjects.element[i] != e.m_lstObjects.element[i]) {
            return 0;
        }
    }
    if (m_lstStrings.num != e.m_lstStrings.num) {
        return 0;
    }
    for (i = 0; i < m_lstStrings.num; i++) {
        if (m_lstStrings.element[i] != e.m_lstStrings.element[i]) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D73A0
int CScriptEvent::SaveEvent(CResGFF* pRes, CResStruct* pStruct)
{
    CResList cList;
    int i;

    pRes->WriteFieldWORD(pStruct, m_nType, "EventId");

    if (m_lstIntegers.num > 0) {
        pRes->AddList(&cList, pStruct, "IntList");
        for (i = 0; i < m_lstIntegers.num; i++) {
            CResStruct cElement;
            pRes->AddListElement(&cElement, &cList, 0x69);
            pRes->WriteFieldINT(&cElement, m_lstIntegers.element[i], "Parameter");
        }
    }
    if (m_lstFloats.num > 0) {
        pRes->AddList(&cList, pStruct, "FloatList");
        for (i = 0; i < m_lstFloats.num; i++) {
            CResStruct cElement;
            pRes->AddListElement(&cElement, &cList, 0x69);
            pRes->WriteFieldFLOAT(&cElement, m_lstFloats.element[i], "Parameter");
        }
    }
    if (m_lstStrings.num > 0) {
        pRes->AddList(&cList, pStruct, "StringList");
        for (i = 0; i < m_lstStrings.num; i++) {
            CResStruct cElement;
            pRes->AddListElement(&cElement, &cList, 0x69);
            pRes->WriteFieldCExoString(&cElement, m_lstStrings.element[i], "Parameter");
        }
    }
    if (m_lstObjects.num > 0) {
        pRes->AddList(&cList, pStruct, "ObjectList");
        for (i = 0; i < m_lstObjects.num; i++) {
            CResStruct cElement;
            pRes->AddListElement(&cElement, &cList, 0x69);
            pRes->WriteFieldDWORD(&cElement, m_lstObjects.element[i], "Parameter");
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D7540
CScriptEvent::CScriptEvent()
{
}

// FUNCTION: SWKOTOR 0x004D7590
CScriptEvent::~CScriptEvent()
{
    m_lstIntegers.SetSize(0);
    m_lstFloats.SetSize(0);
    m_lstObjects.SetSize(0);
    m_lstStrings.SetSize(0);
}

// FUNCTION: SWKOTOR 0x004D77F0
void CScriptEvent::CopyScriptEvent(CScriptEvent* pSource)
{
    int i;
    if (pSource == NULL) {
        return;
    }
    m_nType = pSource->m_nType;
    m_lstIntegers.SetSize(pSource->m_lstIntegers.num);
    for (i = 0; i < pSource->m_lstIntegers.num; i++) {
        m_lstIntegers.element[i] = pSource->m_lstIntegers.element[i];
    }
    m_lstFloats.SetSize(pSource->m_lstFloats.num);
    for (i = 0; i < pSource->m_lstFloats.num; i++) {
        m_lstFloats.element[i] = pSource->m_lstFloats.element[i];
    }
    m_lstObjects.SetSize(pSource->m_lstObjects.num);
    for (i = 0; i < pSource->m_lstObjects.num; i++) {
        m_lstObjects.element[i] = pSource->m_lstObjects.element[i];
    }
    m_lstStrings.SetSize(pSource->m_lstStrings.num);
    for (i = 0; i < pSource->m_lstStrings.num; i++) {
        m_lstStrings.element[i] = pSource->m_lstStrings.element[i];
    }
}
