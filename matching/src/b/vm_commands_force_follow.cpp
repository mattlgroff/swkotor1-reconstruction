// CSWVirtualMachineCommands::ExecuteCommandActionForceFollowObject (round s9).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/server_exo_app.h"

// Client party member entry (0x88 bytes).
struct CSWPartyFollowEntry {
    char pad_0x0[0x58];
    Vector m_vFormationPosition;                            // 0x58 (name guessed)
    char pad_0x64[0x88 - 0x64];
};

class CSWParty {
public:
    int IsPartyMember(unsigned long oidCreature);           // 0x006349C0 (returns the member index)
    Vector& GetFormationPosition(int nIndex) { return m_aMembers[nIndex].m_vFormationPosition; }

    int m_nCount;                                           // 0x000
    CSWPartyFollowEntry m_aMembers[3];                      // 0x004
    char pad_0x19c[0x1bc - 0x19c];
    int m_nFormation;                                       // 0x1bc (name guessed)
    char pad_0x1c0[0x234 - 0x1c0];
    float m_aFormationSpacing[1];                           // 0x234 (name guessed)
};

// The follow offset: the object's orientation scaled by the distance.
static __forceinline Vector VectorScale(CSWSObject* pObject, const float& f)
{
    Vector vResult;
    vResult.x = f * pObject->m_vOrientation.x;
    vResult.y = f * pObject->m_vOrientation.y;
    vResult.z = f * pObject->m_vOrientation.z;
    return vResult;
}

static inline Vector VectorAdd(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// NONMATCH (13): the original adds 0x90 to the position pointer after the first
// fadd; cl hoists the add above it (helper, reference and operand-order forms tried).
// FUNCTION: SWKOTOR 0x0052C960
int CSWVirtualMachineCommands::ExecuteCommandActionForceFollowObject(int nCommandId, int nParameters)
{
    int bTrue = 1;
    unsigned long oidFollow;
    float fFollowDistance = 0.0f;
    if (!g_pVirtualMachine->StackPopObject(&oidFollow)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fFollowDistance)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (oidFollow != INVALID_OBJECT_ID) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
        CGameObject* pFollow = g_pAppManager->GetServerExoApp()->GetGameObject(oidFollow);
        Vector vOffset = VectorScale(pFollow->AsSWSObject(), fFollowDistance);
        Vector vPoint = VectorAdd(pFollow->AsSWSObject()->m_vPosition, vOffset);
        if (pCreature) {
            unsigned long oidClient = g_pAppManager->m_pClientExoApp->ServerToClientObjectId(pCreature->m_idSelf);
            CSWCCreature* pClientCreature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(oidClient);
            if (pClientCreature && pClientCreature->m_bPartyMember) {
                int nIndex = g_pAppManager->m_pClientExoApp->GetSWParty()->IsPartyMember(pClientCreature->m_idSelf);
                if (nIndex >= 0) {
                    vPoint = g_pAppManager->m_pClientExoApp->GetSWParty()->GetFormationPosition(nIndex);
                    CSWParty* pParty = g_pAppManager->m_pClientExoApp->GetSWParty();
                    vPoint += Vector(0.0f, -pParty->m_aFormationSpacing[pParty->m_nFormation], 0.0f);
                }
            }
        }
        Vector vOwn = pCreature->m_vPosition;
        int bFalse = 0;
        pCreature->AddAction(0x37, 0xffff, 3, &oidFollow, 1, &bTrue, 2, &vPoint.x, 2, &vPoint.y, 1, &bFalse,
                             2, &vOwn.x, 2, &vOwn.y);
        pCreature->AddAction(0x3a, 0xffff, 3, &oidFollow, 1, &bTrue, 2, &vPoint.x, 2, &vPoint.y);
    }
    return 0;
}
