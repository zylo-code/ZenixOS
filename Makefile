# Компилятор и флаги
CC = gcc
LD = ld
CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=c11 \
         -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
         -fno-stack-protector -fno-pic -g -fno-exceptions \
         -Ikernel/include -Ikernel/arch/x86_64 -Ikernel/core \
         -Ikernel/drivers -Ikernel/shell

LDFLAGS = -nostdlib -z max-page-size=0x1000 -T linker.ld

# Исходники
C_SOURCES = $(shell find kernel -name '*.c')
ASM_SOURCES = $(shell find kernel -name '*.s')

OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

# Цели
.PHONY: all clean run iso

all: iso

iso: iso_root/kernel
	xorriso -as mkisofs -R -r -J \
		-b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 \
		-boot-info-table -hfsplus -apm-block-size 2048 \
		--efi-boot limine-uefi-cd.bin -efi-boot-part \
		--efi-boot-image --protective-msdos-label \
		iso_root -o ZenixOS.iso
	limine bios-install ZenixOS.iso
	@echo "ISO created: ZenixOS.iso"

iso_root/kernel: $(OBJ) linker.ld
	@mkdir -p iso_root
	$(LD) $(LDFLAGS) $(OBJ) -o $@
	@echo "Kernel linked: $@"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	nasm -f elf64 $< -o $@

run: iso
	qemu-system-x86_64 \
		-M q35 \
		-drive if=pflash,unit=0,format=raw,file=ovmf-code-x86_64.fd,readonly=on \
		-cdrom ZenixOS.iso \
		-m 2G \
		-serial stdio \
		-no-reboot \
		-d cpu_reset,guest_errors

debug: iso
	qemu-system-x86_64 \
		-M q35 \
		-drive if=pflash,unit=0,format=raw,file=ovmf-code-x86_64.fd,readonly=on \
		-cdrom ZenixOS.iso \
		-m 2G \
		-s -S \
		-no-reboot &
	gdb -ex "target remote localhost:1234" \
		-ex "symbol-file iso_root/kernel"

clean:
	rm -rf $(OBJ) iso_root/kernel ZenixOS.iso
	find . -name '*.o' -delete