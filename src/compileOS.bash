#!/bin/bash

source ../PathSetter.bash
nasm bootload.asm

# Create disk images filled with all 0's
dd if=/dev/zero of=floppya.img bs=512 count=2880

# Add disk map and disk directory to disk image
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

# Compile loadFile.c
gcc -o loadFile loadFile.c 

# Use loadFile to copy message.txt
./loadFile message.txt

# compile userlib.c
bcc -ansi -c -o userlib.o userlib.c

#compile proc.c
bcc -ansi -c -o proc.o proc.c

# Assemble lib.asm, compile uprog1.c, link object files, and copy executable file to the disk
as86 lib.asm -o lib.o
bcc -ansi -c -o uprog1.o uprog1.c
ld86 -o uprog1 -d uprog1.o lib.o
./loadfile uprog1

# compile uprog2.c, link object files, and copy executable file to the disk
bcc -ansi -c -o uprog2.o uprog2.c
ld86 -o uprog2 -d uprog2.o lib.o
./loadfile uprog2

# compile shell.c, link object files, and copy executable file to the disk
bcc -ansi -c -o shell.o shell.c
ld86 -o shell -d shell.o userlib.o lib.o # link userlib to shell
./loadfile shell

bcc -ansi -c –o uprog6.o uprog6.c
ld86 -o uprog6 -d uprog6.o lib.o userlib.o
./loadfile uprog6


bcc -ansi -c –o uprog7.o uprog7.c
ld86 -o uprog7 -d uprog7.o lib.o userlib.o
./loadfile uprog7

# compile texteditor.c, link object files, and copy executable file to the disk
bcc -ansi -c -o texteditor.o texteditor.c
ld86 -o texteditor -d texteditor.o userlib.o lib.o
./loadfile texteditor

bcc -ansi -c -o uprog3.o uprog3.c
ld86 -o uprog3 -d uprog3.o userlib.o lib.o
./loadfile uprog3

# Copy boatload program to sector 0 of floppya.img disk image
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc seek=0

# Compile the kernel
bcc -ansi -c -o kernel.o kernel.c

# Assemble the kernel.asm
as86 kernel.asm -o kernel_asm.o

# Link kernel.o and kernel_asm.o files into executable kernel file
ld86 -o kernel -d kernel.o kernel_asm.o proc.o

# Copy kernel file to sector 3
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

# Copy message.txt to sector 30 of the disk
dd if=message.txt of=floppya.img bs=512 count=1 conv=notrunc seek=30

# Booting bochs
bochs -f opsys.bxrc

