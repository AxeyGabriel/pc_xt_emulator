#ifndef __EFFECTIVE_ADDR_H__
#define __EFFECTIVE_ADDR_H__

#include <stdint.h>
#include "8086.h"

void calculate_ea(i8086_t *cpu);
void set_ea8(i8086_t *cpu, uint8_t value);
void set_ea16(i8086_t *cpu, uint16_t value);
uint8_t get_ea8(i8086_t *cpu);
uint16_t get_ea16(i8086_t *cpu);

#endif
