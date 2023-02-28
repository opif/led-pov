MCU=attiny24a
dMCU=attiny24
F_CPU=8000000
CC=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-std=c++11 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.cpp src/*.cpp
HEADERS=src/*.h
SIZE=avr-size -C


all: ${TARGET}.hex

asm: ${SRCS} ${HEADERS}
	${CC} ${CFLAGS} -S ${SRCS}

${TARGET}.bin: Makefile ${SRCS} ${HEADERS}
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${SIZE} --mcu ${MCU} "$@"

${TARGET}.hex: ${TARGET}.bin
	${OBJCOPY} -j .text -j .data -O ihex $? "$@"

flash: ${TARGET}.hex
	avrdude -p ${dMCU} -c usbasp -U flash:w:${TARGET}.hex:i -P usb

fuse:
	avrdude -p ${dMCU} -c usbasp -U lfuse:w:0x7A:m -P usb

clean:
	rm -f *.bin *.hex *.s
