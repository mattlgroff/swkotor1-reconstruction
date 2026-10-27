// CSWVirtualMachineCommands::ExecuteCommandCreateObject (round s10).
// Original quirks kept: the popped location leaks when a later pop fails,
// and a waypoint is created but OBJECT_INVALID is returned.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/vector.h"

class CScriptLocation {
public:
    CScriptLocation();                                      // 0x004CA7A0
    ~CScriptLocation();                                     // 0x004015A0

    Vector m_vPosition;                                     // 0x00
    Vector m_vOrientation;                                  // 0x0c
};

class Quaternion {
public:
    Quaternion(Vector vAxis, float fAngle);                 // 0x004AC960

    float w, x, y, z;
};

float GetFacing(const Vector& vOrientation);                // 0x004AA0F0

class CSWSStore : public CSWSObject {
public:
    CSWSStore(unsigned long nId);                           // 0x005C6AB0
    int LoadFromTemplate(const CResRef& cResRef, int bArg); // 0x005C7760
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg); // 0x005C6E80

    char pad_0xf4[0x258 - 0xf4];
};

class CSWSWaypoint : public CSWSObject {
public:
    CSWSWaypoint(unsigned long nId);                        // 0x005C7E70
    int LoadFromTemplate(const CResRef& cResRef);           // 0x005C83B0
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg); // 0x005C8580

    char pad_0xf4[0x240 - 0xf4];
};

extern const unsigned long OBJECT_INVALID;                  // 0x00746644
extern const unsigned short EFFECT_TYPE_APPEAR;             // 0x007467C0 (name guessed)

// NONMATCH (63): from the safe-location copy on, cl rotates the scratch
// registers (eax/ecx/edx) for every later temporary.
// FUNCTION: SWKOTOR 0x0052F820
int CSWVirtualMachineCommands::ExecuteCommandCreateObject(int nCommandId, int nParameters)
{
    unsigned long oidResult = OBJECT_INVALID;
    CExoString sTemplate;
    CScriptLocation* pLocation;
    int nObjectType;
    int bUseAppearAnimation = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nObjectType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sTemplate)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 4 && !g_pVirtualMachine->StackPopInteger(&bUseAppearAnimation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        CSWSArea* pArea = pModule->GetArea();
        if (pArea) {
            CResRef cResRef(sTemplate.CStr());
            switch (nObjectType) {
            case 2: {
                CSWSItem* pItem = new CSWSItem(INVALID_OBJECT_ID);
                pItem->LoadFromTemplate(cResRef);
                pItem->SetPosition(pLocation->m_vPosition, 1, 1, 0);
                pItem->SetOrientation(pLocation->m_vOrientation);
                pItem->AddToArea(pArea, pLocation->m_vPosition.x, pLocation->m_vPosition.y,
                                 pLocation->m_vPosition.z, 1);
                oidResult = pItem->m_idSelf;
                break;
            }
            case 1: {
                Vector vSafe(0.0f, 0.0f, 0.0f);
                CSWSCreature* pCreature = new CSWSCreature(INVALID_OBJECT_ID, 0);
                if (!pCreature->LoadFromTemplate(cResRef, 0)) {
                    delete pCreature;
                    if (pLocation) {
                        delete pLocation;
                    }
                    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
                        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
                    }
                    return 0;
                }
                pCreature->SetOrientation(pLocation->m_vOrientation);
                vSafe = pLocation->m_vPosition;
                if (pArea->ComputeSafeLocation(pLocation->m_vPosition, 20.0f, pCreature->m_n340, 1, &vSafe)) {
                    pLocation->m_vPosition = vSafe;
                }
                vSafe.z = ((CGameObject*)&pArea->AsGameObject())->AsSWSArea()->ComputeHeight(vSafe, 1, NULL);
                if (bUseAppearAnimation) {
                    CGameEffect* pEffect = new CGameEffect(1);
                    pEffect->m_nType = EFFECT_TYPE_APPEAR;
                    pEffect->SetCreator(m_oidObjectRunScript);
                    pCreature->ApplyEffect(pEffect, 0, 0);
                }
                pCreature->AddToArea(pArea, vSafe.x, vSafe.y, vSafe.z, 1, 1);
                oidResult = pCreature->m_idSelf;
                break;
            }
            case 64: {
                CSWSPlaceable* pPlaceable = new CSWSPlaceable(INVALID_OBJECT_ID);
                pPlaceable->LoadFromTemplate(cResRef, 0);
                pPlaceable->SetPosition(pLocation->m_vPosition, 1, 1, 0);
                pPlaceable->SetOrientation(pLocation->m_vOrientation);
                Vector vAxis(0.0f, 0.0f, 1.0f);
                Quaternion qOrientation(vAxis, GetFacing(pLocation->m_vOrientation) * 0.017453292f);
                pPlaceable->SetOrientation(qOrientation);
                pPlaceable->AddToArea(pArea, pLocation->m_vPosition, 1);
                oidResult = pPlaceable->m_idSelf;
                break;
            }
            case 128: {
                CSWSStore* pStore = new CSWSStore(INVALID_OBJECT_ID);
                pStore->LoadFromTemplate(cResRef, 0);
                pStore->SetPosition(pLocation->m_vPosition, 1, 1, 0);
                pStore->SetOrientation(pLocation->m_vOrientation);
                pStore->AddToArea(pArea, pLocation->m_vPosition.x, pLocation->m_vPosition.y,
                                  pLocation->m_vPosition.z, 1);
                oidResult = pStore->m_idSelf;
                break;
            }
            case 32: {
                CSWSWaypoint* pWaypoint = new CSWSWaypoint(INVALID_OBJECT_ID);
                pWaypoint->LoadFromTemplate(cResRef);
                pWaypoint->SetPosition(pLocation->m_vPosition, 1, 1, 0);
                pWaypoint->SetOrientation(pLocation->m_vOrientation);
                pWaypoint->AddToArea(pArea, pLocation->m_vPosition.x, pLocation->m_vPosition.y,
                                     pLocation->m_vPosition.z, 0);
                break;
            }
            }
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        if (pLocation) {
            delete pLocation;
            pLocation = NULL;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (pLocation) {
        delete pLocation;
        pLocation = NULL;
    }
    return 0;
}
