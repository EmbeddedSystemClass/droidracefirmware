###################################################
#             STM32 Standard Makefile	          #
# Timothy Ryan Hadwen 12/3/2016			  #
###################################################
#             Edit History                        #
# v0.1 - First Revision (Development)		  #
###################################################

# Variables for making this Makefile slightly easier to handle
EPIC_PATH=../../
HAL_PATH=$(EPIC_PATH)/Core/STM32F4xx_HAL_Driver
CMSIS_PATH=$(EPIC_PATH)/Core/CMSIS
FREERTOS_PATH=$(EPIC_PATH)/Core/FreeRTOS/Source
LWIP_PATH=$(EPIC_PATH)/Core/LwIP
PLATFORM_PATH=$(EPIC_PATH)/Platforms/nucleof429zi
PLATFORM_NET_PATH=$(PLATFORM_PATH)/net
TINYPRINTF_PATH=$(EPIC_PATH)/Core/tinyprintf
CLI_PATH=$(EPIC_PATH)/Core/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

PROJ_NAME = main

# Source files
SRCS = *.c
SRCS += $(HAL_PATH)/Src/*.c
SRCS += $(PLATFORM_PATH)/*.c
SRCS += $(FREERTOS_PATH)/*.c $(FREERTOS_PATH)/portable/GCC/ARM_CM4F/*.c $(FREERTOS_PATH)/portable/MemMang/heap_4.c
SRCS += $(FREERTOS_PATH)/CMSIS_RTOS/*.c
SRCS += $(LWIP_PATH)/src/core/*.c $(LWIP_PATH)/src/api/*.c $(LWIP_PATH)/src/core/ipv4/*.c $(LWIP_PATH)/src/netif/*.c $(LWIP_PATH)/system/OS/*.c
SRCS += $(PLATFORM_NET_PATH)/*.c
SRCS += $(CLI_PATH)/*.c
SRCS += $(TINYPRINTF_PATH)/*.c

###################################################
#        These flags shouldn't change		  #
###################################################
CFLAGS  = -g -T$(PLATFORM_PATH)/STM32F429ZITx_FLASH.ld -Wmaybe-uninitialized
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
###################################################


###################################################
#	    These flags CAN change		  #
###################################################
CFLAGS += -I.
CFLAGS += -DSTM32F429xx -DENABLE_DEBUGGER_UART -DENABLE_ETHERNET -DFREERTOS #-DUSE_DHCP
CFLAGS += -I$(CMSIS_PATH)/Include -I$(HAL_PATH)/Inc
CFLAGS += -I$(FREERTOS_PATH)/include -I$(FREERTOS_PATH)/portable/GCC/ARM_CM4F
CFLAGS += -I$(FREERTOS_PATH)/CMSIS_RTOS
CFLAGS += -I$(PLATFORM_PATH)
CFLAGS += -I$(LWIP_PATH)/src/include -I$(LWIP_PATH)/src/include/lwip -I$(LWIP_PATH)/src/include/ipv4 -I$(LWIP_PATH)/src/include/ipv6 -I$(LWIP_PATH)/system/OS
CFLAGS += -I$(TINYPRINTF_PATH)
CFLAGS += -I$(CLI_PATH)
CFLAGS += -I$(PLATFORM_NET_PATH)


# Startup File
SRCS += $(PLATFORM_PATH)/startup_stm32f429xx.s

# Output file names
OBJS = $(SRCS:.c=.o)

###################################################
.PHONY: lib proj

all: proj

install: all prog

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lm -lc -lgcc -lnosys #-lstm32f4
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

prog:
	st-flash write $(PROJ_NAME).bin 0x8000000


clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
