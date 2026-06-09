# ==============================================================
#  Direktorijumi
# ==============================================================
SRC_DIR    = boot/src
PRINT_DIR  = boot/print
DATA_DIR   = boot/data
DISK_DIR   = boot/disk
PM_DIR     = boot/protected_mode
KERNEL_DIR = boot/kernel
CPU_DIR    = boot/kernel/cpu
DRIVER_DIR = boot/kernel/driver
BUILD_DIR  = build

# ==============================================================
#  Output fajlovi
# ==============================================================
BOOT_BIN         = $(BUILD_DIR)/boot.bin
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernel_entry.o
KERNEL_BIN       = $(BUILD_DIR)/kernel.bin
KERNEL_DIS       = $(BUILD_DIR)/kernel.dis
OS_IMAGE         = $(BUILD_DIR)/os-image.bin
LINKER_SCRIPT    = linker.ld

# ==============================================================
#  Flagovi
# ==============================================================
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib \
	-I$(KERNEL_DIR) -I$(DRIVER_DIR) -I$(CPU_DIR)

# ==============================================================
#  Object fajlovi
# ==============================================================
KERNEL_OBJS = \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/screen.o \
	$(BUILD_DIR)/low_level.o \
	$(BUILD_DIR)/util.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/isr.o \
	$(BUILD_DIR)/interrupt.o \
	$(BUILD_DIR)/isr_asm.o \
	$(BUILD_DIR)/pic.o \
	$(BUILD_DIR)/irq.o \
	$(BUILD_DIR)/irq_asm.o \
	$(BUILD_DIR)/timer.o \
	$(BUILD_DIR)/keyboard.o

# ==============================================================
#  Default
# ==============================================================
all: $(OS_IMAGE)

run: all
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

# ==============================================================
#  Disk image
# ==============================================================
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@
	truncate -s 1474560 $@

# ==============================================================
#  Bootloader
# ==============================================================
$(BOOT_BIN): $(SRC_DIR)/boot.asm | $(BUILD_DIR)
	nasm -f bin \
	-I$(SRC_DIR)/ \
	-I$(PRINT_DIR)/ \
	-I$(DATA_DIR)/ \
	-I$(DISK_DIR)/ \
	-I$(PM_DIR)/ \
	-I$(KERNEL_DIR)/ \
	$< -o $@

# ==============================================================
#  Kernel linkovanje
# ==============================================================
$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJS) $(LINKER_SCRIPT)
	ld -m elf_i386 -T $(LINKER_SCRIPT) --oformat binary -o $@ $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJS)

# ==============================================================
#  ASM fajlovi
# ==============================================================
$(KERNEL_ENTRY_OBJ): $(KERNEL_DIR)/kernel_entry.asm | $(BUILD_DIR)
	nasm -f elf32 $< -o $@

$(BUILD_DIR)/interrupt.o: $(CPU_DIR)/interrupt.asm | $(BUILD_DIR)
	nasm -f elf32 $< -o $@

$(BUILD_DIR)/isr_asm.o: $(CPU_DIR)/isr.asm | $(BUILD_DIR)
	nasm -f elf32 $< -o $@
	
$(BUILD_DIR)/irq_asm.o: $(CPU_DIR)/irq.asm | $(BUILD_DIR)
	nasm -f elf32 $< -o $@


# ==============================================================
#  C fajlovi
# ==============================================================
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/screen.o: $(DRIVER_DIR)/screen.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/low_level.o: $(KERNEL_DIR)/low_level.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/util.o: $(KERNEL_DIR)/util.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt.o: $(CPU_DIR)/idt.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/isr.o: $(CPU_DIR)/isr.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)/pic.o: $(CPU_DIR)/pic.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)/irq.o: $(CPU_DIR)/irq.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)/timer.o: $(DRIVER_DIR)/timer.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)/keyboard.o: $(DRIVER_DIR)/keyboard.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

# ==============================================================
#  Build folder
# ==============================================================
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ==============================================================
#  Debug
# ==============================================================
$(KERNEL_DIS): $(KERNEL_BIN)
	ndisasm -b 32 $< > $@

dis: $(KERNEL_DIS)

# ==============================================================
#  Clean
# ==============================================================
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run dis clean
