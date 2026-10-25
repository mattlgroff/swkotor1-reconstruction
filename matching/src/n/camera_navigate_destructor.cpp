class CAurBehavior {
public:
    virtual ~CAurBehavior();
};

// FUNCTION: SWKOTOR 0x006386D0
void __fastcall camera_navigate_destructor_006386d0(void* pThis, void*)
{
    *(void**)pThis = (void*)0x007515f4;
    ((CAurBehavior*)pThis)->CAurBehavior::~CAurBehavior();
}
