// CSWCCreature::GetTargetActions (nwccreature.o, worker C s3): per-target
// action menu dispatch. Virtual slots copied from c/client_exo_app.h.

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08
};

class CSWGuiInterfaceAction;
class CSWCObject;
class CSWCCreature;
class CSWCDoor;
class CSWCTrigger;
class CSWCPlaceable;

typedef CExoArrayList<CSWGuiInterfaceAction> CSWGuiActionList;

class CSWSObject {
public:
    int GetReputation(unsigned long oidSource, int bUnknown);  // 0x0057CB80
};

#include "shared/game_object.h"

class CSWCObject : public CGameObject {
public:
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134();
    virtual int IsHostile();                        // 0x138

    CGameObject* GetServerObject();                 // 0x0063D4B0
};

class CSWCPlaceable : public CSWCObject {
public:
    CSWSObject* GetServerPlaceable();               // 0x00682E00
    void GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);  // 0x006837D0
};

class CSWCDoor : public CSWCObject {
public:
    void GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);  // 0x00684410
};

class CSWCTrigger : public CSWCObject {
public:
    void GetTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions);  // 0x00691F00
};

class CSWCCreature : public CSWCObject {
public:
    void GetTargetActions(CSWCObject* pTarget, int nMenu, CSWGuiActionList& lstActions);
    int GetTargetActionType(CSWCObject* pTarget);   // 0x0060FCC0 (descriptive)
    void GetHostileForcePowers(CSWCObject* pTarget, CSWGuiActionList& lstActions);  // 0x006191F0
    void GetCombatFeatsSkills(CSWGuiActionList& lstActions, CSWCCreature* pTarget); // 0x00619950
    void GetHostileItems(CSWGuiActionList& lstActions, CSWCCreature* pTarget);      // 0x006198E0
    void GetCreatureTargetActions(CSWCCreature* pSource, int nMenu, CSWGuiActionList& lstActions); // 0x00619B10 (descriptive)
};

// FUNCTION: SWKOTOR 0x00619C20
void CSWCCreature::GetTargetActions(CSWCObject* pTarget, int nMenu, CSWGuiActionList& lstActions)
{
    switch (GetTargetActionType(pTarget)) {
    case 1:
        if (pTarget->AsSWCPlaceable()) {
            pTarget->AsSWCPlaceable()->GetTargetActions(this, nMenu, lstActions);
        } else if (pTarget->AsSWCDoor()) {
            pTarget->AsSWCDoor()->GetTargetActions(this, nMenu, lstActions);
        }
        break;
    case 2:
        if (pTarget->AsSWCTrigger()) {
            pTarget->AsSWCTrigger()->GetTargetActions(this, nMenu, lstActions);
        }
        break;
    case 3:
        if (pTarget->AsSWCPlaceable()) {
            if (nMenu == 1 && pTarget->AsSWCPlaceable()->IsHostile()) {
                unsigned long oidSelf = GetServerObject()->m_idSelf;
                if (pTarget->AsSWCPlaceable()->GetServerPlaceable()->GetReputation(oidSelf, 0) <= 10) {
                    GetHostileForcePowers(pTarget, lstActions);
                    break;
                }
            }
            pTarget->AsSWCPlaceable()->GetTargetActions(this, nMenu, lstActions);
        }
        break;
    case 4:
        switch (nMenu) {
        case 0:
            GetCombatFeatsSkills(lstActions, pTarget->AsSWCCreature());
            pTarget->AsSWCCreature()->GetCreatureTargetActions(this, 0, lstActions);
            break;
        case 1:
            GetHostileForcePowers(pTarget, lstActions);
            break;
        case 2:
            GetHostileItems(lstActions, pTarget->AsSWCCreature());
            break;
        }
        break;
    }
}
