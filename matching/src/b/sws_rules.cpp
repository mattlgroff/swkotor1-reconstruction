// CSWSRules: server rules object (vtable 0x007493E0), 0x0057A1F0-0x0057A220.
// Self-contained: CSWRules here has an out-of-line constructor (0x00552C50).

// vtable 0x00748BB4
class CSWRules {
public:
    CSWRules();                                     // 0x00552C50
    virtual ~CSWRules();                            // 0x00551440

    char pad_0x4[0x11c];
};

class CSWSRules : public CSWRules {
public:
    CSWSRules();
    virtual ~CSWSRules();
};

// FUNCTION: SWKOTOR 0x0057A1F0
CSWSRules::CSWSRules()
{
}

// FUNCTION: SWKOTOR 0x0057A210
CSWSRules::~CSWSRules()
{
}

// FUNCTION: SWKOTOR 0x0057A220 SYMBOL: ??_GCSWSRules@@UAEPAXI@Z
// (emitted with the vtable of the constructor above)
