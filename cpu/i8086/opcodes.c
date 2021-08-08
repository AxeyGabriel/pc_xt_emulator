#include <stdint.h>
#include "8086.h"
#include "effective_addr.h"
#include <stdio.h>

/*
 * Note: FUNCTIONS DEFINED
 *  "INTEL SYNTAX"-LIKE
 *  INSTRUCTION_DEST_ORIG(i8086_t *cpu)
 *  otherwise specified
 */

/*
 * ILLEGAL OPCODE
 */
static void illegal_opcode(i8086_t *cpu);

extern void mov_rm8_r8(i8086_t *cpu);               /* 10001000 */
extern void mov_rm16_r16(i8086_t *cpu);             /* 10001001 */
extern void mov_r8_rm8(i8086_t *cpu);               /* 10001010 */
extern void mov_r16_rm16(i8086_t *cpu);             /* 10001011 */
extern void mov_rm8_imm8(i8086_t *cpu);             /* 11000110 | mod 000 r/m */
extern void mov_rm16_imm16(i8086_t *cpu);           /* 11000111 | mod 000 r/m */
extern void mov_al_imm8(i8086_t *cpu);              /* 10110000 */
extern void mov_bl_imm8(i8086_t *cpu);              /* 10110011 */
extern void mov_cl_imm8(i8086_t *cpu);              /* 10110001 */
extern void mov_dl_imm8(i8086_t *cpu);              /* 10110010 */
extern void mov_ah_imm8(i8086_t *cpu);              /* 10110100 */
extern void mov_bh_imm8(i8086_t *cpu);              /* 10110111 */
extern void mov_ch_imm8(i8086_t *cpu);              /* 10110101 */
extern void mov_dh_imm8(i8086_t *cpu);              /* 10110110 */
extern void mov_ax_imm16(i8086_t *cpu);             /* 10111000 */
extern void mov_bx_imm16(i8086_t *cpu);             /* 10111011 */
extern void mov_cx_imm16(i8086_t *cpu);             /* 10111001 */
extern void mov_dx_imm16(i8086_t *cpu);             /* 10111010 */
extern void mov_sp_imm16(i8086_t *cpu);             /* 10111100 */
extern void mov_bp_imm16(i8086_t *cpu);             /* 10111101 */
extern void mov_si_imm16(i8086_t *cpu);             /* 10111110 */
extern void mov_di_imm16(i8086_t *cpu);             /* 10111111 */
extern void mov_al_m8(i8086_t *cpu);                /* 10100000 */
extern void mov_ax_m16(i8086_t *cpu);               /* 10100001 */
extern void mov_m8_al(i8086_t *cpu);                /* 10100010 */
extern void mov_m16_ax(i8086_t *cpu);               /* 10100011 */
extern void mov_sreg_rm16(i8086_t *cpu);            /* 10001110 | mod 0 reg r/m */
extern void mov_rm16_sreg(i8086_t *cpu);            /* 10001100 | mod 0 reg r/m */

extern void push_rm16(i8086_t *cpu);                /* 11111111 | mod 110 r/m */
extern void push_ax(i8086_t *cpu);                  /* 01010000 */
extern void push_bx(i8086_t *cpu);                  /* 01010011 */
extern void push_cx(i8086_t *cpu);                  /* 01010001 */
extern void push_dx(i8086_t *cpu);                  /* 01010010 */
extern void push_sp(i8086_t *cpu);                  /* 01010100 */
extern void push_bp(i8086_t *cpu);                  /* 01010101 */
extern void push_si(i8086_t *cpu);                  /* 01010110 */
extern void push_di(i8086_t *cpu);                  /* 01010111 */
extern void push_es(i8086_t *cpu);                  /* 00000110 */
extern void push_cs(i8086_t *cpu);                  /* 00001110 */
extern void push_ss(i8086_t *cpu);                  /* 00010110 */
extern void push_ds(i8086_t *cpu);                  /* 00011110 */

