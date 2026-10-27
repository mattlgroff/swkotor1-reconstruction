// Event-log helper next to CServerAIMaster: describes a game object id as
// "id[,tag]" plus its object kind.
#include "i/server_exo_app.h"

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                                          // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

// FUNCTION: SWKOTOR 0x004AF410
void GetGameObjectDescription(unsigned long oid, CExoString& sId, CExoString& sType)
{
    CSWSObject* pObject = NULL;
    CGameObject* pGameObject = GetServerExoApp()->GetGameObject(oid);
    CExoString* pTag;
    if (pGameObject && (pObject = pGameObject->AsSWSObject()) != NULL &&
        (pTag = &pObject->m_sTag) != NULL && *pTag != "") {
        sId.Format("%x,%s", oid, *pTag);
    } else {
        sId.Format("%x", oid);
    }

    if (pObject) {
        if (pObject->AsSWSAreaOfEffectObject()) {
            sType = "AOE";
        } else if (pObject->AsSWSCreature()) {
            sType = "Creature";
        } else if (pObject->AsSWSDoor()) {
            sType = "Door";
        } else if (pObject->AsSWSPlaceable()) {
            sType = "Placeable";
        } else if (pObject->AsSWSEncounter()) {
            sType = "Encounter";
        } else if (pObject->AsSWSTrigger()) {
            sType = "Trigger";
        } else if (pObject->AsSWSWaypoint()) {
            sType = "Waypoint";
        } else if (pObject->AsSWSItem()) {
            sType = "Item";
        } else if (pObject->AsSWSSoundObject()) {
            sType = "Sound";
        } else if (pObject->AsSWSStore()) {
            sType = "Store";
        } else {
            sType = "Object";
        }
    } else if (pGameObject) {
        if (pGameObject->AsSWSArea()) {
            sType = "Area";
        } else if (pGameObject->AsSWSModule()) {
            sType = "Module";
        } else {
            sType = "GameObject";
        }
    } else {
        sType = "Unknown";
    }
}
