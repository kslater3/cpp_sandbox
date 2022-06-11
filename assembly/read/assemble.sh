#!/bin/bash
yasm -g dwarf2 -f elf64 read.asm -l read.lst
ld -g -o read read.o
