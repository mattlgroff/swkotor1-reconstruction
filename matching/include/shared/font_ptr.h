// font_ptr.h -- FontPtr
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/h/guivirt.cpp); the other copies' members fall on its members.
#ifndef SHARED_FONT_PTR_H
#define SHARED_FONT_PTR_H


class CAurFont;
class SafePointerBase;

class FontPtr {
public:
    CAurFont* m_p;

    FontPtr() { m_p = 0; }
    ~FontPtr()
    {
        if (m_p)
            m_p->RemoveNoThrow((SafePointerBase*)this);
    }
    void Reset()
    {
        if (m_p)
            m_p->Remove((SafePointerBase*)this);
        m_p = 0;
    }
    FontPtr& operator=(CAurFont* p);            // 0x00447FA0
};

#endif
