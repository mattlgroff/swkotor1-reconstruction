// Client party (swparty.o): CSWParty and its embedded follow-path ring
// CSWPartyFollowDataArray. The 0x1290-byte layout comes from the original
// constructor at 0x00636170 and the member accessors at 0x00634650..0x00637890.
#ifndef MATCHING_N_SW_PARTY_H
#define MATCHING_N_SW_PARTY_H

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector& operator+=(const Vector& v);            // 0x004A9740
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion(Vector vAxis, float fAngle);         // 0x004AC960
};

Vector operator*(const Quaternion& q, const Vector& v); // 0x004A9BB0
Quaternion RotationArc(Vector vFrom, Vector vTo);       // 0x004AB630, descriptive
Vector Normalize(const Vector& v);                      // 0x004AB180
float Magnitude(const Vector& v);                       // 0x004AB100, descriptive
float DegreesToRadians(float fDegrees);                 // 0x004A96D0, descriptive

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    void Format(const char* fmt, ...);              // 0x005E5680
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue); // 0x00413430
};

class CTwoDimArrays {
public:
    char pad_0x0[0xc4];
    C2DA* m_pPartyAI;                                 // +0xC4, descriptive
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                       // +0xB8
};

extern CSWRules* g_pRules;                            // 0x007A3A28

class CSWPartyTable {
public:
    char pad_0x0[0xe8];
    int m_nFollowState;                               // +0xE8
};

extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class CSWCCreature {
public:
    char pad_0x0[0x24];
    Vector m_vPosition;                               // +0x24
};

class CSWSArea;

class CPathfindInformation {
public:
    char pad_0x0[0x34];
    unsigned long m_oidBlocker;                       // +0x34, descriptive
    char pad_0x38[0x210 - 0x38];
    int m_n210;                                       // +0x210
};

class CSWSCreature {
public:
    char pad_0x0[0x4];
    unsigned long m_oidArea;                          // +0x04
    char pad_0x8[0x90 - 0x8];
    Vector m_vPosition;                               // +0x90
    char pad_0x9c[0x340 - 0x9c];
    CPathfindInformation* m_pPathfindInformation;     // +0x340

    CSWSArea* GetArea();                              // 0x004CB120
    void SetPosition(const Vector& vPosition, int bUpdate, int bArg2, int bArg3); // 0x004CD1A0
};

class CClientExoApp {
public:
    CSWCCreature* GetCreatureByGameObjectID(unsigned long nObject); // 0x005ED5B0
    unsigned long GetPlayerCreatureId();                            // 0x005ED550
};

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long nObject); // 0x004AE770
    unsigned long ClientToServerObjectID(unsigned long nObject);    // 0x004AEA30
    CSWPartyTable* GetPartyTable();                                  // 0x004AEE70
    int TestSafeLocation(unsigned long oidArea, const Vector& vPosition); // 0x004AEB50
    int ComputeSafeLocation(const Vector& vPosition, float fRadius, unsigned long oidArea,
                            int bArg, Vector* pResult);              // 0x004AEB60
};

class CAppManager {
public:
    void* m_pUnknown00;                               // +0x00
    CClientExoApp* m_pClientExoApp;                   // +0x04
    CServerExoApp* m_pServerExoApp;                   // +0x08

    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                    // 0x007A39FC

// One 0x88-byte party slot, at party +0x24.
class CSWPartyMember {
public:
    unsigned long m_oid;                              // +0x00
    int m_nFollowState;                               // +0x04
    char pad_0x8[0x18 - 0x8];
    Vector m_vPathPoint;                              // +0x18
    int m_nPathIndex;                                 // +0x24
    Vector m_vFollowPoint;                            // +0x28
    int m_nFollowPointIndex;                          // +0x34
    Vector m_vFormationOffset;                        // +0x38
    char pad_0x44[0x4c - 0x44];
    Vector m_vDirection;                              // +0x4C
    char pad_0x58[0x88 - 0x58];
};

class CSWParty;

// One 0x28-byte follow-path entry.
class CSWPartyFollowData {
public:
    Vector m_vPosition;                               // +0x00
    float m_fFacing;                                  // +0x0C
    Vector m_v10;                                     // +0x10
    Vector m_v1C;                                     // +0x1C

    CSWPartyFollowData()
        : m_vPosition(0.0f, 0.0f, 0.0f), m_v10(0.0f, 0.0f, 0.0f), m_v1C(0.0f, 0.0f, 0.0f)
    {
        m_fFacing = 0.0f;
    }
};

class CSWPartyFollowDataArray {
public:
    CSWPartyFollowData m_aData[100];                  // +0x000
    CSWParty* m_pParty;                               // +0xFA0
    int m_nHead;                                      // +0xFA4
    int m_nCount;                                     // +0xFA8

    CSWPartyFollowDataArray();
    void Reset(const Vector& vPosition, float fFacing);
    CSWPartyFollowData* GetPathData(int nIndex);
    int FindClosestPartyMemeber_Current();
    int FindFurthestPartyMemeber_Current();
    float PointOnLineDistanceFromLeader(int nMember, int bSquared);
    int GetPointBackFromStart(float fDistance, Vector* pPoint, int* pIndex, int nMember);
};

class CSWParty {
public:
    int m_nCount;                                     // +0x000
    char pad_0x4[0x18 - 0x4];
    int m_bChanged;                                   // +0x018
    char pad_0x1c[0x24 - 0x1c];
    CSWPartyMember m_aMembers[3];                     // +0x024
    int m_nFormation;                                 // +0x1BC
    float m_fFollowDistance;                          // +0x1C0
    unsigned char m_bInverted;                        // +0x1C4
    char pad_0x1c5[3];
    Vector m_aFormations[3][3];                       // +0x1C8
    float m_aFormationDistances[3];                   // +0x234
    Vector m_aCustomFormations[3][3];                 // +0x240
    unsigned char m_abCustomFormations[3];            // +0x2AC
    char pad_0x2af[0x2dc - 0x2af];
    CSWPartyFollowDataArray m_cFollow;                // +0x2DC
    char pad_0x1288[0x1290 - 0x1288];

    // Inline here; the exact out-of-line copy is 0x00635460 (sw_party_lookup.cpp).
    CSWCCreature* GetPlayerCharacter()
    {
        if (m_nCount > 0)
            return g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(m_aMembers[0].m_oid);
        return 0;
    }
    // Inline here; the exact out-of-line copy is 0x006346C0 (sw_party_lookup.cpp).
    CSWCCreature* GetCharacter(int nIndex)
    {
        if (nIndex >= 0 && nIndex < m_nCount) {
            unsigned long nObject = m_aMembers[nIndex].m_oid;
            CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
            return pClient->GetCreatureByGameObjectID(nObject);
        }
        return 0;
    }
    int GetIsCharacterInParty(unsigned long nObject);
    int IsPartyMember(unsigned long nObject);       // 0x006349C0

    CSWParty();
    void Reset(const Vector& vPosition, float fFacing, int bFormation); // 0x00637630
    void ResetAllFollowData(const Vector& vPosition, float fFacing, int bFormation);
    int RemoveCharacter(int nIndex);
    int RemoveCharacter(unsigned long nObject);
    void InvertFormation();
    void SetFormation(int nFormation);
    Vector GetWorldFormationPosition(const Vector& vPosition, const Vector& vOrientation, int nIndex);
    void ClearParty();
    float GetFollowRange();
    int IsPartyTogether();
    int TestAndResetPath();
    void RecalculateFollowPoint();
};

#endif
