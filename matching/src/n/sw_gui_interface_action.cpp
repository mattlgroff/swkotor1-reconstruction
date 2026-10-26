// CSWGuiInterfaceAction::Reset, emitted in nwcobject.o's range (0x0063C000).
// Layout follows include/a/sws_creature.h; Reset is a placeholder name.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString& operator=(const char* s);           // 0x005E5140
};

class CResRef {
public:
    char m_resRef[16];
    CResRef operator=(const char* s);               // 0x00406290
};

extern const char g_sEmpty[];                       // 0x0073D71C

struct CSWGuiActionData {
    unsigned long m_n0, m_n4, m_n8, m_nC;
    CSWGuiActionData() { m_n0 = 0; m_n4 = 0; m_n8 = 0; m_nC = 0xffffffff; }
};

class CSWGuiInterfaceAction {
public:
    void Reset();

    CExoString m_sName;                             // 0x00
    unsigned long m_n08;                            // 0x08 (placeholder name)
    CSWGuiActionData m_data;                        // 0x0c (placeholder name)
    unsigned long m_n1C;                            // 0x1c (placeholder name)
    CResRef m_resIcon;                              // 0x20
    unsigned long m_b30 : 1;                        // 0x30 bit 0 (placeholder name)
    unsigned long m_n30 : 4;                        // 0x30 bits 1-4 (placeholder name)
    unsigned long m_n34;                            // 0x34 (placeholder name)
};

// FUNCTION: SWKOTOR 0x0063C000
void CSWGuiInterfaceAction::Reset()
{
    m_sName = g_sEmpty;
    m_n08 = 0xffffffff;
    m_data = CSWGuiActionData();
    m_n1C = 0x7f000000;
    m_resIcon = g_sEmpty;
    m_b30 = 1;
    m_n30 = 0;
    m_n34 = 1;
}
