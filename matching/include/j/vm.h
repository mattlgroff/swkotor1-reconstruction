// Worker J matching headers: the NWScript virtual machine (CVirtualMachine
// facade, CVirtualMachineInternal, CVirtualMachineStack,
// CVirtualMachineScript), 0x005D0F40-0x005D4C10.
#ifndef J_VM_H
#define J_VM_H

#include "j/exo_string.h"
#include "j/res.h"

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType); // 0x004124E0
    int AddStructToStruct(CResStruct* pNewStruct, CResStruct* pStruct, char* szLabel,
                          unsigned long nType);                         // 0x004125B0
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x004118C0
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex); // 0x00411990
    int GetStructFromStruct(CResStruct* pNewStruct, CResStruct* pStruct, char* szLabel); // 0x00411A10
    unsigned char ReadFieldCHAR(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);                // 0x00411AD0
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);               // 0x00411C20
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault); // 0x00411C90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault); // 0x00411D00
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);         // 0x00411EC0
    int ReadFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize, char* szLabel,
                      int& bSuccess, void* pDefault);                   // 0x00412380
    int WriteFieldCHAR(CResStruct* pStruct, unsigned char nValue, char* szLabel); // 0x00412670
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel); // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);  // 0x004127B0
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel); // 0x00412870
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
    int WriteFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize, char* szLabel); // 0x00412C10
};

struct Vector {
    float x;
    float y;
    float z;
};

// Game-side command implementer (CScriptCompiler side, vtable slots used here).
class CVirtualMachineCmdImplementer {
public:
    virtual ~CVirtualMachineCmdImplementer();                       // 0x00
    virtual void InitializeCommands();                              // 0x04
    virtual int RunCommand(int nCommandId, int nParameters);        // 0x08
    virtual void RunScriptStarted(CExoString* sScriptFile);         // 0x0c
    virtual void ReportError(CExoString* sScriptFile, int nError);  // 0x10
    virtual void* CreateGameDefinedStructure(int nType);            // 0x14
    virtual void DestroyGameDefinedStructure(int nType, void* p);   // 0x18
    virtual int Unknown1C();                                        // 0x1c
    virtual void* CopyGameDefinedStructure(int nType, void* p);     // 0x20
    virtual int SaveGameDefinedStructure(int nType, void* p, CResGFF* pGFF, CResStruct* pStruct); // 0x24
    virtual int LoadGameDefinedStructure(int nType, void** ppValue, CResGFF* pGFF, CResStruct* pStruct); // 0x28

    int m_bValidObjectRunScript;                                    // 0x04
    unsigned long m_oidObjectRunScript;                             // 0x08
};

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

class CVirtualMachineInternal : public CResHelper<CResNCS, 2010> {
public:
    CVirtualMachineInternal();                                      // 0x005D4900
    virtual ~CVirtualMachineInternal();                             // 0x005D49B0

    int RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid); // 0x005D45D0
    int RunScriptSituation(void* pScript, unsigned long oid, int bOidValid); // 0x005D4AD0
    int GetRunScriptReturnValue(int* nParameterType, void** pParameter); // 0x005D1820
    void SetCommandImplementer(CVirtualMachineCmdImplementer* pImplementer); // 0x005D1A40
    void InitializeScript(CVirtualMachineScript* pScript, char* pData, int nSize); // 0x005D16F0
    int ReadScriptFile(CExoString* sScriptName);                    // 0x005D2260
    int SetUpScriptSituation(CVirtualMachineScript* pScript);       // 0x005D23E0
    int RunScriptFile(int nInstructionPointer);                     // 0x005D4270
    int ExecuteCode(int* pInstructionPointer, char* pCode, int nCodeSize); // 0x005D2BD0
    int PopReturnValue(int* pValue);                                // 0x005D17B0
    int PushReturnValue(int nValue);                                // 0x005D17F0

    int StackPopInteger(int* nValue);                               // 0x005D24C0
    int StackPushInteger(int nValue);                               // 0x005D1850
    int StackPopFloat(float* fValue);                               // 0x005D2510
    int StackPushFloat(float fValue);                               // 0x005D1880
    int StackPopVector(Vector* vValue);                             // 0x005D2560
    int StackPushVector(Vector vValue);                             // 0x005D18B0
    int StackPopString(CExoString* sValue);                         // 0x005D25E0
    int StackPushString(const CExoString& sValue);                  // 0x005D1930
    int StackPopEngineStructure(int nType, void** pValue);          // 0x005D2630
    int StackPushEngineStructure(int nType, void* pValue);          // 0x005D19C0
    int StackPopObject(unsigned long* oidValue);                    // 0x005D26A0
    int StackPushObject(unsigned long oidValue);                    // 0x005D1A10
    int StackPopCommand(CVirtualMachineScript** pScript);           // 0x005D4380
    int SaveScriptSituation(void* pScript, CResGFF* pGFF, CResStruct* pStruct); // 0x005D47A0
    int LoadScriptSituation(void** pScript, CResGFF* pGFF, CResStruct* pStruct); // 0x005D26F0
    void DeleteScriptSituation(void* pScript);                       // 0x005D48E0
    int DeleteScript(CVirtualMachineScript* pScript);               // 0x005D44F0

    int m_nReturnValueType;                                         // 0x1c
    int m_nReturnValue;                                             // 0x20
    int m_n24;                                                      // 0x24
    int m_nRecursionLevel;                                          // 0x28
    CVirtualMachineScript m_pVirtualMachineScript[8];               // 0x2c
    int m_bValidObjectRunScript[8];                                 // 0x16c
    unsigned long m_oidObjectRunScript[8];                          // 0x18c
    CVirtualMachineStack m_cRunTimeStack;                           // 0x1ac
    int m_nReturnStackDepth;                                        // 0x1c4
    int m_pReturnStack[0x80];                                       // 0x1c8
    int m_nCommandInstructPtr;                                      // 0x3c8
    int m_nCommandStackSize;                                        // 0x3cc
    int m_nCommandBasePointer;                                      // 0x3d0
    CVirtualMachineCmdImplementer* m_pCmdImplementer;               // 0x3d4
};

class CVirtualMachine {
public:
    CVirtualMachine();                                              // 0x005D0F40
    ~CVirtualMachine();                                             // 0x005D0FB0

    int RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid);
    int RunScriptSituation(void* pScript, unsigned long oid, int bOidValid);
    int GetRunScriptReturnValue(int* nParameterType, void** pParameter);
    void SetCommandImplementer(CVirtualMachineCmdImplementer* pImplementer);
    int StackPopInteger(int* nValue);
    int StackPushInteger(int nValue);
    int StackPopFloat(float* fValue);
    int StackPushFloat(float fValue);
    int StackPopVector(Vector* vValue);
    int StackPushVector(Vector vValue);
    int StackPopString(CExoString* sValue);
    int StackPushString(const CExoString& sValue);
    int StackPopEngineStructure(int nType, void** pValue);
    int StackPushEngineStructure(int nType, void* pValue);
    int StackPopObject(unsigned long* oidValue);
    int StackPushObject(unsigned long oidValue);
    int StackPopCommand(CVirtualMachineScript** pScript);
    int SaveScriptSituation(void* pScript, CResGFF* pGFF, CResStruct* pStruct);
    int LoadScriptSituation(void** pScript, CResGFF* pGFF, CResStruct* pStruct);
    void DeleteScriptSituation(void* pScript);

    CVirtualMachineInternal* m_pInternal;                           // 0x00
};

#endif
