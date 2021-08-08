#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "8086.h"
#include "opcodes.h"
#include "prefetch.h"

i8086_t *i8086_new_instance(void)
{
    i8086_t *cpu = malloc(sizeof(i8086_t));

    if(cpu == NULL)
    {
        return NULL;
    }
    
    memset(cpu, 0, sizeof(i8086_t));

    return cpu;
}

void i8086_del_instance(i8086_t *cpu)
{
    if(cpu != NULL)
    {
        free(cpu);
    }
}

void i8086_rst(i8086_t *cpu)
{
    memset(cpu, 0, sizeof(i8086_t));
    cpu->CS = 0xF000;
    cpu->IP = 0xFFF0;
}

void i8086_int(i8086_t *cpu)
{
    cpu->status.irq = 1;
}

void i8086_nmi(i8086_t *cpu)
{
    cpu->status.nmi = 1;
}

void i8086_execute(i8086_t *cpu)
{
    if(cpu->status.nmi)
    {

    }
    else if(cpu->status.irq && (cpu->FLAGS & I8086_IF))
    {

    }
    else if(cpu->status.halt)
    {

    }

    do
    {
        cpu->prefixes &= NEW;
        prefetch_load(cpu);

        opcodes[cpu->prefetch_queue[0]](cpu);

        cpu->IP += cpu->current_inst_size;
    } while(cpu->prefixes & ~NEW);

}

uint8_t i8086_get_r8(i8086_t *cpu, uint8_t reg)
{
    switch(reg)
    {
        case 0x0:
            return cpu->AL;
        case 0x1:
            return cpu->CL;
        case 0x2:
            return cpu->DL;
        case 0x3:
            return cpu->BL;
        case 0x4:
            return cpu->AH;
        case 0x5:
            return cpu->CH;
        case 0x6:
            return cpu->DH;
        case 0x7:
            return cpu->BH;
        default:
            return 0x00;
    }
}

uint16_t i8086_get_r16(i8086_t *cpu, uint8_t reg)
{
    switch(reg)
    {
        case 0x0:
            return get_8086_ax(cpu);
        case 0x1:
            return get_8086_cx(cpu);
        case 0x2:
            return get_8086_dx(cpu);
        case 0x3:
            return get_8086_bx(cpu);
        case 0x4:
            return cpu->SPOINTER;
        case 0x5:
            return cpu->BP;
        case 0x6:
            return cpu->SI;
        case 0x7:
            return cpu->DI;
        default:
            return 0x00;
    }

}

void i8086_set_r8(i8086_t *cpu, uint8_t reg, uint8_t val)
{
    switch(reg)
    {
        case 0x0:
            cpu->AL = val;
            break;
        case 0x1:
            cpu->CL = val;
            break;
        case 0x2:
            cpu->DL = val;
            break;
        case 0x3:
            cpu->BL = val;
            break;
        case 0x4:
            cpu->AH = val;
            break;
        case 0x5:
            cpu->CH = val;
            break;
        case 0x6:
            cpu->DH = val;
            break;
        case 0x7:
            cpu->BH = val;
            break;
        default:
            break;
    }
}

void i8086_set_r16(i8086_t *cpu, uint8_t reg, uint16_t val)
{
    switch(reg)
    {
        case 0x0:
            set_8086_ax(cpu, val);
            break;
        case 0x1:
            set_8086_cx(cpu, val);
            break;
        case 0x2:
            set_8086_dx(cpu, val);
            break;
        case 0x3:
            set_8086_bx(cpu, val);
            break;
        case 0x4:
            cpu->SPOINTER = val;
            break;
        case 0x5:
            cpu->BP = val;
            break;
        case 0x6:
            cpu->SI = val;
            break;
        case 0x7:
            cpu->DI = val;
            break;
        default:
            break;
    }

}
