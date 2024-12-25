PREFIX := i386-elf-7.5.0-Linux-x86_64/bin/i386-elf# Change this
boot.bin: boot/boot.asm 
	nasm -fbin $^ -o $@

boot/%.o: boot/%.asm
	nasm -felf32 -o $@ $<

kernel/%.o: kernel/%.c
	$(PREFIX)-gcc $^ -c -o $@ -Wall -nostdlib  -ffreestanding -fno-stack-protector -nostdlib -mno-red-zone 

utils/%.o: utils/%.c
	$(PREFIX)-gcc $^ -c -o $@ -Wall -nostdlib  -ffreestanding -fno-stack-protector -nostdlib -mno-red-zone 


drivers/%.o: drivers/%.c
	$(PREFIX)-gcc $^ -c -o $@ -Wall -nostdlib  -ffreestanding -fno-stack-protector -nostdlib -mno-red-zone 

drivers/%.o: drivers/%.asm
	nasm -felf32 -o $@ $<

kernel.bin: boot/entry.o kernel/kernel.o utils/ports.o drivers/io.o drivers/isr.o drivers/video.o
	$(PREFIX)-ld $^ --oformat binary -Ttext 0x7e00 -o $@

os.bin: boot.bin kernel.bin
	cat $^ > $@

run: os.bin
	qemu-system-i386 -fda $< -D log.txt -d int

clean: 
	rm boot/entry.o kernel/*.o drivers/*.o utils/*.o *.bin -f