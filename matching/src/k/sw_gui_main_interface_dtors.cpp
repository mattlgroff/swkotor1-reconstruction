// Compiler-generated destructors of the main interface and of its status
// labels (vtables 0x00753CC0 / 0x00753EA8 share the ??_G), declared here
// with minimal classes.

class CSWGuiLabel {
public:
    virtual ~CSWGuiLabel();                         // 0x00419D10
};

class CSWGuiMainInterfaceStatus : public CSWGuiLabel {
public:
    CSWGuiMainInterfaceStatus();
    ~CSWGuiMainInterfaceStatus();                   // 0x0068AD10
};

// Defined only to emit the vtable and its scalar deleting destructor.
CSWGuiMainInterfaceStatus::CSWGuiMainInterfaceStatus()
{
}

// FUNCTION: SWKOTOR 0x0068ACF0 SYMBOL: ??_GCSWGuiMainInterfaceStatus@@UAEPAXI@Z
// CSWGuiMainInterfaceStatus::`scalar deleting destructor' (compiler generated).

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();                         // 0x0040CF70
};

class CSWGuiMainInterface : public CSWGuiPanel {
public:
    CSWGuiMainInterface();
    virtual ~CSWGuiMainInterface();                 // 0x0068B170
};

// Defined only to emit the vtable and its scalar deleting destructor.
CSWGuiMainInterface::CSWGuiMainInterface()
{
}

// FUNCTION: SWKOTOR 0x0068D3B0 SYMBOL: ??_GCSWGuiMainInterface@@UAEPAXI@Z
// CSWGuiMainInterface::`scalar deleting destructor' (compiler generated).
