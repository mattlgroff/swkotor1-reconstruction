// CSWSSoundObject and CSWSAmbientSound
// (b005c_f2_sound_luo / b005c_sound_object_clears, 0x005C8660-0x005CA02A).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef(const char *szName);
    CResRef operator=(const CResRef &cResRef);
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

int operator==(const Vector &a, const Vector &b);

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    T &operator[](int i) { return element[i]; }
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
};

class CExoArrayListULong {
};

struct CExoLinkedListNode;

class CExoLinkedListInternal {
public:
    void *GetAtPos(CExoLinkedListNode *pos);
    void *GetNext(CExoLinkedListNode *&pos);
    CExoLinkedListNode *pHead;
    CExoLinkedListNode *pTail;
    unsigned long nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListNode *GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T *GetAtPos(CExoLinkedListNode *pos) { return (T *)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T *GetNext(CExoLinkedListNode *&pos) { return (T *)m_pcExoLinkedListInternal->GetNext(pos); }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

struct CResStruct {
    unsigned long m_nIndex;
};

struct CResList {
    unsigned long m_nType;
    unsigned long m_nIndex;
    unsigned long m_nCount;
    char pad[0x8];
};

class CResGFF {
public:
    CResGFF(unsigned short nType, char *szSignature, const CResRef &cResRef);
    virtual ~CResGFF();
    void GetTopLevelStruct(CResStruct *pStruct);
    int GetList(CResList *pList, CResStruct *pStruct, char *szLabel);
    unsigned long GetListCount(CResList *pList);
    int GetListElement(CResStruct *pOut, CResList *pList, unsigned long nIndex);
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess, int nDefault);
    int ReadFieldINT(CResStruct *pStruct, char *szLabel, int &bSuccess, int nDefault);
    unsigned long ReadFieldDWORD(CResStruct *pStruct, char *szLabel, int &bSuccess, unsigned long nDefault);
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault);
    CResRef ReadFieldCResRef(CResStruct *pStruct, char *szLabel, int &bSuccess, const CResRef &cDefault);
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pOut, CResList *pList, unsigned long nType);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    char pad_0x4[0x90];
    int m_bLoaded;                   // +0x94
    char pad_0x98[0x8];
};

class CNWMessage {
public:
    void WriteBOOL(int bValue);
    void WriteBYTE(unsigned char nValue, int nBits);
    void WriteWORD(unsigned short nValue, int nBits);
    void WriteDWORD(unsigned long nValue, int nBits);
    void WriteFLOAT(float fValue, float fScale, int nBits);
    void WriteCResRef(CResRef cResRef, int nSize);
};

class CSWSPlayer;

class CSWSMessage : public CNWMessage {
public:
    void WriteOBJECTIDServer(unsigned long oid);
    int SendServerToPlayerSoundObject_Play(CSWSPlayer *pPlayer, unsigned long oid);
    int SendServerToPlayerSoundObject_Stop(CSWSPlayer *pPlayer, unsigned long oid);
    int SendServerToPlayerSoundObject_ChangePosition(CSWSPlayer *pPlayer, unsigned long oid, Vector v);
    int SendServerToPlayerAmbientMusicPlay(unsigned long nPlayer, int bPlay);
    int SendServerToPlayerAmbientMusicSetDelay(unsigned long nPlayer, int nDelay);
    int SendServerToPlayerAmbientMusicChangeTrack(unsigned long nPlayer, int bDay, int nTrack);
    int SendServerToPlayerAmbientBattleMusicPlay(unsigned long nPlayer, int bPlay);
    int SendServerToPlayerAmbientBattleMusicChange(unsigned long nPlayer, int nTrack);
    int SendServerToPlayerAmbientSoundLoopPlay(unsigned long nPlayer, int bPlay);
    int SendServerToPlayerAmbientSoundLoopChange(unsigned long nPlayer, int bDay, int nSound);
    int SendServerToPlayerAmbientSoundVolumeChange(unsigned long nPlayer, int bDay, int nVolume);
};

