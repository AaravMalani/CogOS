# CogOS: A 32 bit Operating System 

## Features
- PS/2 keyboard support (via 8042 keyboard controller)
- Custom bootloader using BIOS (or CSM in the case of newer CPUs)

## Running
1. Install the required tools (`sudo apt install nasm gcc qemu-system-x86 make`)
2. Create a [GCC cross-compiler](https://wiki.osdev.org/GCC_Cross-Compiler) for the `i386-elf` target architecture (i486, i586 and i686 work too, but use `qemu-system-x86_64` in the Makefile) 
3. Set the `PREFIX` in the Makefile to `bin-location/ix86-elf`, for example `$HOME/opt/cross/bin/i686-elf` following the previous tutorial
4. Execute `make run`

## Goals
- Reaching userland (setting up paging, building a page frame allocator, etc.)
- Creating a process scheduler
- Filesystem support (probably FAT12 or FAT32 at first)
- Networking
- Adding mouse support
- Reaching long mode