extern void pop_rm16(i8086_t *cpu);                 /* 10001111 | mod 000 r/m */
extern void pop_ax(i8086_t *cpu);                   /* 01011000 */
extern void pop_bx(i8086_t *cpu);                   /* 01011011 */
extern void pop_cx(i8086_t *cpu);                   /* 01011001 */
extern void pop_dx(i8086_t *cpu);                   /* 01011010 */
extern void pop_sp(i8086_t *cpu);                   /* 01011100 */
extern void pop_bp(i8086_t *cpu);                   /* 01011101 */
extern void pop_si(i8086_t *cpu);                   /* 01011110 */
extern void pop_di(i8086_t *cpu);                   /* 01011111 */
extern void pop_es(i8086_t *cpu);                   /* 00000111 */
extern void pop_cs(i8086_t *cpu);                   /* 00001111 */
extern void pop_ss(i8086_t *cpu);                   /* 00010111 */
extern void pop_ds(i8086_t *cpu);                   /* 00011111 */

extern void xchg_rm8_r8(i8086_t *cpu);              /* 10000110 */
extern void xchg_rm16_r16(i8086_t *cpu);            /* 10000111 */
extern void xchg_ax_ax(i8086_t *cpu);               /* 10010000 */
extern void xchg_bx_ax(i8086_t *cpu);               /* 10010011 */
extern void xchg_cx_ax(i8086_t *cpu);               /* 10010001 */
extern void xchg_dx_ax(i8086_t *cpu);               /* 10010010 */
extern void xchg_sp_ax(i8086_t *cpu);               /* 10010100 */
extern void xchg_bp_ax(i8086_t *cpu);               /* 10010101 */
extern void xchg_si_ax(i8086_t *cpu);               /* 10010110 */
extern void xchg_di_ax(i8086_t *cpu);               /* 10010111 */

extern void in_al_imm8(i8086_t *cpu);               /* 11100100 */
extern void in_ax_imm8(i8086_t *cpu);               /* 11100101 */
extern void in_al_dx(i8086_t *cpu);                 /* 11101100 */
extern void in_ax_dx(i8086_t *cpu);                 /* 11101101 */

extern void out_imm8_al(i8086_t *cpu);              /* 11100110 */
extern void out_imm8_ax(i8086_t *cpu);              /* 11100111 */
extern void out_dx_al(i8086_t *cpu);                /* 11101110 */
extern void out_dx_ax(i8086_t *cpu);                /* 11101111 */

extern void xlat(i8086_t *cpu);                     /* 11010111 */
extern void lea(i8086_t *cpu);                      /* 10001101 */
extern void lds(i8086_t *cpu);                      /* 11000101 */
extern void les(i8086_t *cpu);                      /* 11000100 */
extern void lahf(i8086_t *cpu);                     /* 10011111 */
extern void sahf(i8086_t *cpu);                     /* 10011110 */
extern void pushf(i8086_t *cpu);                    /* 10011100 */
extern void popf(i8086_t *cpu);                     /* 10011101 */

extern void add_rm8_r8(i8086_t *cpu);               /* 00000000 */
extern void add_rm16_r16(i8086_t *cpu);             /* 00000001 */
extern void add_r8_rm8(i8086_t *cpu);               /* 00000010 */
extern void add_r16_rm16(i8086_t *cpu);             /* 00000011 */
extern void add_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 000 r/m */
extern void add_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 000 r/m */
extern void add_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 000 r/m */
extern void add_al_imm8(i8086_t *cpu);              /* 00000100 */
extern void add_ax_imm16(i8086_t *cpu);             /* 00000101 */

extern void adc_rm8_r8(i8086_t *cpu);               /* 00010000 */
extern void adc_rm16_r16(i8086_t *cpu);             /* 00010001 */
extern void adc_r8_rm8(i8086_t *cpu);               /* 00010010 */
extern void adc_r16_rm16(i8086_t *cpu);             /* 00010011 */
extern void adc_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 010 r/m */
extern void adc_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 010 r/m */
extern void adc_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 010 r/m */
extern void adc_al_imm8(i8086_t *cpu);              /* 00010100 */
extern void adc_ax_imm16(i8086_t *cpu);             /* 00010101 */

