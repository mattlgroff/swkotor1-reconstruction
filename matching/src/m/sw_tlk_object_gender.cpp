// CSWTlkTable derives the displayed gender from the object's creature data.

class CSWCObject;
class CSWCCreature;
class CSWSCreature;

class CGameObject {
public:
    virtual void V00();
    virtual void V04();
    virtual void SetWorldTime(unsigned long nDay, unsigned long nTime);
    virtual CSWCObject* AsSWCObject();                // vtable +0x0C
    virtual void V10();
    virtual void* AsSWCDoor();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void* AsSWCArea();
    virtual void V28();
    virtual CSWCCreature* AsSWCCreature();            // vtable +0x2C
    virtual CSWSCreature* AsSWSCreature();
    virtual void* GetItemModel();

    unsigned long m_oidSelf;
    unsigned char m_nObjectType;
};

class CSWCCreatureAppearance {
public:
    char pad_0x0[0x31];
    unsigned char m_nGender;                          // +0x31
};

class CSWCCreature : public CGameObject {
public:
    char pad_0x0C[0x2F8 - 0x0C];
    CSWCCreatureAppearance* m_pAppearance;            // +0x2F8
};

class CSWTlkTable {
public:
    char pad_0x0[0x58];
    CGameObject* m_pObject;                           // +0x58

    unsigned char GetObjectGender();
};

extern unsigned char g_nDefaultObjectGender;           // 0x0073ECD0

// FUNCTION: SWKOTOR 0x00674B40 SYMBOL: ?GetObjectGender@CSWTlkTable@@QAEEXZ
unsigned char CSWTlkTable::GetObjectGender()
{
    if (m_pObject != 0 && m_pObject->AsSWCObject() != 0) {
        if (m_pObject->AsSWCCreature() != 0) {
            if (m_pObject->AsSWCCreature()->m_pAppearance != 0) {
                return m_pObject->AsSWCCreature()->m_pAppearance->m_nGender;
            }
        }
    }
    return g_nDefaultObjectGender;
}
