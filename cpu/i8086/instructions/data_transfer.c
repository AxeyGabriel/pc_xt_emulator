#include <stdio.h>
#include <stdint.h>
#include "../8086.h"
#include "../effective_addr.h"

void mov_rm8_r8(i8086_t *cpu)
{

    uint8_t reg;
    uint8_t value;

    calculate_ea(cpu); // Calculate destination
    reg = extract_opcode_reg(cpu->prefetch_queue[1]); // REG in MOD REG R/M
    value = i8086_get_r8(cpu, reg);
    set_ea8(cpu, value);

    cpu->current_inst_size += 1;
}

void mov_rm16_r16(i8086_t *cpu)
{


}

void mov_r8_rm8(i8086_t *cpu)
{


}

void mov_r16_rm16(i8086_t *cpu)
{


}

void mov_rm8_imm8(i8086_t *cpu)
{


}

void mov_rm16_imm16(i8086_t *cpu)
{


}

void mov_al_imm8(i8086_t *cpu)
{


}

void mov_bl_imm8(i8086_t *cpu)
{


}

void mov_cl_imm8(i8086_t *cpu)
{


}

void mov_dl_imm8(i8086_t *cpu)
{


}

void mov_ah_imm8(i8086_t *cpu)
{


}

void mov_bh_imm8(i8086_t *cpu)
{


}

void mov_ch_imm8(i8086_t *cpu)
{


}

void mov_dh_imm8(i8086_t *cpu)
{


}

void mov_ax_imm16(i8086_t *cpu)
{


}

void mov_bx_imm16(i8086_t *cpu)
{


}

void mov_cx_imm16(i8086_t *cpu)
{


}

void mov_dx_imm16(i8086_t *cpu)
{


}

void mov_sp_imm16(i8086_t *cpu)
{


}

void mov_bp_imm16(i8086_t *cpu)
{


}

void mov_si_imm16(i8086_t *cpu)
{


}

void mov_di_imm16(i8086_t *cpu)
{


}

void mov_al_m8(i8086_t *cpu)
{


}

void mov_ax_m16(i8086_t *cpu)
{


}

void mov_m8_al(i8086_t *cpu)
{


}

void mov_m16_ax(i8086_t *cpu)
{


}

void mov_sreg_rm16(i8086_t *cpu)
{


}

void mov_rm16_sreg(i8086_t *cpu)
{


}

void push_rm16(i8086_t *cpu)
{


}

void push_ax(i8086_t *cpu)
{


}

void push_bx(i8086_t *cpu)
{


}

void push_cx(i8086_t *cpu)
{


}

void push_dx(i8086_t *cpu)
{


}

void push_sp(i8086_t *cpu)
{


}

void push_bp(i8086_t *cpu)
{


}

void push_si(i8086_t *cpu)
{


}

void push_di(i8086_t *cpu)
{


}

void push_es(i8086_t *cpu)
{


}

void push_cs(i8086_t *cpu)
{


}

void push_ss(i8086_t *cpu)
{


}

void push_ds(i8086_t *cpu)
{


}

void pop_rm16(i8086_t *cpu)
{


}

void pop_ax(i8086_t *cpu)
{


}

void pop_bx(i8086_t *cpu)
{


}

void pop_cx(i8086_t *cpu)
{


}

void pop_dx(i8086_t *cpu)
{


}

void pop_sp(i8086_t *cpu)
{


}

void pop_bp(i8086_t *cpu)
{


}

void pop_si(i8086_t *cpu)
{


}

void pop_di(i8086_t *cpu)
{


}

void pop_es(i8086_t *cpu)
{


}

void pop_cs(i8086_t *cpu)
{


}

void pop_ss(i8086_t *cpu)
{


}

void pop_ds(i8086_t *cpu)
{


}

void xchg_rm8_r8(i8086_t *cpu)
{


}

void xchg_rm16_r16(i8086_t *cpu)
{


}

void xchg_ax_ax(i8086_t *cpu)
{


}

void xchg_bx_ax(i8086_t *cpu)
{


}

void xchg_cx_ax(i8086_t *cpu)
{


}

void xchg_dx_ax(i8086_t *cpu)
{


}

void xchg_sp_ax(i8086_t *cpu)
{


}

void xchg_bp_ax(i8086_t *cpu)
{


}

void xchg_si_ax(i8086_t *cpu)
{


}

void xchg_di_ax(i8086_t *cpu)
{


}

void in_al_imm8(i8086_t *cpu)
{


}

void in_ax_imm8(i8086_t *cpu)
{


}

void in_al_dx(i8086_t *cpu)
{


}

void in_ax_dx(i8086_t *cpu)
{


}

void out_imm8_al(i8086_t *cpu)
{


}

void out_imm8_ax(i8086_t *cpu)
{


}

void out_dx_al(i8086_t *cpu)
{


}

void out_dx_ax(i8086_t *cpu)
{


}

void xlat(i8086_t *cpu)
{


}

void lea(i8086_t *cpu)
{


}

void lds(i8086_t *cpu)
{


}

void les(i8086_t *cpu)
{


}

void lahf(i8086_t *cpu)
{


}

void sahf(i8086_t *cpu)
{


}

void pushf(i8086_t *cpu)
{


}

void popf(i8086_t *cpu)
{


}
