// Worker B matching header: CSWVirtualMachineCommands (NWScript engine
// command implementations, 0x0052xxxx-0x0054xxxx) and the CVirtualMachine
// facade it calls (0x005D1000-0x005D1110, worker J).
#ifndef B_VM_COMMANDS_H
#define B_VM_COMMANDS_H

#include "b/exo_string.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"
#include "b/vector.h"

#define VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW  (-2000)
#define VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW (-2001)


class CVirtualMachine {
public:
    int StackPopInteger(int* nValue);                       // 0x005D1000
    int StackPushInteger(int nValue);                       // 0x005D1010
    int StackPopFloat(float* fValue);                       // 0x005D1020
    int StackPushFloat(float fValue);                       // 0x005D1030
    int StackPopVector(Vector* vValue);                   // 0x005D1040
    int StackPushVector(Vector vValue);                   // 0x005D1050
    int StackPopString(CExoString* sValue);                 // 0x005D1080
    int StackPushString(const CExoString& sValue);          // 0x005D1090
    int StackPopEngineStructure(int nType, void** pValue);  // 0x005D10A0
    int StackPushEngineStructure(int nType, void* pValue);  // 0x005D10B0
    int StackPopObject(unsigned long* oidValue);            // 0x005D10C0
    int StackPushObject(unsigned long oidValue);            // 0x005D10D0
    int StackPopCommand(class CVirtualMachineScript** ppScript); // 0x005D10E0
    int RunScriptSituation(void* pScript, unsigned long oidSelf, int bArg); // 0x005D0FD0
    int RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid); // 0x005D0FC0

    void* m_pInternal;                                      // 0x00
};

#include "shared/globals.h"

#define ENGINE_STRUCTURE_EFFECT 0
#define ENGINE_STRUCTURE_EVENT  1
#define ENGINE_STRUCTURE_LOCATION 2
#define ENGINE_STRUCTURE_TALENT 3


class CResGFF;
struct CResStruct;

// Abstract command implementer the virtual machine calls back into
// (vtable 0x007450AC: every slot but the destructor is _purecall).
#include "shared/sw_virtual_machine_commands.h"

#endif
