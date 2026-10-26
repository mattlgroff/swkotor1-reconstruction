// Worker C matching headers: shared Exo foundation types.
#ifndef C_EXO_BASE_H
#define C_EXO_BASE_H

class CResRef;

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString(int n);                              // 0x005E5BC0
    CExoString(const char* s);                      // 0x005E5A90
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10

    void Insert(const CExoString& s, int nPosition);    // 0x005E57B0
    int CompareNoCase(const char* s) const;         // 0x005E58C0
    int ComparePrefixNoCase(const CExoString& s, int nSize) const;   // 0x005E5900
    int StripNonAlphaNumeric(int bLower, int bAllowEmail, int bAllowPunctuation);   // 0x005E5950
    int CompareNoCase(const CExoString& s) const;   // 0x005E6450
    int AsINT() const;                              // 0x005E5630
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    unsigned char IsNotEmpty(int b) const;          // 0x005E5600 (= operator[])

    // r8 (matching/src/c/exo_string_r8.cpp)
    CExoString(const char* s, int nLength);         // 0x005E5B70
    int operator==(const CExoString& s) const;      // 0x005E5200
    int operator==(const char* s) const;            // 0x005E5290
    int operator!=(const CExoString& s) const;      // 0x005E5310
    int operator!=(const char* s) const;            // 0x005E5390
    int operator<(const CExoString& s) const;       // 0x005E5410
    int operator>(const CExoString& s) const;       // 0x005E5490
    int operator<=(const CExoString& s) const;      // 0x005E5500
    int operator>=(const CExoString& s) const;      // 0x005E5580
    char operator[](int n) const;                   // 0x005E5600
    float AsFLOAT() const;                          // 0x005E5650
    void Format(const char* format, ...);           // 0x005E5680
    int IsEmpty() const;                            // 0x005E58A0
    int Find(const CExoString& s, int nStart) const;    // 0x005E5EB0
    int Find(char c, int nStart) const;             // 0x005E5F20
    CExoString Left(int nCount) const;              // 0x005E5F70
    CExoString LowerCase() const;                   // 0x005E6080
    CExoString Right(int nCount) const;             // 0x005E6170
    CExoString SubString(int nStart, int nCount) const; // 0x005E6270
    CExoString UpperCase() const;                   // 0x005E6360
};

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    void* RemoveHead();                             // 0x005E9C80
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return pos ? (T*)((CExoLinkedListNode*)pos)->pObject : 0; }
    T* GetHead()
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListPosition AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    T& operator[](int i) { return element[i]; }
    int count(CExoString s);
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void RemoveIndex(int i);                        // out of line (e.g. 0x0048DDE0)
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
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

#endif