class CSWSCreature;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();     // +0x30
    unsigned long GetID() { return m_idSelf; }
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSPlayer {
public:
    CGameObject *GetGameObject();
    char pad_0x0[0x4];
    unsigned long m_nPlayerID;       // +0x4
};

class CResHelperArea {
public:
    virtual ~CResHelperArea();
    char pad_0x4[0x118];
};

class CSWSArea : public CResHelperArea, public CGameObject {
public:
    CGameObject &AsGameObject() { return *this; }
    void RemoveSoundObject(unsigned long oid);
    void AddSoundObject(unsigned long oid, int bFlag);
};

class CSWSModule {
public:
    void AddObjectToLookupTable(CExoString *pTag, unsigned long oid);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CSWSMessage *GetSWSMessage();
    CExoLinkedList<CSWSPlayer> *GetPlayerList();
};

class CClientSoundObject {
public:
    void SetVolume(int nVolume);
    void SetPitchVariation(float f);
    void SetFixedVariance(float f);
    void FadeAndStop(float fTime);
};

class CClientExoApp {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);
    CClientSoundObject *GetSoundObjectByGameObjectID(unsigned long oid);
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp *m_pClientExoApp;  // +0x4
    CServerExoApp *m_pServerExoApp;  // +0x8
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern const unsigned char g_nObjectTypeSound;     // 0x0074663E

class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nType, unsigned long oid, int bFlag);
    virtual ~CSWSObject();
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);
    void SetTag(const CExoString &sTag);
    Vector GetPosition() { return m_vPosition; }

    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0x18c];
};

class CSWSSoundObject : public CSWSObject {
public:
    CSWSSoundObject(unsigned long oid);
    virtual ~CSWSSoundObject();
    void SaveSound(CResGFF *pGFF, CResStruct *pStruct);
    void AddToArea(CSWSArea *pArea, int bFlag);
    void PackIntoMessage(CSWSMessage *pMsg);
    void Play();
    void Stop();
    void FadeAndStop(float fTime);
    void ChangePitchVariation(float f);
    void ChangeFixedVariance(float f);
    void ChangeVolume(int nVolume);
    void ChangePosition(Vector v);
    int LoadSound(CResGFF *pGFF, CResStruct *pStruct);
    int LoadFromTemplate(const CResRef &cResRef);
    float GetPitchVariation() { return m_fPitchVariation; }
    float GetFixedVariance() { return m_fFixedVariance; }
    float GetMinDistance() { return m_fMinDistance; }
    float GetMaxDistance() { return m_fMaxDistance; }
    float GetRandomRangeX() { return m_fRandomRangeX; }
    float GetRandomRangeY() { return m_fRandomRangeY; }

    int m_bActive;                   // +0x228
    int m_bPositional;               // +0x22c
    int m_bLooping;                  // +0x230
    int m_nVolume;                   // +0x234
    int m_nVolumeVrtn;               // +0x238
    int m_nTimes;                    // +0x23c
    float m_fPitchVariation;         // +0x240
    unsigned long m_nHours;          // +0x244
    float m_fFixedVariance;          // +0x248
    unsigned char m_nGeneratedType;  // +0x24c
    int m_bRandomPosition;           // +0x250
    float m_fRandomRangeX;           // +0x254
    float m_fRandomRangeY;           // +0x258
    CExoArrayList<CResRef> m_aSounds; // +0x25c
    unsigned long m_nInterval;       // +0x268
    unsigned long m_nIntervalVrtn;   // +0x26c
    float m_fMinDistance;            // +0x270
    float m_fMaxDistance;            // +0x274
    int m_bContinuous;               // +0x278
    int m_bRandom;                   // +0x27c
};

// FUNCTION: SWKOTOR 0x005C8660
CSWSSoundObject::~CSWSSoundObject()
{
}

