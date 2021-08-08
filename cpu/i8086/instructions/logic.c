#include <stdio.h>
#include "../8086.h"
#include "../effective_addr.h"

/***********/
/* NOT     */
/***********/
void not_rm8(i8086_t *cpu)
{
    calculate_ea(cpu);
    cpu->current_inst_size += 1;

    set_ea8(cpu, ~get_ea8(cpu));
}

void not_rm16(i8086_t *cpu)
{
    calculate_ea(cpu);
    cpu->current_inst_size += 1;

    set_ea16(cpu, ~get_ea16(cpu));
}

/***********/
/* SHL     */
/***********/
void shl_rm8(i8086_t *cpu)
{
    register uint8_t tmp;

    calculate_ea(cpu);
    cpu->current_inst_size += 1;

    tmp = get_ea8(cpu);

    if(tmp & 0x80) // Test if will carry
        cpu->FLAGS |= I8086_CF;
    else
        cpu->FLAGS &= ~I8086_CF;

    tmp <<= 1;



    set_ea8(cpu, tmp);
}

void shl_rm16(i8086_t *cpu)
{


}

void shl_rm8_cl(i8086_t *cpu)
{


}

void shl_rm16_cl(i8086_t *cpu)
{


}

/***********/
/* SHR     */
/***********/
void shr_rm8(i8086_t *cpu)
{


}

void shr_rm16(i8086_t *cpu)
{


}

void shr_rm8_cl(i8086_t *cpu)
{


}

void shr_rm16_cl(i8086_t *cpu)
{


}

/***********/
/* SAR     */
/***********/
void sar_rm8(i8086_t *cpu)
{


}

void sar_rm16(i8086_t *cpu)
{


}

void sar_rm8_cl(i8086_t *cpu)
{


}

void sar_rm16_cl(i8086_t *cpu)
{


}

/***********/
/* ROL     */
/***********/
void rol_rm8(i8086_t *cpu)
{


}

void rol_rm16(i8086_t *cpu)
{


}

void rol_rm8_cl(i8086_t *cpu)
{


}

void rol_rm16_cl(i8086_t *cpu)
{


}

/***********/
/* ROR     */
/***********/
void ror_rm8(i8086_t *cpu)
{


}

void ror_rm16(i8086_t *cpu)
{


}

void ror_rm8_cl(i8086_t *cpu)
{


}

void ror_rm16_cl(i8086_t *cpu)
{


}

/***********/
/* RCL     */
/***********/
void rcl_rm8(i8086_t *cpu)
{


}

void rcl_rm16(i8086_t *cpu)
{


}

void rcl_rm8_cl(i8086_t *cpu)
{


}

void rcl_rm16_cl(i8086_t *cpu)
{


}

/***********/
/* RCR     */
/***********/
void rcr_rm8(i8086_t *cpu)
{


}

void rcr_rm16(i8086_t *cpu)
{


}

void rcr_rm8_cl(i8086_t *cpu)
{


}

void rcr_rm16_cl(i8086_t *cpu)
{


}


/***********/
/* AND     */
/***********/
void and_rm8_r8(i8086_t *cpu)
{


}

void and_rm16_r16(i8086_t *cpu)
{


}

void and_r8_rm8(i8086_t *cpu)
{


}

void and_r16_rm16(i8086_t *cpu)
{


}

void and_rm8_imm8(i8086_t *cpu)
{


}

void and_rm16_imm16(i8086_t *cpu)
{


}

void and_rm16_imm8(i8086_t *cpu)
{


}

void and_al_imm8(i8086_t *cpu)
{


}
        
void and_ax_imm16(i8086_t *cpu)
{


}

/***********/
/* TEST    */
/***********/
void test_rm8_r8(i8086_t *cpu)
{


}

void test_rm16_r16(i8086_t *cpu)
{


}

void test_rm8_imm8(i8086_t *cpu)
{


}

void test_rm16_imm16(i8086_t *cpu)
{


}

void test_al_imm8(i8086_t *cpu)
{


}

void test_ax_imm16(i8086_t *cpu)
{


}

/***********/
/* OR      */
/***********/
void or_rm8_r8(i8086_t *cpu)
{


}

void or_rm16_r16(i8086_t *cpu)
{


}

void or_r8_rm8(i8086_t *cpu)
{


}

void or_r16_rm16(i8086_t *cpu)
{


}

void or_rm8_imm8(i8086_t *cpu)
{


}

void or_rm16_imm16(i8086_t *cpu)
{


}

void or_rm16_imm8(i8086_t *cpu)
{


}

void or_al_imm8(i8086_t *cpu)
{


}

void or_ax_imm16(i8086_t *cpu)
{


}

/***********/
/* XOR     */
/***********/
void xor_rm8_r8(i8086_t *cpu)
{


}

void xor_rm16_r16(i8086_t *cpu)
{


}

void xor_r8_rm8(i8086_t *cpu)
{


}

void xor_r16_rm16(i8086_t *cpu)
{


}

void xor_rm8_imm8(i8086_t *cpu)
{


}

void xor_rm16_imm16(i8086_t *cpu)
{


}

void xor_rm16_imm8(i8086_t *cpu)
{


}

void xor_al_imm8(i8086_t *cpu)
{


}

void xor_ax_imm16(i8086_t *cpu)
{


}
