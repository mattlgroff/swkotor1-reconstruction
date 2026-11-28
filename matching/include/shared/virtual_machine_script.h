// virtual_machine_script.h -- CVirtualMachineScript
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/vm.h); the other copies' members fall on its members.
#ifndef SHARED_VIRTUAL_MACHINE_SCRIPT_H
#define SHARED_VIRTUAL_MACHINE_SCRIPT_H

#include "shared/exo_string.h"

class CVirtualMachineScript {
public:
    CVirtualMachineScript();                                        // 0x005D1740
    ~CVirtualMachineScript();                                       // 0x005D4550

    CVirtualMachineStack* m_pStack;                                 // 0x00
    int m_nStackSize;                                               // 0x04
    int m_nInstructPtr;                                             // 0x08
    int m_nSecondaryInstructPtr;                                    // 0x0c
    char* m_pCode;                                                  // 0x10
    int m_nCodeSize;                                                // 0x14
    CExoString m_sScriptName;                                       // 0x18
    int m_nLoadedFromSave;                                          // 0x20
    int m_n24;                                                      // 0x24
};

#endif
