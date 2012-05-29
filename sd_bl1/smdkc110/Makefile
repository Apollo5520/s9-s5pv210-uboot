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

TARGET0    = skdv210-BL1-sd0.bin
O_TARGET0  = skdv210-BL1-sd0.elf
TARGET2    = skdv210-BL1-sd2.bin
O_TARGET2  = skdv210-BL1-sd2.elf
#OBJS      += Main.o usbd-otg-hs.o otgdev.o
OBJS0      = Init.o main-sd0.o
OBJS2      = Init.o main-sd2.o 

ifdef CONFIG_EVT1
FLAG4REV  = S5PC110_EVT1
TEXT_BASE = 0xD0020010
BL1_LDS   = skdv210-BL1.lds
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
all: $(TARGET0) $(TARGET2) $(MKBL1) $(TARGET0).padding $(TARGET2).padding sd_fdisk

$(TARGET0):      $(OBJS0)
	$(LD) -T $(BL1_LDS) -Ttext $(TEXT_BASE) -o $(O_TARGET0) -Map skdv210-BL1-sd0.map $(OBJS0)
	$(OBJCOPY) -O binary $(O_TARGET0) $(TARGET0)

$(TARGET0).padding:	$(TARGET0) $(MKBL1)
	./$(MKBL1) $(TARGET0) $(TARGET0).padding
#	rm -f skdv210-sd0.bin
#	cat $(TARGET0).padding >skdv210-sd0.bin
#	cat ../../u-boot.bin >>skdv210-sd0.bin

$(TARGET2):      $(OBJS2)
	$(LD) -T $(BL1_LDS) -Ttext $(TEXT_BASE) -o $(O_TARGET2) -Map skdv210-BL1-sd2.map $(OBJS2)
	$(OBJCOPY) -O binary $(O_TARGET2) $(TARGET2)

$(TARGET2).padding:	$(TARGET2) $(MKBL1)
	./$(MKBL1) $(TARGET2) $(TARGET2).padding
#	rm -f skdv210-sd2.bin
#	cat $(TARGET2).padding >skdv210-sd2.bin
#	cat ../../u-boot.bin >>skdv210-sd2.bin

$(MKBL1):		mkbl1.c
	gcc $(CFLAGS_HOST) -o $(MKBL1) mkbl1.c
	
sd_fdisk:		sd_fdisk.c
	gcc $(CFLAGS_HOST) -o sd_fdisk sd_fdisk.c

main-sd0.o:			main-sd0.c
	$(CC) $(CFLAGS) -c -o main-sd0.o main-sd0.c

main-sd2.o:			main-sd2.c
	$(CC) $(CFLAGS) -c -o main-sd2.o main-sd2.c

usbd-otg-hs.o: usbd-otg-hs.c
	$(CC) $(CFLAGS) -c -o usbd-otg-hs.o usbd-otg-hs.c

otgdev.o: otgdev.c
	$(CC) $(CFLAGS) -c -o otgdev.o otgdev.c

Init.o:			Init.s
	$(AS) $(AFLAGS) -o Init.o Init.s

dep:
	gccmakedep $(OBJS0:.o=.c)(OBJS0:.o=.s)
	gccmakedep $(OBJS2:.o=.c)(OBJS2:.o=.s)

clean:
	rm -rf $(OBJS0) $(OBJS2) $(MKBL1) sd_fdisk *.bin *.elf *.padding *.map

# DO NOT DELETE

