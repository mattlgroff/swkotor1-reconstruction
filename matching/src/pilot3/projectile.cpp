// Pilot group 3: CSWSObject::BroadcastSafeProjectile.

struct Vector {
    float x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
};

class CGameObject;
class CSWSArea;
class CSWSPlayer;

class CSWSItem {
public:
    char pad_0x0[0xc];
    unsigned char m_nWeaponType; // +0xc
};

class CSWSInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCreature {
public:
    char pad_0x0[0x4];
    unsigned long m_idSelf;      // +0x4
    char pad_0x8[0x88];
    Vector m_vPosition;          // +0x90
    char pad_0x9c[0xa2c - 0x9c];
    CSWSInventory *m_pInventory; // +0xa2c
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24();
    virtual CSWSArea *AsSWSArea(); // +0x28
};

class CSWSArea {
public:
    char pad_0x0[0x190];
    CExoArrayList<unsigned long> m_aGameObjects; // +0x190
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

class CSWSMessage {
public:
    int SendServerToPlayerSafeProjectile(CSWSPlayer *pPlayer, unsigned long oidOriginator,
                                         unsigned long oidTarget, Vector vOrigin, Vector vTarget,
                                         unsigned long a4, unsigned long a5, unsigned long a6,
                                         char nWeaponType, unsigned long a7, unsigned long a8,
                                         unsigned long a9, unsigned long a10);
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
    CSWSPlayer *GetClientObjectByObjectId(unsigned long id);
    CGameObjectArray *GetObjectArray();
    CSWSMessage *GetSWSMessage();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp; // +0x8
};

extern CAppManager *g_pAppManager;
extern float g_fSafeProjectileRange;

class CSWSObject {
public:
    void BroadcastSafeProjectile(unsigned long oidOriginator, unsigned long oidTarget,
                                 const Vector &vOrigin, const Vector &vTarget,
                                 unsigned long a4, unsigned long a5, unsigned long a6,
                                 unsigned long a7, unsigned long a8, unsigned long a9,
                                 unsigned long a10);
    char pad_0x0[0x8c];
    unsigned long m_oidArea; // +0x8c
};

// FUNCTION: SWKOTOR 0x004CDD30
void CSWSObject::BroadcastSafeProjectile(unsigned long oidOriginator, unsigned long oidTarget,
                                         const Vector &vOrigin, const Vector &vTarget,
                                         unsigned long a4, unsigned long a5, unsigned long a6,
                                         unsigned long a7, unsigned long a8, unsigned long a9,
                                         unsigned long a10)
{
    char nWeaponType = (char)0xff;
    CSWSCreature *pOriginator = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidOriginator);
    if (pOriginator) {
        CSWSInventory *pInventory = pOriginator->m_pInventory;
        CSWSItem *pItem = pInventory->GetItemInSlot(0x10);
        if (pItem)
            nWeaponType = pItem->m_nWeaponType;
    }

    CGameObject *pObject = 0;
    CGameObjectArray *pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
    pArray->GetGameObject(m_oidArea, &pObject);
    if (!pObject)
        return;
    CSWSArea *pArea = pObject->AsSWSArea();
    if (!pArea)
        return;
    CExoArrayList<unsigned long> *pList = &pArea->m_aGameObjects;
    if (!pList)
        return;

    for (int i = 0; i < pList->num; i++) {
        CSWSCreature *pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pList->element[i]);
        if (!pCreature)
            continue;
        CSWSPlayer *pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(pCreature->m_idSelf);
        if (!pPlayer)
            continue;

        Vector v1, v2;
        v1.x = pCreature->m_vPosition.x - vOrigin.x;
        v1.y = pCreature->m_vPosition.y - vOrigin.y;
        v1.z = pCreature->m_vPosition.z - vOrigin.z;
        v2.x = pCreature->m_vPosition.x - vTarget.x;
        v2.y = pCreature->m_vPosition.y - vTarget.y;
        v2.z = pCreature->m_vPosition.z - vTarget.z;
        float fRangeSq = g_fSafeProjectileRange * g_fSafeProjectileRange;
        if (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z <= fRangeSq ||
            v2.x * v2.x + v2.y * v2.y + v2.z * v2.z <= fRangeSq) {
            g_pAppManager->m_pServerExoApp->GetSWSMessage()->SendServerToPlayerSafeProjectile(
                pPlayer, oidOriginator, oidTarget, vOrigin, vTarget, a4, a5, a6,
                nWeaponType, a7, a8, a9, a10);
        }
    }
}
