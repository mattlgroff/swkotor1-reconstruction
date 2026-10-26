// AurControlPart: apply a controller line to a part of a named gob through a
// conjoined copy of the part's model node (0x0044b900).
extern "C" int __cdecl sscanf(const char* s, const char* fmt, ...);

class Part;

class MdlNode {
public:
    ~MdlNode();                                                     // 0x0044a040
    MdlNode* ConJoin();                                             // 0x0044b140
    void InternalParseField(char* szLine);                          // 0x0043fa10
    void InternalControl(Part* pPart, float f1, int n, float f2);  // 0x0043f890
};

class Part {
public:
    void* m_pVtbl;                          // +0x00
    MdlNode* m_pNode;                       // +0x04
};

class Gob {
public:
    virtual void V000(); virtual void V004(); virtual void V008(); virtual void V00c();
    virtual void V010(); virtual void V014(); virtual void V018(); virtual void V01c();
    virtual void V020(); virtual void V024(); virtual void V028(); virtual void V02c();
    virtual void V030(); virtual void V034(); virtual void V038(); virtual void V03c();
    virtual void V040(); virtual void V044(); virtual void V048(); virtual void V04c();
    virtual void V050(); virtual void V054(); virtual void V058(); virtual void V05c();
    virtual void V060(); virtual void V064(); virtual void V068(); virtual void V06c();
    virtual void V070(); virtual void V074(); virtual void V078(); virtual void V07c();
    virtual void V080(); virtual void V084(); virtual void V088(); virtual void V08c();
    virtual void V090(); virtual void V094(); virtual void V098(); virtual void V09c();
    virtual void V0a0(); virtual void V0a4(); virtual void V0a8(); virtual void V0ac();
    virtual void V0b0(); virtual void V0b4(); virtual void V0b8(); virtual void V0bc();
    virtual void V0c0(); virtual void V0c4(); virtual void V0c8(); virtual void V0cc();
    virtual void V0d0(); virtual void V0d4(); virtual void V0d8(); virtual void V0dc();
    virtual void V0e0(); virtual void V0e4(); virtual void V0e8(); virtual void V0ec();
    virtual void V0f0(); virtual void V0f4(); virtual void V0f8(); virtual void V0fc();
    virtual void V100(); virtual void V104(); virtual void V108();
    virtual Part* FindPart(char* szName);   // +0x10c
};

Gob* __cdecl GobListFind(char* szName);         // 0x00440460
char* __cdecl SkipWhitespace(char* s);          // 0x00463510

// FUNCTION: SWKOTOR 0x0044b900
// Name not recorded ("<gob> <part> <controller line>").
unsigned char AurControlPart(char* szCommand)
{
    char szGob[64];
    char szPart[64];
    int nRead;
    szPart[0] = 0;
    szGob[0] = 0;
    if (sscanf(szCommand, "%s%s%n", szGob, szPart, &nRead) < 2 || !szGob[0] || !szPart[0])
        return 0;
    Gob* pGob = GobListFind(szGob);
    if (!pGob)
        return 0;
    Part* pPart = pGob->FindPart(szPart);
    if (!pPart)
        return 0;
    MdlNode* pNode = pPart->m_pNode->ConJoin();
    pNode->InternalParseField(SkipWhitespace(szCommand + nRead));
    pNode->InternalControl(pPart, 0, 0, 1.0f);
    delete pNode;
    return 1;
}
