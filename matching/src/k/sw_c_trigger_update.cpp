// CSWCTrigger::AIUpdate (vtable slot 0x74). Not in the function table:
// starts after 0x006920B0's padding.
#include "k/sw_c_trigger.h"

// FUNCTION: SWKOTOR 0x00692410
// NONMATCH: 10 diff insns, scratch-register choice only (the original uses
// eax for the delete vtable, edx for the slot-0xe8 vtable and eax for the
// final app load; ours edx / eax / ecx). The tail registers come out right
// only when the SubtractWorldTimes app load is in the wrong (late) form,
// and writing the update arm first inverts the block layout. Tried inline
// helper, result locals, local / accessor / direct app expressions.
void CSWCTrigger::AIUpdate()
{
    unsigned long day, tick, elapsedDay, elapsedTick;
    CClientExoApp* app = g_pAppManager->m_pClientExoApp;
    app->GetWorldTimer()->GetWorldTime(&day, &tick);
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->SubtractWorldTimes(day, tick, m_nDay, m_nTick, &elapsedDay, &elapsedTick);
    if (m_nCountdown) {
        if (m_nCountdown > m_nElapsed)
            m_nCountdown -= m_nElapsed;
        else
            m_nCountdown = 0;
    }
    m_nElapsed = elapsedTick;
    if (m_nE4) {
        if (m_n13C <= elapsedTick) {
            CGameObject* removed;
            CGameObjectArray* objects = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
            if (objects->Delete(m_oid, &removed) == g_nObjectArraySuccess && removed) {
                delete this;
                return;
            }
        }
        else {
            m_n13C -= elapsedTick;
        }
        return;
    }
    CalculatePosition();
    UnknownE8();
    UpdateDetection();
    UpdateAnimation();
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&day, &tick);
    m_nDay = day;
    m_nTick = tick;
}
