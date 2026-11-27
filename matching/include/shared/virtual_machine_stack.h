// virtual_machine_stack.h -- CVirtualMachineStack
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/vm.h); the other copies' members fall on its members.
#ifndef SHARED_VIRTUAL_MACHINE_STACK_H
#define SHARED_VIRTUAL_MACHINE_STACK_H


class CResGFF;
struct CResStruct;
class CVirtualMachineInternal;

class CVirtualMachineStack {
public:
    CVirtualMachineStack()
    {
        m_nStackPointer = 0;
        m_nBasePointer = 0;
        m_nTotalSize = 0;
        m_pStackTypes = 0;
        m_pStackNodes = 0;
    }
    ~CVirtualMachineStack();                                        // 0x005D28E0
    void AddToTopOfStack(int nType);                                // 0x005D15A0
    void ModifyIntegerAtLocation(int nLocation, int nValue);        // 0x005D16C0
    void AssignLocationToLocation(int nSrcLocation, int nDstLocation); // 0x005D1C40
    int SaveStack(CResGFF* pGFF, CResStruct* pStruct);              // 0x005D1D80
    int LoadStack(CResGFF* pGFF, CResStruct* pStruct);              // 0x005D1EF0
    void ClearStack();                                              // 0x005D2140
    void SetStackPointer(int nStackPointer);                        // 0x005D21F0
    void CopyFromStack(CVirtualMachineStack* pStack, int nSizeOfStack, int nBasePointer); // 0x005D28F0

    int m_nStackPointer;                                            // 0x00
    int m_nBasePointer;                                             // 0x04
    int m_nTotalSize;                                               // 0x08
    char* m_pStackTypes;                                            // 0x0c
    int* m_pStackNodes;                                             // 0x10
    CVirtualMachineInternal* m_pVMachine;                           // 0x14
};

#endif
