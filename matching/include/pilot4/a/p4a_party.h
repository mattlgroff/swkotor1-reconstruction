// Pilot 4 arm A: CSWParty::Reset (0x00637630) declarations.
#ifndef P4A_PARTY_H
#define P4A_PARTY_H

#include "shared/vector_ops.h"

class Quaternion {
public:
    float w, x, y, z;

    Quaternion(Vector vAxis, float fAngle);             // 0x004AC960
};

Vector operator*(const Quaternion& q, const Vector& v); // 0x004A9BB0

#include "shared/globals.h"

class CSWSArea {
public:
    int ComputeSafeLocation(const Vector& vPosition, float fRadius, int nArg,
                            int bArg, Vector* pResult); // 0x004BE860
};

class CSWSCreature {
public:
    char pad_0x0[0x8c];
    unsigned long m_oidArea;                            // 0x8c
    char pad_0x90[0x340 - 0x90];
    int m_n340;                                         // 0x340
};

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);         // 0x004AE780
    unsigned long ClientToServerObjectID(unsigned long oid);    // 0x004AEA30
};

class CAppManager {
public:
    void* m_pUnknown00;                                 // 0x00
    void* m_pClientExoApp;                              // 0x04
    CServerExoApp* m_pServerExoApp;                     // 0x08

    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};


class CSWFollowArray {
public:
    void Reset(Vector* pPosition, float fYaw);    // 0x00634B60
    void ProcessPoint(Vector* pPosition, float fYaw, float fDist); // 0x00636A30
};

class CSWParty {
public:
    void Reset(Vector* pPosition, float fYaw, int bFormation);  // 0x00637630

    char pad_0x0[0x24];
    unsigned long m_oidLeader;                          // 0x24
    char pad_0x28[0xd4 - 0x28];
    Vector m_vMember1;                                  // 0xd4
    int m_nMember1E0;                                   // 0xe0
    char pad_0xe4[0x15c - 0xe4];
    Vector m_vMember2;                                  // 0x15c
    int m_nMember2168;                                  // 0x168
    char pad_0x16c[0x2dc - 0x16c];
    CSWFollowArray m_cFollow;                           // 0x2dc
};

#endif
