#!/bin/bash

set -e

# Always run from the script directory so relative paths work.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Очистка предыдущих сборок
rm -f build/* iso_root/kernel

# Компиляция
gcc -ffreestanding -O2 -Wall -Wextra -fno-exceptions -std=c11 \
    -Isrc -Isrc/drivers -Isrc/shell -Isrc/boot/limine \
    -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -fno-stack-protector -fno-pic -g -c kernel.c -o build/kernel.o

# Проверка успешности компиляции
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

gcc -ffreestanding -O2 -Wall -Wextra -fno-exceptions -std=c11 \
    -Isrc -Isrc/drivers -Isrc/shell -Isrc/boot/limine \
    -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -fno-stack-protector -fno-pic -g -c system/memory.c -o build/memory.o

gcc -ffreestanding -O2 -Wall -Wextra -fno-exceptions -std=c11 \
    -Isrc -Isrc/drivers -Isrc/shell -Isrc/boot/limine \
    -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -fno-stack-protector -fno-pic -g -c drivers/terminal/terminal.c -o build/terminal.o

gcc -ffreestanding -O2 -Wall -Wextra -fno-exceptions -std=c11 \
    -Isrc -Isrc/drivers -Isrc/shell -Isrc/boot/limine \
    -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -fno-stack-protector -fno-pic -g -c drivers/keyboard/keyboard.c -o build/keyboard.o

gcc -ffreestanding -O2 -Wall -Wextra -fno-exceptions -std=c11 \
    -Isrc -Isrc/drivers -Isrc/shell -Isrc/boot/limine \
    -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -fno-stack-protector -fno-pic -g -c io/io.c -o build/io.o

gcc -ffreestanding -O2 -Wall -Wextra -fno-exceptions -std=c11 \
    -Isrc -Isrc/drivers -Isrc/shell -Isrc/boot/limine \
    -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -fno-stack-protector -fno-pic -g -c shell/shell.c -o build/shell.o

# Проверка успешности компиляции
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Линковка
ld -nostdlib -z max-page-size=0x1000 -T linker.ld \
    build/*.o -o iso_root/kernel

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