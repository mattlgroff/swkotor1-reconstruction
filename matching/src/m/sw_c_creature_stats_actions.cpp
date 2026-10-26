// CSWCCreatureStats radial-menu force power actions (nwccreaturestats.o).

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

struct CSWGuiInterfaceAction;

class CSWCCreatureStats_ClassInfo {
public:
    // 0x0064A870; descriptive name, see the caller below.
    void AppendForceActions(CExoArrayList<CSWGuiInterfaceAction>* pActions, int nType,
                            int nArg2, int nGoodEvil, int nArg3, int nArg4, float fArg5);
    char pad_0x0[0x20];
};

class CSWCCreatureStats {
public:
    void AppendForceActions(CExoArrayList<CSWGuiInterfaceAction>* pActions, int nType,
                            int nArg2, int nArg3, int nArg4, float fArg5);

    short GetGoodEvil() { return m_nGoodEvil; }

    char pad_0x0[0x32];
    unsigned char m_nNumMultiClasses;               // 0x32
    char pad_0x33[0x80 - 0x33];
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0xd4 - 0x82];
    CSWCCreatureStats_ClassInfo m_ClassInfo[2];     // 0xd4
};

// Descriptive name (CSWCCreature::GetSelfForcePowers passes type 3).
// The unused local list only contributes its destructor.
// FUNCTION: SWKOTOR 0x0064AF10
void CSWCCreatureStats::AppendForceActions(CExoArrayList<CSWGuiInterfaceAction>* pActions,
                                           int nType, int nArg2, int nArg3, int nArg4,
                                           float fArg5)
{
    CExoArrayList<unsigned long> lstUnused;
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        m_ClassInfo[i].AppendForceActions(pActions, nType, nArg2, GetGoodEvil(), nArg3, nArg4, fArg5);
    }
}