// FUNCTION: SWKOTOR 0x005C86D0
void CSWSSoundObject::SaveSound(CResGFF *pGFF, CResStruct *pStruct)
{
    Vector vPos = GetPosition();
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bActive, "Active");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bPositional, "Positional");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bLooping, "Looping");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_nVolume, "Volume");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_nVolumeVrtn, "VolumeVrtn");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_nTimes, "Times");
    pGFF->WriteFieldFLOAT(pStruct, GetPitchVariation(), "PitchVariation");
    pGFF->WriteFieldDWORD(pStruct, m_nHours, "Hours");
    pGFF->WriteFieldDWORD(pStruct, m_nGeneratedType, "GeneratedType");
    pGFF->WriteFieldDWORD(pStruct, m_nInterval, "Interval");
    pGFF->WriteFieldDWORD(pStruct, m_nIntervalVrtn, "IntervalVrtn");
    pGFF->WriteFieldFLOAT(pStruct, GetMinDistance(), "MinDistance");
    pGFF->WriteFieldFLOAT(pStruct, GetMaxDistance(), "MaxDistance");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bContinuous, "Continuous");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bRandom, "Random");
    pGFF->WriteFieldFLOAT(pStruct, GetFixedVariance(), "FixedVariance");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bRandomPosition, "RandomPosition");
    pGFF->WriteFieldFLOAT(pStruct, GetRandomRangeX(), "RandomRangeX");
    pGFF->WriteFieldFLOAT(pStruct, GetRandomRangeY(), "RandomRangeY");
    pGFF->WriteFieldFLOAT(pStruct, vPos.x, "XPosition");
    pGFF->WriteFieldFLOAT(pStruct, vPos.y, "YPosition");
    pGFF->WriteFieldFLOAT(pStruct, vPos.z, "ZPosition");
    CResList cList;
    pGFF->AddList(&cList, pStruct, "Sounds");
    for (int i = 0; i < m_aSounds.num; i++) {
        CResStruct cElement;
        pGFF->AddListElement(&cElement, &cList, 0);
        pGFF->WriteFieldCResRef(&cElement, m_aSounds.element[i], "Sound");
    }
}

// FUNCTION: SWKOTOR 0x005C8950
void CSWSSoundObject::AddToArea(CSWSArea *pArea, int bFlag)
{
    if (GetArea())
        GetArea()->RemoveSoundObject(m_idSelf);
    SetArea(0);
    if (pArea) {
        SetArea(pArea);
        pArea->AddSoundObject(m_idSelf, bFlag);
        g_pAppManager->GetServerExoApp()->GetModule()->AddObjectToLookupTable(&m_sTag, GetID());
    }
}

// FUNCTION: SWKOTOR 0x005C89C0
void CSWSSoundObject::PackIntoMessage(CSWSMessage *pMsg)
{
    Vector vPos = GetPosition();
    pMsg->WriteOBJECTIDServer(m_idSelf);
    pMsg->WriteBOOL(m_bActive);
    pMsg->WriteBOOL(m_bPositional);
    pMsg->WriteBOOL(m_bLooping);
    pMsg->WriteBYTE((unsigned char)m_nVolume, 8);
    pMsg->WriteBYTE((unsigned char)m_nVolumeVrtn, 8);
    pMsg->WriteBYTE((unsigned char)m_nTimes, 8);
    pMsg->WriteFLOAT(GetPitchVariation(), 1.0f, 32);
    pMsg->WriteFLOAT(GetFixedVariance(), 1.0f, 32);
    pMsg->WriteDWORD(m_nHours, 32);
    pMsg->WriteBYTE(m_nGeneratedType, 8);
    pMsg->WriteDWORD(m_nInterval, 32);
    pMsg->WriteDWORD(m_nIntervalVrtn, 32);
    pMsg->WriteFLOAT(GetMinDistance(), 1.0f, 32);
    pMsg->WriteFLOAT(GetMaxDistance(), 1.0f, 32);
    pMsg->WriteBOOL(m_bContinuous);
    pMsg->WriteBOOL(m_bRandom);
    pMsg->WriteBOOL(m_bRandomPosition);
    pMsg->WriteFLOAT(GetRandomRangeX(), 1.0f, 32);
    pMsg->WriteFLOAT(GetRandomRangeY(), 1.0f, 32);
    pMsg->WriteFLOAT(vPos.x, 1.0f, 32);
    pMsg->WriteFLOAT(vPos.y, 1.0f, 32);
    pMsg->WriteFLOAT(vPos.z, 1.0f, 32);
    unsigned short nSounds = (unsigned short)m_aSounds.num;
    pMsg->WriteWORD(nSounds, 16);
    for (unsigned short i = 0; i < nSounds; i++)
        pMsg->WriteCResRef(m_aSounds.element[i], 16);
}

