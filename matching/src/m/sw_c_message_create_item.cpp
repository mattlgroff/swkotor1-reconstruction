// CSWCMessage::CreateItem (nwchandlemessage.o): builds a client item and
// resolves its icon and model resources, falling back to the base item's
// defaults.

#include "shared/exo_string.h"

#include "shared/res_ref.h"

void* operator new(unsigned int nSize);
void operator delete(void* p);

class CExoResMan {
public:
    int Exists(const CResRef& cRes, unsigned short nType, unsigned long* pSize);  // 0x00408BC0
};

#include "shared/globals.h"

class CSWBaseItem {
public:
    CResRef GetIconResRef(short nVariation, char nTexture);     // 0x005B3050
    CResRef GetModelResRef(short nVariation, char nTexture);    // 0x005B30E0
    CResRef GetDefaultIcon() { return m_cDefaultIcon; }
    CResRef GetDefaultModel() { return m_cDefaultModel; }

    char pad_0x0[0x4c];
    CResRef m_cDefaultIcon;                         // 0x4c
    CResRef m_cDefaultModel;                        // 0x5c
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34
};


class CSWCItem {
public:
    CSWCItem(unsigned long nBaseItem);              // 0x00646230
    char pad_0x0[0x11c];
    unsigned char m_nModelVariation;                // 0x11c
    char m_nTextureVariation;                       // 0x11d
    char pad_0x11e[0x128 - 0x11e];
    CResRef m_cModel;                               // 0x128
    CResRef m_cIcon;                                // 0x138
    char pad_0x148[0x1a8 - 0x148];
};

class CSWCMessage {
public:
    CSWCItem* CreateItem(unsigned long nBaseItem, unsigned char nModelVariation, char nTextureVariation);
};

// FUNCTION: SWKOTOR 0x0064B540
CSWCItem* CSWCMessage::CreateItem(unsigned long nBaseItem, unsigned char nModelVariation, char nTextureVariation)
{
    CSWCItem* pItem = new CSWCItem(nBaseItem);
    if (!pItem) {
        return 0;
    }
    CSWBaseItem* pBaseItem = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
    if (!pBaseItem) {
        return 0;
    }
    CResRef cRes;
    cRes = pBaseItem->GetIconResRef(nModelVariation, nTextureVariation);
    if (!g_pExoResMan->Exists(cRes, 3, 0) && !g_pExoResMan->Exists(cRes, 0xbbf, 0)) {
        cRes = pBaseItem->GetDefaultIcon();
        if (!g_pExoResMan->Exists(cRes, 3, 0) || !g_pExoResMan->Exists(cRes, 0xbbf, 0)) {
            CExoString sMessage;
            CExoString sIcon;
            CExoString sDefault;
            pBaseItem->GetIconResRef(nModelVariation, nTextureVariation).CopyToString(sIcon);
            cRes.CopyToString(sDefault);
            sMessage.Format("Icon %s nor the default icon %s could be loaded.", sIcon.CStr(), sDefault.CStr());
        }
    }
    pItem->m_cIcon = cRes;
    pItem->m_nModelVariation = nModelVariation;
    pItem->m_nTextureVariation = nTextureVariation;
    cRes = pBaseItem->GetModelResRef(nModelVariation, nTextureVariation);
    if (!g_pExoResMan->Exists(cRes, 0x7d2, 0)) {
        cRes = pBaseItem->GetDefaultModel();
        if (!g_pExoResMan->Exists(cRes, 0x7d2, 0)) {
            CExoString sMessage;
            CExoString sModel;
            CExoString sDefault;
            pBaseItem->GetModelResRef(nModelVariation, nTextureVariation).CopyToString(sModel);
            cRes.CopyToString(sDefault);
            sMessage.Format("Model %s nor the default model %s could be loaded.", sModel.CStr(), sDefault.CStr());
        }
    }
    pItem->m_cModel = cRes;
    return pItem;
}
