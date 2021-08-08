#include "8086.h"

/*
 * MOD = 00
 */
static void ea_bx_si(i8086_t *cpu);
static void ea_bx_di(i8086_t *cpu);
static void ea_bp_si(i8086_t *cpu);
static void ea_bp_di(i8086_t *cpu);
static void ea_si(i8086_t *cpu);
static void ea_di(i8086_t *cpu);
static void ea_xxxx(i8086_t *cpu);
static void ea_bx(i8086_t *cpu);

/*
 * MOD = 01
 */
static void ea_bx_si_xx(i8086_t *cpu);
static void ea_bx_di_xx(i8086_t *cpu);
static void ea_bp_si_xx(i8086_t *cpu);
static void ea_bp_di_xx(i8086_t *cpu);
static void ea_si_xx(i8086_t *cpu);
static void ea_di_xx(i8086_t *cpu);
static void ea_bp_xx(i8086_t *cpu);
static void ea_bx_xx(i8086_t *cpu);

/*
 * MOD = 10
 */
static void ea_bx_si_xxxx(i8086_t *cpu);
static void ea_bx_di_xxxx(i8086_t *cpu);
static void ea_bp_si_xxxx(i8086_t *cpu);
static void ea_bp_di_xxxx(i8086_t *cpu);
static void ea_si_xxxx(i8086_t *cpu);
static void ea_di_xxxx(i8086_t *cpu);
static void ea_bp_xxxx(i8086_t *cpu);
static void ea_bx_xxxx(i8086_t *cpu);

/*
 * MOD = 11
 */
static void ea_reg(i8086_t *cpu);

/*
 * Opcode MOD R/M
 * Lookup Table
 * (See i8086 datasheet)
 */
static void (* const effective_address[32])(i8086_t *cpu) =
{
    ea_bx_si,      ea_bx_di,      ea_bp_si,      ea_bp_di,      /* 00 - 07 */
    ea_si,         ea_di,         ea_xxxx,       ea_bx,                           
    ea_bx_si_xx,   ea_bx_di_xx,   ea_bp_si_xx,   ea_bp_di_xx,   /* 08 - 0F */
    ea_si_xx,      ea_di_xx,      ea_bp_xx,      ea_bx_xx,                     
    ea_bx_si_xxxx, ea_bx_di_xxxx, ea_bp_si_xxxx, ea_bp_di_xxxx, /* 10 - 17 */
    ea_si_xxxx,    ea_di_xxxx,    ea_bp_xxxx,    ea_bx_xxxx,             
    ea_reg,        ea_reg,        ea_reg,        ea_reg,        /* 18 - 1F */
    ea_reg,        ea_reg,        ea_reg,        ea_reg
};

/******************************************/
/* EFFECTIVE ADDRESS CALCULATION ROUTINES */
/******************************************/
 
/*
 * MOD = 00
 */
static void ea_bx_si(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1; 
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + cpu->SI;
    cpu->current_inst_size = 1;
}

static void ea_bx_di(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + cpu->DI;
    cpu->current_inst_size = 1;
}

static void ea_bp_si(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP + cpu->SI;
    cpu->current_inst_size = 1;
}

static void ea_bp_di(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP + cpu->DI;
    cpu->current_inst_size = 1;
}

static void ea_si(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->SI;
    cpu->current_inst_size = 1;
}

static void ea_di(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->DI;
    cpu->current_inst_size = 1;
}

static void ea_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = (cpu->prefetch_queue[3] << 8) \
                                 | cpu->prefetch_queue[2];
    cpu->current_inst_size = 3;
}

static void ea_bx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu);
    cpu->current_inst_size = 1;
}

/*
 * MOD = 01
 */
static void ea_bx_si_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + cpu->SI + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

static void ea_bx_di_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + cpu->DI + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

static void ea_bp_si_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP + cpu->SI + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

static void ea_bp_di_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP + cpu->DI + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

