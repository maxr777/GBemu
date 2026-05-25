#!/bin/bash

clear

gcc -o GBemu desktop_gbemu.c -s -O2 -lraylib -lGL -lm \
-lpthread -ldl -lrt -lX11 -std=c11 -Wall && clear && ./GBemu
