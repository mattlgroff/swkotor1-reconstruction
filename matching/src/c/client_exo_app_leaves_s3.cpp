// clientexoappinternal.o leaves without a project name record (worker C s3):
// an unnamed record destructor, the 12-byte nearest-object list search and
// the camera-on-a-stick target head position.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

// Member at +0x14 whose destructor is the shared empty out-of-line body.
class CClientRecordPayload {
public:
    ~CClientRecordPayload();                        // 0x004015A0
    char pad_0x0[0x1c];
};

// Unnamed record: strings at +0x0C/+0x30 around a payload at +0x14.
class CClientStringRecord {
public:
    ~CClientStringRecord();

    char pad_0x0[0xc];
    CExoString m_sFirst;                            // 0x0c
    CClientRecordPayload m_cPayload;                // 0x14
    CExoString m_sSecond;                           // 0x30
};

// FUNCTION: SWKOTOR 0x005F1090
CClientStringRecord::~CClientStringRecord()
{
}

struct CNearestStruct {
    unsigned long m_oidObject;                      // 0x00
    float m_fDistance;                              // 0x04
    unsigned long m_nType;                          // 0x08
};

typedef int (*NearestCompareFn)(unsigned long nKey, CNearestStruct cEntry);

// 12-byte nearest-object list (CExoArrayList<CNearestStruct> layout).
class CNearestStructList {
public:
    CNearestStruct* element;                        // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    int Find(unsigned long nKey, NearestCompareFn pCompare);
};

// FUNCTION: SWKOTOR 0x005F4FB0
int CNearestStructList::Find(unsigned long nKey, NearestCompareFn pCompare)
{
    if (pCompare != 0) {
        for (int i = 0; i < num; i++) {
            if (pCompare(nKey, element[i])) {
                return i;
            }
        }
    }
    return -1;
}

struct Vector {
    float x, y, z;
};

class CSWCameraTarget {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual Vector GetPosition();                   // 0x64
};

class CSWCameraOnAStick {
public:
    Vector GetTargetHeadPosition();

    char pad_0x0[0x14];
    CSWCameraTarget* m_pTarget;                     // 0x14
    char pad_0x18[0x54];
    float m_fHeadHeight;                            // 0x6c
};

// FUNCTION: SWKOTOR 0x005F5180
Vector CSWCameraOnAStick::GetTargetHeadPosition()
{
    Vector v = m_pTarget->GetPosition();
    v.z += m_fHeadHeight;
    return v;
}