extern void inc_dec_rm8(i8086_t *cpu);              /* 11111110 */
extern void inc_rm16(i8086_t *cpu);                 /* 11111111 | mod 000 r/m */
extern void inc_ax(i8086_t *cpu);                   /* 01000000 */
extern void inc_bx(i8086_t *cpu);                   /* 01000011 */
extern void inc_cx(i8086_t *cpu);                   /* 01000001 */
extern void inc_dx(i8086_t *cpu);                   /* 01000010 */
extern void inc_sp(i8086_t *cpu);                   /* 01000100 */
extern void inc_bp(i8086_t *cpu);                   /* 01000101 */
extern void inc_si(i8086_t *cpu);                   /* 01000110 */
extern void inc_di(i8086_t *cpu);                   /* 01000111 */

extern void aaa(i8086_t *cpu);                      /* 00110111 */
extern void daa(i8086_t *cpu);                      /* 00100111 */

extern void sub_rm8_r8(i8086_t *cpu);               /* 00101000 */
extern void sub_rm16_r16(i8086_t *cpu);             /* 00101001 */
extern void sub_r8_rm8(i8086_t *cpu);               /* 00101010 */
extern void sub_r16_rm16(i8086_t *cpu);             /* 00101011 */
extern void sub_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 101 r/m */
extern void sub_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 101 r/m */
extern void sub_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 101 r/m */
extern void sub_al_imm8(i8086_t *cpu);              /* 00101100 */
extern void sub_ax_imm16(i8086_t *cpu);             /* 00101101 */

extern void ssb_rm8_r8(i8086_t *cpu);               /* 00011000 */
extern void ssb_rm16_r16(i8086_t *cpu);             /* 00011001 */
extern void ssb_r8_rm8(i8086_t *cpu);               /* 00011010 */
extern void ssb_r16_rm16(i8086_t *cpu);             /* 00011011 */
extern void ssb_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 011 r/m */
extern void ssb_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 011 r/m */
extern void ssb_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 011 r/m */
extern void ssb_al_imm8(i8086_t *cpu);              /* 00011100 */
extern void ssb_ax_imm16(i8086_t *cpu);             /* 00011101 */

extern void dec_rm16(i8086_t *cpu);                 /* 11111111 | mod 001 r/m */
extern void dec_ax(i8086_t *cpu);                   /* 01001000 */
extern void dec_bx(i8086_t *cpu);                   /* 01001011 */
extern void dec_cx(i8086_t *cpu);                   /* 01001001 */
extern void dec_dx(i8086_t *cpu);                   /* 01001010 */
extern void dec_sp(i8086_t *cpu);                   /* 01001100 */
extern void dec_bp(i8086_t *cpu);                   /* 01001101 */
extern void dec_si(i8086_t *cpu);                   /* 01001110 */
extern void dec_di(i8086_t *cpu);                   /* 01001111 */

extern void neg_rm8(i8086_t *cpu);                  /* 11110110 | mod 011 r/m */
extern void neg_rm16(i8086_t *cpu);                 /* 11110111 | mod 011 r/m */

extern void cmp_rm8_r8(i8086_t *cpu);               /* 00111000 */
extern void cmp_rm16_r16(i8086_t *cpu);             /* 00111001 */
extern void cmp_r8_rm8(i8086_t *cpu);               /* 00111010 */
extern void cmp_r16_rm16(i8086_t *cpu);             /* 00111011 */
extern void cmp_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 111 r/m */
extern void cmp_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 111 r/m */
extern void cmp_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 111 r/m */
extern void cmp_al_imm8(i8086_t *cpu);              /* 00111100 */
extern void cmp_ax_imm16(i8086_t *cpu);             /* 00111101 */

extern void aas(i8086_t *cpu);                      /* 00111111 */
extern void das(i8086_t *cpu);                      /* 00101111 */

extern void mul_rm8(i8086_t *cpu);                  /* 11110110 | mod 100 r/m */
extern void mul_rm16(i8086_t *cpu);                 /* 11110111 | mod 100 r/m */

extern void imul_rm8(i8086_t *cpu);                 /* 11110110 | mod 101 r/m */
extern void imul_rm16(i8086_t *cpu);                /* 11110111 | mod 101 r/m */

extern void aam(i8086_t *cpu);                      /* 11010100 | 00001010 */

extern void div_rm8(i8086_t *cpu);                  /* 11110110 | mod 110 r/m */
extern void div_rm16(i8086_t *cpu);                 /* 11110111 | mod 110 r/m */
extern void idiv_rm8(i8086_t *cpu);                 /* 11110110 | mod 111 r/m */
extern void idiv_rm16(i8086_t *cpu);                /* 11110110 | mod 111 r/m */

