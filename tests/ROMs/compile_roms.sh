#!/bin/bash
set -e

cd "$(dirname "$0")"

rgbasm -o cartridge_header_test.o cartridge_header_test.asm
rgblink -o cartridge_header_test.gb cartridge_header_test.o

# -f hg to update checksums without overwriting the logo region
rgbfix -f hg -t "HEADER_TEST" -l 0xA5 -k "4Z" -m 0x01 -r 0x00 -n 0x03 -j cartridge_header_test.gb
