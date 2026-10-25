class PartEmitter;

template <class T>
class PointerList {
public:
    void AddUnique(T value);
};

class WindManager {
public:
    char pad_0x00[0x84c];
    PointerList<PartEmitter*> m_lEmitters;

    void RegisterWindyEmitter(PartEmitter* pEmitter);
};

// FUNCTION: SWKOTOR 0x004A6470
void WindManager::RegisterWindyEmitter(PartEmitter* pEmitter)
{
    m_lEmitters.AddUnique(pEmitter);
}
