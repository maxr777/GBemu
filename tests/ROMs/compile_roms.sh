#!/bin/bash
set -e

cd "$(dirname "$0")"

rm -f cartridge_header_test.o
rm -f cartridge_header_test.gb

rgbasm -o cartridge_header_test.o cartridge_header_test.asm
rgblink -o cartridge_header_test.gb cartridge_header_test.o

# title: -t
# cgb flag: -c (compatible) / -C (only)
# sgb flag: -s
# new license: -k "XX"
# old license: -l 0xXX
# cartridge type: -m 0xXX
# RAM size: -r 0xXX
# ROM version: -n 0xXX
# destination (jp/non-jp): -j
# nintendo logo + checksums: -v
rgbfix -v -t "HEADER_TEST" -l 0xA5 -k "4Z" -m 0x01 -r 0x00 -n 0x03 -j cartridge_header_test.gb
