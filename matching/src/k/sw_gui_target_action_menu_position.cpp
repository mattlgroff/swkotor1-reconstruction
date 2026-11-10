// CSWGuiTargetActionMenu::PositionMenu: places the target action menu next to
// the target's menu point, inside the menu bounds rectangle.
#include <stdlib.h>
#include "k/sw_gui_main_interface.h"

// NONMATCH (136): flow and x87 compares match; the original reloads the
// bounds fields for every expression (and in the __max/__min re-evaluation)
// where cl CSEs them, so the placement arms and clamp are shorter.
// FUNCTION: SWKOTOR 0x00686090
void CSWGuiTargetActionMenu::PositionMenu(CSWCCreature* pPC, CSWCObject* pTarget)
{
    if (pTarget) {
        m_bShown = 1;
        int bActions = 0;
        for (int i = 0; i < 3 && !bActions; i++) {
            if (m_aLists[i].m_nCount) {
                bActions = 1;
            }
        }
        int nOldHeight = m_extent.height;
        if (bActions) {
            m_extent.height = m_aActions[0].m_btnFrame.m_extent.height +
                              m_aActions[0].m_btnFrame.m_extent.top;
        }
        else {
            m_extent.height = m_pbHealth.m_extent.height + m_pbHealth.m_extent.top;
        }
        if (m_bMouseOver || m_fHiliteTimer > 0.0f) {
            m_extent.top += nOldHeight - m_extent.height;
            return;
        }
        Vector vPoint;
        vPoint.x = 0.0f;
        vPoint.y = 0.0f;
        vPoint.z = 0.0f;
        int nType;
        if (pTarget->AsSWCCreature()) {
            nType = pTarget->AsSWCCreature()->GetMenuPoint(vPoint);
        }
        else {
            nType = pTarget->GetReticlePoint(vPoint);
        }
        int nScreenWidth = g_pAppManager->GetClientExoApp()->GetGuiManager()->m_nWidth;
        int nScreenHeight = g_pAppManager->GetClientExoApp()->GetGuiManager()->m_nHeight;
        if (nType) {
            if (nType == 2) {
                if (nScreenWidth / 2 > vPoint.x) {
                    m_extent.left = m_rectBounds.width - m_extent.width + m_rectBounds.left;
                }
                else {
                    m_extent.left = m_rectBounds.left;
                }
                if (m_rectBounds.top + 32 > vPoint.y) {
                    m_extent.top = m_rectBounds.height + m_rectBounds.top - m_extent.height - 32;
                }
                else if (m_rectBounds.top + m_rectBounds.height - 32 < vPoint.y) {
                    m_extent.top = m_rectBounds.top;
                }
                else if (nScreenHeight / 2 > vPoint.y) {
                    m_extent.top = nScreenHeight - (int)(m_extent.height + vPoint.y + 32.0f);
                }
                else {
                    m_extent.top = (int)(vPoint.y - 32.0f - m_extent.height);
                }
            }
            else {
                int nHalfWidth = m_extent.width / 2;
                float fLeft = vPoint.x - nHalfWidth;
                if (m_rectBounds.left > fLeft) {
                    m_extent.left = m_rectBounds.left;
                    if (m_rectBounds.top + 32 > vPoint.y) {
                        m_extent.top = m_rectBounds.top;
                    }
                    else if (m_rectBounds.top + m_rectBounds.height - 32 < vPoint.y) {
                        m_extent.top = m_rectBounds.height + m_rectBounds.top - m_extent.height - 32;
                    }
                    else {
                        m_extent.top = (int)(vPoint.y - 32.0f - m_extent.height);
                    }
                }
                else if (m_rectBounds.width + m_rectBounds.left < vPoint.x + nHalfWidth) {
                    m_extent.left = m_rectBounds.width - m_extent.width + m_rectBounds.left;
                    if (m_rectBounds.top + 32 > vPoint.y) {
                        m_extent.top = m_rectBounds.top;
                    }
                    else if (m_rectBounds.top + m_rectBounds.height - 32 < vPoint.y) {
                        m_extent.top = m_rectBounds.height + m_rectBounds.top - m_extent.height - 32;
                    }
                    else {
                        m_extent.top = (int)(vPoint.y - 32.0f - m_extent.height);
                    }
                }
                else if (m_rectBounds.top + 32 > vPoint.y) {
                    m_extent.top = m_rectBounds.top;
                    m_extent.left = (int)fLeft;
                }
                else if (m_rectBounds.top + m_rectBounds.height - 32 < vPoint.y) {
                    m_extent.top = m_rectBounds.height + m_rectBounds.top - m_extent.height - 32;
                    m_extent.left = (int)fLeft;
                }
                else {
                    m_extent.left = (int)vPoint.x - nHalfWidth;
                    m_extent.top = (int)vPoint.y - m_extent.height - 32;
                }
            }
        }
        m_extent.left = __max(m_rectBounds.left,
                              __min(m_extent.left,
                                    m_rectBounds.width - m_extent.width + m_rectBounds.left));
        m_extent.top = __max(m_rectBounds.top,
                             __min(m_extent.top,
                                   m_rectBounds.height + m_rectBounds.top - m_extent.height));
    }
    else {
        m_bShown = 0;
    }
}