extern void aad(i8086_t *cpu);                      /* 11010101 | 00001010 */
extern void cbw(i8086_t *cpu);                      /* 10011000 */
extern void cwd(i8086_t *cpu);                      /* 10011001 */

extern void not_rm8(i8086_t *cpu);                  /* 11110110 | mod 010 r/m */
extern void not_rm16(i8086_t *cpu);                 /* 11110111 | mod 010 r/m */

extern void shl_rm8(i8086_t *cpu);                  /* 11010000 | mod 100 r/m */
extern void shl_rm16(i8086_t *cpu);                 /* 11010001 | mod 100 r/m */
extern void shl_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 100 r/m */
extern void shl_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 100 r/m */

extern void shr_rm8(i8086_t *cpu);                  /* 11010000 | mod 101 r/m */
extern void shr_rm16(i8086_t *cpu);                 /* 11010001 | mod 101 r/m */
extern void shr_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 101 r/m */
extern void shr_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 101 r/m */

extern void sar_rm8(i8086_t *cpu);                  /* 11010000 | mod 111 r/m */
extern void sar_rm16(i8086_t *cpu);                 /* 11010001 | mod 111 r/m */
extern void sar_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 111 r/m */
extern void sar_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 111 r/m */

extern void rol_rm8(i8086_t *cpu);                  /* 11010000 | mod 000 r/m */
extern void rol_rm16(i8086_t *cpu);                 /* 11010001 | mod 000 r/m */
extern void rol_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 000 r/m */
extern void rol_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 000 r/m */

extern void ror_rm8(i8086_t *cpu);                  /* 11010000 | mod 001 r/m */
extern void ror_rm16(i8086_t *cpu);                 /* 11010001 | mod 001 r/m */
extern void ror_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 001 r/m */
extern void ror_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 001 r/m */

extern void rcl_rm8(i8086_t *cpu);                  /* 11010000 | mod 010 r/m */
extern void rcl_rm16(i8086_t *cpu);                 /* 11010001 | mod 010 r/m */
extern void rcl_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 010 r/m */
extern void rcl_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 010 r/m */

extern void rcr_rm8(i8086_t *cpu);                  /* 11010000 | mod 011 r/m */
extern void rcr_rm16(i8086_t *cpu);                 /* 11010001 | mod 011 r/m */
extern void rcr_rm8_cl(i8086_t *cpu);               /* 11010010 | mod 011 r/m */
extern void rcr_rm16_cl(i8086_t *cpu);              /* 11010011 | mod 011 r/m */

extern void and_rm8_r8(i8086_t *cpu);               /* 00100000 */
extern void and_rm16_r16(i8086_t *cpu);             /* 00100001 */
extern void and_r8_rm8(i8086_t *cpu);               /* 00100010 */
extern void and_r16_rm16(i8086_t *cpu);             /* 00100011 */
extern void and_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 100 r/m */
extern void and_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 100 r/m */
extern void and_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 100 r/m */
extern void and_al_imm8(i8086_t *cpu);              /* 00100100 */
extern void and_ax_imm16(i8086_t *cpu);             /* 00100101 */

extern void test_rm8_r8(i8086_t *cpu);              /* 10000100 */
extern void test_rm16_r16(i8086_t *cpu);            /* 10000101 */
extern void test_rm8_imm8(i8086_t *cpu);            /* 11110110 | mod 000 r/m */
extern void test_rm16_imm16(i8086_t *cpu);          /* 11110111 | mod 000 r/m */
extern void test_al_imm8(i8086_t *cpu);             /* 10101000 */
extern void test_ax_imm16(i8086_t *cpu);            /* 10101001 */

extern void or_rm8_r8(i8086_t *cpu);                /* 00001000 */
extern void or_rm16_r16(i8086_t *cpu);              /* 00001001 */
extern void or_r8_rm8(i8086_t *cpu);                /* 00001010 */
extern void or_r16_rm16(i8086_t *cpu);              /* 00001011 */
extern void or_rm8_imm8(i8086_t *cpu);              /* 10000000 | mod 001 r/m */
extern void or_rm16_imm16(i8086_t *cpu);            /* 10000001 | mod 001 r/m */
extern void or_rm16_imm8(i8086_t *cpu);             /* 10000011 | mod 001 r/m */
extern void or_al_imm8(i8086_t *cpu);               /* 00001100 */
extern void or_ax_imm16(i8086_t *cpu);              /* 00001101 */