// FUNCTION: SWKOTOR 0x005C8BD0
void CSWSSoundObject::Play()
{
    m_bActive = 1;
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoLinkedList<CSWSPlayer> *pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pMsg && pList) {
        CExoLinkedListNode *pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer *pPlayer = pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pPlayer->GetGameObject()->AsSWSCreature())
                pMsg->SendServerToPlayerSoundObject_Play(pPlayer, m_idSelf);
            pList->GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x005C8C70
void CSWSSoundObject::Stop()
{
    m_bActive = 0;
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoLinkedList<CSWSPlayer> *pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pMsg && pList) {
        CExoLinkedListNode *pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer *pPlayer = pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pPlayer->GetGameObject()->AsSWSCreature())
                pMsg->SendServerToPlayerSoundObject_Stop(pPlayer, m_idSelf);
            pList->GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x005C8D10
void CSWSSoundObject::FadeAndStop(float fTime)
{
    m_bActive = 0;
    unsigned long oid = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(GetID());
    CClientSoundObject *pSound = g_pAppManager->GetClientExoApp()->GetSoundObjectByGameObjectID(oid);
    if (pSound)
        pSound->FadeAndStop(fTime);
}

// FUNCTION: SWKOTOR 0x005C8D50
void CSWSSoundObject::ChangePitchVariation(float f)
{
    if (f != m_fPitchVariation) {
        m_fPitchVariation = f;
        unsigned long oid = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(GetID());
        CClientSoundObject *pSound = g_pAppManager->GetClientExoApp()->GetSoundObjectByGameObjectID(oid);
        if (pSound)
            pSound->SetPitchVariation(f);
    }
}

// FUNCTION: SWKOTOR 0x005C8DA0
void CSWSSoundObject::ChangeFixedVariance(float f)
{
    if (f != m_fFixedVariance) {
        m_fFixedVariance = f;
        unsigned long oid = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(GetID());
        CClientSoundObject *pSound = g_pAppManager->GetClientExoApp()->GetSoundObjectByGameObjectID(oid);
        if (pSound)
            pSound->SetFixedVariance(f);
    }
}

// FUNCTION: SWKOTOR 0x005C8DF0
void CSWSSoundObject::ChangeVolume(int nVolume)
{
    if (nVolume > 0x7f)
        nVolume = 0x7f;
    nVolume = nVolume < 0 ? 0 : nVolume;
    if (nVolume != m_nVolume) {
        m_nVolume = nVolume;
        unsigned long oid = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(GetID());
        CClientSoundObject *pSound = g_pAppManager->GetClientExoApp()->GetSoundObjectByGameObjectID(oid);
        if (pSound)
            pSound->SetVolume(nVolume);
    }
}

// FUNCTION: SWKOTOR 0x005C8E50
void CSWSSoundObject::ChangePosition(Vector v)
{
    if (v == m_vPosition)
        return;
    SetPosition(v, 1, 1, 0);
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoLinkedList<CSWSPlayer> *pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pMsg && pList) {
        CExoLinkedListNode *pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer *pPlayer = pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pPlayer->GetGameObject()->AsSWSCreature())
                pMsg->SendServerToPlayerSoundObject_ChangePosition(pPlayer, m_idSelf, v);
            pList->GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x005C9020 SYMBOL: ??_GCSWSSoundObject@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005C8F30
CSWSSoundObject::CSWSSoundObject(unsigned long oid)
    : CSWSObject(g_nObjectTypeSound, oid, 0)
{
    m_bActive = 1;
    m_bPositional = 1;
    m_bLooping = 0;
    m_nVolumeVrtn = 0;
    m_fPitchVariation = 0.0f;
    m_nHours = 0;
    m_nGeneratedType = 0;
    m_nInterval = 0;
    m_nIntervalVrtn = 0;
    m_bContinuous = 0;
    m_bRandom = 0;
    m_bRandomPosition = 0;
    m_fRandomRangeX = 0.0f;
    m_fRandomRangeY = 0.0f;
    m_nVolume = 0x7f;
    m_nTimes = 3;
    m_fFixedVariance = 1.0f;
    m_fMinDistance = 10.0f;
    m_fMaxDistance = 20.0f;
}

