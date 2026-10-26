// Out-of-line CExoLinkedList constructor instantiated in nwsarea.o.
// The element type is not proven; CSWSAreaListEntry is a placeholder name.
#include "a/sws_area.h"

struct CSWSAreaListEntry;

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x00506D90 SYMBOL: ??0?$CExoLinkedList@UCSWSAreaListEntry@@@@QAE@XZ
template <> CExoLinkedList<CSWSAreaListEntry>::CExoLinkedList()
{
    m_pList = new CExoLinkedListInternal;
}
#pragma auto_inline(on)