extern void xor_rm8_r8(i8086_t *cpu);               /* 00110000 */
extern void xor_rm16_r16(i8086_t *cpu);             /* 00110001 */
extern void xor_r8_rm8(i8086_t *cpu);               /* 00110010 */
extern void xor_r16_rm16(i8086_t *cpu);             /* 00110011 */
extern void xor_rm8_imm8(i8086_t *cpu);             /* 10000000 | mod 110 r/m */
extern void xor_rm16_imm16(i8086_t *cpu);           /* 10000001 | mod 110 r/m */
extern void xor_rm16_imm8(i8086_t *cpu);            /* 10000011 | mod 110 r/m */
extern void xor_al_imm8(i8086_t *cpu);              /* 00110100 */
extern void xor_ax_imm16(i8086_t *cpu);             /* 00110101 */

extern void rep(i8086_t *cpu);                      /* 11110011 */
extern void repne(i8086_t *cpu);                    /* 11110010 */

extern void movsb(i8086_t *cpu);                    /* 10100100 */
extern void movsw(i8086_t *cpu);                    /* 10100101 */

extern void cmpsb(i8086_t *cpu);                    /* 10100110 */
extern void cmpsw(i8086_t *cpu);                    /* 10100111 */

extern void scasb(i8086_t *cpu);                    /* 10101110 */
extern void scasw(i8086_t *cpu);                    /* 10101111 */

extern void lodsb(i8086_t *cpu);                    /* 10101100 */
extern void lodsw(i8086_t *cpu);                    /* 10101101 */

extern void stosb(i8086_t *cpu);                    /* 10101010 */
extern void stosw(i8086_t *cpu);                    /* 10101011 */

extern void call_direct_seg(i8086_t *cpu);          /* 11101000 */
extern void call_indirect_seg(i8086_t *cpu);        /* 11111111 | mod 010 r/m */
extern void call_direct_interseg(i8086_t *cpu);     /* 10011010 */
extern void call_indirect_interseg(i8086_t *cpu);   /* 11111111 | mod 011 r/m */

extern void jmp_direct_seg(i8086_t *cpu);           /* 11101001 */
extern void jmp_direct_segshort(i8086_t *cpu);      /* 11101011 */
extern void jmp_indirect_seg(i8086_t *cpu);         /* 11111111 | mod 100 r/m */
extern void jmp_direct_interseg(i8086_t *cpu);      /* 11101010 */
extern void jmp_indirect_interseg(i8086_t *cpu);    /* 11111111 | mod 101 r/m */

extern void ret_seg(i8086_t *cpu);                  /* 11000011 */
extern void ret_seg_add(i8086_t *cpu);              /* 11000010 */
extern void ret_interseg(i8086_t *cpu);             /* 11001011 */
extern void ret_interseg_add(i8086_t *cpu);         /* 11001010 */

extern void je_jz(i8086_t *cpu);                    /* 01110100 */
extern void jl_jnge(i8086_t *cpu);                  /* 01111100 */
extern void jle_jng(i8086_t *cpu);                  /* 01111110 */
extern void jb_jnae(i8086_t *cpu);                  /* 01110010 */
extern void jbe_jna(i8086_t *cpu);                  /* 01110110 */
extern void jp_jpe(i8086_t *cpu);                   /* 01111010 */
extern void jo(i8086_t *cpu);                       /* 01110000 */
extern void js(i8086_t *cpu);                       /* 01111000 */
extern void jne_jnz(i8086_t *cpu);                  /* 01110101 */
extern void jnl_jge(i8086_t *cpu);                  /* 01111101 */
extern void jnle_jg(i8086_t *cpu);                  /* 01111111 */
extern void jnb_jae(i8086_t *cpu);                  /* 01110011 */
extern void jnbe_ja(i8086_t *cpu);                  /* 01110111 */
extern void jnp_jpo(i8086_t *cpu);                  /* 01111011 */
extern void jno(i8086_t *cpu);                      /* 01110001 */
extern void jns(i8086_t *cpu);                      /* 01111001 */
extern void loop(i8086_t *cpu);                     /* 11100010 */
extern void loopz_loope(i8086_t *cpu);              /* 11100001 */
extern void loopnz_loopne(i8086_t *cpu);            /* 11100000 */
extern void jcxz(i8086_t *cpu);                     /* 11100011 */

