#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

KERNEL = CC_kernel

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo 编译汇编文件 $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o $(KERNEL) 

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) $(KERNEL)

.PHONY:update_image
update_image:
	sudo losetup /dev/loop21 CCOS.img -o 1048576
	sudo mount /dev/loop21 /mnt/kernel
	sudo cp $(KERNEL) /mnt/kernel/$(KERNEL)
	sleep 1
	sudo losetup -d /dev/loop21
	sudo umount /mnt/kernel

.PHONY:mount_image
mount_image:
	sudo losetup /dev/loop21 CCOS.img -o 1048576
	sudo mount /dev/loop21 /mnt/kernel

.PHONY:umount_image
umount_image:
	sudo losetup -d /dev/loop21
	sudo umount /mnt/kernel

.PHONY:qemu
qemu:
	qemu -hda CCOS.img -boot a

.PHONY: bochs
bochs:
	bochs -f tools/bochsrc.txt

.PHONY:debug
debug:
	qemu -S -s -hda CCOS.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit
