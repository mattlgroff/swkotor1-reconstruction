// vertex_program_array.h -- VertexProgramArray
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/i/vertex_program_gl.cpp); the other copies' members fall on its members.
#ifndef SHARED_VERTEX_PROGRAM_ARRAY_H
#define SHARED_VERTEX_PROGRAM_ARRAY_H


class VertexProgram;

struct VertexProgramArray {
    VertexProgram** data;
    int size;
    int capacity;

    void Remove(VertexProgram* p);                      // 0x00445080

    // declared by other copies
    void Allocate(int count);
};

#endif