extern void int_specified(i8086_t *cpu);            /* 11001101 */
extern void int_type_3(i8086_t *cpu);               /* 11001100 */

extern void into(i8086_t *cpu);                     /* 11001110 */
extern void iret(i8086_t *cpu);                     /* 11001111 */
extern void clc(i8086_t *cpu);                      /* 11111000 */
extern void cmc(i8086_t *cpu);                      /* 11110101 */
extern void stc(i8086_t *cpu);                      /* 11111001 */
extern void cld(i8086_t *cpu);                      /* 11111100 */
extern void std(i8086_t *cpu);                      /* 11111101 */
extern void cli(i8086_t *cpu);                      /* 11111010 */
extern void sti(i8086_t *cpu);                      /* 11111011 */
extern void hlt(i8086_t *cpu);                      /* 11110100 */
extern void wait(i8086_t *cpu);                     /* 10011011 */
extern void esc(i8086_t *cpu);                      /* 11011xxx | mod xxx r/m */

extern void es_prefix(i8086_t *cpu);
extern void cs_prefix(i8086_t *cpu);
extern void ss_prefix(i8086_t *cpu);
extern void ds_prefix(i8086_t *cpu);
extern void lock_prefix(i8086_t *cpu);

/**********************************************/
/* 2nd BYTE INSTRUCTION DECODING ROUTINES     */
/**********************************************/
static void op_ff(i8086_t *cpu);
static void op_d0(i8086_t *cpu);
static void op_d1(i8086_t *cpu);
static void op_d2(i8086_t *cpu);
static void op_d3(i8086_t *cpu);
static void op_80(i8086_t *cpu);
static void op_81(i8086_t *cpu);
static void op_83(i8086_t *cpu);
static void op_f6(i8086_t *cpu);
static void op_f7(i8086_t *cpu);

/**********************************************/
/* 2nd BYTE INSTRUCTION DECODING LOOKUP TABLE */
/**********************************************/
void(* const op_ff_lut[8])(i8086_t *cpu) =
{
    inc_rm16, dec_rm16, call_indirect_seg,                  /* 00 - 02 */
    call_indirect_interseg, jmp_indirect_seg,               /* 03 - 04 */
    jmp_indirect_interseg, push_rm16, illegal_opcode        /* 05 - 07 */
};

void(* const op_d0_lut[8])(i8086_t *cpu) =
{
    rol_rm8, ror_rm8, rcl_rm8, rcr_rm8,                     /* 00 - 03 */
    shl_rm8, shr_rm8, illegal_opcode, sar_rm8               /* 04 - 07 */
};

void(* const op_d1_lut[8])(i8086_t *cpu) =
{
    rol_rm16, ror_rm16, rcl_rm16, rcr_rm16,                 /* 00 - 03 */
    shl_rm16, shr_rm16, illegal_opcode, sar_rm8             /* 04 - 07 */
};

void(* const op_d2_lut[8])(i8086_t *cpu) =
{
    rol_rm8_cl, ror_rm8_cl, rcl_rm8_cl, rcr_rm8_cl,         /* 00 - 03 */
    shl_rm8_cl, shr_rm8_cl, illegal_opcode, sar_rm8_cl      /* 04 - 07 */
};

void(* const op_d3_lut[8])(i8086_t *cpu) =
{
    rol_rm16_cl, ror_rm16_cl, rcl_rm16_cl, rcr_rm16_cl,     /* 00 - 03 */
    shl_rm16_cl, shr_rm16_cl, illegal_opcode, sar_rm16_cl   /* 04 - 07 */
};

void(* const op_80_lut[8])(i8086_t *cpu) =
{
    add_rm8_imm8, or_rm8_imm8, adc_rm8_imm8,                /* 00 - 02 */
    ssb_rm8_imm8, and_rm8_imm8, sub_rm8_imm8,               /* 03 - 05 */
    xor_rm8_imm8, cmp_rm8_imm8                              /* 06 - 07 */
};

