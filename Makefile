MCU=attiny13
FUSE_L=0x7A
FUSE_H=0xFF
F_CPU=128000
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main

SRCS = main.c 
	
all: 
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex	
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf





flash:
	${AVRDUDE} -p ${MCU} -c usbasp -v -v -v -v -u -B 3 -U flash:w:${TARGET}.hex:i -F -P usb


fuse:
	$(AVRDUDE) -p ${MCU} -c usbasp -v -B 3 hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m
	
clean:
	rm -f *.c~ *.o *.elf *.hex
