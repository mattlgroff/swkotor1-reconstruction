// CSWVirtualMachineCommands::ExecuteCommandSetFacing (round s9).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/server_exo_app.h"
#include <math.h>

extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)

class CSWCCreatureFacing {
public:
    void SetLockOrientation(unsigned long oidTarget, int bLock); // 0x00610950 (name guessed)
};

// Client game object view: slot 0x2c returns the client creature.
class CSWCObjectFacingView {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CSWCCreatureFacing* AsSWCCreature();            // 0x2c
};

inline Vector VectorSubtract(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// SetFacing (10) and SetFacingPoint (0x8f) share this handler.
// FUNCTION: SWKOTOR 0x00542830
int CSWVirtualMachineCommands::ExecuteCommandSetFacing(int nCommandId, int nParameters)
{
    Vector vFacing(0.0f, 0.0f, 0.0f);
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    bool bSet = false;
    Vector vPoint(0.0f, 0.0f, 0.0f);
    CSWSObject* pObject;
    if (nCommandId == 10) {
        float fAngle;
        if (!g_pVirtualMachine->StackPopFloat(&fAngle)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        float fRadians = fAngle * 0.017453292f;
        vFacing = Vector((float)cos(fRadians), (float)sin(fRadians), 0.0f);
        bSet = true;
    } else if (nCommandId == 0x8f) {
        if (!g_pVirtualMachine->StackPopVector(&vPoint)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    if (pObjectArray->GetGameObject(m_oidObjectRunScript, (CGameObject**)&pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        if (nCommandId == 0x8f) {
            Vector vDelta = VectorSubtract(vPoint, pObject->m_vPosition);
            if (vDelta.x != 0.0f || vDelta.y != 0.0f || vDelta.z != 0.0f) {
                vFacing = Normalize(vDelta);
                bSet = true;
            }
        }
        if (m_bValidObjectRunScript == 1 && bSet) {
            unsigned long oidClient = g_pAppManager->m_pClientExoApp->ServerToClientObjectId(pObject->m_idSelf);
            CSWCObjectFacingView* pClientObject = (CSWCObjectFacingView*)g_pAppManager->m_pClientExoApp->GetGameObject(oidClient);
            if (pClientObject && pClientObject->AsSWCCreature()) {
                pClientObject->AsSWCCreature()->SetLockOrientation(INVALID_OBJECT_ID, 0);
            }
            pObject->SetDialogOwner(INVALID_OBJECT_ID);
            pObject->SetOrientation(vFacing);
        }
    }
    return 0;
}
