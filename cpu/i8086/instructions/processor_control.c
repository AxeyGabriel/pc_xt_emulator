#include <stdio.h>
#include "../8086.h"
#include "../effective_addr.h"

void clc(i8086_t *cpu)
{
    cpu->FLAGS &= ~I8086_CF;
    cpu->current_inst_size = 1;
}

void cmc(i8086_t *cpu)
{
    cpu->FLAGS ^= I8086_CF;
    cpu->current_inst_size = 1;
}

void stc(i8086_t *cpu)
{
    cpu->FLAGS |= I8086_CF;
    cpu->current_inst_size = 1;
}

void cld(i8086_t *cpu)
{
    cpu->FLAGS &= ~I8086_DF;
    cpu->current_inst_size = 1;
}

void std(i8086_t *cpu)
{
    cpu->FLAGS |= I8086_DF;
    cpu->current_inst_size = 1;
}

void cli(i8086_t *cpu)
{
    cpu->FLAGS &= ~I8086_IF;
    cpu->current_inst_size = 1;
}

void sti(i8086_t *cpu)
{
    cpu->FLAGS |= I8086_IF;
    cpu->current_inst_size = 1;
}

void hlt(i8086_t *cpu)
{
    cpu->status.halt = 1;
    cpu->current_inst_size = 1;
}

void wait(i8086_t *cpu)
{
    //TODO
    cpu->current_inst_size = 1;
}

void esc(i8086_t *cpu)
{
    //TODO
    calculate_ea(cpu);
    cpu->current_inst_size += 1;
}

void es_prefix(i8086_t *cpu)
{
    cpu->prefixes |= NEW | SEG_OVERRIDE;
    cpu->SEGMENT_OVERRIDE = cpu->ES;
    cpu->current_inst_size = 1;
}

void cs_prefix(i8086_t *cpu)
{
    cpu->prefixes |= NEW | SEG_OVERRIDE;
    cpu->SEGMENT_OVERRIDE = cpu->CS;
    cpu->current_inst_size = 1;
}

void ss_prefix(i8086_t *cpu)
{
    cpu->prefixes |= NEW | SEG_OVERRIDE;
    cpu->SEGMENT_OVERRIDE = cpu->SS;
    cpu->current_inst_size = 1;
}

void ds_prefix(i8086_t *cpu)
{
    cpu->prefixes |= NEW | SEG_OVERRIDE;
    cpu->SEGMENT_OVERRIDE = cpu->DS;
    cpu->current_inst_size = 1;
}

void lock_prefix(i8086_t *cpu)
{
    cpu->prefixes |= NEW | LOCK;
    cpu->current_inst_size = 1;
}
