#ifndef __OPCODES_H__
#define __OPCODES_H__

#include "8086.h"

extern void(* const opcodes[256])(i8086_t *cpu);

#endif
