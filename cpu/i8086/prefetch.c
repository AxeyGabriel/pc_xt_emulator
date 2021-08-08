#include <stdint.h>
#include "8086.h"

void prefetch_load(i8086_t *cpu)
{
    uint8_t i;
    uint16_t data;

    for(i = 0; i < 6; i+=2)
    {
        data = cpu->readw_memory(\
            linearize_memory(cpu->CS, (cpu->IP + i)));

        cpu->prefetch_queue[i] = data & 0xFF;
        cpu->prefetch_queue[i + 1] = (data >> 8) & 0xFF;
    }
}
