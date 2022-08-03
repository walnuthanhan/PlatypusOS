#!/bin/bash

source ../PathSetter.bash

# Create disk images filled with all 0's
dd if=/dev/zero of=floppya.img bs=512 count=2880

# Copy boatload program to sector 0 of floppya.img disk image
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc seek=0

# Compile the kernel
bcc -ansi -c -o kernel.o kernel.c

# Assemble the kernel.asm
as86 kernel.asm -o kernel_asm.o

# Link kernel.o and kernel_asm.o files into executable kernel file
ld86 -o kernel -d kernel.o kernel_asm.o

# Copy kernel file to sector 3
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

# Booting bochs
bochs -f opsys.bxrc

