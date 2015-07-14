GRFT=GRLIB_ADD_WIDE_FONT_SUPPORT
MDL_NUM_DEF=TPD_280=0x280
include default.mk
include makedefs.mk

CFLAGS+=-DtBoolean=bool
CFLAGS+=-include stdbool.h
CFLAGS+=-include stdint.h
CFLAGS+=-D${MDL_NUM_DEF}
CFLAGS+=-DTPD_USE_NO_MOS
CFLAGS+=-D${GRFT}

all: clean
all: ${TARGET}.axf

clean:
	@-rm -f *.o
	@-rm -f *.d

${TARGET}.axf: main.o
include frames.mk


SCATTERgcc_${TARGET}=default.ld
LDFLAGSgcc_${TARGET}=-Map ${TARGET}.map

${TARGET}.axf: default.ld


update:
	echo sflash.exe ${TARGET}.bin -d -p 0x1800 -r 0x1800 -d -c ${PORT} -s 60	
	sflash.exe ${TARGET}.bin -d -p 0x1800 -r 0x1800 -d -c ${PORT} -s 60	

jtag:	
	lmflash.exe -r -o 0x1800 ${TARGET}.bin