static void ea_si_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->SI + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

static void ea_di_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->DI + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

static void ea_bp_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->BP + \
                                 (int8_t)cpu->prefetch_queue[2];
}

static void ea_bx_xx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + \
                                 (int8_t)cpu->prefetch_queue[2];
    cpu->current_inst_size = 2;
}

/*
 * MOD = 10
 */
static void ea_bx_si_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + cpu->SI +   \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

static void ea_bx_di_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) + cpu->DI +   \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

static void ea_bp_si_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP + cpu->SI +            \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

static void ea_bp_di_xxxx(i8086_t *cpu)
{

    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP + cpu->DI +            \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

static void ea_si_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->SI +                      \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

static void ea_di_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = cpu->DI +                      \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;

}

static void ea_bp_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->SS;
    cpu->effective_addr.offset = cpu->BP +                      \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

static void ea_bx_xxxx(i8086_t *cpu)
{
    cpu->effective_addr.memory = 1;
    cpu->effective_addr.segment = cpu->DS;
    cpu->effective_addr.offset = get_8086_bx(cpu) +             \
                                 ((cpu->prefetch_queue[3] << 8) \
                                  | cpu->prefetch_queue[2]);
    cpu->current_inst_size = 3;
}

/*
 * MOD = 11
 */
static void ea_reg(i8086_t *cpu)
{
    cpu->effective_addr.memory = 0;
    cpu->effective_addr.segment = 0x0000;
    cpu->effective_addr.offset = cpu->prefetch_queue[1] & 0x07;
    cpu->current_inst_size = 1;
}

/*******************************************/
/* EFFECTIVE ADDRESS MANIPULATION ROUTINES */
/*******************************************/

void calculate_ea(i8086_t *cpu)
{
    uint8_t index;
    /*
     * OPCODE STRUCTURE:
     * Byte 0 -> OPCODE, DIR, SIZE
     * Byte 1 -> REG/MEM + DISP LENGTH,
     *  REG OPERAND, OPERAND FOR EA
     * Byte 2 -> DISPLACEMENT LOW
     * Byte 3 -> DISPLACEMENT HIGH
     */

    index = (cpu->prefetch_queue[1] & 0x07) \
            | ((cpu->prefetch_queue[1] & 0xC0) >> 3);
    effective_address[index](cpu);

    if(cpu->prefixes & SEG_OVERRIDE)
        cpu->effective_addr.segment = cpu->SEGMENT_OVERRIDE;
}

void set_ea8(i8086_t *cpu, uint8_t value)
{
    if(cpu->effective_addr.memory)
    {
        cpu->writeb_memory(\
            linearize_memory(cpu->effective_addr.segment,\
                cpu->effective_addr.offset),\
            value);
    }
    else
    {
       i8086_set_r8(cpu, cpu->effective_addr.offset, value);
    }
}

void set_ea16(i8086_t *cpu, uint16_t value)
{
    if(cpu->effective_addr.memory)
    {
        cpu->writew_memory(\
            linearize_memory(cpu->effective_addr.segment,\
                cpu->effective_addr.offset),\
            value);
    }
    else
    {
       i8086_set_r16(cpu, cpu->effective_addr.offset, value);
    }

}

uint8_t get_ea8(i8086_t *cpu)
{
    if(cpu->effective_addr.memory)
    {
        return cpu->readb_memory(\
            linearize_memory(cpu->effective_addr.segment,\
                cpu->effective_addr.offset));
    }
    else
    {
        return i8086_get_r8(cpu, cpu->effective_addr.offset);
    }
}

uint16_t get_ea16(i8086_t *cpu)
{
    if(cpu->effective_addr.memory)
    {
        return cpu->readw_memory(\
            linearize_memory(cpu->effective_addr.segment,\
                cpu->effective_addr.offset));
    }
    else
    {
        return i8086_get_r16(cpu, cpu->effective_addr.offset);
    }
}
