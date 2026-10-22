// CExoInputInternal mouse/event members (b005e6 cluster), worker C r8.

#include <string.h>
#include <math.h>
#include "c/exo_input.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

// Plain event description (vtable 0x0074D5FC, 0x20 bytes): the zeroed
// +0x04..+0x0c block is a non-polymorphic base, so it is written before the
// vtable pointer.
struct CExoInputEventDescData {
    CExoInputEventDescData() { m_n4 = 0; m_n8 = 0; m_nC = 0; }
    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    unsigned long m_nC;                             // 0x0c
};

class CExoInputEventDescBasic : public CExoInputEventDescData {
public:
    virtual void Unknown00();
    unsigned long m_n10;                            // 0x10
    unsigned long m_n14;                            // 0x14
    unsigned long m_n18;                            // 0x18
    unsigned long m_n1c;                            // 0x1c
};

// FUNCTION: SWKOTOR 0x005E0000
int CExoInputInternal::SetMouseCursor(int nCursor, int nOption, int bAlternate)
{
    int nEffective = nCursor;
    m_nUnknown3ac = bAlternate;
    if (bAlternate) {
        nEffective = nCursor + nCursor % 2;
    }
    if (m_nCursor != nEffective) {
        m_nCursor = nCursor;
        if (m_nMouseMode == 0) {
            HCURSOR hCursor = LoadCursorA(g_hInstance, (const char*)(unsigned short)nEffective);
            if (hCursor) {
                HCURSOR hOld = SetCursor(hCursor);
                if (hOld != hCursor) {
                    DestroyCursor(hOld);
                }
            }
        }
    }
    return nEffective;
}

// FUNCTION: SWKOTOR 0x005E00F0
void CExoInputInternal::GetMouseDelta(float* pX, float* pY)
{
    *pX = m_fMouseDeltaX;
    *pY = m_fMouseDeltaY;
}

// FUNCTION: SWKOTOR 0x005E0110
void CExoInputInternal::UpdateMouseDelta(float f)
{
    CExoMouseState state;
    memset(&state, 0, sizeof(state));
    if (m_pRawInput->GetMouseState(&state) == 1) {
        m_fMouseDeltaX = max(-1.0f, min(1.0f, (float)(-state.lX) / g_fMouseDeltaScale));
        m_fMouseDeltaY = max(-1.0f, min(1.0f, (float)(-state.lY) / g_fMouseDeltaScale));
    } else {
        m_fMouseDeltaX = 0.0f;
        m_fMouseDeltaY = 0.0f;
    }
}

// FUNCTION: SWKOTOR 0x005E0710
void CExoInputInternal::ClearEvents()
{
    if (m_pRawInput == 0) {
        return;
    }
    m_pRawInput->FlushMouseData();
    m_pRawInput->FlushKeyboardData();
    for (int i = 0; i < m_aEventDescs.num; i++) {
        CExoInputEventDesc* pDesc = m_aEventDescs[i];
        if (pDesc) {
            switch (pDesc->m_nType) {
            case 4:
                pDesc->m_nValue34 = 0;
            case 3:
                pDesc->m_nValue24 = 0;
            case 0:
            case 1:
            case 2:
            case 5:
                pDesc->m_nValue = 0;
            }
        }
    }
    CExoLinkedListPosition pos = m_lstRepeatEvents.GetHeadPos();
    while (pos) {
        CExoInputRepeatEvent* pRepeat = m_lstRepeatEvents.GetAtPos(pos);
        pRepeat->m_nUnknown0c = 0;
        pRepeat->m_nUnknown18 = 0;
        m_lstRepeatEvents.GetNext(pos);
    }
    while (!m_lstBufferedEvents.IsEmpty()) {
        CExoInputBufferedEvent* pEvent = m_lstBufferedEvents.RemoveHead();
        if (pEvent) {
            if (pEvent->m_pPayload) {
                delete pEvent->m_pPayload;
                pEvent->m_pPayload = 0;
            }
            delete pEvent;
        }
    }
}

// FUNCTION: SWKOTOR 0x005E0930
void CExoInputInternal::RefreshMouseCursor(int b)
{
    int nCursor = m_nCursor;
    m_nCursor = -1;
    if (nCursor == -1) {
        nCursor = 1;
    }
    SetMouseCursor(nCursor, m_nCursorOption, b);
}

