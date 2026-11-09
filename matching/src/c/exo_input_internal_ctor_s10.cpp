// CExoInputInternal constructor (exoinputinternal.o, Mac name
// CExoInputInternal::CExoInputInternal(HINSTANCE__*, HWND__*)), worker C s10.
// Self-contained: the member containers need constructors and destructors
// that exo_input.h's users must not see. The key-map index constants have no
// project names; they are named after their addresses.

void* __cdecl operator new(unsigned int n);         // 0x006FA7E6
void __cdecl operator delete(void* p);              // 0x006FA390

typedef void* HINSTANCE;
typedef void* HWND;

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
};

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();                      // 0x005E9AB0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedList();                               // 0x005E0350
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);   // 0x0051B720 (unsigned long)
    T& operator[](int i) { return element[i]; }
    void Allocate(int n);                           // 0x005E03C0
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
};

class CExoCriticalSection {
public:
    void* m_pcExoCriticalSectionInternal;           // 0x00

    CExoCriticalSection();                          // 0x005E9290
    ~CExoCriticalSection();                         // 0x005E9320
};

class CExoInputClass {
public:
    CExoInputClass();                               // 0x005E0A80
    ~CExoInputClass();                              // 0x005E0AF0

    CExoString m_sName;                             // 0x00
    char pad_0x8[0x1c];
    CExoArrayList<unsigned long> m_aBits;           // 0x24
};

class CExoInputEventDesc;
struct CExoInputRepeatEvent;
struct CExoInputActiveEvent;
struct CExoInputBufferedEvent;

class CExoRawInputInternal {
public:
    CExoRawInputInternal(HINSTANCE hInstance, HWND hWnd);   // 0x005E3E80
    ~CExoRawInputInternal();                        // 0x005E3F90

    int m_bInitialized;                             // 0x00
    char pad_0x4[0x18 - 0x4];
    int m_nJoysticks;                               // 0x18
    char pad_0x1c[0x34 - 0x1c];
};

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    void* m_p0;                                     // 0x00
    CExoTimers* m_pcExoTimers;                      // 0x04
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

