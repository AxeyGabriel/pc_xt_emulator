#
# File: Makefile
# Description: This Makefile compiles main.c
#
# 2017, Axey Gabriel Müller Endres
#
#
OBJ = main.o \
	  cpu/i8086/8086.o cpu/i8086/effective_addr.o \
	  cpu/i8086/opcodes.o cpu/i8086/prefetch.o \
	  cpu/i8086/instructions/arithmetic.o \
	  cpu/i8086/instructions/control_transfer.o \
	  cpu/i8086/instructions/data_transfer.o \
	  cpu/i8086/instructions/logic.o \
	  cpu/i8086/instructions/processor_control.o \
	  cpu/i8086/instructions/string_manipulation.o
OUTPUT = main
CFLAGS = -mmcu=atmega2560 -Wall -Wno-main -Wno-unused-function -O4 -fshort-enums -ffreestanding
LDFLAGS = -mrelax
LIBS = 
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

all: $(OUTPUT).elf $(OUTPUT).hex
listing: $(OUTPUT).elf $(OUTPUT).lss

debug: CFLAGS += -D__MAKE_DEBUG__
debug: $(OUTPUT).hex

.PHONY: clean

clean:
	@rm $(OBJ) *.elf *.hex

$(OUTPUT).elf: $(OBJ) rom_bios.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	@avr-size -C --mcu=atmega2560 main.elf

rom_bios.o: rom_bios.rom
	$(OBJCOPY) -I binary -O elf32-avr -B avr6 \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_rom_bios_rom_start=ROM_BIOS \
	--redefine-sym _binary_rom_bios_rom_size=ROM_BIOS_SIZE \
	--redefine-sym _binary_roM_bios_rom_end=ROM_BIOS_END \
	$(<) $(@)

$(OUTPUT).hex: $(OUTPUT).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(OUTPUT).lss: $(OUTPUT).elf
	$(OBJDUMP) -h -S -z $< > $@
