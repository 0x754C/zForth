SRCS += boot_c906a.s boot_c906b.s
SRCS += main_c906a.c main_c906b.c
SRCS += uart.c

VPATH := ../zforth

SRCS += ../zforth/zforth.c
INCS += -I. -I../zforth

VARIANT ?= cv181x

FW_NAME ?= $(VARIANT)-zforth
CROSS_COMPILE ?= riscv64-unknown-elf-
CC = $(CROSS_COMPILE)gcc
OD = $(CROSS_COMPILE)objdump
OC = $(CROSS_COMPILE)objcopy
SZ = $(CROSS_COMPILE)size

LINK_SCRIPT ?= $(VARIANT)-link.ld

CFLAGS += -Wall -Wextra \
	-Wno-attributes \
	-Wno-int-conversion \
	-Wno-unused-parameter \
	-Wno-clobbered \
        -march=rv64imac_zicsr -mabi=lp64 \
        -Os -g3 \
        -fdata-sections -ffunction-sections -Wl,--gc-sections \
        -nostartfiles \
        -T $(LINK_SCRIPT)

all: $(FW_NAME).elf $(FW_NAME).bin $(FW_NAME).dis $(FW_NAME).E fip.bin
	$(SZ) $(FW_NAME).elf

$(FW_NAME).elf:
	$(CC) $(CFLAGS) $(INCS) $(SRCS) $(LIBS) -o $(FW_NAME).elf

$(FW_NAME).E:
	$(CC) $(CFLAGS) $(INCS) $(SRCS) $(LIBS) -E > $(FW_NAME).E

$(FW_NAME).dis: $(FW_NAME).elf
	$(OD) -a -s -S $(FW_NAME).elf > $(FW_NAME).dis

$(FW_NAME).bin: $(FW_NAME).elf
	$(OC) -O binary $(FW_NAME).elf $(FW_NAME).bin

clean:
	rm -f *.out $(FW_NAME).E $(FW_NAME).dis $(FW_NAME).elf $(FW_NAME).bin
	rm -f blcp.bin fip.bin chip_conf.bin

chip_conf.bin:
	python3 chip_conf.py chip_conf.bin

blcp.bin:
	touch blcp.bin

fip.bin: $(FW_NAME).bin chip_conf.bin blcp.bin
	python3 fiptool.py -v genfip \
		--CHIP_CONF=chip_conf.bin \
		--BL2=$(FW_NAME).bin \
		--BLCP_IMG_RUNADDR=0x05200200 \
		--BLCP_PARAM_LOADADDR=0 \
		--BLCP=blcp.bin \
		fip.bin
