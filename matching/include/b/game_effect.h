// CGameEffect (0x8c bytes), shared by the worker B creature and VM headers.
#ifndef B_GAME_EFFECT_H
#define B_GAME_EFFECT_H

class CResGFF;
struct CResStruct;

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                          // 0x00503E40
    ~CGameEffect();                                         // 0x00503590
    void SetInteger(int nIndex, int nValue);                // 0x005036A0
    void SetFloat(int nIndex, float fValue);                // 0x005036D0
    void SetObjectID(int nIndex, unsigned long oid);        // 0x005036F0
    void SetCreator(unsigned long oid);                     // 0x00503A00
    void SetNumIntegers(int nNumIntegers);                  // 0x00503630
    void UpdateLinked();                                    // 0x00504240
    void SetLinked(CGameEffect* pChild, CGameEffect* pParent); // 0x00504810
    void SaveGameEffect(CResGFF* pRes, CResStruct* pStruct); // 0x00503790
    void CopyEffect(CGameEffect* pOther, int bArg);         // 0x00504090
    void LoadGameEffect(CResGFF* pRes, CResStruct* pStruct); // 0x005043A0

    unsigned short GetDurationType() { return m_nDurationType; }
    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }
    void SetSubType_Supernatural() { m_nSubType = (m_nSubType & 0xffe7) | 0x8; }

    unsigned __int64 m_nID;                                 // 0x00
    unsigned short m_nType;                                 // 0x08
    union {
        unsigned short m_nSubType;                          // 0x0a
        struct {
            unsigned short m_nDurationType : 3;             // 0x0a (low 3 bits)
            unsigned short m_nSubTypeHigh : 13;
        };
    };
    float m_fDuration;                                      // 0x0c
    char pad_0x10[0x18 - 0x10];
    unsigned long m_oidCreator;                             // 0x18
    char pad_0x1c[0x20 - 0x1c];
    int m_n20;                                              // 0x20 (name guessed)
    int m_n24;                                              // 0x24 (name guessed)
    char pad_0x28[0x8c - 0x28];
};
#pragma pack(pop)

#endif
