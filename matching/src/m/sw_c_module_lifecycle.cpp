// CSWCModule construction and destruction (nwcmodule.o). Field names
// follow m/sw_c_module_camera.h; this translation unit declares the full
// polymorphic layout.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_pResRef[16];
};

class Vector {
public:
    float x, y, z;

    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Remove(T value) throw();                   // 0x00445080
};

class Gob {
public:
    virtual ~Gob();
    CExoArrayList<Gob**> m_lWatchers;               // +0x04
};

class GobRef {
public:
    Gob* m_p;

    GobRef() { m_p = 0; }
    ~GobRef()
    {
        if (m_p) {
            CExoArrayList<Gob**>& l = m_p->m_lWatchers;
            l.Remove(&m_p);
        }
    }
};

class CSWCameraSettings {
public:
    int m_nValid;                                   // +0x00
    int m_nType;                                    // +0x04
    Gob* m_pCamera;                                 // +0x08
    GobRef m_rTarget;                               // +0x0c
    int m_nStyle;                                   // +0x10
    int m_n14;                                      // +0x14
    float m_f18;                                    // +0x18
    int m_nVideoEffect;                             // +0x1c
};

class CGameObject {
public:
    CGameObject(unsigned char nObjectType, unsigned long oid);  // 0x004D7D60
    virtual ~CGameObject() {}

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CSWCArea {
public:
    virtual ~CSWCArea();
    void UnloadArea();                              // 0x00609410
};

class CSWCModuleCamera {
public:
    virtual ~CSWCModuleCamera();
};

class CCameraBehavior {
public:
    virtual ~CCameraBehavior();
};

extern const unsigned char OBJECT_TYPE_MODULE;      // 0x00746632
extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern const unsigned long g_nModuleDefaultA4;      // 0x0074410C, descriptive

class CSWCModule : public CGameObject {
public:
    CSWCModule();
    virtual ~CSWCModule();

    int m_nCameraMode;                              // +0x0c
    CExoString m_sModuleName;                       // +0x10
    CResRef m_cResRef18;                            // +0x18
    CResRef m_cResRef28;                            // +0x28
    CExoString m_sString38;                         // +0x38
    CSWCModuleCamera* m_pCamera;                    // +0x40
    int m_n44;                                      // +0x44
    CSWCArea* m_pArea;                              // +0x48
    Vector m_vFollowPosition;                       // +0x4c
    Vector m_v58;                                   // +0x58
    Vector m_vCameraUp;                             // +0x64
    unsigned long m_oidFollow;                      // +0x70
    int m_n74;                                      // +0x74
    int m_n78;                                      // +0x78
    int m_n7C;                                      // +0x7c
    unsigned long m_nCameraFlags;                   // +0x80
    int m_n84;                                      // +0x84
    int m_n88;                                      // +0x88
    int m_bClearViewRequest;                        // +0x8c
    unsigned char m_n90;                            // +0x90
    float m_fCameraTilt;                            // +0x94
    float m_fCameraTurn;                            // +0x98
    float m_fCameraZoom;                            // +0x9c
    unsigned char m_nMinutesPerHour;                // +0xa0
    unsigned long m_nA4;                            // +0xa4
    int m_nA8;                                      // +0xa8
    int m_nAC;                                      // +0xac
    int m_nB0;                                      // +0xb0
    unsigned char m_nTimeOfDayState;                // +0xb4
    int m_nB8;                                      // +0xb8
    int m_nBC;                                      // +0xbc
    int m_nC0;                                      // +0xc0
    int m_nC4;                                      // +0xc4
    int m_nC8;                                      // +0xc8
    int m_nCC;                                      // +0xcc
    int m_nD0;                                      // +0xd0
    unsigned char m_nD4;                            // +0xd4
    unsigned char m_nLastCameraOption;              // +0xd5
    int m_bResetCameraParams;                       // +0xd8
    int m_nShakeRemaining;                          // +0xdc
    int m_nShakeE0;                                 // +0xe0
    int m_nShakeDelay;                              // +0xe4
    int m_nShakeDuration;                           // +0xe8
    int m_bShakeChanged;                            // +0xec
    int m_nBump;                                    // +0xf0
    int m_nBumpDuration;                            // +0xf4
    int m_bBumpChanged;                             // +0xf8
    int m_bFollowValid;                             // +0xfc
    int m_bTurnChanged;                             // +0x100
    int m_bZoomChanged;                             // +0x104
    int m_n108;                                     // +0x108
    CSWCameraSettings m_cSavedCamera;               // +0x10c
    CCameraBehavior* m_pCameraModel;                // +0x12c
    CExoString m_sCameraName;                       // +0x130
};

// FUNCTION: SWKOTOR 0x00641CD0
CSWCModule::~CSWCModule()
{
    m_sModuleName = "";
    if (m_pArea) {
        m_pArea->UnloadArea();
        delete m_pArea;
        m_pArea = 0;
    }
    if (m_pCamera) {
        delete m_pCamera;
        m_pCamera = 0;
    }
    if (m_pCameraModel) {
        delete m_pCameraModel;
        m_pCameraModel = 0;
    }
}

// FUNCTION: SWKOTOR 0x00643F40
CSWCModule::CSWCModule()
    : CGameObject(OBJECT_TYPE_MODULE, 0x7f000000)
{
    m_vFollowPosition = Vector(0.0f, 0.0f, 0.0f);
    m_v58 = Vector(0.0f, 0.0f, 0.0f);
    m_pCamera = 0;
    m_n44 = 0;
    m_pArea = 0;
    m_nCameraMode = -1;
    m_nCameraFlags = 0;
    m_n84 = 0;
    m_n88 = 0;
    m_vCameraUp = Vector(0.0f, 0.0f, 1.0f);
    m_oidFollow = OBJECT_INVALID;
    m_n74 = 0;
    m_n78 = 0;
    m_n7C = 0;
    m_n90 = 0;
    m_bClearViewRequest = 0;
    m_fCameraTilt = 0.0f;
    m_fCameraTurn = 0.0f;
    m_fCameraZoom = 0.0f;
    m_sModuleName = "";
    m_nA4 = g_nModuleDefaultA4;
    m_nA8 = 1;
    m_nAC = 1;
    m_bShakeChanged = 1;
    m_bBumpChanged = 1;
    m_bFollowValid = 1;
    m_nB0 = 0;
    m_nTimeOfDayState = 0;
    m_nB8 = 0;
    m_nBC = 0;
    m_nD4 = 0;
    m_nC8 = 0;
    m_nCC = 0;
    m_nD0 = 0;
    m_bResetCameraParams = 0;
    m_nShakeRemaining = 0;
    m_nShakeE0 = 0;
    m_nShakeDelay = 0;
    m_nBump = 0;
    m_nBumpDuration = 0;
    m_bTurnChanged = 0;
    m_bZoomChanged = 0;
    m_n108 = 0;
    m_cSavedCamera.m_nValid = 0;
    m_pCameraModel = 0;
    m_nMinutesPerHour = 5;
    m_nLastCameraOption = 0xff;
}