// NONMATCH (42): result-store scheduling only; the original defers each fstp /
// DWORD result store past the next call's label push. Frame, loop and Add match.
// FUNCTION: SWKOTOR 0x005C9040
int CSWSSoundObject::LoadSound(CResGFF *pGFF, CResStruct *pStruct)
{
    int bSuccess;
    float fValue;
    Vector vPos(0.0f, 0.0f, 0.0f);
    CResRef cResRef;
    SetTag(pGFF->ReadFieldCExoString(pStruct, "Tag", bSuccess, m_sTag));
    {
        int nValue = m_bActive;
        m_bActive = pGFF->ReadFieldBYTE(pStruct, "Active", bSuccess, nValue);
    }
    {
        int nValue = m_bPositional;
        m_bPositional = pGFF->ReadFieldBYTE(pStruct, "Positional", bSuccess, nValue);
    }
    {
        int nValue = m_bLooping;
        m_bLooping = pGFF->ReadFieldBYTE(pStruct, "Looping", bSuccess, nValue);
    }
    {
        int nValue = m_nVolume;
        m_nVolume = pGFF->ReadFieldBYTE(pStruct, "Volume", bSuccess, nValue);
    }
    {
        int nValue = m_nVolumeVrtn;
        m_nVolumeVrtn = pGFF->ReadFieldBYTE(pStruct, "VolumeVrtn", bSuccess, nValue);
    }
    {
        int nValue = m_nTimes;
        m_nTimes = pGFF->ReadFieldBYTE(pStruct, "Times", bSuccess, nValue);
    }
    fValue = m_fPitchVariation;
    m_fPitchVariation = pGFF->ReadFieldFLOAT(pStruct, "PitchVariation", bSuccess, fValue);
    m_nHours = pGFF->ReadFieldDWORD(pStruct, "Hours", bSuccess, m_nHours);
    m_nGeneratedType = (unsigned char)pGFF->ReadFieldDWORD(pStruct, "GeneratedType", bSuccess, m_nGeneratedType);
    m_nInterval = pGFF->ReadFieldDWORD(pStruct, "Interval", bSuccess, m_nInterval);
    m_nIntervalVrtn = pGFF->ReadFieldDWORD(pStruct, "IntervalVrtn", bSuccess, m_nIntervalVrtn);
    fValue = m_fMinDistance;
    m_fMinDistance = pGFF->ReadFieldFLOAT(pStruct, "MinDistance", bSuccess, fValue);
    fValue = m_fMaxDistance;
    m_fMaxDistance = pGFF->ReadFieldFLOAT(pStruct, "MaxDistance", bSuccess, fValue);
    {
        int nValue = m_bContinuous;
        m_bContinuous = pGFF->ReadFieldBYTE(pStruct, "Continuous", bSuccess, nValue);
    }
    {
        int nValue = m_bRandom;
        m_bRandom = pGFF->ReadFieldBYTE(pStruct, "Random", bSuccess, nValue);
    }
    fValue = m_fFixedVariance;
    m_fFixedVariance = pGFF->ReadFieldFLOAT(pStruct, "FixedVariance", bSuccess, fValue);
    {
        int nValue = m_bRandomPosition;
        m_bRandomPosition = pGFF->ReadFieldBYTE(pStruct, "RandomPosition", bSuccess, nValue);
    }
    fValue = m_fRandomRangeX;
    m_fRandomRangeX = pGFF->ReadFieldFLOAT(pStruct, "RandomRangeX", bSuccess, fValue);
    fValue = m_fRandomRangeY;
    m_fRandomRangeY = pGFF->ReadFieldFLOAT(pStruct, "RandomRangeY", bSuccess, fValue);
    vPos.x = pGFF->ReadFieldFLOAT(pStruct, "XPosition", bSuccess, 0.0f);
    vPos.y = pGFF->ReadFieldFLOAT(pStruct, "YPosition", bSuccess, 0.0f);
    vPos.z = pGFF->ReadFieldFLOAT(pStruct, "ZPosition", bSuccess, 0.0f);
    SetPosition(vPos, 1, 1, 0);
    CResList cList;
    if (pGFF->GetList(&cList, pStruct, "Sounds")) {
        int nCount = pGFF->GetListCount(&cList);
        for (int i = 0; i < nCount; i++) {
            CResStruct cElement;
            if (pGFF->GetListElement(&cElement, &cList, i)) {
                cResRef = pGFF->ReadFieldCResRef(&cElement, "Sound", bSuccess, "");
                if (bSuccess)
                    m_aSounds.Add(cResRef);
            }
        }
    }
    return bSuccess;
}

