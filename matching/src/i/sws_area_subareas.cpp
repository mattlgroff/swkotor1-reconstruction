// CSWSArea sub-area (trigger / area-of-effect / encounter / door)
// queries over the +0x1A0 object-id list (0x004BF2C0-0x004BF644).
#include "i/sws_area.h"

// FUNCTION: SWKOTOR 0x004BF2C0
int CSWSArea::IntersectingLineSegment(Vector* pStart, Vector* pEnd,
                                      CExoArrayList<unsigned long>* pList)
{
    int bFound = 0;
    for (int i = 0; i < m_aSubAreas.num; i++) {
        CGameObject* pObject = GetServerExoApp()->GetGameObject(m_aSubAreas.element[i]);
        if (pObject == NULL) {
            continue;
        }
        int nCount = 0;
        for (int j = 0; j < pList->num; j++) {
            if (pList->element[j] == m_aSubAreas.element[i]) {
                nCount++;
            }
        }
        if (nCount != 0) {
            continue;
        }
        if (pObject->AsSWSTrigger()) {
            if (pObject->AsSWSTrigger()->LineSegmentIntersectTrigger(pStart, pEnd)) {
                bFound = 1;
                pList->AddInline(m_aSubAreas.element[i]);
            }
        }
        else if (pObject->AsSWSAreaOfEffectObject()) {
            if (pObject->AsSWSAreaOfEffectObject()->LineSegmentIntersectAreaOfEffect(*pStart, *pEnd)) {
                bFound = 1;
                pList->Add(m_aSubAreas.element[i]);
            }
        }
        else if (pObject->AsSWSEncounter()) {
            if (pObject->AsSWSEncounter()->LineSegmentIntersectActivateArea(pStart, pEnd)) {
                bFound = 1;
                pList->Add(m_aSubAreas.element[i]);
            }
        }
    }
    return bFound;
}

// FUNCTION: SWKOTOR 0x004BF470
int CSWSArea::InSubAreas(Vector* pPosition, CExoArrayList<unsigned long>* pList)
{
    int bFound = 0;
    unsigned long oidMissing = OBJECT_INVALID;
    for (int i = 0; i < m_aSubAreas.num; i++) {
        int nCount = 0;
        for (int j = 0; j < pList->num; j++) {
            if (pList->element[j] == m_aSubAreas.element[i]) {
                nCount++;
            }
        }
        if (nCount != 0) {
            continue;
        }
        CGameObject* pObject = GetServerExoApp()->GetGameObject(m_aSubAreas.element[i]);
        if (pObject == NULL) {
            oidMissing = m_aSubAreas.element[i];
            continue;
        }
        if (pObject->AsSWSTrigger()) {
            if (pObject->AsSWSTrigger()->InTrigger(pPosition)) {
                bFound = 1;
                pList->Add(m_aSubAreas.element[i]);
            }
        }
        else if (pObject->AsSWSAreaOfEffectObject()) {
            if (pObject->AsSWSAreaOfEffectObject()->InAreaOfEffect(*pPosition)) {
                bFound = 1;
                pList->Add(m_aSubAreas.element[i]);
            }
        }
        else if (pObject->AsSWSEncounter()) {
            if (pObject->AsSWSEncounter()->InActivateArea(pPosition)) {
                bFound = 1;
                pList->Add(m_aSubAreas.element[i]);
            }
        }
        if (pObject->AsSWSDoor()) {
            if (pObject->AsSWSDoor()->InDoorArea(*pPosition)) {
                bFound = 1;
                pList->Add(m_aSubAreas.element[i]);
            }
        }
    }
    if (oidMissing != OBJECT_INVALID) {
        m_aSubAreas.Remove(oidMissing);
    }
    return bFound;
}
