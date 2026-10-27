// Floating combat text over objects (CSWGuiFloatyText, vtable 0x00753EA8)
// and the main interface list that owns it (+0x5cb4).
#include "k/sw_gui_main_interface.h"

// 0x154 bytes.
class CSWGuiFloatyText : public CSWGuiLabel {
public:
    CSWGuiFloatyText() {}
    void Init(unsigned long oid, const CExoString& sText, const Vector& vColor,
              float fDuration);                     // 0x00688E10
    float m_fTimeLeft;                              // 0x140
    float m_fDuration;                              // 0x144
    unsigned long m_oidObject;                      // 0x148
    unsigned char m_nStack;                         // 0x14c
    short m_nScreenX;                               // 0x14e
    short m_nScreenY;                               // 0x150
};

// FUNCTION: SWKOTOR 0x0068B7C0
void CSWGuiMainInterface::AddFloatyText(unsigned long oid, const CExoString& sText,
                                        const Vector& vColor, float fDuration)
{
    CSWGuiFloatyText* pText = new CSWGuiFloatyText;
    pText->Init(oid, sText, vColor, fDuration);
    for (int i = m_lstControls.num - 1; i >= 0; i--) {
        CSWGuiFloatyText* pOld = (CSWGuiFloatyText*)m_lstControls.element[i];
        if (pOld->m_oidObject == oid) {
            if (++pOld->m_nStack <= 5) {
                pOld->m_extent.top -= pOld->m_extent.height;
                pOld->SetExtent(pOld->m_extent);
            } else {
                if (m_lstControls.element[i]) {
                    delete m_lstControls.element[i];
                }
                int j = i;
                m_lstControls.num--;
                for (; j < m_lstControls.num; j++) {
                    m_lstControls.element[j] = m_lstControls.element[j + 1];
                }
            }
        }
    }
    m_lstControls.Add(pText);
}
