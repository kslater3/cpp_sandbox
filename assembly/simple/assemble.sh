#!/bin/bash
yasm -g dwarf2 -f elf64 simple.asm -l simple.lst
ld -g -o simple simple.o
