#include "../8086.h"
#include <stdio.h>

void call_direct_seg(i8086_t *cpu)
{


}

void call_indirect_seg(i8086_t *cpu)
{


}

void call_direct_interseg(i8086_t *cpu)
{


}

void call_indirect_interseg(i8086_t *cpu)
{



}

void jmp_direct_seg(i8086_t *cpu)
{


}

void jmp_direct_segshort(i8086_t *cpu)
{


}

void jmp_indirect_seg(i8086_t *cpu)
{

}

void jmp_direct_interseg(i8086_t *cpu)
{
    cpu->IP = (cpu->prefetch_queue[2] << 8)\
        | cpu->prefetch_queue[1];

    cpu->CS = (cpu->prefetch_queue[4] << 8)\
        | cpu->prefetch_queue[3];
}

void jmp_indirect_interseg(i8086_t *cpu)
{


}

void ret_seg(i8086_t *cpu)
{


}

void ret_seg_add(i8086_t *cpu)
{


}

void ret_interseg(i8086_t *cpu)
{


}

void ret_interseg_add(i8086_t *cpu)
{


}

void je_jz(i8086_t *cpu)
{


}

void jl_jnge(i8086_t *cpu)
{


}

void jle_jng(i8086_t *cpu)
{


}

void jb_jnae(i8086_t *cpu)
{


}

void jbe_jna(i8086_t *cpu)
{


}

void jp_jpe(i8086_t *cpu)
{


}

void jo(i8086_t *cpu)
{


}

void js(i8086_t *cpu)
{


}

void jne_jnz(i8086_t *cpu)
{


}

void jnl_jge(i8086_t *cpu)
{


}

void jnle_jg(i8086_t *cpu)
{


}

void jnb_jae(i8086_t *cpu)
{


}

void jnbe_ja(i8086_t *cpu)
{


}

void jnp_jpo(i8086_t *cpu)
{


}

void jno(i8086_t *cpu)
{


}

void jns(i8086_t *cpu)
{


}

void loop(i8086_t *cpu)
{


}

void loopz_loope(i8086_t *cpu)
{


}

void loopnz_loopne(i8086_t *cpu)
{


}

void jcxz(i8086_t *cpu)
{


}

void int_specified(i8086_t *cpu)
{


}

void int_type_3(i8086_t *cpu)
{


}

void into(i8086_t *cpu)
{


}

void iret(i8086_t *cpu)
{


}