extern const unsigned long g_nKeyMapIndex_74D3E0;                   // 0x0074D3E0 (0)
extern const unsigned long g_nKeyMapIndex_74D3E4;                   // 0x0074D3E4 (1)
extern const unsigned long g_nKeyMapIndex_74D3E8;                   // 0x0074D3E8 (2)
extern const unsigned long g_nInputMouseAxisX;                      // 0x0074D3EC (3)
extern const unsigned long g_nInputMouseAxisY;                      // 0x0074D3F0 (4)
extern const unsigned long g_nKeyMapIndex_74D3F4;                   // 0x0074D3F4 (5)
extern const unsigned long g_nKeyMapIndex_74D3F8;                   // 0x0074D3F8 (6)
extern const unsigned long g_nKeyMapIndex_74D3FC;                   // 0x0074D3FC (7)
extern const unsigned long g_nKeyMapIndex_74D400;                   // 0x0074D400 (8)
extern const unsigned long g_nKeyMapIndex_74D404;                   // 0x0074D404 (9)
extern const unsigned long g_nKeyMapIndex_74D408;                   // 0x0074D408 (10)
extern const unsigned long g_nKeyMapIndex_74D40C;                   // 0x0074D40C (11)
extern const unsigned long g_nKeyMapIndex_74D410;                   // 0x0074D410 (12)
extern const unsigned long g_nKeyMapIndex_74D414;                   // 0x0074D414 (13)
extern const unsigned long g_nKeyMapIndex_74D418;                   // 0x0074D418 (14)
extern const unsigned long g_nKeyMapIndex_74D41C;                   // 0x0074D41C (15)
extern const unsigned long g_nKeyMapIndex_74D420;                   // 0x0074D420 (16)
extern const unsigned long g_nKeyMapIndex_74D424;                   // 0x0074D424 (17)
extern const unsigned long g_nKeyMapIndex_74D428;                   // 0x0074D428 (18)
extern const unsigned long g_nKeyMapIndex_74D42C;                   // 0x0074D42C (19)
extern const unsigned long g_nKeyMapIndex_74D430;                   // 0x0074D430 (20)
extern const unsigned long g_nKeyMapIndex_74D434;                   // 0x0074D434 (21)
extern const unsigned long g_nKeyMapIndex_74D438;                   // 0x0074D438 (22)
extern const unsigned long g_nKeyMapIndex_74D43C;                   // 0x0074D43C (23)
extern const unsigned long g_nKeyMapIndex_74D440;                   // 0x0074D440 (24)
extern const unsigned long g_nKeyMapIndex_74D444;                   // 0x0074D444 (25)
extern const unsigned long g_nKeyMapIndex_74D448;                   // 0x0074D448 (26)
extern const unsigned long g_nKeyMapIndex_74D44C;                   // 0x0074D44C (27)
extern const unsigned long g_nKeyMapIndex_74D450;                   // 0x0074D450 (28)
extern const unsigned long g_nKeyMapIndex_74D454;                   // 0x0074D454 (29)
extern const unsigned long g_nKeyMapIndex_74D458;                   // 0x0074D458 (30)
extern const unsigned long g_nKeyMapEscapeKey;                      // 0x0074D45C (31)
extern const unsigned long g_nKeyMapIndex_74D460;                   // 0x0074D460 (32)
extern const unsigned long g_nKeyMapIndex_74D464;                   // 0x0074D464 (33)
extern const unsigned long g_nKeyMapIndex_74D468;                   // 0x0074D468 (34)
extern const unsigned long g_nKeyMapIndex_74D46C;                   // 0x0074D46C (35)
extern const unsigned long g_nKeyMapIndex_74D470;                   // 0x0074D470 (36)
extern const unsigned long g_nKeyMapIndex_74D474;                   // 0x0074D474 (37)
extern const unsigned long g_nKeyMapIndex_74D478;                   // 0x0074D478 (38)
extern const unsigned long g_nKeyMapIndex_74D47C;                   // 0x0074D47C (39)
extern const unsigned long g_nKeyMapIndex_74D480;                   // 0x0074D480 (40)
extern const unsigned long g_nKeyMapIndex_74D484;                   // 0x0074D484 (41)
extern const unsigned long g_nKeyMapIndex_74D488;                   // 0x0074D488 (42)
extern const unsigned long g_nKeyMapIndex_74D48C;                   // 0x0074D48C (43)
extern const unsigned long g_nKeyMapIndex_74D490;                   // 0x0074D490 (44)
extern const unsigned long g_nKeyMapIndex_74D494;                   // 0x0074D494 (45)
extern const unsigned long g_nKeyMapIndex_74D498;                   // 0x0074D498 (46)
extern const unsigned long g_nKeyMapIndex_74D49C;                   // 0x0074D49C (47)
extern const unsigned long g_nKeyMapIndex_74D4A0;                   // 0x0074D4A0 (48)
extern const unsigned long g_nKeyMapIndex_74D4A4;                   // 0x0074D4A4 (49)
extern const unsigned long g_nKeyMapIndex_74D4A8;                   // 0x0074D4A8 (50)
extern const unsigned long g_nKeyMapIndex_74D4AC;                   // 0x0074D4AC (51)
extern const unsigned long g_nKeyMapIndex_74D4B0;                   // 0x0074D4B0 (52)
extern const unsigned long g_nKeyMapIndex_74D4B4;                   // 0x0074D4B4 (53)
extern const unsigned long g_nKeyMapIndex_74D4B8;                   // 0x0074D4B8 (54)
extern const unsigned long g_nKeyMapIndex_74D4BC;                   // 0x0074D4BC (55)
extern const unsigned long g_nKeyMapIndex_74D4C0;                   // 0x0074D4C0 (56)
extern const unsigned long g_nKeyMapIndex_74D4C4;                   // 0x0074D4C4 (57)
extern const unsigned long g_nKeyMapIndex_74D4C8;                   // 0x0074D4C8 (58)
extern const unsigned long g_nKeyMapIndex_74D4CC;                   // 0x0074D4CC (59)
extern const unsigned long g_nKeyMapIndex_74D4D0;                   // 0x0074D4D0 (60)
extern const unsigned long g_nKeyMapIndex_74D4D4;                   // 0x0074D4D4 (61)
extern const unsigned long g_nKeyMapIndex_74D4D8;                   // 0x0074D4D8 (62)
extern const unsigned long g_nKeyMapIndex_74D4DC;                   // 0x0074D4DC (63)
extern const unsigned long g_nKeyMapIndex_74D4E0;                   // 0x0074D4E0 (64)
extern const unsigned long g_nKeyMapIndex_74D4E4;                   // 0x0074D4E4 (65)
extern const unsigned long g_nKeyMapIndex_74D4E8;                   // 0x0074D4E8 (66)
extern const unsigned long g_nKeyMapIndex_74D4EC;                   // 0x0074D4EC (67)
extern const unsigned long g_nKeyMapIndex_74D4F0;                   // 0x0074D4F0 (68)
extern const unsigned long g_nKeyMapIndex_74D4F4;                   // 0x0074D4F4 (69)
extern const unsigned long g_nKeyMapIndex_74D4F8;                   // 0x0074D4F8 (70)
extern const unsigned long g_nKeyMapIndex_74D4FC;                   // 0x0074D4FC (71)
extern const unsigned long g_nKeyMapIndex_74D500;                   // 0x0074D500 (72)
extern const unsigned long g_nKeyMapIndex_74D504;                   // 0x0074D504 (73)
extern const unsigned long g_nKeyMapIndex_74D508;                   // 0x0074D508 (74)
extern const unsigned long g_nKeyMapIndex_74D50C;                   // 0x0074D50C (75)
extern const unsigned long g_nKeyMapIndex_74D510;                   // 0x0074D510 (76)
extern const unsigned long g_nKeyMapIndex_74D514;                   // 0x0074D514 (77)
extern const unsigned long g_nKeyMapIndex_74D518;                   // 0x0074D518 (78)
extern const unsigned long g_nKeyMapIndex_74D51C;                   // 0x0074D51C (79)
extern const unsigned long g_nKeyMapIndex_74D520;                   // 0x0074D520 (80)
extern const unsigned long g_nKeyMapIndex_74D524;                   // 0x0074D524 (81)
extern const unsigned long g_nKeyMapIndex_74D528;                   // 0x0074D528 (82)
extern const unsigned long g_nKeyMapIndex_74D52C;                   // 0x0074D52C (83)
extern const unsigned long g_nKeyMapIndex_74D530;                   // 0x0074D530 (84)
extern const unsigned long g_nKeyMapIndex_74D534;                   // 0x0074D534 (85)
extern const unsigned long g_nKeyMapIndex_74D538;                   // 0x0074D538 (86)
extern const unsigned long g_nKeyMapIndex_74D53C;                   // 0x0074D53C (87)
extern const unsigned long g_nKeyMapIndex_74D540;                   // 0x0074D540 (88)
extern const unsigned long g_nKeyMapIndex_74D544;                   // 0x0074D544 (89)
extern const unsigned long g_nKeyMapIndex_74D548;                   // 0x0074D548 (90)
extern const unsigned long g_nKeyMapIndex_74D54C;                   // 0x0074D54C (91)
extern const unsigned long g_nKeyMapIndex_74D550;                   // 0x0074D550 (92)
extern const unsigned long g_nKeyMapIndex_74D554;                   // 0x0074D554 (93)
extern const unsigned long g_nKeyMapIndex_74D558;                   // 0x0074D558 (94)
extern const unsigned long g_nKeyMapIndex_74D55C;                   // 0x0074D55C (95)
extern const unsigned long g_nKeyMapIndex_74D560;                   // 0x0074D560 (96)
extern const unsigned long g_nKeyMapIndex_74D564;                   // 0x0074D564 (97)
extern const unsigned long g_nKeyMapIndex_74D568;                   // 0x0074D568 (98)
extern const unsigned long g_nKeyMapIndex_74D56C;                   // 0x0074D56C (99)
extern const unsigned long g_nKeyMapIndex_74D570;                   // 0x0074D570 (100)
extern const unsigned long g_nKeyMapIndex_74D574;                   // 0x0074D574 (101)
extern const unsigned long g_nKeyMapIndex_74D578;                   // 0x0074D578 (102)
extern const unsigned long g_nKeyMapIndex_74D57C;                   // 0x0074D57C (103)
extern const unsigned long g_nKeyMapIndex_74D580;                   // 0x0074D580 (104)
extern const unsigned long g_nKeyMapIndex_74D584;                   // 0x0074D584 (105)
extern const unsigned long g_nKeyMapIndex_74D588;                   // 0x0074D588 (106)
extern const unsigned long g_nKeyMapIndex_74D58C;                   // 0x0074D58C (107)
extern const unsigned long g_nKeyMapIndex_74D590;                   // 0x0074D590 (108)
extern const unsigned long g_nKeyMapIndex_74D594;                   // 0x0074D594 (110)
extern const unsigned long g_nKeyMapIndex_74D598;                   // 0x0074D598 (111)
extern const unsigned long g_nKeyMapIndex_74D59C;                   // 0x0074D59C (112)
extern const unsigned long g_nKeyMapIndex_74D5A0;                   // 0x0074D5A0 (113)
extern const unsigned long g_nKeyMapIndex_74D5A4;                   // 0x0074D5A4 (114)
extern const unsigned long g_nKeyMapIndex_74D5A8;                   // 0x0074D5A8 (115)
extern const unsigned long g_nKeyMapIndex_74D5AC;                   // 0x0074D5AC (116)
extern const unsigned long g_nKeyMapIndex_74D5B0;                   // 0x0074D5B0 (117)
extern const unsigned long g_nKeyMapIndex_74D5B4;                   // 0x0074D5B4 (118)
extern const unsigned long g_nKeyMapIndex_74D5B8;                   // 0x0074D5B8 (119)
extern const unsigned long g_nKeyMapIndex_74D5BC;                   // 0x0074D5BC (120)
extern const unsigned long g_nKeyMapIndex_74D5C0;                   // 0x0074D5C0 (121)
extern const unsigned long g_nKeyMapIndex_74D5C4;                   // 0x0074D5C4 (122)
extern const unsigned long g_nKeyMapIndex_74D5C8;                   // 0x0074D5C8 (123)
extern const unsigned long g_nKeyMapIndex_74D5CC;                   // 0x0074D5CC (124)
extern const unsigned long g_nKeyMapIndex_74D5D0;                   // 0x0074D5D0 (125)
extern const unsigned long g_nKeyMapIndex_74D5D4;                   // 0x0074D5D4 (126)
extern const unsigned long g_nKeyMapIndex_74D5D8;                   // 0x0074D5D8 (127)
extern const unsigned long g_nKeyMapIndex_74D5DC;                   // 0x0074D5DC (128)
extern const unsigned long g_nKeyMapIndex_74D5E0;                   // 0x0074D5E0 (129)
extern const unsigned long g_nKeyMapIndex_74D5E4;                   // 0x0074D5E4 (130)

