// CSWCMessage player-to-server senders (0x00677410-0x00678030).
// Self-contained declarations (message stream API from worker A's
// nw_message.h, net layer from worker J).
#ifndef M_SW_C_MESSAGE_H
#define M_SW_C_MESSAGE_H

#include <stddef.h>

#define OBJECT_INVALID 0x7f000000

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

struct Vector {
    float x, y, z;
};

int operator==(const Vector& a, const Vector& b);   // 0x004AA980

class CNetLayer {
public:
    int SendMessageToPlayer(unsigned long nPlayer, unsigned char* pData,
                            unsigned long nSize, unsigned long nFlags);  // 0x005D4CB0
};

class CGuiInGame {
public:
    char pad_0x0[0xb4];
    void* m_pB4;                                    // 0xb4
};

class CClientExoApp {
public:
    // vtable 0x0074E6D8
    virtual void v00(int a, int b, int c);
    virtual void v04();
    virtual float v08();
    virtual void v0C();
    virtual CNetLayer* GetNetLayer();               // slot 4, 0x005ED6E0
    CGuiInGame* GetInGameGui();                     // 0x005ED690
};

class CServerExoApp {
public:
    void TogglePauseState(unsigned char nState);    // 0x004AE960
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CExoSound {
public:
    int PlayOneShotSound(const CResRef& cResRef, unsigned char nGroup,
                         unsigned long nDelay, unsigned char nVolume,
                         float fFixedVariance, float fPitchVariance);  // 0x005D5E00
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern unsigned long PLAYERID_SERVER;               // 0x0074C528 (0xFFFFFFFD)

class CNWMessage {
public:
    int GetWriteMessage(unsigned char** ppBuffer, unsigned long* pSize);  // 0x004D6440
    void WriteBYTE(unsigned char nValue, int nBits);        // 0x004D62C0
    void WriteWORD(unsigned short nValue, int nBits);       // 0x004D6300
    void WriteDWORD(unsigned long nValue, int nBits);       // 0x004D6340
    void WriteVOIDPtr(void* pData, unsigned long nSize);    // 0x004D63E0
    void WriteOBJECTID(unsigned long nValue);               // 0x004D68E0 (folded)
    void WriteBOOL(int bValue);                             // 0x004D68E0 (folded)
    void WriteFLOAT(float fValue, float fScale, int nBits); // 0x004D6920
    void WriteCExoString(const CExoString& sValue, int nBits);  // 0x004D6960
};

class CSWMessage : public CNWMessage {
public:
    void CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId,
                            unsigned long nFlags);  // 0x004D68A0
};

class CSWCMessage : public CSWMessage {
public:
    int SendPlayerToServerMessage(unsigned char nMajor, unsigned char nMinor,
                                  unsigned char* pData, unsigned long nSize);
    int SendPlayerToServer_03_02();
    int SendPlayerToServer_03_04(unsigned long a, const CExoString& b, const CExoString& c);
    int SendPlayerToServer_03_05(unsigned long a, const CExoString& b, const CExoString& c);
    int SendPlayerToServer_01_00();
    int SendPlayerToServer_19_02(unsigned long a, unsigned long b);
    int SendPlayerToServerInput_CastSpell(unsigned long nSpell, unsigned char a,
                                          unsigned char b, unsigned char c, int bFlag,
                                          const Vector* pPos, unsigned long oidTarget);
    int SendPlayerToServerGuiPause();
    int SendPlayerToServer_1C_0A();
    int SendPlayerToServerInput_GiveItem(unsigned long a, unsigned long b,
                                         unsigned long c, unsigned long d);
    int SendPlayerToServer_04_03();
    int SendPlayerToServer_70_0F();
    int SendPlayerToServer_0F_04();
    int SendPlayerToServer_0D_01(int a);
    int SendPlayerToServerInput_Attack(unsigned long oid, unsigned short b);
    int SendPlayerToServerInput_UseSkill(unsigned char a, unsigned char b,
                                         unsigned long oid, const Vector& v);
    int SendPlayerToServerInput_UseItem(unsigned long oidItem, unsigned char nSlot,
                                        unsigned char nSub, unsigned long oidArea,
                                        const Vector* pPos);
    int SendPlayerToServer_06_0B(unsigned long a);
    int SendPlayerToServer_06_03(unsigned long a, unsigned short b);
    int SendPlayerToServer_06_0C(unsigned long a, unsigned long b);
    int SendPlayerToServer_06_21();
    int SendPlayerToServer_06_08(unsigned long a);
    int SendPlayerToServer_0C_01(unsigned long a, unsigned long b);
    int SendPlayerToServerLogin_LoginLocalCharacter(unsigned char* pData,
                                                    unsigned long nSize, int bFlag);
    int SendPlayerToServer_02_0F();
};

#endif
