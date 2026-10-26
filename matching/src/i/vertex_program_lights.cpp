// VertexProgram::FindNumberOfAllowedLights (aurvertexprog.o, 0x004A2320):
// each ';' in the source is one instruction of the 114-slot budget, and
// a light costs 14.
#include <string.h>

struct VertexProgram {
    unsigned int unknown_00;
    char* original_source;
    char program_text[0x2000];
    unsigned int* parameter_words;
    unsigned int program_id;
    int allowed_lights;
    int generation;
    int enabled;

    void FindNumberOfAllowedLights();
};

// FUNCTION: SWKOTOR 0x004A2320
void VertexProgram::FindNumberOfAllowedLights()
{
    int nInstructions = 0;
    for (int i = 0; i < (int)strlen(original_source); i++) {
        if (original_source[i] == ';') {
            nInstructions++;
        }
    }
    int nLights = (114 - nInstructions) / 14;
    allowed_lights = nLights;
    allowed_lights = nLights > 3 ? 3 : nLights;
}
