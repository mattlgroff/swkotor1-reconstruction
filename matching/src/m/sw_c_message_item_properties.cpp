// CSWCMessage active item property handlers (nwchandlemessage.o).

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

struct CSWCItemProperty {
    unsigned short m_nPropertyName;                 // 0x00
    unsigned short m_nSubType;                      // 0x02
    unsigned short m_n04;                           // 0x04
    unsigned short m_nCostValue;                    // 0x06
    unsigned char m_nParam1Value;                   // 0x08
    char pad_0x9[3];
    int m_bActive;                                  // 0x0c
    unsigned char m_nUsesLeft;                      // 0x10
    unsigned char m_nUpgradeType;                   // 0x11
};

struct CSWBaseItemData {
    char pad_0x0[0xac];
    unsigned char m_nEquipType;                     // 0xac (descriptive)
};

class CSWCItem {
public:
    CSWBaseItemData* GetBaseItem();                 // 0x005B4790
    void SetNumActiveProperties(unsigned long nCount);  // 0x00646660
    CSWCItemProperty* GetActiveProperty(int nIndex);    // 0x006466A0
    void SetStackSize(int nSize);                   // 0x00646810
    void SetItemPropertyUsesLeft(int nIndex, unsigned short nUses);   // 0x006469B0

    char pad_0x0[0x10c];
    int m_nCharges;                                 // 0x10c (descriptive)
    char pad_0x110[4];
    int m_bIdentified;                              // 0x114 (descriptive)
    int m_bStolen;                                  // 0x118 (descriptive)
    char pad_0x11c[4];
    int m_nActiveProperties;                        // 0x120
    char pad_0x124[0x17c - 0x124];
    unsigned char m_nLightsaberColorProperty;       // 0x17c (descriptive)
    char pad_0x17d[3];
    unsigned long m_nCost;                          // 0x180 (descriptive)
    char pad_0x184[4];
    int m_bPlot;                                    // 0x188 (descriptive)
    CExoString m_sName;                             // 0x18c
};

class CGuiInGame;

class CClientExoApp {
public:
    CSWCItem* GetItemByGameObjectID(unsigned long nId);   // 0x005ED5A0
    CGuiInGame* GetInGameGui();                     // 0x005ED690
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCMessage {
public:
    int MessageReadOverflow(int bError);            // 0x004D6230
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned short ReadWORD(int nBits);             // 0x004D6750
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    unsigned long ReadOBJECTIDClient();             // 0x00692630
    CExoString ReadCExoLocStringClient(CResRef& cRes, unsigned long* pStrRef);  // 0x00692660
    int HandleServerToPlayerActiveItemProperties(unsigned long nId);
    int HandleServerToPlayerActiveItemPropertyUpdate(unsigned char nMinor);
};

// FUNCTION: SWKOTOR 0x0064E3D0
int CSWCMessage::HandleServerToPlayerActiveItemProperties(unsigned long nId)
{
    CExoString sName;
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(nId);
    if (pItem && pItem->GetBaseItem()->m_nEquipType >= 0x1f && pItem->GetBaseItem()->m_nEquipType <= 0x26) {
        short nCharges = ReadWORD(16);
        if (MessageReadOverflow(1)) {
            return 0;
        }
        pItem->m_nCharges = nCharges;
    }
    sName = ReadCExoLocStringClient(CResRef(), 0);
    int bIdentified = ReadBOOL();
    unsigned long nCost = ReadDWORD(32);
    unsigned long nStackSize = ReadDWORD(32);
    int bPlot = ReadBOOL();
    int bStolen = ReadBOOL();
    unsigned char nCount = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nCount > 0) {
        if (pItem) {
            pItem->SetNumActiveProperties(nCount);
        }
        for (int i = 0; i < nCount; i++) {
            unsigned short nName = ReadWORD(16);
            unsigned short nSubType = ReadWORD(16);
            unsigned short nCostValue = ReadWORD(16);
            unsigned char nParam = ReadBYTE(8);
            if (MessageReadOverflow(1)) {
                return 0;
            }
            if (pItem) {
                pItem->GetActiveProperty(i)->m_nPropertyName = nName;
                pItem->GetActiveProperty(i)->m_nSubType = nSubType;
                pItem->GetActiveProperty(i)->m_nCostValue = nCostValue;
                pItem->GetActiveProperty(i)->m_nParam1Value = nParam;
                pItem->GetActiveProperty(i)->m_bActive = 1;
                pItem->GetActiveProperty(i)->m_nUsesLeft = 0;
                pItem->GetActiveProperty(i)->m_nUpgradeType = 0xff;
                if (nName == 10 && pItem->m_nLightsaberColorProperty == -1) {
                    pItem->m_nLightsaberColorProperty = i;
                }
            }
        }
    }
    if (pItem) {
        pItem->m_bIdentified = bIdentified;
        pItem->m_nCost = nCost;
        pItem->SetStackSize(nStackSize);
        pItem->m_sName = sName;
        pItem->m_bPlot = bPlot;
        pItem->m_bStolen = bStolen;
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui();
    unsigned char nActive = ReadBYTE(8);
    unsigned char nUsesMask = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    for (int j = 0; j < 8; j++) {
        if (nUsesMask & (1 << j)) {
            unsigned char nUses = ReadBYTE(8);
            if (MessageReadOverflow(1)) {
                return 0;
            }
            if (pItem) {
                pItem->SetItemPropertyUsesLeft(j, nUses);
                CSWCItemProperty* pProperty = pItem->GetActiveProperty(j);
                if (pProperty) {
                    pProperty->m_bActive = (nActive & (1 << j)) != 0;
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00651560
// The uses loop runs over 64 bits of an 8-entry array (original bug).
int CSWCMessage::HandleServerToPlayerActiveItemPropertyUpdate(unsigned char nMinor)
{
    unsigned char nActive;
    unsigned char anUses[8];
    unsigned long nId = ReadOBJECTIDClient();
    nActive = ReadBYTE(8);
    unsigned char nUsesMask = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nMinor == 1) {
        int i;
        for (i = 0; i < 8; i++) {
            if (nUsesMask & (1 << i)) {
                unsigned char nUses = ReadBYTE(8);
                if (MessageReadOverflow(1)) {
                    return 0;
                }
                anUses[i] = nUses;
            }
        }
        CSWCItem* pItem = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(nId);
        if (pItem) {
            for (i = 0; i < pItem->m_nActiveProperties; i++) {
                CSWCItemProperty* pProperty = pItem->GetActiveProperty(i);
                if (pProperty) {
                    pProperty->m_bActive = (nActive & (1 << i)) != 0;
                }
            }
            for (i = 0; i < 0x40; i++) {
                if (nUsesMask & (1 << i)) {
                    pItem->SetItemPropertyUsesLeft(i, anUses[i]);
                }
            }
        }
    }
    return 1;
}
