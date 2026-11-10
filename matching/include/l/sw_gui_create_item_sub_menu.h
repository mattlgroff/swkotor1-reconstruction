// Worker L matching headers: the debug item-creation sub menu
// CSWGuiCreateItemSubMenu (swguiSubCreateItem.o, layout "debug", vtable
// 0x00756B30, 0x4BC bytes, 0x006BD980-0x006BE2B0). It lists the UTI
// templates whose BaseItem matches the base item picked in
// CSWGuiItemCreateMenu, and gives the chosen one to the player creature.
#ifndef L_SW_GUI_CREATE_ITEM_SUB_MENU_H
#define L_SW_GUI_CREATE_ITEM_SUB_MENU_H

#include "l/sw_gui_debug_menu.h"

class CExoStringList {
public:
    CExoStringList();                               // 0x005E9530
    ~CExoStringList();                              // 0x005E9860
    void Clear();                                   // 0x005E9590
    CExoString* GetString(int nIndex);              // 0x005E95E0
    void Add(CExoString* pString);                  // 0x005E98B0
    int GetCount();                                 // 0x00675DD0 (folded getter)

    char pad_0x0[0xc];
    int m_bSorted;                                  // 0x0c
    char pad_0x10[0x4];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CRes {
public:
    virtual ~CRes();                                // 0x00409D40
    void* Demand();                                 // 0x00409B10
    int Release();                                  // 0x00409B80

    char pad_0x4[0x24];
};

class CResGFF : public CRes {
public:
    CResGFF();                                      // 0x004105A0
    void GetTopLevelStruct(CResStruct* pStruct);    // 0x00411240
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess,
                             const CResRef& cDefault);  // 0x00411E10
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault);  // 0x00411C90

    char pad_0x28[0x65];
    char m_pFileType[4];                            // 0x8d
    char pad_0x91[0xf];
};

class CExoResMan {
public:
    CExoStringList* GetResOfType(unsigned short nType, int bUnused);    // 0x00407390
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType);   // 0x004074D0
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    unsigned long ReleaseResObject(CRes* pRes);     // 0x00409CF0
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

extern "C" char* __cdecl strncpy(char* szDest, const char* szSrc, unsigned int nCount);

// Control list with the inline constructor and the releasing SetSize the
// sub menu uses for its buttons.
class CSWGuiControlList : public CExoArrayList<CSWGuiControl*> {
public:
    CSWGuiControlList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    CSWGuiControl*& operator[](int i) { return element[i]; }
    void SetSizeFree(int nSize)
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(nSize);
        }
        num = nSize;
    }
};

class CSWGuiCreateItemSubMenu : public CSWGuiPanel {
public:
    CSWGuiCreateItemSubMenu(CSWGuiManager* pManager);   // 0x006BDC60
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006BD980)

    CSWGuiButton* CreateButton(const CExoString& sText, PFN_GUI_CALLBACK pfnCallback,
                               CSWGuiButton* pProto);   // 0x006BDA90
    void OnBack(CSWGuiControl* pControl);           // 0x006BDBA0 (descriptive)
    void OnItemSelected(CSWGuiControl* pControl);   // 0x006BDBE0 (descriptive)
    void LoadItems(int nBaseItem);                  // 0x006BDEA0
    void ClearItems();                              // 0x006BE2B0

    CSWGuiListBox m_lbOptions;                      // 0x64
    CSWGuiLabel m_lblBuild;                         // 0x344
    int m_nSelected;                                // 0x484 (-0x63: none)
    CSWGuiControlList m_lstButtons;                 // 0x488
    CExoStringList m_lstNames;                      // 0x494 (template resrefs)
    CExoStringList m_lstResRefs;                    // 0x4a8 (UTI file names)
};

#endif