// FUNCTION: SWKOTOR 0x005C94E0
int CSWSSoundObject::LoadFromTemplate(const CResRef &cResRef)
{
    CResGFF *pGFF = new CResGFF(0x7f3, "UTS ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        return 0;
    }
    CResStruct cStruct;
    pGFF->GetTopLevelStruct(&cStruct);
    int bResult = LoadSound(pGFF, &cStruct);
    delete pGFF;
    return bResult;
}

// ---------------------------------------------------------------------------

class CAmbientSoundState {
public:
    CAmbientSoundState()
    {
        m_bMusicPlaying = 1;
        m_nMusicDelay = 5000;
        m_nMusicDay = 2;
        m_nMusicNight = 3;
        m_bBattleMusicPlaying = 0;
        m_nMusicBattle = 1;
        m_bAmbientPlaying = 1;
        m_nAmbientSndDay = 1;
        m_nAmbientSndNight = 2;
        m_nAmbientSndDayVol = 0;
        m_nAmbientSndNitVol = 0;
    }

    int m_bMusicPlaying;             // +0x4
    int m_nMusicDelay;               // +0x8
    int m_nMusicDay;                 // +0xc
    int m_nMusicNight;               // +0x10
    int m_bBattleMusicPlaying;       // +0x14
    int m_nMusicBattle;              // +0x18
    int m_bAmbientPlaying;           // +0x1c
    int m_nAmbientSndDay;            // +0x20
    int m_nAmbientSndNight;          // +0x24
    unsigned char m_nAmbientSndDayVol; // +0x28
    unsigned char m_nAmbientSndNitVol; // +0x29
};

class CSWSAmbientSound : public CAmbientSoundState {
public:
    CSWSAmbientSound(unsigned long oidArea);
    virtual ~CSWSAmbientSound();
    int Load(CResGFF *pGFF, CResStruct *pStruct);
    int Save(CResGFF *pGFF, CResStruct *pStruct);
    void PackIntoMessage(CSWSMessage *pMsg);
    int GetPlayersInArea(CExoArrayList<unsigned long> *pList);
    void PlayMusic(int bPlay);
    void SetMusicDelay(int nDelay);
    void SetMusicDayTrack(int nTrack);
    void SetMusicNightTrack(int nTrack);
    void PlayBattleMusic(int bPlay);
    void SetBattleMusicTrack(int nTrack);
    void PlayAmbientSound(int bPlay);
    void SetAmbientDayTrack(int nTrack);
    void SetAmbientNightTrack(int nTrack);
    void SetAmbientDayVolume(int nVolume);
    void SetAmbientNightVolume(int nVolume);

    unsigned long m_oidArea;         // +0x2c
};

// NONMATCH (2): the original materialises `xor edx,edx` before the first
// store; store order and the late vptr store (inline base ctor) match.
// FUNCTION: SWKOTOR 0x005C95A0
CSWSAmbientSound::CSWSAmbientSound(unsigned long oidArea)
    : m_oidArea(oidArea)
{
}

