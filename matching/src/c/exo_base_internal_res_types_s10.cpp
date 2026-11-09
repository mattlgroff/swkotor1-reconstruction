// CExoBaseInternal::InitResTypes (exobaseinternal.o), worker C s10: builds the
// resource type / file extension table. The Mac builds call it
// CreateResourceExtensionTable; InitResTypes is the name this project's
// matched constructor already uses.

void* __cdecl operator new(unsigned int n);         // 0x006FA7E6

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
};

class CExoBaseInternal {
public:
    int m_nResTypes;                                // 0x00
    unsigned short* m_pResTypes;                    // 0x04
    CExoString* m_pResTypeExtensions;               // 0x08

    void InitResTypes();
};

// FUNCTION: SWKOTOR 0x005E6D20
void CExoBaseInternal::InitResTypes()
{
    m_nResTypes = 88;
    m_pResTypes = new unsigned short[m_nResTypes];
    m_pResTypeExtensions = new CExoString[m_nResTypes];
    m_pResTypes[0] = 0;
    m_pResTypeExtensions[0] = "res";
    m_pResTypes[1] = 1;
    m_pResTypeExtensions[1] = "bmp";
    m_pResTypes[2] = 2;
    m_pResTypeExtensions[2] = "mve";
    m_pResTypes[3] = 3;
    m_pResTypeExtensions[3] = "tga";
    m_pResTypes[4] = 4;
    m_pResTypeExtensions[4] = "wav";
    m_pResTypes[5] = 7;
    m_pResTypeExtensions[5] = "ini";
    m_pResTypes[6] = 10;
    m_pResTypeExtensions[6] = "txt";
    m_pResTypes[7] = 2022;
    m_pResTypeExtensions[7] = "txi";
    m_pResTypes[8] = 9999;
    m_pResTypeExtensions[8] = "key";
    m_pResTypes[9] = 9998;
    m_pResTypeExtensions[9] = "bif";
    m_pResTypes[10] = 9997;
    m_pResTypeExtensions[10] = "erf";
    m_pResTypes[11] = 2000;
    m_pResTypeExtensions[11] = "plh";
    m_pResTypes[12] = 2001;
    m_pResTypeExtensions[12] = "tex";
    m_pResTypes[13] = 2002;
    m_pResTypeExtensions[13] = "mdl";
    m_pResTypes[14] = 2007;
    m_pResTypeExtensions[14] = "lua";
    m_pResTypes[15] = 2003;
    m_pResTypeExtensions[15] = "thg";
    m_pResTypes[16] = 2008;
    m_pResTypeExtensions[16] = "slt";
    m_pResTypes[17] = 2009;
    m_pResTypeExtensions[17] = "nss";
    m_pResTypes[18] = 2010;
    m_pResTypeExtensions[18] = "ncs";
    m_pResTypes[19] = 2011;
    m_pResTypeExtensions[19] = "mod";
    m_pResTypes[20] = 2012;
    m_pResTypeExtensions[20] = "are";
    m_pResTypes[21] = 2013;
    m_pResTypeExtensions[21] = "set";
    m_pResTypes[22] = 2014;
    m_pResTypeExtensions[22] = "ifo";
    m_pResTypes[23] = 2015;
    m_pResTypeExtensions[23] = "bic";
    m_pResTypes[24] = 2016;
    m_pResTypeExtensions[24] = "wok";
    m_pResTypes[25] = 6;
    m_pResTypeExtensions[25] = "plt";
    m_pResTypes[26] = 2005;
    m_pResTypeExtensions[26] = "fnt";
    m_pResTypes[27] = 2017;
    m_pResTypeExtensions[27] = "2da";
    m_pResTypes[28] = 2018;
    m_pResTypeExtensions[28] = "tlk";
    m_pResTypes[29] = 8;
    m_pResTypeExtensions[29] = "mp3";
    m_pResTypes[30] = 9;
    m_pResTypeExtensions[30] = "mpg";
    m_pResTypes[31] = 2024;
    m_pResTypeExtensions[31] = "bti";
    m_pResTypes[32] = 2025;
    m_pResTypeExtensions[32] = "uti";
    m_pResTypes[33] = 2026;
    m_pResTypeExtensions[33] = "btc";
    m_pResTypes[34] = 2027;
    m_pResTypeExtensions[34] = "utc";
    m_pResTypes[35] = 2031;
    m_pResTypeExtensions[35] = "btt";
    m_pResTypes[36] = 2032;
    m_pResTypeExtensions[36] = "utt";
    m_pResTypes[37] = 2023;
    m_pResTypeExtensions[37] = "git";
    m_pResTypes[38] = 2029;
    m_pResTypeExtensions[38] = "dlg";
    m_pResTypes[39] = 2030;
    m_pResTypeExtensions[39] = "itp";
    m_pResTypes[40] = 2033;
    m_pResTypeExtensions[40] = "dds";
    m_pResTypes[41] = 2034;
    m_pResTypeExtensions[41] = "bts";
    m_pResTypes[42] = 2035;
    m_pResTypeExtensions[42] = "uts";
    m_pResTypes[43] = 2036;
    m_pResTypeExtensions[43] = "ltr";
    m_pResTypes[44] = 2037;
    m_pResTypeExtensions[44] = "gff";
    m_pResTypes[45] = 2038;
    m_pResTypeExtensions[45] = "fac";
    m_pResTypes[46] = 2039;
    m_pResTypeExtensions[46] = "bte";
    m_pResTypes[47] = 2040;
    m_pResTypeExtensions[47] = "ute";
    m_pResTypes[48] = 2041;
    m_pResTypeExtensions[48] = "btd";
    m_pResTypes[49] = 2042;
    m_pResTypeExtensions[49] = "utd";
    m_pResTypes[50] = 2043;
    m_pResTypeExtensions[50] = "btp";
    m_pResTypes[51] = 2044;
    m_pResTypeExtensions[51] = "utp";
    m_pResTypes[52] = 2045;
    m_pResTypeExtensions[52] = "dft";
    m_pResTypes[53] = 2046;
    m_pResTypeExtensions[53] = "gic";
    m_pResTypes[54] = 2047;
    m_pResTypeExtensions[54] = "gui";
    m_pResTypes[55] = 2048;
    m_pResTypeExtensions[55] = "css";
    m_pResTypes[56] = 2049;
    m_pResTypeExtensions[56] = "ccs";
    m_pResTypes[57] = 2050;
    m_pResTypeExtensions[57] = "btm";
    m_pResTypes[58] = 2051;
    m_pResTypeExtensions[58] = "utm";
    m_pResTypes[59] = 2052;
    m_pResTypeExtensions[59] = "dwk";
    m_pResTypes[60] = 2053;
    m_pResTypeExtensions[60] = "pwk";
    m_pResTypes[61] = 2054;
    m_pResTypeExtensions[61] = "btg";
    m_pResTypes[62] = 2055;
    m_pResTypeExtensions[62] = "utg";
    m_pResTypes[63] = 2056;
    m_pResTypeExtensions[63] = "jrl";
    m_pResTypes[64] = 2057;
    m_pResTypeExtensions[64] = "sav";
    m_pResTypes[65] = 2058;
    m_pResTypeExtensions[65] = "utw";
    m_pResTypes[66] = 2059;
    m_pResTypeExtensions[66] = "4pc";
    m_pResTypes[67] = 2060;
    m_pResTypeExtensions[67] = "ssf";
    m_pResTypes[68] = 2061;
    m_pResTypeExtensions[68] = "hak";
    m_pResTypes[69] = 3000;
    m_pResTypeExtensions[69] = "lyt";
    m_pResTypes[70] = 3001;
    m_pResTypeExtensions[70] = "vis";
    m_pResTypes[71] = 3002;
    m_pResTypeExtensions[71] = "rim";
    m_pResTypes[72] = 11;
    m_pResTypeExtensions[72] = "wma";
    m_pResTypes[73] = 3003;
    m_pResTypeExtensions[73] = "pth";
    m_pResTypes[74] = 3005;
    m_pResTypeExtensions[74] = "bwm";
    m_pResTypes[75] = 12;
    m_pResTypeExtensions[75] = "wmv";
    m_pResTypes[76] = 3004;
    m_pResTypeExtensions[76] = "lip";
    m_pResTypes[77] = 2062;
    m_pResTypeExtensions[77] = "nwm";
    m_pResTypes[78] = 2063;
    m_pResTypeExtensions[78] = "bik";
    m_pResTypes[79] = 3006;
    m_pResTypeExtensions[79] = "txb";
    m_pResTypes[80] = 3007;
    m_pResTypeExtensions[80] = "tpc";
    m_pResTypes[81] = 3008;
    m_pResTypeExtensions[81] = "mdx";
    m_pResTypes[82] = 3009;
    m_pResTypeExtensions[82] = "rsv";
    m_pResTypes[83] = 3010;
    m_pResTypeExtensions[83] = "sig";
    m_pResTypes[84] = 13;
    m_pResTypeExtensions[84] = "xmv";
    m_pResTypes[85] = 3011;
    m_pResTypeExtensions[85] = "xbx";
    m_pResTypes[86] = 14;
    m_pResTypeExtensions[86] = "log";
    m_pResTypes[87] = 0xffff;
    m_pResTypeExtensions[87] = "";
}
