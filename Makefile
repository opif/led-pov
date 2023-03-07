MCU=attiny24a
dMCU=attiny24
F_CPU=1000000
CC=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-std=c++17 -Wall -Og -mmcu=${MCU} -DF_CPU=${F_CPU} --param=min-pagesize=0
TARGET=main
SRCS=main.cpp src/*.cpp
HEADERS=src/*.h
SIZE=avr-size -C
AVRDUDE=avrdude -c stk5000v2 -b 19200 -P /dev/ttyACM0


all: ${TARGET}.hex

asm: ${SRCS} ${HEADERS}
	${CC} ${CFLAGS} -S ${SRCS}

${TARGET}.bin: Makefile ${SRCS} ${HEADERS}
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${SIZE} --mcu ${MCU} "$@"

${TARGET}.hex: ${TARGET}.bin
	${OBJCOPY} -j .text -j .data -O ihex $? "$@"

flash: ${TARGET}.hex
	${AVRDUDE} -p ${dMCU} -U flash:w:${TARGET}.hex:i

fuse:
	${AVRDUDE} -p ${dMCU} -U lfuse:w:0x62:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m

clean:
	rm -f *.bin *.hex *.s
