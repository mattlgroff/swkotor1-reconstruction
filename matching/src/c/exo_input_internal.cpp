// CExoInputInternal event bookkeeping (b005e6_exo_input_internal).

#include "c/exo_input.h"

// FUNCTION: SWKOTOR 0x005E0320
CExoString CExoInputInternal::GetDisplayCharacterForKey(unsigned long nKey)
{
    return m_pRawInput->GetKeyName(m_aKeyMap[nKey]);
}

// FUNCTION: SWKOTOR 0x005E0480
CExoInputEventDesc* CExoInputInternal::IsControlUsed(unsigned long nControl, int nSlot, int nClass)
{
    if (nClass < 0 || nClass >= 6 || nSlot < 0 || nSlot >= 6 || nControl >= 0x84) {
        return 0;
    }
    CExoLinkedListPosition pos = m_aClasses[nClass].m_lstSlots[nSlot].GetHeadPos();
    while (pos) {
        unsigned long* pIndex = m_aClasses[nClass].m_lstSlots[nSlot].GetAtPos(pos);
        CExoInputEventDesc* pDesc = m_aEventDescs[*pIndex];
        if (pDesc->m_nControl == nControl) {
            return pDesc;
        }
        if (pDesc->HasAlternate() && pDesc->m_nAltControl == nControl) {
            return pDesc;
        }
        m_aClasses[nClass].m_lstSlots[nSlot].GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E05F0
void CExoInputInternal::ClearEventRepeat(unsigned long nEvent)
{
    CExoInputEventDesc* pDesc = m_aEventDescs[nEvent];
    if (pDesc == 0 || pDesc->m_nType != 1) {
        return;
    }
    CExoLinkedListPosition pos = m_lstRepeatEvents.GetHeadPos();
    while (pos) {
        CExoInputRepeatEvent* pRepeat = m_lstRepeatEvents.GetAtPos(pos);
        if (pRepeat && pRepeat->m_nEvent == nEvent) {
            pRepeat->m_nUnknown0c = 0;
            pRepeat->m_nUnknown18 = 0;
            pRepeat->m_nUnknown10 = 0;
            pRepeat->m_nUnknown14 = 0;
            return;
        }
        m_lstRepeatEvents.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x005E0670
void CExoInputInternal::CoolDownEvent(unsigned long nEvent, int bCoolDown)
{
    CExoLinkedListPosition pos = m_lstCoolDown.GetHeadPos();
    CExoInputEventDesc** ppDesc = &m_aEventDescs[nEvent];
    if (*ppDesc == 0) {
        return;
    }
    (*ppDesc)->m_bCoolDown = bCoolDown;
    while (pos) {
        CExoInputEventDesc* pOther = *m_lstCoolDown.GetAtPos(pos);
        if (pOther && pOther->m_nId == (*ppDesc)->m_nId) {
            if (!bCoolDown) {
                m_lstCoolDown.Remove(pos);
            }
            return;
        }
        m_lstCoolDown.GetNext(pos);
    }
    m_lstCoolDown.AddTail(ppDesc);
}

// FUNCTION: SWKOTOR 0x005E08C0
void CExoInputInternal::BufferEvent(unsigned long nValue1, unsigned long nValue2, unsigned long nEvent)
{
    CExoInputBufferedEvent* pEvent = new CExoInputBufferedEvent;
    pEvent->m_nEvent = nEvent;
    pEvent->m_n8 = 0;
    pEvent->m_n4 = 0;
    pEvent->m_pPayload = new CExoInputBufferedPayload();
    pEvent->m_pPayload->m_nValue1 = nValue1;
    pEvent->m_pPayload->m_nValue2 = nValue2;
    pEvent->m_pPayload->m_n10 = 0;
    pEvent->m_pPayload->m_n14 = 0;
    m_lstBufferedEvents.AddHead(pEvent);
}

// FUNCTION: SWKOTOR 0x005E0D80
int CExoInputInternal::IsEventInClass(int nEvent, int nClass)
{
    // NONMATCH (14 diff insns): register allocation / scheduling only. The
    // original computes the class base (this + class*0x30) before nWord and
    // loads the old count after it (word in ebx, count in edi, base in esi).
    if (nClass >= 0 && nClass < 6) {
        int nCount = m_aClasses[nClass].m_aBits.num;
        int nWord = nEvent / 32;
        if (nWord >= nCount) {
            m_aClasses[nClass].m_aBits.Allocate(nWord * 2 + 1);
            int nNew = m_aClasses[nClass].m_aBits.num;
            for (int i = nCount; i < nNew; i++) {
                m_aClasses[nClass].m_aBits[i] = 0;
            }
        }
        if (m_aClasses[nClass].m_aBits[nWord] & (1 << (nEvent % 32))) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E2360
CExoInputEventDesc* CExoInputInternal::GetEvent(int nEvent, int nClass)
{
    CExoInputEventDesc* pDesc = 0;
    if (nClass >= 0 && nClass < 6) {
        if (IsEventInClass(nEvent, nClass)) {
            pDesc = m_aEventDescs[nEvent];
        }
    } else if (nClass == -1 && (unsigned long)nEvent < (unsigned long)m_aEventDescs.num) {
        pDesc = m_aEventDescs[nEvent];
    }
    return pDesc;
}

// FUNCTION: SWKOTOR 0x005E2F70
void CExoInputInternal::ClearAllInputEventDescriptions()
{
    for (int nClass = 0; nClass < 6; nClass++) {
        for (int nSlot = 0; nSlot < m_nSlots; nSlot++) {
            while (!m_aClasses[nClass].m_lstSlots[nSlot].IsEmpty()) {
                m_aClasses[nClass].m_lstSlots[nSlot].RemoveHead();
            }
        }
        for (int i = 0; i < m_aClasses[nClass].m_aBits.num; i++) {
            m_aClasses[nClass].m_aBits[i] = 0;
        }
    }
    m_aEventDescs.num = 0;
}

// FUNCTION: SWKOTOR 0x005E0070
void CExoInputInternal::HideMouse()
{
    tagCURSORINFO ci;
    ci.cbSize = sizeof(ci);
    if (m_nMouseMode == 0) {
        if (GetCursorInfo(&ci) && (ci.flags & 1)) {
            while (ShowCursor(0) >= 0) {
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005E00B0
void CExoInputInternal::ShowMouse()
{
    tagCURSORINFO ci;
    ci.cbSize = sizeof(ci);
    if (m_nMouseMode == 0) {
        if (GetCursorInfo(&ci) && !(ci.flags & 1)) {
            while (ShowCursor(1) < 0) {
            }
        }
    }
}
