// Small method-table leaves for the in-game character panel.

extern "C" void __fastcall gui_m48_slider_destruct_0041bb40(void* self, void* edx);

// FUNCTION: SWKOTOR 0x006AF940
void __fastcall kotor_fastcall_006af940(void* self, void* edx)
{
    gui_m48_slider_destruct_0041bb40(self, edx);
}

void __cdecl operator delete(void* p);

// FUNCTION: SWKOTOR 0x006AF950
void* __fastcall kotor_fastcall_006af950(void* self, void* edx, unsigned int flags)
{
    kotor_fastcall_006af940(self, edx);
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}
