// Shared camera-behavior teardown alias. The alias member name is inferred;
// the direct target is the established CAurBehavior destructor at 0x0049FA70.
class CAurBehavior {
public:
    virtual ~CAurBehavior();
    void DestroyBaseAlias();
};

// FUNCTION: SWKOTOR 0x006DB4E0
void CAurBehavior::DestroyBaseAlias()
{
    CAurBehavior::~CAurBehavior();
}