void(* const op_81_lut[8])(i8086_t *cpu) =
{
    add_rm16_imm16, or_rm16_imm16, adc_rm16_imm16,          /* 00 - 02 */
    ssb_rm16_imm16, and_rm16_imm16, sub_rm16_imm16,         /* 03 - 05 */
    xor_rm16_imm16, cmp_rm16_imm16                          /* 06 - 07 */
};

void(* const op_83_lut[8])(i8086_t *cpu) =
{
    add_rm16_imm8, or_rm16_imm8, adc_rm16_imm8,             /* 00 - 02 */
    ssb_rm16_imm8, and_rm16_imm8, sub_rm16_imm8,            /* 03 - 05 */
    xor_rm16_imm8, cmp_rm16_imm8                            /* 06 - 07 */
};

void(* const op_f6_lut[8])(i8086_t *cpu) =
{
    test_rm8_imm8, illegal_opcode, not_rm8,                 /* 00 - 02 */
    neg_rm8, mul_rm8, imul_rm8, div_rm8, idiv_rm8           /* 03 - 07 */
};

void(* const op_f7_lut[8])(i8086_t *cpu) =
{
    test_rm16_imm16, illegal_opcode, not_rm16,              /* 00 - 02 */
    neg_rm16, mul_rm16, imul_rm16, div_rm16, idiv_rm16      /* 03 - 07 */
};

