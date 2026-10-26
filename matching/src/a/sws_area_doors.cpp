// CSWSArea door-plane crossing test (0x005064D0). The name is descriptive:
// it returns the first static door box crossed by the segment whose door
// plane separates the (raised) end points, or OBJECT_INVALID.
#include "a/sws_area.h"


// FUNCTION: SWKOTOR 0x005064D0
unsigned long CSWSArea::GetDoorCrossed(Vector* pFrom, Vector* pTo)
{
    Vector vTo;
    Vector vFrom = *pFrom;
    vFrom.z = vFrom.z + 0.5f;
    vTo = *pTo;
    vTo.z = vTo.z + 0.5f;
    for (int i = 0; i < m_nDoorBoxes; i++) {
        if (!BoxIntersect(&vFrom, &vTo, &m_pDoorBoxes[i].vMin, &m_pDoorBoxes[i].vMax)) {
            continue;
        }
        unsigned long oidDoor = m_pDoorBoxIds[i];
        CSWSDoor* pDoor = GetServerExoApp()->GetDoorByGameObjectID(oidDoor);
        if (pDoor == NULL) {
            continue;
        }
        Vector vPosition = pDoor->m_vPosition;
        int bFrom = (vFrom - vPosition).Dot(pDoor->m_vOrientation) > 0.0f;
        int bTo = (vTo - vPosition).Dot(pDoor->m_vOrientation) > 0.0f;
        if (bFrom != bTo) {
            return oidDoor;
        }
    }
    return 0x7f000000;
}

Vector Normalize(const Vector& v);                  // 0x004AB180

// FUNCTION: SWKOTOR 0x00508180
void CSWSArea::BudgeCreatures(Vector* pOrigin, Vector* pMin, Vector* pMax, unsigned long oidDoor,
                              int bUseActionPoints)
{
    CExoArrayList<unsigned long>* pObjects = &m_aGameObjects;
    int nIndex = 0;
    GetFirstObjectIndiceByX(&nIndex, pOrigin->x - pMin->x - 3.0f);
    Vector vTarget(0.0f, 0.0f, 0.0f);
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (pObjects == NULL || pObjects->num <= 0) {
        return;
    }
    int i = nIndex;
    if (i >= pObjects->num) {
        return;
    }
    do {
        CSWSCreature* pCreature =
            GetServerExoApp()->GetCreatureByGameObjectID(pObjects->element[i]);
        i++;
        if (pCreature == NULL) {
            continue;
        }
        vPosition = pCreature->m_vPosition;
        if (pMax->x + 3.0f <= vPosition.x) {
            return;
        }
        if (pMin->y - 3.0f >= vPosition.y || pMax->y + 3.0f <= vPosition.y) {
            continue;
        }
        if (TestSafeLocationPoint(vPosition, pCreature->m_pPathfindInformation)) {
            continue;
        }
        if (bUseActionPoints) {
            CSWSDoor* pDoor = GetServerExoApp()->GetDoorByGameObjectID(oidDoor);
            if (pDoor) {
                vTarget = pDoor->GetNearestActionPoint(vPosition, 0);
            } else {
                CSWSPlaceable* pPlaceable = GetServerExoApp()->GetPlaceableByGameObjectID(oidDoor);
                if (pPlaceable) {
                    vTarget = pPlaceable->GetNearestActionPoint(vPosition);
                }
            }
        } else {
            vTarget = vPosition + Normalize(vPosition - *pOrigin);
        }
        if (ComputeSafeLocation(vPosition, 10.0f, pCreature->m_pPathfindInformation, 0, &vTarget)) {
            pCreature->SetPosition(vTarget, 1, 1, 0);
        }
    } while (i < pObjects->num);
}
