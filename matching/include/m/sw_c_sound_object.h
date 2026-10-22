// Worker M matching headers: client sound object CSWCSoundObject (vtable
// 0x007522A8, 0x16C bytes, base CSWCObject vtable 0x007516C8),
// 0x0066AD00-0x0066BF02, plus CExoArrayList<CResRef>::DelIndex 0x00648100.
#ifndef M_SW_C_SOUND_OBJECT_H
#define M_SW_C_SOUND_OBJECT_H

#include <stdlib.h>

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef operator=(const CResRef& r);            // 0x00406120
    CResRef operator=(const char* s);               // 0x00406290
    int operator==(const char* s) const;            // 0x004060D0
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size) {
            element = new T[array_size];
        }
    }
    __forceinline ~CExoArrayList() { delete[] element; element = 0; }
    void Allocate(int n);                           // <CResRef>: 0x00506E20
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void DelIndex(int i);                           // <CResRef>: 0x00648100
};

class CExoRand {
public:
    unsigned long Rand();                           // 0x005EA490
};

class CExoBase {
public:
    char pad_0x0[0x10];
    CExoRand* m_pRand;                              // 0x10
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    void SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1,
                            unsigned long nDay2, unsigned long nTime2,
                            unsigned long* pDay, unsigned long* pTime);   // 0x004AE460
};

class CSWCModule {
public:
    char pad_0x0[0xb0];
    unsigned long m_nHour;                          // 0xb0
    unsigned char m_nTimeOfDay;                     // 0xb4
};

class CSWCObject;

class CClientAIMaster {
public:
    int AddObject(CSWCObject* pObject);             // 0x00604A20
};

class CGameObject;

class CGameObjectArray {
public:
    unsigned char AddExternalObject(unsigned long* pId, CGameObject* pObject, int bArg);   // 0x004D7FF0
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CSWCModule* GetModule();                        // 0x005ED530
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

extern const unsigned char OBJECT_TYPE_SOUND;       // 0x0074663E
extern const unsigned char GAME_OBJECT_ADD_OK;      // 0x0074666C

class CSWCMessage {
public:
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned short ReadWORD(int nBits);             // 0x004D6750
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    float ReadFLOAT(float fScale, int nBits);       // 0x004D67B0
    CResRef ReadCResRef(int nBits);                 // 0x004D6180
    unsigned long ReadOBJECTIDClient();             // 0x00692630
};

class CExoSoundSource {
public:
    CExoSoundSource();                              // 0x005D5870
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char n);         // 0x005D5900
    void Set3D(int b);                              // 0x005D5910
    int IsHardwarePlayingSoundPleaseDontUseThis();  // 0x005D5920
    unsigned long Play();                           // 0x005D5930
    void SetVolume(unsigned char n);                // 0x005D5950
    void SetPitchVariation(float f);                // 0x005D5980
    void SetLooping(int b);                         // 0x005D59D0
    void SetPosition(const Vector& v, int b);       // 0x005D59E0
    void Stop();                                    // 0x005D5A20
    void SetFixedVariance(float f);                 // 0x005D5A30
    int GetLooping();                               // 0x005D6190
    void SetDistances(float fMin, float fMax);      // 0x005D61A0
    void SetResRef(const CResRef& cRes, int b);     // 0x005D61C0

    void* m_pInternal;                              // 0x04
};

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200
    int IsPlaying();                                // 0x005D5AF0
    unsigned long Play(const CResRef& cRes, int a, int b, int c, int d);   // 0x005D5B10
    void SetLooping(int b);                         // 0x005D5B40
    void SetVolume(unsigned char n);                // 0x005D5B50
    void Stop();                                    // 0x005D5B80
    void FadeAndStop(unsigned long nFade);          // 0x005D5B90
    void SetPriorityGroup(unsigned char n);         // 0x005D5BA0
    int GetLooping();                               // 0x005D6280
    void Set3D(int b);                              // 0x005D6290
    void SetDistances(float fMin, float fMax);      // 0x005D62A0
    void SetPosition(const Vector& v);              // 0x005D62C0

    void* m_pInternal;                              // 0x00
};

// CExoArrayList<unsigned long> with its out-of-line Add.
class CExoArrayListObjectId {
public:
    unsigned long* element;
    int num;
    int array_size;
    void Add(unsigned long t);                      // 0x006449D0
};

class CSWCArea {
public:
    char pad_0x0[0x1d0];
    CExoArrayListObjectId m_lstSoundObjects;         // 0x1d0
};

class CSWCSoundObject;

class CGameObject {
public:
    virtual ~CGameObject();                         // 0x00
    virtual void SetId(unsigned long id);           // 0x04
    virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c

    unsigned long m_idSelf;                         // 0x04
};

class CSWCObject : public CGameObject {
public:
    CSWCObject(unsigned char nObjectType);          // 0x0063E530
    virtual ~CSWCObject();                          // 0x0063E800
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68();
    virtual CSWCSoundObject* AsSWCSoundObject();    // 0x6c
    virtual void v70();
    virtual void AIUpdate() = 0;                    // 0x74
    virtual int Unknown78(int a, int b, int c) = 0; // 0x78
    virtual void Unknown7C(int a) = 0;              // 0x7c
    virtual void Unknown80() = 0;                   // 0x80
    virtual void AddToArea(CSWCArea* pArea);        // 0x84
    virtual void v88();
    virtual void SetPosition(const Vector& v);      // 0x8c (base 0x0063F340)

