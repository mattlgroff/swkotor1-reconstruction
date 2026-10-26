// CSWCArea construction and destruction (worker c, s6). Member names are
// descriptive where neither symbols.csv nor the playable source names them.

void __cdecl operator delete(void*);

class Vector {
public:
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    char* m_sString;
    unsigned long m_nBufferLength;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { delete[] element; element = 0; }
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
    void DelIndex(int i)
    {
        num--;
        for (int k = i; k < num; k++)
            element[k] = element[k + 1];
    }
};

// Arrays whose destructor only frees a non-null buffer (descriptive).
template <class T>
class CExoArrayListNoClear {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayListNoClear() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayListNoClear()
    {
        if (element)
            delete[] element;
    }
};

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80
    void* pHead;
    void* pTail;
    unsigned long nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();                               // 0x00605800
    ~CExoLinkedList()
    {
        if (m_pcExoLinkedListInternal) {
            delete m_pcExoLinkedListInternal;
        }
    }
    void* RemoveHead() { return m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListInternal* m_pcExoLinkedListInternal;
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pnDay, unsigned long* pnTime);   // 0x004ADE40
};

class CGameObject;
class CSWCCreature;
class CSWCPlaceable;

class CGameObjectArray {
public:
    unsigned char Delete(unsigned long id);         // 0x004D8290
    unsigned char Delete(unsigned long id, CGameObject** ppObject);   // 0x004D81A0
};

class CClientObject {
public:
    virtual ~CClientObject();
};

class CSWCCreature : public CClientObject {
};

class CSWCSoundObject : public CClientObject {
public:
    void Stop();                                    // 0x0066B130
};

class CClientAIList {
public:
    int RemoveObject(CClientObject* pObject);       // 0x00604810
};

class CClientAIMaster {
public:
    int RemoveObject(CClientObject* pObject);       // 0x00604A30
    void* m_p0;
    CClientAIList m_lList;                          // +0x04
};

class CGuiInGame {
public:
    void FadeAndStopDialogAmbientTrack();           // 0x0062FB90
};

class CClientExoApp {
public:
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CSWCCreature* GetCreatureByGameObjectID(unsigned long id);        // 0x005ED5B0
    CSWCSoundObject* GetSoundObjectByGameObjectID(unsigned long id);  // 0x005ED620
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
};

void AurGrassClearInfo();                           // 0x004A67A0
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                 // +0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned char OBJECT_TYPE_AREA;        // 0x00746633

class CSWCArea;

class CSWCAmbientSound {
public:
    CSWCAmbientSound(CSWCArea* pArea);              // 0x0068DF40
    ~CSWCAmbientSound();                            // 0x0068E0D0
    void StopAll(int bFade);                        // 0x0068E1B0
    char pad[0x10c];
};

class CSWSArea {
public:
    void DetachFromClientArea();                    // 0x00506A70
};

class CSWCAreaPolyObject {                          // descriptive (+0x260 array)
public:
    virtual ~CSWCAreaPolyObject();
};

class CSWCAreaOwnedObject {                         // descriptive (+0x264)
public:
    virtual ~CSWCAreaOwnedObject();
};

class CSWCAreaObjectRef {                          // descriptive list payload
public:
    virtual ~CSWCAreaObjectRef();
};

struct CSWCAreaEntry218 {                           // descriptive
    unsigned long m_n0;
    CSWCAreaOwnedObject* m_pObject;                 // +0x04
};

class CSWArea {
public:
    CSWArea();                                      // 0x0058F6B0
    virtual ~CSWArea();                             // 0x0058F7A0
    void ResetBlock();                              // 0x0058F800, descriptive

    char pad_0x4[0xa0];
    unsigned long m_nA4;                            // +0xa4
    char pad_0xa8[0x58];
};

