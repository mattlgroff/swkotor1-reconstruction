// CExoInputInternal leaves, CExoDeviceBuffer, CExoRawInputInternal
// (b005e0_exo_foundation_leaves, b005e7_exo_string_array_misc).

#include "c/exo_input.h"

CExoInputInternal::CExoInputInternal()
{
}

// FUNCTION: SWKOTOR 0x005E2340 SYMBOL: ??_GCExoInputInternal@@UAEPAXI@Z
// CExoInputInternal::`scalar deleting destructor' (compiler generated).

// FUNCTION: SWKOTOR 0x005E0520
int CExoInputInternal::SetEventRepeatable(unsigned long nEvent, unsigned long nInitialDelay, unsigned long nRepeatDelay)
{
    CExoInputEventDesc* pDesc = m_aEventDescs[nEvent];
    if (pDesc == 0 || pDesc->m_nType != 1) {
        return 0;
    }
    CExoLinkedListPosition pos = m_lstRepeatEvents.GetHeadPos();
    while (pos) {
        CExoInputRepeatEvent* pRepeat = m_lstRepeatEvents.GetAtPos(pos);
        if (pRepeat && pRepeat->m_nEvent == nEvent) {
            pRepeat->m_nInitialDelay = nInitialDelay;
            pRepeat->m_nRepeatDelay = nRepeatDelay;
            return 1;
        }
        m_lstRepeatEvents.GetNext(pos);
    }
    CExoInputRepeatEvent* pRepeat = new CExoInputRepeatEvent;
    if (pRepeat == 0) {
        return 0;
    }
    pRepeat->m_nEvent = nEvent;
    pRepeat->m_nInitialDelay = nInitialDelay;
    pRepeat->m_nRepeatDelay = nRepeatDelay;
    pRepeat->m_nUnknown0c = 0;
    pRepeat->m_nUnknown18 = 0;
    pRepeat->m_nUnknown14 = 0;
    pRepeat->m_nUnknown10 = 0;
    m_lstRepeatEvents.AddHead(pRepeat);
    return 1;
}

// FUNCTION: SWKOTOR 0x005E1740
int CExoInputInternal::SetMouseMode(unsigned char nMode)
{
    if (nMode == m_nMouseMode) {
        return 1;
    }
    m_nMouseMode = nMode;
    if (nMode == 1) {
        while (ShowCursor(0) >= 0) {
        }
    }
    int nCursor = m_nCursor;
    m_nCursor = -1;
    if (nCursor == -1) {
        nCursor = 1;
    }
    m_nUnknown3ac = 0;
    if (nCursor != -1) {
        m_nCursor = nCursor;
        if (!m_nMouseMode) {
            HCURSOR hCursor = LoadCursorA(g_hInstance, (const char*)(unsigned short)nCursor);
            if (hCursor) {
                HCURSOR hOld = ::SetCursor(hCursor);
                if (hOld != hCursor) {
                    DestroyCursor(hOld);
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005E17E0 SYMBOL: ??0?$CExoArrayList@UCExoInputEvent@@@@QAE@ABV0@@Z
template <class T>
CExoArrayList<T>::CExoArrayList(const CExoArrayList<T>& o)
{
    num = 0;
    array_size = 0;
    element = 0;
    for (int i = 0; i < o.num; i++) {
        Add(o.element[i]);
    }
}

template class CExoArrayList<CExoInputEvent>;

class CExoDeviceBuffer {
public:
    void* m_pData;                                  // 0x00

    ~CExoDeviceBuffer();
};

// FUNCTION: SWKOTOR 0x005E2FF0
CExoDeviceBuffer::~CExoDeviceBuffer()
{
    if (m_pData) {
        delete m_pData;
        m_pData = 0;
    }
}

// FUNCTION: SWKOTOR 0x005E3F90
// Listed as a ShutDownDirectInput thunk; ~CExoInputInternal calls it on the
// +0x140 object before freeing it, so it is the destructor.
CExoRawInputInternal::~CExoRawInputInternal()
{
    ShutDownDirectInput();
}
