rm -rf build/* 2>/dev/null

echo "Building..."
nasm -f bin boot/boot.asm -o build/boot.bin

echo "Run QEMU"
qemu-system-x86_64 --drive format=raw,file=build/boot.bin
