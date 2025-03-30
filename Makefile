PREFIX := i386-elf-7.5.0-Linux-x86_64/bin/i386-elf# Change this
CFLAGS := -Iinclude -Wall -nostdlib  -ffreestanding -fno-stack-protector -nostdlib -mno-red-zone
boot.bin: boot/boot.asm 
	nasm -fbin $^ -o $@

boot/%.o: boot/%.asm
	nasm -felf32 -o $@ $<

kernel/%.o: kernel/%.c
	$(PREFIX)-gcc $^ -c -o $@ $(CFLAGS)

utils/%.o: utils/%.c
	$(PREFIX)-gcc $^ -c -o $@ $(CFLAGS)


drivers/%.o: drivers/%.c
	$(PREFIX)-gcc $^ -c -o $@ $(CFLAGS)

drivers/io/%.o: drivers/io/%.c
	$(PREFIX)-gcc $^ -c -o $@ $(CFLAGS)

drivers/%.o: drivers/%.asm
	nasm -felf32 -o $@ $<

kernel.bin: boot/entry.o kernel/kernel.o utils/ports.o drivers/io/interrupts.o drivers/io/pic.o drivers/io/floppy.o drivers/isr.o drivers/video.o drivers/mm.o drivers/paging.o
	$(PREFIX)-ld $^ -Tlinker.ld -o $@

os.bin: boot.bin kernel.bin
	cat $^ > $@
	dd if=/dev/zero of=$@ bs=1 count=1 seek=2072575

run: os.bin
	qemu-system-i386 -fda $< -D log.txt -d int
	
clean: 
	rm boot/entry.o kernel/*.o drivers/*.o drivers/io/*.o utils/*.o *.bin -f