    char pad_0x8[0xfc - 0x8];
};

class CSWCSoundObject : public CSWCObject {
public:
    CSWCSoundObject();                              // 0x0066B200
    ~CSWCSoundObject();                             // 0x0066B370

    CSWCSoundObject* AsSWCSoundObject();            // 0x00641DB0 (shared)
    void AIUpdate();                                // 0x0066BBD0
    int Unknown78(int a, int b, int c);             // 0x0066B360
    void Unknown7C(int a);
    void Unknown80();
    void SetPosition(const Vector& v);              // 0x0066B310

    void SetVolume(int nVolume);                    // 0x0066AD00
    void SetPitchVariation(float f);                // 0x0066AD40
    void SetFixedVariance(float f);                 // 0x0066AD80
    void SetPositionElevated(const Vector& v);      // 0x0066ADC0
    void SetActive(int bActive);                    // 0x0066AE30
    void UpdateTimeOfDay();                         // 0x0066AE90
    unsigned long GetTimeDiff();                    // 0x0066AF30
    void RandomizeVolume();                         // 0x0066AFA0
    void RandomizeInterval();                       // 0x0066B000
    void Play();                                    // 0x0066B050
    void Stop();                                    // 0x0066B130
    void FadeAndStop(unsigned long nFade);          // 0x0066B190
    void LoadFromMessage(CSWCMessage* pMsg);        // 0x0066B420
    int AddToAreaAndAI(CSWCArea* pArea, CGameObjectArray* pArray);  // 0x0066B8A0
    void ShuffleSounds();                           // 0x0066B980
    CResRef GetNextSound();                         // 0x0066BAD0

    // inline copies
    float GetPitchVariation() { return m_fPitchVariation; }
    float GetFixedVariance() { return m_fFixedVariance; }
    __forceinline void StopIfPlaying()
    {
        if (m_bStreaming) {
            if (m_pStream->IsPlaying()) {
                m_pStream->Stop();
            }
        }
        else {
            if (m_pSource->IsHardwarePlayingSoundPleaseDontUseThis()) {
                m_pSource->Stop();
            }
        }
    }
    __forceinline int GetLoopingInline()
    {
        if (!m_bStreaming && m_pSource) {
            return m_pSource->GetLooping();
        }
        else if (m_pStream) {
            return m_pStream->GetLooping();
        }
        return 0;
    }
    __forceinline void SetActiveInline(int bActive)
    {
        if (m_bActive != bActive) {
            m_nIntervalLeft = 0;
            m_bActive = bActive;
            if (bActive) {
                if (GetLoopingInline()) {
                    m_bLoopPending = 1;
                }
            }
        }
    }
    __forceinline void SetLoopingInline(int b)
    {
        if (!m_bStreaming && m_pSource) {
            m_pSource->SetLooping(b);
        }
        else if (m_pStream) {
            m_pStream->SetLooping(b);
        }
    }
    __forceinline void Set3DInline(int b)
    {
        if (!m_bStreaming && m_pSource) {
            m_pSource->Set3D(b);
        }
        else if (m_pStream) {
            m_pStream->Set3D(b);
        }
    }
    __forceinline void SetDistancesInline(float fMin, float fMax)
    {
        if (!m_bStreaming && m_pSource) {
            m_pSource->SetDistances(fMin, fMax);
        }
        else if (m_pStream) {
            m_pStream->SetDistances(fMin, fMax);
        }
    }
    __forceinline void RandomizeIntervalInline()
    {
        if (m_nInterval) {
            int nInterval = m_nInterval;
            unsigned long nVar = m_nIntervalVariation;
            if (nVar > 0) {
                nInterval += g_pExoBase->m_pRand->Rand() % (nVar * 2) - nVar;
                nInterval = nInterval < 0 ? 0 : nInterval;
            }
            m_nIntervalLeft = nInterval;
        }
    }

    CExoSoundSource* m_pSource;                     // 0xfc
    CExoStreamingSoundSource* m_pStream;            // 0x100
    int m_bActive;                                  // 0x104
    int m_nVolume;                                  // 0x108
    int m_nVolumeVariation;                         // 0x10c
    int m_nTimes;                                   // 0x110
    int m_bInTime;                                  // 0x114
    float m_fElevation;                             // 0x118
    float m_fPitchVariation;                        // 0x11c
    unsigned long m_nHours;                         // 0x120
    float m_fFixedVariance;                         // 0x124
    int m_bLoopPending;                             // 0x128
    unsigned char m_nPriority;                      // 0x12c
    int m_bRandomPosition;                          // 0x130
    float m_fRandomRangeX;                          // 0x134
    float m_fRandomRangeY;                          // 0x138
    CExoArrayList<CResRef>* m_pSounds;              // 0x13c
    int m_nCurrentSound;                            // 0x140
    unsigned long m_nInterval;                      // 0x144
    unsigned long m_nIntervalVariation;             // 0x148
    unsigned long m_nIntervalLeft;                  // 0x14c
    unsigned long m_nPlayTimeLeft;                  // 0x150
    int m_bContinuous;                              // 0x154
    int m_bRandom;                                  // 0x158
    int m_bFirstSound;                              // 0x15c
    unsigned long m_nStartDay;                      // 0x160
    unsigned long m_nStartTime;                     // 0x164
    int m_bStreaming;                               // 0x168
};

#endif
