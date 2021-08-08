#define F_CPU       16000000UL
#define BAUD        57600
#define USE_2X
#define __DELAY_ROUND_CLOSEST__

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <inttypes.h>

#include "cpu/i8086/8086.h"
#include "cpu/i8086/opcodes.h"


void uart_init(void);
void uart_putchar(char c, FILE *stream);
FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

extern const uint8_t ROM_BIOS[8192] PROGMEM;

uint16_t readwm(uint32_t addr)
{
    static uint16_t data = 0;
    
    if(addr >= 0xFE000 && addr <= 0xFFFFE)
    {
          data = pgm_read_word_far(&ROM_BIOS[(size_t) addr - 0xFE000]);
    }

    return data;
}

void dump(i8086_t *cpu)
{
    printf("AX: 0x%04x     CX: 0x%04x\n"
        "DX: 0x%04x     BX: 0x%04x\n"
        "CS: 0x%04x     SS: 0x%04x\n"
        "DS: 0x%04x     ES: 0x%04x\n"
        "SP: 0x%04x     BP: 0x%04x\n"
        "SI: 0x%04x     DI: 0x%04x\n"
        "IP: 0x%04x     FLAGS: 0x%04x\n",\
         get_8086_ax(cpu), get_8086_cx(cpu),\
         get_8086_dx(cpu), get_8086_bx(cpu),\
         cpu->CS, cpu->SS, cpu->DS, cpu->ES,\
         cpu->SPOINTER, cpu->BP, cpu->SI, cpu->DI,\
         cpu->IP, cpu->FLAGS);

    uint8_t i;

    printf("PREFETCH QUEUE:");
    for(i = 0; i < 6; i++)
        printf(" 0x%02x", cpu->prefetch_queue[i]);
    puts("\n");
}

void main(void)
{
    stdout = &uart_output;
    
    printf("IBM-PC Compatible Emulator\n");
    printf("Written by Axey Gabriel Muller Endres\n");
    printf("Start date: 03 November, 2017\n\n");

    i8086_t *cpu = i8086_new_instance();
    i8086_rst(cpu);
    cpu->readw_memory = &readwm;

    printf("sizeof i8086_t: %d\n", sizeof(i8086_t));
    
    cpu->prefetch_queue[0] = 0b11110110;
    cpu->prefetch_queue[1] = 0b11010101;
    opcodes[cpu->prefetch_queue[0]](cpu);
    dump(cpu);
    //uint16_t count;
    //for(count = 0; count < 128; count++)
    //{
    //    i8086_execute(cpu);
    //    dump(cpu);
    //}
    uint8_t a = (cpu->AL << 5) | (cpu->AL >> (-5 & 7));
    printf("%d\n", a);

    i8086_del_instance(cpu);
}

void uart_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */  

}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);                  
    }
    
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}
