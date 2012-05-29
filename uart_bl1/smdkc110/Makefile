#
# BL1 of U-Boot for OneNAND
#

sinclude ../../include/autoconf.mk

CROSS_COMPILE	= /usr/local/arm/arm-2010q1/bin/arm-none-linux-gnueabi-

AS        = $(CROSS_COMPILE)as
LD        = $(CROSS_COMPILE)ld
CC        = $(CROSS_COMPILE)gcc
AR        = $(CROSS_COMPILE)ar
NM        = $(CROSS_COMPILE)nm
STRIP     = $(CROSS_COMPILE)strip
OBJCOPY   = $(CROSS_COMPILE)objcopy
OBJDUMP   = $(CROSS_COMPILE)objdump

MKBL1     = mkbl1

TARGET    = skdv210-BL1-uart.bin
O_TARGET  = skdv210-BL1-uart.elf
OBJS      = Init.o
OBJS      += Main.o

ifdef CONFIG_EVT1
FLAG4REV  = S5PC110_EVT1
TEXT_BASE = 0xD0020010
BL1_LDS   = skdv210-BL1-uart.lds
else
FLAG4REV  = S5PC110_EVT0
TEXT_BASE = 0xD0030000
BL1_LDS   = BL1.lds
endif

ifdef CONFIG_MCP_SINGLE
AFLAGS        = --defsym $(FLAG4REV)=0x1 --defsym S5PV210=0x1
CFLAGS        = -I../../include -Os -nostdlib -D$(FLAG4REV) -DS5PV210
#CFLAGS       = -g -Os -fno-strict-aliasing  -fno-common -ffixed-r8 -msoft-float -D$(FLAG4REV) -DS5PV210 -D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE_ADD) $(INCLUDE) -fno-builtin -ffreestanding -nostdinc -isystem /usr/local/arm/arm-2010q1/lib/gcc/arm-none-linux-gnueabi/4.4.1/include -pipe -DCONFIG_ARM -D__ARM__ -mabi=apcs-gnu -mno-thumb-interwork -Wall -Wstrict-prototypes -fno-stack-protector -c -o
CFLAGS_HOST   = -D$(FLAG4REV) -DS5PV210
else
AFLAGS        = --defsym $(FLAG4REV)=0x1
CFLAGS        = -I../../include -Os -D$(FLAG4REV)
CFLAGS_HOST		= -D$(FLAG4REV)
endif

#
all: $(TARGET)

$(TARGET):      $(OBJS)
	$(LD) -T $(BL1_LDS) -Ttext $(TEXT_BASE) -o $(O_TARGET) -Map skdv210-BL1-uart.map $(OBJS)
	$(OBJCOPY) -O binary $(O_TARGET) $(TARGET)

Main.o:			Main.c
	$(CC) $(CFLAGS) -c -o Main.o Main.c

Init.o:			Init.s
	$(AS) $(AFLAGS) -o Init.o Init.s

dep:
	gccmakedep $(OBJS:.o=.c)(OBJS:.o=.s)

clean:
	rm -rf $(OBJS) $(TARGET) $(O_TARGET) skdv210-BL1-uart.map

# DO NOT DELETE

