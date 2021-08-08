#ifndef __8086_H__
#define __8086_H__

#include <stdint.h>

#define I8086_CF 0x0001 /* Carry            */
#define I8086_PF 0x0004 /* Parity           */
#define I8086_AF 0x0010 /* Adjust           */
#define I8086_ZF 0x0040 /* Zero             */
#define I8086_SF 0x0080 /* Sign             */
#define I8086_TF 0x0100 /* Trap             */
#define I8086_IF 0x0200 /* Interrupt Enable */
#define I8086_DF 0x0400 /* Diretion         */
#define I8086_OF 0x0800 /* Overflow         */

typedef struct
{
    uint8_t AH;         /* Accumulator(AX) MSB  */
    uint8_t AL;         /* LSB                  */

    uint8_t BH;         /* Base(BX) MSB         */
    uint8_t BL;         /* LSB                  */

    uint8_t CH;         /* Counter(CX) MSB      */
    uint8_t CL;         /* LSB                  */

    uint8_t DH;         /* Data(DX) MSB         */
    uint8_t DL;         /* LSB                  */

    uint16_t CS;        /* Code Segment         */
    uint16_t SS;        /* Stack Segment        */
    uint16_t DS;        /* Data Segment         */
    uint16_t ES;        /* Extra Segment        */

    uint16_t SPOINTER;  /* Stack Pointer        */
    uint16_t BP;        /* Base Pointer         */
    uint16_t SI;        /* Source Index         */
    uint16_t DI;        /* Destination Index    */

    uint16_t IP;        /* Instruction Pointer  */

    uint16_t FLAGS;     /* Flags                */

    uint16_t SEGMENT_OVERRIDE;  /* Tmp Segment  */

    /*
     * IO Ports
     */
    void     (*outportb)(uint16_t port, uint8_t value);
    void     (*outportw)(uint16_t port, uint16_t value);
    uint8_t  (*inportb)(uint16_t port);
    uint16_t (*inportw)(uint16_t port);

    /*
     * Memory
     */
    uint16_t (*readw_memory)(uint32_t address);
    uint8_t  (*readb_memory)(uint32_t address);
    void     (*writeb_memory)(uint32_t address, uint8_t value);
    void     (*writew_memory)(uint32_t address, uint8_t value);

    /*
     * Interrupts
     */
    void     (*device);                     /* IRQ Controller Struct Address */
    uint8_t  (*inta)(void *device_addr);    /* IRQ Controller Inta Function  */

    enum __attribute__((__packed__))
    {
        SEG_OVERRIDE =  (1 << 1), /* Segment Override */
        NEW          =  (1 << 2), /* New instruction, stop executing */
        REP          =  (1 << 3), /* String manipulation */
        REPN         =  (1 << 4), /* String manipulation */
        LOCK         =  (1 << 5), /* Bus Lock */
        KEEP         =  (1 << 6)  /* */
    } prefixes;

    struct
    {
        uint8_t nmi: 1;
        uint8_t halt: 1;
        uint8_t irq: 1;
    } status;

    struct
    {
        uint16_t segment;
        uint16_t offset;
        uint8_t memory: 1;  /* Differentiate between register and memory */
    } effective_addr;

    uint8_t prefetch_queue[6];
    uint8_t current_inst_size;

} i8086_t;

/* 
 * Macros to set/extract values
 * to/from a CPU instance
 */
#define set_8086_ax(cpu_regs, value) \
    cpu_regs -> AH = value >> 8; \
    cpu_regs -> AL = value & 0xFF;

#define set_8086_bx(cpu_regs, value) \
    cpu_regs -> BH = value >> 8; \
    cpu_regs -> BL = value & 0xFF;

#define set_8086_cx(cpu_regs, value) \
    cpu_regs -> CH = value >> 8; \
    cpu_regs -> CL = value & 0xFF;

#define set_8086_dx(cpu_regs, value) \
    cpu_regs -> DH = value >> 8; \
    cpu_regs -> DL = value & 0xFF;

#define get_8086_ax(cpu_regs) \
    ((cpu_regs -> AH << 8) | (cpu_regs -> AL))

#define get_8086_bx(cpu_regs) \
    ((cpu_regs -> BH << 8) | (cpu_regs -> BL))

#define get_8086_cx(cpu_regs) \
    ((cpu_regs -> CH << 8) | (cpu_regs -> CL))

#define get_8086_dx(cpu_regs) \
    ((cpu_regs -> DH << 8) | (cpu_regs -> DL))

#define linearize_memory(segment, offset) \
    (((uint32_t)segment << 4) | (uint32_t)offset)

/*
 * Opcode Helper Macros
 */
#define extract_opcode_reg(val) \
    ((val >> 3) & 0x07)

i8086_t *i8086_new_instance(void);
void i8086_del_instance(i8086_t *cpu);

void i8086_int(i8086_t *cpu);
void i8086_nmi(i8086_t *cpu);

void i8086_execute(i8086_t *cpu);
void i8086_rst(i8086_t *cpu);

uint8_t i8086_get_r8(i8086_t *cpu, uint8_t reg);
uint16_t i8086_get_r16(i8086_t *cpu, uint8_t reg);
void i8086_set_r8(i8086_t *cpu, uint8_t reg, uint8_t val);
void i8086_set_r16(i8086_t *cpu, uint8_t reg, uint16_t val);

#endif