// FUNCTION: SWKOTOR 0x005E0960
void CExoInputInternal::SetMousePos(int x, int y)
{
    tagPOINT pt;
    pt.x = x;
    pt.y = g_nScreenHeight - y - 1;
    ClientToScreen(g_hWnd, &pt);
    SetCursorPos(pt.x, pt.y);
    m_bMousePosSet = 1;
    for (int i = 0; i < m_aActiveEvents.num; i++) {
        CExoInputActiveEvent* pActive = &m_aActiveEvents[i];
        if (pActive) {
            CExoInputEventDesc* pDesc = pActive->m_pDesc;
            if (pDesc) {
                if (pDesc->m_nControl == g_nInputMouseAxisX && pDesc->IsAxis()) {
                    pDesc->m_nValue = x;
                }
                if (pDesc->m_nControl == g_nInputMouseAxisY && pDesc->IsAxis()) {
                    pDesc->m_nValue = y;
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005E0E20
// NONMATCH (27 diff insns): switch body only. The original sinks the common
// m_nId/m_nControl stores into one tail and tail-duplicates the m_nType /
// m_nSlot stores into both arms of the `new` null check for types 3 and
// 0/1/2/5; a goto/default-return tail reshuffles the prologue registers.
int CExoInputInternal::CreateNewEvent(unsigned long nEvent, int nType, int nSlot, unsigned long nControl, unsigned long nAltControl)
{
    if (nSlot >= 6) {
        return 0;
    }
    int nOld = m_aEventDescs.num;
    if (nEvent >= (unsigned long)nOld) {
        m_aEventDescs.SetSize(nEvent * 2 + 1);
        int nNew = m_aEventDescs.num;
        for (int i = nOld; i < nNew; i++) {
            m_aEventDescs[i] = 0;
        }
    }
    if (m_aEventDescs[nEvent]) {
        return 0;
    }
    CExoInputEventDesc* pDesc = 0;
    switch (nType) {
    case 3:
        pDesc = (CExoInputEventDesc*)new CExoInputEventDescType3();
        pDesc->m_nType = 3;
        pDesc->m_nSlot = nSlot;
        pDesc->m_nId = nEvent;
        pDesc->m_nControl = nControl;
        break;
    case 0:
    case 1:
    case 2:
    case 5:
        pDesc = (CExoInputEventDesc*)new CExoInputEventDescBasic();
        pDesc->m_nType = nType;
        pDesc->m_nSlot = nSlot;
        pDesc->m_nId = nEvent;
        pDesc->m_nControl = nControl;
        break;
    case 4:
        pDesc = (CExoInputEventDesc*)new CExoInputEventDescType4();
        pDesc->m_nSlot = nSlot;
        pDesc->m_nType = 4;
        pDesc->m_nAltControl = nAltControl;
        pDesc->m_nId = nEvent;
        pDesc->m_nControl = nControl;
        break;
    }
    m_aEventDescs[nEvent] = pDesc;
    return 1;
}

// FUNCTION: SWKOTOR 0x005E0FA0
int CExoInputInternal::AddEvent(int nEvent, unsigned long nClass)
{
    if (nClass < 6) {
        CExoInputEventDesc* pDesc = m_aEventDescs[nEvent];
        if (pDesc && !IsEventInClass(nEvent, nClass)) {
            if (pDesc->m_nControl != 0x84 && IsControlUsed(pDesc->m_nControl, pDesc->m_nSlot, nClass)) {
                return 0;
            }
            if (pDesc->HasAlternate() && pDesc->m_nAltControl != 0x84
                && IsControlUsed(pDesc->m_nAltControl, pDesc->m_nSlot, nClass)) {
                return 0;
            }
            pDesc->m_nRefCount++;
            if (pDesc->m_nSlot == g_nInputAnySlot) {
                m_aClasses[nClass].m_lstAnySlot.AddHead(&pDesc->m_nId);
                return 1;
            }
            m_aClasses[nClass].m_lstSlots[pDesc->m_nSlot].AddHead(&pDesc->m_nId);
            m_aClasses[nClass].m_aBits[nEvent / 32] |= 1 << (nEvent % 32);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E10B0
int CExoInputInternal::RemoveEvent(int nEvent)
{
    CExoInputEventDesc** ppDesc = &m_aEventDescs[nEvent];
    CExoInputEventDesc* pDesc = *ppDesc;
    if (pDesc == 0) {
        return -1;
    }
    *ppDesc = 0;
    int nClass = -1;
    for (int i = 0; i < 6; i++) {
        if (IsEventInClass(nEvent, i)) {
            nClass = i;
            break;
        }
    }
    if (nClass == -1) {
        return -1;
    }
    CExoLinkedListPosition pos = m_aClasses[nClass].m_lstSlots[pDesc->m_nSlot].GetHeadPos();
    while (pos) {
        m_aClasses[nClass].m_lstSlots[pDesc->m_nSlot].GetAtPos(pos);
        if (nEvent) {
            break;
        }
        m_aClasses[nClass].m_lstSlots[pDesc->m_nSlot].GetNext(pos);
    }
    if (pos == 0) {
        return -1;
    }
    m_aClasses[nClass].m_lstSlots[pDesc->m_nSlot].Remove(pos);
    m_aClasses[nClass].m_aBits[nEvent / 32] &= 0 << (nEvent % 32);
    delete pDesc;
    return nClass;
}

// FUNCTION: SWKOTOR 0x005E11D0
int CExoInputInternal::ScaleEvent(int nEvent, float fScaleA, float fScaleB)
{
    CExoInputEventDesc* pDesc = m_aEventDescs[nEvent];
    if (pDesc) {
        if (pDesc->IsAxis()) {
            pDesc->m_fScale28 = fScaleA;
            pDesc->m_fScale2c = fScaleB;
            pDesc->m_bScaled = 1;
            return 1;
        }
        switch (pDesc->m_nType) {
        case 0:
        case 2:
        case 5: {
            CExoInputEventDesc* pNew = (CExoInputEventDesc*)new CExoInputEventDescType3(pDesc);
            pNew->m_fScale28 = fScaleA;
            pNew->m_fScale2c = fScaleB;
            pNew->m_bScaled = 1;
            int nClass = RemoveEvent(nEvent);
            m_aEventDescs[nEvent] = pNew;
            AddEvent(nEvent, nClass);
            return 1;
        }
        case 1:
        case 3:
        case 4:
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E0810
void CExoInputInternal::WriteEventToFile(int nValue, int nIndex, CExoFile* pFile)
{
    if (pFile == 0 && nIndex >= 0 && nIndex < m_nActiveEventLimit) {
        return;
    }
    CExoInputEventDesc* pDesc = m_aActiveEvents[nIndex].m_pDesc;
    CExoString sLine;
    nIndex = m_aActiveEvents[nIndex].m_n0;
    sLine.Format("%d %c %d %d\n", nValue, 'I', pDesc->m_nId, nIndex);
    pFile->Write(sLine);
}

// FUNCTION: SWKOTOR 0x005E0230
int CExoInputInternal::GetLastCapturedKeyboardKey()
{
    CExoKeyboardBuffer buffer;
    int nKey = 0;
    if (m_pRawInput == 0 || m_bCaptureKeys == 0) {
        return 0;
    }
    m_pRawInput->GetKeyboardBuffer(&buffer);
    for (unsigned long i = 0; i < buffer.m_nCount; i++) {
        if (buffer.m_pData[i].dwData & 0x80) {
            for (int k = g_nFirstKeyIndex; k < g_nLastKeyIndex; k++) {
                if (buffer.m_pData[i].dwOfs == m_aKeyMap[k]) {
                    nKey = k;
                    break;
                }
            }
        }
    }
    return nKey;
}

// FUNCTION: SWKOTOR 0x005E23C0
// NONMATCH: the original materialises fValue = 0.0f with fld/fst and keeps
// it in ST0 for the out-of-range return; here it is an integer store, which
// shifts the frame and the merged return blocks.
float CExoInputInternal::PollInput(int nEvent, int nClass)
{
    float fValue = 0.0f;
    if (nClass < 0 || nClass >= 6) {
        return fValue;
    }
    if (!IsEventInClass(nEvent, nClass)) {
        return 0.0f;
    }
    CExoInputEventDesc* pDesc = GetEvent(nEvent, nClass);
    switch (pDesc->m_nType) {
    case 0:
    case 2:
    case 5:
        fValue = GetControlValue(pDesc, pDesc->m_nControl, pDesc->m_nValue);
        break;
    case 1:
        return 0.0f;
    case 4:
        fValue = GetControlValue(pDesc, pDesc->m_nControl, pDesc->m_nValue24)
               - GetControlValue(pDesc, pDesc->m_nAltControl, pDesc->m_nValue34);
        break;
    case 3:
        fValue = GetControlValue(pDesc, pDesc->m_nControl, pDesc->m_nValue24 - pDesc->m_nValue);
        pDesc->m_nValue = pDesc->m_nValue24;
        break;
    }
    if (pDesc->IsAxis()) {
        float fPow;
        if (fValue < 0.0f) {
            fPow = (float)pow(fValue, pDesc->m_fScale2c);
            if (fPow > 0.0f) {
                fPow = -fPow;
            }
        } else {
            fPow = (float)pow(fValue, pDesc->m_fScale2c);
        }
        return fPow * pDesc->m_fScale28;
    }
    return fValue;
}
