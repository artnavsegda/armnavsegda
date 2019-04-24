CC = arm-none-eabi-gcc
# LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
# CFLAGS += -mcpu=cortex-m3 -mthumb
# LDFLAGS += -nostdlib

%.elf: %.o
	$(CC) -o $@ $<

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

