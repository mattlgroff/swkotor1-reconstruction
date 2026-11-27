// object_id.h -- OBJECT_INVALID, the object id no object has (CODE_RULES.md, R2)
//
// The units that compare against it in memory include this header. Units
// whose bytes use the immediate 0x7f000000 define it as a macro instead and
// never include this header.
#ifndef SHARED_OBJECT_ID_H
#define SHARED_OBJECT_ID_H

extern const unsigned long OBJECT_INVALID;          // 0x00746644

#endif