// FUNCTION: SWKOTOR 0x005C95F0
int CSWSAmbientSound::Load(CResGFF *pGFF, CResStruct *pStruct)
{
    int bSuccess;
    m_nMusicDelay = pGFF->ReadFieldINT(pStruct, "MusicDelay", bSuccess, m_nMusicDelay);
    m_nMusicDay = pGFF->ReadFieldINT(pStruct, "MusicDay", bSuccess, m_nMusicDay);
    m_nMusicNight = pGFF->ReadFieldINT(pStruct, "MusicNight", bSuccess, m_nMusicNight);
    m_nMusicBattle = pGFF->ReadFieldINT(pStruct, "MusicBattle", bSuccess, m_nMusicBattle);
    m_nAmbientSndDay = pGFF->ReadFieldINT(pStruct, "AmbientSndDay", bSuccess, m_nAmbientSndDay);
    m_nAmbientSndNight = pGFF->ReadFieldINT(pStruct, "AmbientSndNight", bSuccess, m_nAmbientSndNight);
    m_nAmbientSndDayVol = (unsigned char)pGFF->ReadFieldINT(pStruct, "AmbientSndDayVol", bSuccess, m_nAmbientSndDayVol);
    m_nAmbientSndNitVol = (unsigned char)pGFF->ReadFieldINT(pStruct, "AmbientSndNitVol", bSuccess, m_nAmbientSndNitVol);
    return bSuccess;
}

// FUNCTION: SWKOTOR 0x005C96E0
int CSWSAmbientSound::Save(CResGFF *pGFF, CResStruct *pStruct)
{
    pGFF->WriteFieldINT(pStruct, m_nMusicDelay, "MusicDelay");
    pGFF->WriteFieldINT(pStruct, m_nMusicDay, "MusicDay");
    pGFF->WriteFieldINT(pStruct, m_nMusicNight, "MusicNight");
    pGFF->WriteFieldINT(pStruct, m_nMusicBattle, "MusicBattle");
    pGFF->WriteFieldINT(pStruct, m_nAmbientSndDay, "AmbientSndDay");
    pGFF->WriteFieldINT(pStruct, m_nAmbientSndNight, "AmbientSndNight");
    pGFF->WriteFieldINT(pStruct, m_nAmbientSndDayVol, "AmbientSndDayVol");
    return pGFF->WriteFieldINT(pStruct, m_nAmbientSndNitVol, "AmbientSndNitVol");
}

// FUNCTION: SWKOTOR 0x005C9780
void CSWSAmbientSound::PackIntoMessage(CSWSMessage *pMsg)
{
    pMsg->WriteBOOL(m_bMusicPlaying);
    pMsg->WriteBOOL(m_bBattleMusicPlaying);
    pMsg->WriteBOOL(m_bAmbientPlaying);
    pMsg->WriteDWORD(m_nMusicDelay, 32);
    pMsg->WriteDWORD(m_nMusicDay, 32);
    pMsg->WriteDWORD(m_nMusicNight, 32);
    pMsg->WriteDWORD(m_nMusicBattle, 32);
    pMsg->WriteDWORD(m_nAmbientSndDay, 32);
    pMsg->WriteDWORD(m_nAmbientSndNight, 32);
    pMsg->WriteBYTE(m_nAmbientSndDayVol, 8);
    pMsg->WriteBYTE(m_nAmbientSndNitVol, 8);
}

// FUNCTION: SWKOTOR 0x005C9820
int CSWSAmbientSound::GetPlayersInArea(CExoArrayList<unsigned long> *pList)
{
    pList->Clear();
    CExoLinkedList<CSWSPlayer> *pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pPlayers) {
        CExoLinkedListNode *pos = pPlayers->GetHeadPos();
        while (pos) {
            CSWSPlayer *pPlayer = pPlayers->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject()) {
                CSWSCreature *pCreature = pPlayer->GetGameObject()->AsSWSCreature();
                if (pCreature && ((CSWSObject *)pCreature)->GetArea() &&
                    ((CSWSObject *)pCreature)->GetArea()->GetID() == m_oidArea)
                    pList->Add(pPlayer->m_nPlayerID);
            }
            pPlayers->GetNext(pos);
        }
    }
    return pList->num;
}

// FUNCTION: SWKOTOR 0x005C9900
void CSWSAmbientSound::PlayMusic(int bPlay)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    m_bMusicPlaying = bPlay;
    int nPlayers = GetPlayersInArea(&aPlayers);
    for (int i = 0; i < nPlayers; i++)
        pMsg->SendServerToPlayerAmbientMusicPlay(aPlayers.element[i], bPlay);
}

