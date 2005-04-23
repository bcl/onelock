# makfile configuration

# REMINDER: msp430-gdbproxy --debug --port=2000 msp430
# REMINDER: msp430-gdb onelock.elf

NAME            = onelock
OBJECTS         = main.o interrupts.o dco.o onewire.o
CPU             = msp430x1101

CFLAGS          = -mmcu=${CPU} -O2 -Wall  -g

#switch the compiler (for the internal make rules)
CC              = msp430-gcc

.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is runwithout args
all: ${NAME}.elf ${NAME}.a43 ${NAME}.lst

#confgigure the next line if you want to use the serial download
download: download-jtag
#download: download-bsl

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@

download-jtag: all
	msp430-jtag -e ${NAME}.elf

download-bsl: all
	msp430-bsl -e ${NAME}.elf

clean:
	rm -f ${NAME} ${NAME}.a43 ${NAME}.lst *.o

#backup archive
dist:
	tar czf dist.tgz *.c *.h *.txt makefile

#dummy target as dependecy if something has to be build everytime
FORCE:

#project dependencies
main.o: main.c dco.h interrupts.h hardware.h Makefile
interrupts.o: hardware.h interrupts.h Makefile
dco.o: dco.h hardware.h Makefile
onewire.o: onewire.h hardware.h Makefile
