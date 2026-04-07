#!/bin/bash

# Очистка предыдущих сборок
rm -f build/* iso_root/kernel

# Компиляция
gcc -g -O2 -pipe -Wall -Wextra -std=gnu11 -nostdinc -ffreestanding -fno-stack-protector -fno-stack-check \
    -fno-lto -fno-PIC -ffunction-sections -fdata-sections -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx \
    -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -I boot -MMD -MP -c kernel.c -o build/kernel.o

# Проверка успешности компиляции
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

gcc -g -O2 -pipe -Wall -Wextra -std=gnu11 -nostdinc -ffreestanding -fno-stack-protector -fno-stack-check \
    -fno-lto -fno-PIC -ffunction-sections -fdata-sections -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx \
    -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -I boot -MMD -MP -c memory.c -o build/memory.o

# Проверка успешности компиляции
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Линковка
ld -m elf_x86_64 -nostdlib -static -z max-page-size=0x1000 --gc-sections -T linker.ld  build/kernel.o build/memory.o -o iso_root/kernel

if [ $? -ne 0 ]; then
    echo "Linking failed!"
    exit 1
fi

echo "Kernel built successfully!"
xorriso -as mkisofs -R -r -J -b limine-bios-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	-apm-block-size 2048 --efi-boot limine-uefi-cd.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	iso_root -o ZenixOS.iso

if [ $? -ne 0 ]; then
    echo "ISO creation failed!"
    exit 1
fi

limine bios-install ZenixOS.iso

echo "ISO created successfully!"