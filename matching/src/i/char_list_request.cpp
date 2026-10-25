template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    void AddUnique(T value);
};

class CServerExoAppInternal {
public:
    char pad_0x00[0x100ec];
    CExoArrayList<unsigned long> m_lCharListRequests;

    int AddCharListRequest(unsigned long nPlayerId);
};

#pragma warning(disable: 4716)
// FUNCTION: SWKOTOR 0x004B6CB0 SYMBOL: ?AddCharListRequest@CServerExoAppInternal@@QAEHK@Z
int CServerExoAppInternal::AddCharListRequest(unsigned long nPlayerId)
{
    m_lCharListRequests.AddUnique(nPlayerId);
}
#pragma warning(default: 4716)
