// the out-of-line CExoString default ctor at 0x005B3190 (own file so that
// callers such as CSWBaseItem::CSWBaseItem keep the call).

class CExoString {
public:
    CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

// FUNCTION: SWKOTOR 0x005B3190 SYMBOL: ??0CExoString@@QAE@XZ
CExoString::CExoString()
{
    m_sString = 0;
    m_nBufferLength = 0;
}