class CExoInputInternal {
public:
    CExoInputInternal(HINSTANCE hInstance, HWND hWnd);
    virtual ~CExoInputInternal();                   // 0x005E0B80

    int m_nUnknown04;                               // 0x04
    CExoInputClass m_aClasses[6];                   // 0x08
    CExoArrayList<CExoInputEventDesc*> m_aEventDescs;   // 0x128
    CExoLinkedList<CExoInputEventDesc*> m_lstCoolDown;  // 0x134
    CExoLinkedList<CExoInputRepeatEvent> m_lstRepeatEvents; // 0x138
    unsigned long m_nStartTime;                     // 0x13c (descriptive)
    CExoRawInputInternal* m_pRawInput;              // 0x140
    CExoArrayList<CExoInputActiveEvent> m_aActiveEvents;  // 0x144
    int m_nActiveEventLimit;                        // 0x150
    int m_nUnknown154;                              // 0x154
    int m_nSlots;                                   // 0x158
    CExoCriticalSection m_cCriticalSection;         // 0x15c
    int m_nUnknown160;                              // 0x160
    unsigned long m_aKeyMap[0x84];                  // 0x164
    CExoLinkedList<CExoInputBufferedEvent> m_lstBufferedEvents;   // 0x374
    unsigned char m_nMouseMode;                     // 0x378
    char pad_0x379[0x37c - 0x379];
    int m_nMouseX;                                  // 0x37c
    int m_nMouseY;                                  // 0x380
    int m_bMousePosSet;                             // 0x384
    int m_nUnknown388;                              // 0x388
    int m_nUnknown38c;                              // 0x38c
    int m_nUnknown390;                              // 0x390
    int m_nUnknown394;                              // 0x394
    int m_nCursor;                                  // 0x398
    int m_nCursorOption;                            // 0x39c
    float m_fMouseDeltaX;                           // 0x3a0
    float m_fMouseDeltaY;                           // 0x3a4
    int m_bCaptureKeys;                             // 0x3a8
    int m_nUnknown3ac;                              // 0x3ac
};

