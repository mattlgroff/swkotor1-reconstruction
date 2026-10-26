// Worker L matching headers: the key-mapping options panel
// CSWGuiInGameOptKeyMappings (layout "OPTKeyMapping", vtable 0x00759358,
// size 0xF34, swguiinOPTKeyMapping.o) and the CSWGuiKeyMapButton entries it
// owns. Member offsets follow the constructor 0x006EDC90 and destructor
// 0x006EC3D0.
#ifndef L_SW_GUI_KEYMAPPINGS_H
#define L_SW_GUI_KEYMAPPINGS_H

#include <stddef.h>
#include "l/sw_gui_keymap_button.h"
#include "l/sw_gui_in_game_abilities.h"

// CExoArrayList<T> with the inline constructor, growth and the destructor
// that clears the element pointer after freeing it.
template <class T>
class CExoArrayListKM {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayListKM()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayListKM()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int nSize);                       // 0x005E03C0 (folded <int>)
    T& operator[](int i) { return element[i]; }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 0x10);
        }
        element[num] = t;
        num++;
    }
    __forceinline void AddInline(T t)
    {
        if (num == array_size) {
            int nSize = array_size ? array_size * 2 : 0x10;
            T* pOld = element;
            array_size = nSize;
            element = new T[nSize];
            for (int i = 0; i < num; i++) {
                element[i] = pOld[i];
            }
            if (pOld) {
                delete[] pOld;
            }
        }
        element[num] = t;
        num++;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (int i = nIndex; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
};

// Input event description (exoinput.o); +0x1c key, +0x30 alternate key.
class CExoInputEvent {
public:
    virtual int IsAxis();                           // 0x00
    virtual int HasAlternate();                     // 0x04
    char pad_0x4[0x18];
    unsigned long m_nKey;                           // 0x1c
    char pad_0x20[0x10];
    unsigned long m_nAltKey;                        // 0x30
};

class CExoInput {
public:
    CExoInputEvent* GetEvent(unsigned long nId, int nClass);    // 0x005DF4C0
    int GetLastCapturedKeyboardKey();               // 0x005DF630
    void SetCaptureKeyboardKeys(int bCapture);      // 0x005DF720
};

extern CExoInput* g_pExoInput;                      // 0x007A39E4

// CExoIni is declared in l/sw_gui_options_in_game.h.

extern const unsigned long g_nKeyMapKeyBase;        // 0x0074D3D8
extern const unsigned long g_nKeyMapEscapeKey;      // 0x0074D45C

class CSWGuiInGameOptKeyMappings : public CSWGuiPanel {
public:
    CSWGuiInGameOptKeyMappings(CSWGuiManager* pManager);  // 0x006EDC90
    virtual ~CSWGuiInGameOptKeyMappings();          // 0x006EC3D0
    void Update(float fDelta);                      // 0x38 (0x006EE040)
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006EC510)

    void SetEditing(int bEditing);                  // 0x006EC3B0
    void FixFilterColors(CSWGuiControl* pControl);  // 0x006EC580
    void FillListBox(int nFilter);                  // 0x006ECFD0
    int IsKeyUsedAlready(unsigned long nKey, int nId, int nType, unsigned long nClassFlags);   // 0x006ED1E0
    void OnDefaultClick(CSWGuiControl* pControl);   // 0x006ED2D0
    void OnAcceptClick(CSWGuiControl* pControl);    // 0x006ED310
    void OnFilterMove(CSWGuiControl* pControl);     // 0x006ED390
    void OnFilterGame(CSWGuiControl* pControl);     // 0x006ED3E0
    void OnFilterMini(CSWGuiControl* pControl);     // 0x006ED430
    void SetCaptureEvent(CSWGuiKeyMapButton* pButton);  // 0x006ED480
    CSWGuiKeyMapButton* CreateOption(int nId, CSWGuiButton* pProto, int nType);   // 0x006ED610
    void FillListWithEvents();                      // 0x006ED930
    int UpdateEvent(int nId, int nType, unsigned long nKey, unsigned long nClassFlags);   // 0x006EDA80

    CExoArrayListKM<CSWGuiKeyMapButton*> m_lstButtons;  // 0x64
    CSWGuiLabel m_lblTitle;                         // 0x70
    CSWGuiButton m_btnDefault;                      // 0x1b0
    CSWGuiButton m_btnAccept;                       // 0x374
    CSWGuiButton m_btnCancel;                       // 0x538
    CSWGuiButton m_btnFilterMove;                   // 0x6fc
    CSWGuiButton m_btnFilterGame;                   // 0x8c0
    CSWGuiButton m_btnFilterMini;                   // 0xa84
    CSWGuiListBox m_lstEvents;                      // 0xc48
    int m_nFilter;                                  // 0xf28
    int m_bEditing;                                 // 0xf2c
    CSWGuiKeyMapButton* m_pCaptureButton;           // 0xf30
};

typedef char CSWGuiInGameOptKeyMappings_layout_check[
    (offsetof(CSWGuiInGameOptKeyMappings, m_lblTitle) == 0x70 &&
     offsetof(CSWGuiInGameOptKeyMappings, m_btnFilterMini) == 0xA84 &&
     offsetof(CSWGuiInGameOptKeyMappings, m_lstEvents) == 0xC48 &&
     offsetof(CSWGuiInGameOptKeyMappings, m_nFilter) == 0xF28 &&
     sizeof(CSWGuiInGameOptKeyMappings) == 0xF34) ? 1 : -1];

#endif
