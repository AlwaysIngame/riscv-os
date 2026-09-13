LIMINE_BINARY_URL := https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz
OVMF_URL := https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/edk2-ovmf.tar.gz

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/kernel.elf

LINK_SCRIPT := $(SRC_DIR)/link.ld
SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

CC := clang
CFLAGS := -target riscv64-unknown-elf -std=c23 -Wall -Werror -Wextra -g -O3 -flto -ffreestanding -nostdlib -Iinclude
LDFLAGS := -fuse-ld=lld -flto

kernel: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(LINK_SCRIPT)
	$(CC) -T $(LINK_SCRIPT) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(BUILD_DIR)

misc/limine.conf:

${BUILD_DIR}/disk/EFI/BOOT/BOOTRISCV64.EFI:
	mkdir -p $(BUILD_DIR)/disk/EFI/BOOT
	curl -sL $(LIMINE_BINARY_URL) | tar -xzf - -O limine-binary/BOOTRISCV64.EFI > $@

${BUILD_DIR}/ovmf-code-riscv64.fd:
	mkdir -p $(BUILD_DIR)
	curl -sL $(OVMF_URL) | tar -xzf - -O edk2-ovmf/ovmf-code-riscv64.fd > $@
	dd if=/dev/zero of=$@ bs=1 count=0 seek=33554432

disk: ${BUILD_DIR}/disk/EFI/BOOT/BOOTRISCV64.EFI misc/limine.conf ${TARGET}
	cp misc/limine.conf $(BUILD_DIR)/disk/
	cp ${TARGET} $(BUILD_DIR)/disk/

run: disk ${BUILD_DIR}/ovmf-code-riscv64.fd
	qemu-system-riscv64 -M virt -smp 4 -m 2G -nographic \
	-drive if=pflash,file=${BUILD_DIR}/ovmf-code-riscv64.fd,format=raw,unit=0 \
	-drive file=fat:rw:$(BUILD_DIR)/disk/,format=raw

.PHONY: kernel clean run disk