/**********************************************/
/* 1st BYTE INSTRUCTION DECODING LOOKUP TABLE */
/**********************************************/
void(* const opcodes[256])(i8086_t *cpu) =
{
    add_rm8_r8,     add_rm16_r16,   add_r8_rm8,             /* 00 - 02 */
    add_r16_rm16,   add_al_imm8,    add_ax_imm16,           /* 03 - 05 */
    push_es,        pop_es,         or_rm8_r8,              /* 06 - 08 */
    or_rm16_r16,    or_r8_rm8,      or_r16_rm16,            /* 09 - 11 */
    or_al_imm8,     or_ax_imm16,    push_cs,                /* 12 - 14 */
    pop_cs,         adc_rm8_r8,     adc_rm16_r16,           /* 15 - 17 */
    adc_r8_rm8,     adc_r16_rm16,   adc_al_imm8,            /* 18 - 20 */
    adc_ax_imm16,   push_ss,        pop_ss,                 /* 21 - 23 */
    ssb_rm8_r8,     ssb_rm16_r16,   ssb_r8_rm8,             /* 24 - 26 */
    ssb_r16_rm16,   ssb_al_imm8,    ssb_ax_imm16,           /* 27 - 29 */
    push_ds,        pop_ds,         and_rm8_r8,             /* 30 - 32 */
    and_rm16_r16,   and_r8_rm8,     and_r16_rm16,           /* 33 - 35 */
    and_al_imm8,    and_ax_imm16,   es_prefix,              /* 36 - 38 */
    daa,            sub_rm8_r8,     sub_rm16_r16,           /* 39 - 41 */
    sub_r8_rm8,     sub_r16_rm16,   sub_al_imm8,            /* 42 - 44 */
    sub_ax_imm16,   cs_prefix,      das,                    /* 45 - 47 */
    xor_rm8_r8,     xor_rm16_r16,   xor_r8_rm8,
    xor_r16_rm16,   xor_al_imm8,    xor_ax_imm16,
    ss_prefix,      aaa,            cmp_rm8_r8,
    cmp_rm16_r16,   cmp_r8_rm8,     cmp_r16_rm16,
    cmp_al_imm8,    cmp_ax_imm16,   ds_prefix,
    aas,            inc_ax,         inc_cx,
    inc_dx,         inc_bx,         inc_sp,
    inc_bp,         inc_si,         inc_di,
    dec_ax,         dec_cx,         dec_dx,
    dec_bx,         dec_sp,         dec_bp,
    dec_si,         dec_di,         push_ax,
    push_cx,        push_dx,        push_bx,
    push_sp,        push_bp,        push_si,
    push_di,        pop_ax,         pop_cx,
    pop_dx,         pop_bx,         pop_sp,
    pop_bp,         pop_si,         pop_di,
    illegal_opcode, illegal_opcode, illegal_opcode,
    illegal_opcode, illegal_opcode, illegal_opcode,
    illegal_opcode, illegal_opcode, illegal_opcode,
    illegal_opcode, illegal_opcode, illegal_opcode,
    illegal_opcode, illegal_opcode, illegal_opcode,
    illegal_opcode, jo,             jno,
    jb_jnae,        jnb_jae,        je_jz,
    jne_jnz,        jbe_jna,        jnbe_ja,
    js,             jns,            jp_jpe,
    jnp_jpo,        jl_jnge,        jnl_jge,
    jle_jng,        jnle_jg,        op_80,
    op_81,          illegal_opcode, op_83,
    test_rm8_r8,    test_rm16_r16,  xchg_rm8_r8,
    xchg_rm16_r16,  mov_rm8_r8,     mov_rm16_r16,
    mov_r8_rm8,     mov_r16_rm16,   mov_rm16_sreg,
    lea,            mov_sreg_rm16,  pop_rm16,
    xchg_ax_ax,     xchg_cx_ax,     xchg_dx_ax,
    xchg_bx_ax,     xchg_sp_ax,     xchg_bp_ax,
    xchg_si_ax,     xchg_di_ax,     cbw,
    cwd,            call_direct_interseg, wait,
    pushf,          popf,           sahf,
    lahf,           mov_al_m8,      mov_ax_m16,
    mov_m16_ax,     mov_m8_al,      movsb,
    movsw,
    cmpsb,          cmpsw,          test_al_imm8,
    test_ax_imm16,  stosb,          stosw,
    lodsb,          lodsw,          scasb,
    scasw,          mov_al_imm8,    mov_cl_imm8,
    mov_dl_imm8,    mov_bl_imm8,    mov_ah_imm8,
    mov_ch_imm8,    mov_dh_imm8,    mov_bh_imm8,
    mov_ax_imm16,   mov_cx_imm16,   mov_dx_imm16,
    mov_bx_imm16,   mov_sp_imm16,   mov_bp_imm16,
    mov_si_imm16,   mov_di_imm16,   illegal_opcode,
    illegal_opcode, ret_seg_add,    ret_seg,
    les,            lds,            mov_rm8_imm8,
    mov_rm16_imm16, illegal_opcode, illegal_opcode,
    ret_interseg_add, ret_interseg, int_type_3,
    int_specified,  into,           iret,
    op_d0,          op_d1,          op_d2,
    op_d3,          aam,            aad,
    illegal_opcode, xlat,           esc,
    esc,            esc,            esc,
    esc,            esc,            esc,
    esc,            loopnz_loopne,  loopz_loope,
    loop,           jcxz,           in_al_imm8,
    in_ax_imm8,     out_imm8_al,    out_imm8_ax,
    call_direct_seg, jmp_direct_seg, jmp_direct_interseg,
    jmp_direct_segshort, in_al_dx,  in_ax_dx,
    out_dx_al,      out_dx_ax,      lock_prefix,
    illegal_opcode, repne,          rep,
    hlt,            cmc,            op_f6,
    op_f7,          clc,            stc,
    cli,            sti,            cld,
    std,            inc_dec_rm8,    op_ff
};

static void op_ff(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_ff_lut[address](cpu);
}

static void op_d0(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_d0_lut[address](cpu);

}

static void op_d1(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_d1_lut[address](cpu);

}

static void op_d2(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_d2_lut[address](cpu);

}

static void op_d3(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_d3_lut[address](cpu);

}

static void op_80(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_80_lut[address](cpu);

}

static void op_81(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_81_lut[address](cpu);

}

static void op_83(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_83_lut[address](cpu);

}

static void op_f6(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_f6_lut[address](cpu);

}

static void op_f7(i8086_t *cpu)
{
    uint8_t address;
    address = (cpu->prefetch_queue[1] >> 3) & 0x07;
    op_f7_lut[address](cpu);

}

static void illegal_opcode(i8086_t *cpu)
{
    /* 
     * Undefined opcode
     */

    #ifdef __MAKE_DEBUG__
        printf("i8086 [ERROR]: UNDEFINED OPCODE");
    #endif

    cpu->current_inst_size = 1;
}