class CGameObject {
public:
    CGameObject(unsigned char nObjectType, unsigned long oid);  // 0x004D7D60
    virtual ~CGameObject() {}
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();              // +0x2c
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();            // +0x4c

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CSWCArea : public CSWArea, public CGameObject {
public:
    CSWCArea();
    virtual ~CSWCArea();
    void UnloadArea();
    void DeleteObjects();                           // 0x00606DD0
    void ClearUnknownLists();                       // 0x006075B0

    CResRef m_cResRef;                              // +0x10c
    CExoArrayList<unsigned long> m_l11C;            // +0x11c
    Vector m_v128;                                  // +0x128
    Vector m_v134;                                  // +0x134
    Vector m_v140;                                  // +0x140
    Vector m_v14C;                                  // +0x14c
    Vector m_v158;                                  // +0x158
    Vector m_v164;                                  // +0x164
    Vector m_v170;                                  // +0x170
    CExoString m_sName;                             // +0x17c
    CSWCAreaOwnedObject* m_pScene;                  // +0x184, descriptive
    CSWCAreaOwnedObject* m_pSun;                    // +0x188, descriptive
    CSWCAreaOwnedObject* m_apWeather[6];            // +0x18c, descriptive
    CSWCAreaOwnedObject* m_p1A4;                    // +0x1a4
    char pad_0x1a8[0xc];
    unsigned long m_n1B4;                           // +0x1b4
    unsigned long m_n1B8;                           // +0x1b8
    unsigned long m_n1BC;                           // +0x1bc
    unsigned long m_n1C0;                           // +0x1c0
    CExoArrayList<unsigned long> m_l1C4;            // +0x1c4
    CExoArrayList<unsigned long> m_lSoundObjects;   // +0x1d0
    CSWCAmbientSound* m_pAmbientSound;              // +0x1dc
    unsigned long m_n1E0;                           // +0x1e0
    unsigned long m_n1E4;                           // +0x1e4
    unsigned long m_n1E8;                           // +0x1e8
    unsigned long m_n1EC;                           // +0x1ec
    unsigned char m_n1F0;                           // +0x1f0
    unsigned char m_n1F1;                           // +0x1f1
    unsigned long m_n1F4;                           // +0x1f4
    unsigned long m_n1F8;                           // +0x1f8
    unsigned long m_nWorldDay;                      // +0x1fc
    unsigned long m_nWorldTime;                     // +0x200
    CExoLinkedList<CSWCAreaObjectRef> m_l204;       // +0x204
    unsigned long m_n208;                           // +0x208
    CExoArrayList<unsigned long> m_l20C;            // +0x20c
    CExoArrayList<CSWCAreaEntry218*> m_l218;        // +0x218
    CExoArrayList<unsigned long> m_lPlaceables;     // +0x224
    unsigned long m_n230;                           // +0x230
    unsigned long m_n234;                           // +0x234
    CExoArrayListNoClear<unsigned long> m_l238;     // +0x238
    CExoArrayListNoClear<unsigned long> m_l244;     // +0x244
    void* m_p250;                                   // +0x250
    unsigned char m_n254;                           // +0x254
    unsigned char m_n255;                           // +0x255
    unsigned char m_n256;                           // +0x256
    unsigned long m_n258;                           // +0x258
    unsigned long m_n25C;                           // +0x25c
    CSWCAreaPolyObject* m_p260;                     // +0x260
    CSWCAreaOwnedObject* m_p264;                    // +0x264
    unsigned long m_aoid268[4];                     // +0x268
    unsigned char m_n278;                           // +0x278
    unsigned char m_n279;                           // +0x279
    CSWSArea* m_pServerArea;                        // +0x27c
    CExoLinkedList<CSWCAreaObjectRef> m_l280;       // +0x280
};

// FUNCTION: SWKOTOR 0x00606090
CSWCArea::CSWCArea()
    : CGameObject(OBJECT_TYPE_AREA, 0x7f000000)
{
    m_sName = "";
    m_pScene = 0;
    m_pSun = 0;
    for (int i = 0; i < 6; i++)
        m_apWeather[i] = 0;
    ResetBlock();
    m_p1A4 = 0;
    m_v128 = Vector(0.0f, 0.0f, 0.0f);
    m_v134 = Vector(0.0f, 0.0f, 0.0f);
    m_v140 = Vector(0.0f, 0.0f, 0.0f);
    m_v14C = Vector(0.0f, 0.0f, 0.0f);
    m_v158 = Vector(0.0f, 0.0f, 0.0f);
    m_v164 = Vector(0.0f, 0.0f, 0.0f);
    m_n1B4 = 0;
    m_n1B8 = 0;
    m_nA4 = 0;
    m_v170 = Vector(0.0f, 0.0f, 0.0f);
    m_n1BC = 0;
    m_n1C0 = 1;
    m_pAmbientSound = new CSWCAmbientSound(this);
    m_n1E0 = 0;
    m_n1E4 = 0;
    m_n1E8 = 0;
    m_n1EC = 0;
    m_n1F0 = 0;
    m_n1F1 = 0;
    m_n1F4 = 0;
    m_n1F8 = 0;
    unsigned long nDay, nTime;
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    m_nWorldDay = nDay;
    m_nWorldTime = nTime;
    m_n254 = 0xff;
    m_n255 = 0xff;
    m_n256 = 0xff;
    m_n208 = 0;
    m_n230 = 0;
    m_n234 = 0;
    m_p250 = 0;
    m_n258 = 0;
    m_n25C = 0;
    m_p260 = 0;
    m_p264 = 0;
    m_n278 = 0;
    m_pServerArea = 0;
    m_n279 = 1;
}

// FUNCTION: SWKOTOR 0x00609410
// NONMATCH (19): DelIndex register and instruction ordering differs in both loops.
void CSWCArea::UnloadArea()
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui())
        g_pAppManager->GetClientExoApp()->GetInGameGui()->FadeAndStopDialogAmbientTrack();
    AurGrassClearInfo();
    ClearUnknownLists();
    m_l11C.Clear();
    int i;
    for (i = 0; i < 6; i++) {
        if (m_apWeather[i]) {
            delete m_apWeather[i];
            m_apWeather[i] = 0;
        }
    }

    CGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
    CClientAIMaster* pAIMaster = g_pAppManager->GetClientExoApp()->GetClientAIMaster();
    int nSounds = m_lSoundObjects.num;
    for (i = 0; i < nSounds; i++) {
        CSWCSoundObject* pSound = g_pAppManager->GetClientExoApp()->GetSoundObjectByGameObjectID(m_lSoundObjects.element[i]);
        if (pSound) {
            pSound->Stop();
            pAIMaster->RemoveObject(pSound);
            pObjects->Delete(m_lSoundObjects.element[i]);
            delete pSound;
        }
    }
    m_lSoundObjects.Clear();

    if (m_pAmbientSound)
        m_pAmbientSound->StopAll(0);
    if (m_p1A4) {
        delete m_p1A4;
        m_p1A4 = 0;
    }
    m_n1E0 = 0;
    m_n1E4 = 0;
    m_n1E8 = 0;
    m_n1EC = 0;
    m_n1F0 = 0;
    m_n1F1 = 0;
    m_n1F4 = 0;
    m_n1F8 = 0;
    DeleteObjects();
    if (m_pSun) {
        delete m_pSun;
        m_pSun = 0;
    }
    if (m_pScene) {
        delete m_pScene;
        m_pScene = 0;
    }

    while (m_l280.m_pcExoLinkedListInternal->pHead) {
        CSWCAreaObjectRef* pRef = (CSWCAreaObjectRef*)m_l280.RemoveHead();
        if (pRef)
            delete pRef;
    }
    while (m_l204.m_pcExoLinkedListInternal->pHead)
        m_l204.RemoveHead();

    for (i = m_l218.num - 1; i >= 0; i--) {
        CSWCAreaEntry218* pEntry = m_l218.element[i];
        if (pEntry) {
            if (pEntry->m_pObject)
                delete pEntry->m_pObject;
            delete m_l218.element[i];
            m_l218.DelIndex(i);
        }
    }

    for (i = m_lPlaceables.num - 1; i >= 0; i--) {
        CGameObject* pObject;
        if (g_pAppManager->GetClientExoApp()->GetGameObjectArray()->Delete(m_lPlaceables.element[i], &pObject) == GAME_OBJECT_VALID
            && pObject && pObject->AsSWCPlaceable())
            delete pObject;
        m_lPlaceables.DelIndex(i);
    }

    if (m_p250) {
        ::operator delete(m_p250);
        m_p250 = 0;
        m_n254 = 0xff;
        m_n255 = 0xff;
        m_n256 = 0xff;
        m_n258 = 0;
    }

    for (i = 0; i < 4; i++) {
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_aoid268[i]);
        if (pCreature) {
            CClientAIMaster* pMaster = g_pAppManager->GetClientExoApp()->GetClientAIMaster();
            pMaster->m_lList.RemoveObject(pCreature);
            CGameObject* pObject;
            if (g_pAppManager->GetClientExoApp()->GetGameObjectArray()->Delete(m_aoid268[i], &pObject) == GAME_OBJECT_VALID
                && pObject && pObject->AsSWCCreature() == pCreature)
                delete pCreature;
        }
        m_aoid268[i] = 0x7f000000;
    }
    m_n278 = 0;
    m_n279 = 1;
}

// FUNCTION: SWKOTOR 0x0060A0F0
CSWCArea::~CSWCArea()
{
    UnloadArea();
    if (m_pAmbientSound) {
        delete m_pAmbientSound;
    }
    m_pAmbientSound = 0;
    if (m_p260) {
        delete[] m_p260;
    }
    m_p260 = 0;
    if (m_p264) {
        delete m_p264;
    }
    if (m_pServerArea) {
        m_pServerArea->DetachFromClientArea();
        m_pServerArea = 0;
    }
}

// The real constructor emits both compiler-generated destructor bodies.
// FUNCTION: SWKOTOR 0x00606480 SYMBOL: ??_ECSWCArea@@WBAA@AEPAXI@Z
// FUNCTION: SWKOTOR 0x0060A2E0 SYMBOL: ??_GCSWCArea@@UAEPAXI@Z