// NONMATCH (11): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C99A0
void CSWSAmbientSound::SetMusicDelay(int nDelay)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (m_nMusicDelay != nDelay) {
        m_nMusicDelay = nDelay;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientMusicSetDelay(aPlayers[i], nDelay);
    }
}

// FUNCTION: SWKOTOR 0x005C9A40
void CSWSAmbientSound::SetMusicDayTrack(int nTrack)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    m_nMusicDay = nTrack;
    int nPlayers = GetPlayersInArea(&aPlayers);
    for (int i = 0; i < nPlayers; i++)
        pMsg->SendServerToPlayerAmbientMusicChangeTrack(aPlayers.element[i], 1, nTrack);
}

// NONMATCH (14): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9AE0
void CSWSAmbientSound::SetMusicNightTrack(int nTrack)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (m_nMusicNight != nTrack) {
        m_nMusicNight = nTrack;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientMusicChangeTrack(aPlayers.element[i], 0, nTrack);
    }
}

// NONMATCH (11): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9B90
void CSWSAmbientSound::PlayBattleMusic(int bPlay)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (m_bBattleMusicPlaying != bPlay) {
        m_bBattleMusicPlaying = bPlay;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientBattleMusicPlay(aPlayers.element[i], bPlay);
    }
}

// NONMATCH (11): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9C30
void CSWSAmbientSound::SetBattleMusicTrack(int nTrack)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (m_nMusicBattle != nTrack) {
        m_nMusicBattle = nTrack;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientBattleMusicChange(aPlayers.element[i], nTrack);
    }
}

// FUNCTION: SWKOTOR 0x005C9CD0
void CSWSAmbientSound::PlayAmbientSound(int bPlay)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    m_bAmbientPlaying = bPlay;
    int nPlayers = GetPlayersInArea(&aPlayers);
    for (int i = 0; i < nPlayers; i++)
        pMsg->SendServerToPlayerAmbientSoundLoopPlay(aPlayers.element[i], bPlay);
}

// NONMATCH (11): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9D70
void CSWSAmbientSound::SetAmbientDayTrack(int nTrack)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (m_nAmbientSndDay != nTrack) {
        m_nAmbientSndDay = nTrack;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientSoundLoopChange(aPlayers.element[i], 1, nTrack);
    }
}

// NONMATCH (14): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9E20
void CSWSAmbientSound::SetAmbientNightTrack(int nTrack)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (m_nAmbientSndNight != nTrack) {
        m_nAmbientSndNight = nTrack;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientSoundLoopChange(aPlayers.element[i], 0, nTrack);
    }
}

// NONMATCH (23): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9ED0
void CSWSAmbientSound::SetAmbientDayVolume(int nVolume)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (nVolume >= 0 && nVolume <= 100 && m_nAmbientSndDayVol != nVolume) {
        m_nAmbientSndDayVol = (unsigned char)nVolume;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientSoundVolumeChange(aPlayers.element[i], 1, nVolume);
    }
}

// NONMATCH (34): register choice only; the original keeps aPlayers.element in
// ebp (zero-initialised from ebp, loop-invariant, reused for the free) and spills
// pMsg to the frame; ours keeps pMsg in ebp. Tried early return, a local element
// pointer, declaration order, operator[], else-return and a reference parameter.
// FUNCTION: SWKOTOR 0x005C9F80
void CSWSAmbientSound::SetAmbientNightVolume(int nVolume)
{
    CSWSMessage *pMsg = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoArrayList<unsigned long> aPlayers;
    if (nVolume >= 0 && nVolume <= 100 && m_nAmbientSndNitVol != nVolume) {
        m_nAmbientSndNitVol = (unsigned char)nVolume;
        int nPlayers = GetPlayersInArea(&aPlayers);
        for (int i = 0; i < nPlayers; i++)
            pMsg->SendServerToPlayerAmbientSoundVolumeChange(aPlayers.element[i], 0, nVolume);
    }
}
