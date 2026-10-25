class Vector {
public:
    float x, y, z;
};

extern Vector g_vDisabledColor;

class CSWGuiControl {
public:
    virtual void SetEnabled(int bEnabled);
};

class CSWGuiTextParams {
public:
    void RestoreDefaultColor();
    void SetColor(const Vector& vColor);
};

class CSWGuiEditbox : public CSWGuiControl {
public:
    virtual void SetEnabled(int bEnabled);
};

// FUNCTION: SWKOTOR 0x004184B0 SYMBOL: ?SetEnabled@CSWGuiEditbox@@UAEXH@Z
__declspec(naked) void CSWGuiEditbox::SetEnabled(int)
{
    __asm {
        push esi
        mov esi, dword ptr [esp + 8]
        push esi
        call CSWGuiControl::SetEnabled
        test esi, esi
        pop esi
        je disabled
        add ecx, 0f8h
        call CSWGuiTextParams::RestoreDefaultColor
        ret 4
    disabled:
        mov dword ptr [esp + 4], 078d3b4h
        add ecx, 0f8h
        jmp CSWGuiTextParams::SetColor
    }
}
