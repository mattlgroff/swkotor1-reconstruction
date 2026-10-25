struct VertexProgram {
    unsigned int unknown_00;
    char* original_source;
    char program_text[0x2000];
    unsigned int* parameter_words;
    unsigned int program_id;
    int allowed_lights;
    int generation;
    int enabled;

    int FindNumberOfAllowedLights();
};

extern void __cdecl AddVertexProgramToManager_004a2770(VertexProgram* program);

// FUNCTION: SWKOTOR 0x004A27D0
VertexProgram* __fastcall VertexProgram_construct_004a27d0(VertexProgram* self, void*,
                                                           void* first_argument, void* second_argument)
{
    self->original_source = (char*)first_argument;
    self->parameter_words = (unsigned int*)second_argument;
    self->program_id = 0;
    self->allowed_lights = 0;
    self->generation = -1;
    self->FindNumberOfAllowedLights();
    self->allowed_lights = 3;
    self->enabled = 0;
    AddVertexProgramToManager_004a2770(self);
    return self;
}
