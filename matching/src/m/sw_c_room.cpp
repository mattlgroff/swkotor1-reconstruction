// CSWCRoom (client room, 0x48 bytes, vtable 0x00751A34): constructor,
// destructor, model spawning and the scripted room animation.
// Names follow the playable build (SpawnRoom, PlayAnimation); the scene's
// room/walkmesh factory slots keep descriptive names.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
    void Format(const char* fmt, ...);              // 0x005E5680
};

class CResRef {
public:
    char m_resRef[16];
    char* GetResRefStr() const;                     // 0x00405FE0
};

struct Vector {
    float x, y, z;
};

struct Quaternion {
    float w, x, y, z;
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* sName, float fSpeed, int nFlags, int nUnused);  // +0x18
};

class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CAurObject* NewRoom(const char* sName, const Vector& vPosition,
                                const Quaternion& qOrientation);           // +0x70 (descriptive)
    virtual void* NewRoomWalkmesh(const char* sName);                      // +0x74 (descriptive)
};

class CSWRoom {
public:
    CSWRoom();                                      // 0x005C7D10
    virtual ~CSWRoom();                             // 0x005C7D00
    Vector m_vPosition;                             // 0x04
    Quaternion m_qOrientation;                      // 0x10
    CResRef m_cResRef;                              // 0x20
    unsigned long m_nEnvAudio;                      // 0x30
    float m_fAmbientScale;                          // 0x34
    int m_bNoRoom;                                  // 0x38
};

class CSWCRoom : public CSWRoom {
public:
    CSWCRoom();
    virtual ~CSWCRoom();
    CAurObject* SpawnRoom(CAurScene* pScene);
    void PlayAnimation(unsigned short nAnimation);

    void* m_pWalkmesh;                              // 0x3c
    CAurObject* m_pModel;                           // 0x40
    int m_n44;                                      // 0x44
};

// FUNCTION: SWKOTOR 0x00646080
CSWCRoom::CSWCRoom()
{
    m_pWalkmesh = 0;
    m_pModel = 0;
    m_n44 = -1;
}

// FUNCTION: SWKOTOR 0x006460B0
CSWCRoom::~CSWCRoom()
{
}

// FUNCTION: SWKOTOR 0x006460C0
CAurObject* CSWCRoom::SpawnRoom(CAurScene* pScene)
{
    if (m_bNoRoom) {
        return 0;
    }
    CExoString sName(m_cResRef.GetResRefStr());
    m_pModel = pScene->NewRoom(sName.CStr(), m_vPosition, m_qOrientation);
    if (m_pModel) {
        m_pWalkmesh = pScene->NewRoomWalkmesh(sName.CStr());
    }
    return m_pModel;
}

// FUNCTION: SWKOTOR 0x00646180
void CSWCRoom::PlayAnimation(unsigned short nAnimation)
{
    if (!m_pModel) {
        return;
    }
    CExoString sName;
    if (nAnimation < 10) {
        sName.Format("scriptloop0%d", nAnimation);
    } else {
        sName.Format("scriptloop%d", nAnimation);
    }
    m_pModel->PlayAnimation(sName.CStr(), 1.0f, 8, 0);
}