// NONMATCH (the first 0x98d bytes, including the whole key map, are identical):
// in the class loop the original builds the empty list once before the loop
// and frees a known-null pointer after each assignment; ours rebuilds the
// temporary and reloads its element pointer every iteration.
// FUNCTION: SWKOTOR 0x005E1880
CExoInputInternal::CExoInputInternal(HINSTANCE hInstance, HWND hWnd)
{
    m_nUnknown154 = 0;
    m_pRawInput = 0;
    m_nActiveEventLimit = 0;
    m_nUnknown04 = 0;
    m_bCaptureKeys = 0;
    m_nUnknown388 = 0;
    m_nUnknown38c = 0;
    m_nUnknown390 = 0;
    m_nUnknown394 = 0;
    m_aKeyMap[g_nKeyMapIndex_74D5AC] = 0x30;
    m_aKeyMap[g_nKeyMapIndex_74D5B0] = 0x31;
    m_aKeyMap[g_nKeyMapIndex_74D5B4] = 0x32;
    m_aKeyMap[g_nKeyMapIndex_74D5B8] = 0x33;
    m_aKeyMap[g_nKeyMapIndex_74D5BC] = 0x34;
    m_aKeyMap[g_nKeyMapIndex_74D5C0] = 0x35;
    m_aKeyMap[g_nKeyMapIndex_74D5C4] = 0x36;
    m_aKeyMap[g_nKeyMapIndex_74D5C8] = 0x37;
    m_aKeyMap[g_nKeyMapIndex_74D5CC] = 0x38;
    m_aKeyMap[g_nKeyMapIndex_74D5D0] = 0x39;
    m_aKeyMap[g_nKeyMapIndex_74D5D4] = 0x3a;
    m_aKeyMap[g_nKeyMapIndex_74D5D8] = 0x384;
    m_aKeyMap[g_nKeyMapIndex_74D5DC] = 0x38c;
    m_aKeyMap[g_nKeyMapIndex_74D5E0] = 0x388;
    m_aKeyMap[g_nKeyMapIndex_74D5E4] = 0x390;
    m_aKeyMap[g_nKeyMapIndex_74D594] = 0x0;
    m_aKeyMap[g_nKeyMapIndex_74D598] = 0x4;
    m_aKeyMap[g_nKeyMapIndex_74D59C] = 0x20;
    m_aKeyMap[g_nKeyMapIndex_74D5A0] = 0x18;
    m_aKeyMap[g_nKeyMapIndex_74D5A4] = 0x1c;
    m_aKeyMap[g_nKeyMapIndex_74D5A8] = 0x20;
    m_aKeyMap[g_nKeyMapIndex_74D3E0] = 0xc;
    m_aKeyMap[g_nKeyMapIndex_74D3E4] = 0xd;
    m_aKeyMap[g_nKeyMapIndex_74D3E8] = 0xe;
    m_aKeyMap[g_nInputMouseAxisX] = 0x0;
    m_aKeyMap[g_nInputMouseAxisY] = 0x4;
    m_aKeyMap[g_nKeyMapIndex_74D3F4] = 0x8;
    m_aKeyMap[g_nKeyMapIndex_74D3F8] = 0x1c;
    m_aKeyMap[g_nKeyMapIndex_74D53C] = 0x39;
    m_aKeyMap[g_nKeyMapIndex_74D3FC] = 0xcb;
    m_aKeyMap[g_nKeyMapIndex_74D400] = 0xcd;
    m_aKeyMap[g_nKeyMapIndex_74D404] = 0xc8;
    m_aKeyMap[g_nKeyMapIndex_74D408] = 0xd0;
    m_aKeyMap[g_nKeyMapIndex_74D40C] = 0x4f;
    m_aKeyMap[g_nKeyMapIndex_74D410] = 0x50;
    m_aKeyMap[g_nKeyMapIndex_74D414] = 0x51;
    m_aKeyMap[g_nKeyMapIndex_74D418] = 0x4b;
    m_aKeyMap[g_nKeyMapIndex_74D41C] = 0x4c;
    m_aKeyMap[g_nKeyMapIndex_74D420] = 0x4d;
    m_aKeyMap[g_nKeyMapIndex_74D424] = 0x47;
    m_aKeyMap[g_nKeyMapIndex_74D428] = 0x48;
    m_aKeyMap[g_nKeyMapIndex_74D42C] = 0x49;
    m_aKeyMap[g_nKeyMapIndex_74D430] = 0x52;
    m_aKeyMap[g_nKeyMapIndex_74D438] = 0x4a;
    m_aKeyMap[g_nKeyMapIndex_74D43C] = 0x4e;
    m_aKeyMap[g_nKeyMapIndex_74D434] = 0x53;
    m_aKeyMap[g_nKeyMapIndex_74D440] = 0x2a;
    m_aKeyMap[g_nKeyMapIndex_74D444] = 0x36;
    m_aKeyMap[g_nKeyMapIndex_74D448] = 0x38;
    m_aKeyMap[g_nKeyMapIndex_74D44C] = 0xb8;
    m_aKeyMap[g_nKeyMapIndex_74D450] = 0x1d;
    m_aKeyMap[g_nKeyMapIndex_74D454] = 0x9d;
    m_aKeyMap[g_nKeyMapIndex_74D458] = 0xf;
    m_aKeyMap[g_nKeyMapEscapeKey] = 0x1;
    m_aKeyMap[g_nKeyMapIndex_74D460] = 0xc7;
    m_aKeyMap[g_nKeyMapIndex_74D464] = 0xcf;
    m_aKeyMap[g_nKeyMapIndex_74D468] = 0xc9;
    m_aKeyMap[g_nKeyMapIndex_74D46C] = 0xd1;
    m_aKeyMap[g_nKeyMapIndex_74D470] = 0xd2;
    m_aKeyMap[g_nKeyMapIndex_74D474] = 0xd3;
    m_aKeyMap[g_nKeyMapIndex_74D478] = 0xb7;
    m_aKeyMap[g_nKeyMapIndex_74D47C] = 0x3b;
    m_aKeyMap[g_nKeyMapIndex_74D480] = 0x3c;
    m_aKeyMap[g_nKeyMapIndex_74D484] = 0x3d;
    m_aKeyMap[g_nKeyMapIndex_74D488] = 0x3e;
    m_aKeyMap[g_nKeyMapIndex_74D48C] = 0x3f;
    m_aKeyMap[g_nKeyMapIndex_74D490] = 0x40;
    m_aKeyMap[g_nKeyMapIndex_74D494] = 0x41;
    m_aKeyMap[g_nKeyMapIndex_74D498] = 0x42;
    m_aKeyMap[g_nKeyMapIndex_74D49C] = 0x43;
    m_aKeyMap[g_nKeyMapIndex_74D4A0] = 0x44;
    m_aKeyMap[g_nKeyMapIndex_74D4A4] = 0x57;
    m_aKeyMap[g_nKeyMapIndex_74D4A8] = 0x58;
    m_aKeyMap[g_nKeyMapIndex_74D54C] = 0x64;
    m_aKeyMap[g_nKeyMapIndex_74D550] = 0x65;
    m_aKeyMap[g_nKeyMapIndex_74D554] = 0x66;
    m_aKeyMap[g_nKeyMapIndex_74D4AC] = 0x1e;
    m_aKeyMap[g_nKeyMapIndex_74D4B0] = 0x30;
    m_aKeyMap[g_nKeyMapIndex_74D4B4] = 0x2e;
    m_aKeyMap[g_nKeyMapIndex_74D4B8] = 0x20;
    m_aKeyMap[g_nKeyMapIndex_74D4BC] = 0x12;
    m_aKeyMap[g_nKeyMapIndex_74D4C0] = 0x21;
    m_aKeyMap[g_nKeyMapIndex_74D4C4] = 0x22;
    m_aKeyMap[g_nKeyMapIndex_74D4C8] = 0x23;
    m_aKeyMap[g_nKeyMapIndex_74D4CC] = 0x17;
    m_aKeyMap[g_nKeyMapIndex_74D4D0] = 0x24;
    m_aKeyMap[g_nKeyMapIndex_74D4D4] = 0x25;
    m_aKeyMap[g_nKeyMapIndex_74D4D8] = 0x26;
    m_aKeyMap[g_nKeyMapIndex_74D4DC] = 0x32;
    m_aKeyMap[g_nKeyMapIndex_74D4E0] = 0x31;
    m_aKeyMap[g_nKeyMapIndex_74D4E4] = 0x18;
    m_aKeyMap[g_nKeyMapIndex_74D4E8] = 0x19;
    m_aKeyMap[g_nKeyMapIndex_74D4EC] = 0x10;
    m_aKeyMap[g_nKeyMapIndex_74D4F0] = 0x13;
    m_aKeyMap[g_nKeyMapIndex_74D4F4] = 0x1f;
    m_aKeyMap[g_nKeyMapIndex_74D4F8] = 0x14;
    m_aKeyMap[g_nKeyMapIndex_74D4FC] = 0x16;
    m_aKeyMap[g_nKeyMapIndex_74D500] = 0x2f;
    m_aKeyMap[g_nKeyMapIndex_74D504] = 0x11;
    m_aKeyMap[g_nKeyMapIndex_74D508] = 0x2d;
    m_aKeyMap[g_nKeyMapIndex_74D50C] = 0x15;
    m_aKeyMap[g_nKeyMapIndex_74D510] = 0x2c;
    m_aKeyMap[g_nKeyMapIndex_74D514] = 0x2;
    m_aKeyMap[g_nKeyMapIndex_74D518] = 0x3;
    m_aKeyMap[g_nKeyMapIndex_74D51C] = 0x4;
    m_aKeyMap[g_nKeyMapIndex_74D520] = 0x5;
    m_aKeyMap[g_nKeyMapIndex_74D524] = 0x6;
    m_aKeyMap[g_nKeyMapIndex_74D528] = 0x7;
    m_aKeyMap[g_nKeyMapIndex_74D52C] = 0x8;
    m_aKeyMap[g_nKeyMapIndex_74D530] = 0x9;
    m_aKeyMap[g_nKeyMapIndex_74D534] = 0xa;
    m_aKeyMap[g_nKeyMapIndex_74D538] = 0xb;
    m_aKeyMap[g_nKeyMapIndex_74D558] = 0xc;
    m_aKeyMap[g_nKeyMapIndex_74D55C] = 0xd;
    m_aKeyMap[g_nKeyMapIndex_74D560] = 0xe;
    m_aKeyMap[g_nKeyMapIndex_74D564] = 0x1a;
    m_aKeyMap[g_nKeyMapIndex_74D568] = 0x1b;
    m_aKeyMap[g_nKeyMapIndex_74D56C] = 0x27;
    m_aKeyMap[g_nKeyMapIndex_74D570] = 0x28;
    m_aKeyMap[g_nKeyMapIndex_74D574] = 0x29;
    m_aKeyMap[g_nKeyMapIndex_74D578] = 0x2b;
    m_aKeyMap[g_nKeyMapIndex_74D57C] = 0x33;
    m_aKeyMap[g_nKeyMapIndex_74D580] = 0x34;
    m_aKeyMap[g_nKeyMapIndex_74D584] = 0x35;
    m_aKeyMap[g_nKeyMapIndex_74D588] = 0x37;
    m_aKeyMap[g_nKeyMapIndex_74D58C] = 0xb3;
    m_aKeyMap[g_nKeyMapIndex_74D590] = 0xb5;
    m_aKeyMap[g_nKeyMapIndex_74D540] = 0x9c;
    m_aKeyMap[g_nKeyMapIndex_74D544] = 0x3a;
    m_aKeyMap[g_nKeyMapIndex_74D548] = 0xc5;
    m_pRawInput = new CExoRawInputInternal(hInstance, hWnd);
    if (m_pRawInput) {
        if (!m_pRawInput->m_bInitialized) {
            delete m_pRawInput;
            m_pRawInput = 0;
        } else {
            m_nSlots = m_pRawInput->m_nJoysticks + 2;
            if (m_pRawInput->m_nJoysticks > 4) {
                m_nSlots = 6;
            }
            m_aEventDescs.SetSize(1);
            m_aEventDescs[0] = 0;
            for (int i = 0; i < 6; i++) {
                m_aClasses[i].m_sName = "Unnamed Input Class";
                m_aClasses[i].m_aBits.SetSize(1);
                m_aClasses[i].m_aBits = CExoArrayList<unsigned long>();
            }
            m_nStartTime = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
            m_nUnknown160 = 0;
            m_nMouseMode = 0;
            m_nMouseX = -1;
            m_nMouseY = -1;
            m_bMousePosSet = 0;
            m_nCursor = -1;
            m_nCursorOption = 0;
            m_nUnknown3ac = 0;
        }
    }
}
