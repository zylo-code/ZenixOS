#!/bin/bash

set -e

# Always run from the script directory so relative paths work.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

qemu-system-x86_64 \
	-M q35 \
    -drive if=pflash,unit=0,format=raw,file=ovmf-code-x86_64.fd,readonly=on \
	-cdrom ZenixOS.iso \
	-m 2G