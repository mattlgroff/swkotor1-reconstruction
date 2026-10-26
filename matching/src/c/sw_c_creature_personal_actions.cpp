// CSWCCreature::GetPersonalActions (nwccreature.o, worker C s3): dispatches
// the personal action menu to the self force power and usable item lists.

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08
};

class CSWGuiInterfaceAction;

class CSWCCreature {
public:
    void GetPersonalActions(int nType, CExoArrayList<CSWGuiInterfaceAction>& lstActions);
    void GetSelfForcePowers(CExoArrayList<CSWGuiInterfaceAction>& lstActions);  // 0x00616230
    void GetUsableItems(CExoArrayList<CSWGuiInterfaceAction>& lstActions, unsigned char nFlags); // 0x006197D0
};

// FUNCTION: SWKOTOR 0x00619DB0
void CSWCCreature::GetPersonalActions(int nType, CExoArrayList<CSWGuiInterfaceAction>& lstActions)
{
    switch (nType) {
    case 0:
        GetSelfForcePowers(lstActions);
        break;
    case 1:
        GetUsableItems(lstActions, 1);
        break;
    case 2:
        GetUsableItems(lstActions, 2);
        break;
    case 3:
        GetUsableItems(lstActions, 4);
        break;
    }
}
