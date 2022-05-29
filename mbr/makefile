BUILD_DIR = ./obj
ENTRY_POINT = 0xc0001500
AS = nasm
CC = gcc
LD = ld
KERNEL = src/lib/kernel
USER = src/lib/user
FS = src/lib/fs
DEVICE = src/device
SLIB = src/lib
LIB = -I src/lib/kernel/ -I src/device/ -I src/lib/user/ -I src/lib/ -I src/lib/fs/
ASFLAGS = -f elf
CFLAGS = -m32 $(LIB) -c -fno-builtin 
LDFLAGS = -m elf_i386 -Ttext $(ENTRY_POINT) -e main -Map $(BUILD_DIR)/kernel.map
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/init.o $(BUILD_DIR)/interrupt.o $(BUILD_DIR)/timer.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/print.o $(BUILD_DIR)/debug.o $(BUILD_DIR)/string.o $(BUILD_DIR)/bitmap.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/thread.o  $(BUILD_DIR)/list.o $(BUILD_DIR)/switch.o $(BUILD_DIR)/sync.o $(BUILD_DIR)/console.o $(BUILD_DIR)/keyboard.o $(BUILD_DIR)/ioqueue.o $(BUILD_DIR)/tss.o $(BUILD_DIR)/process.o $(BUILD_DIR)/syscall-init.o $(BUILD_DIR)/syscall.o $(BUILD_DIR)/mystdio.o $(BUILD_DIR)/kstdio.o $(BUILD_DIR)/ide.o $(BUILD_DIR)/dir.o $(BUILD_DIR)/file.o $(BUILD_DIR)/fs.o $(BUILD_DIR)/inode.o $(BUILD_DIR)/fork.o $(BUILD_DIR)/shell.o $(BUILD_DIR)/buildin_cmd.o $(BUILD_DIR)/exec.o $(BUILD_DIR)/wait_exit.o $(BUILD_DIR)/pipe.o
 
$(BUILD_DIR)/main.o: $(KERNEL)/main.c $(KERNEL)/print.h $(KERNEL)/stdint.h $(KERNEL)/init.h $(KERNEL)/debug.h $(KERNEL)/memory.h $(KERNEL)/thread.h $(USER)/tss.h $(USER)/process.h $(KERNEL)/kstdio.h $(SLIB)/buildin_cmd.h
	$(CC) $(CFLAGS) $< -o $@
$(BUILD_DIR)/init.o: $(KERNEL)/init.c $(KERNEL)/print.h $(KERNEL)/interrupt.h $(KERNEL)/init.h $(DEVICE)/timer.h $(KERNEL)/memory.h $(DEVICE)/keyboard.h $(USER)/tss.h $(DEVICE)/ide.h 
	$(CC) $(CFLAGS) $< -o $@
$(BUILD_DIR)/interrupt.o: $(KERNEL)/interrupt.c $(KERNEL)/interrupt.h $(KERNEL)/stdint.h $(KERNEL)/global.h $(KERNEL)/io.h 
	$(CC) $(CFLAGS) $< -o $@ 
$(BUILD_DIR)/timer.o: $(DEVICE)/timer.c $(DEVICE)/timer.h $(KERNEL)/stdint.h $(KERNEL)/io.h $(KERNEL)/print.h 
	$(CC) $(CFLAGS) $< -o $@ 
$(BUILD_DIR)/debug.o: $(KERNEL)/debug.c $(KERNEL)/print.h $(KERNEL)/stdint.h $(KERNEL)/debug.h $(KERNEL)/interrupt.h 
	$(CC) $(CFLAGS) $< -o $@
$(BUILD_DIR)/string.o: $(KERNEL)/string.c $(KERNEL)/string.h $(KERNEL)/stdint.h $(KERNEL)/debug.h $(KERNEL)/global.h 
	$(CC) $(CFLAGS) $< -o $@
$(BUILD_DIR)/bitmap.o: $(KERNEL)/bitmap.c $(KERNEL)/bitmap.h $(KERNEL)/stdint.h $(KERNEL)/debug.h  
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/memory.o: $(KERNEL)/memory.c $(KERNEL)/memory.h $(KERNEL)/stdint.h $(KERNEL)/bitmap.h  $(KERNEL)/print.h $(KERNEL)/debug.h 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/thread.o: $(KERNEL)/thread.c $(KERNEL)/thread.h $(KERNEL)/stdint.h $(KERNEL)/string.h  $(KERNEL)/memory.h $(KERNEL)/global.h $(KERNEL)/list.h 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/list.o: $(KERNEL)/list.c $(KERNEL)/list.h $(KERNEL)/global.h $(KERNEL)/interrupt.h   
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/sync.o: $(KERNEL)/sync.c $(KERNEL)/sync.h $(KERNEL)/list.h $(KERNEL)/thread.h $(KERNEL)/stdint.h  $(KERNEL)/interrupt.h  
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/console.o: $(KERNEL)/console.c $(KERNEL)/console.h $(KERNEL)/print.h $(KERNEL)/sync.h  $(KERNEL)/stdint.h  $(KERNEL)/thread.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: $(DEVICE)/keyboard.c $(DEVICE)/keyboard.h $(KERNEL)/print.h $(KERNEL)/io.h $(KERNEL)/interrupt.h $(KERNEL)/global.h $(KERNEL)/ioqueue.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/ioqueue.o: $(KERNEL)/ioqueue.c $(KERNEL)/ioqueue.h $(KERNEL)/global.h $(KERNEL)/print.h $(KERNEL)/stdint.h $(KERNEL)/thread.h $(KERNEL)/sync.h $(KERNEL)/debug.h $(KERNEL)/interrupt.h
	$(CC) $(CFLAGS) $< -o $@ 
$(BUILD_DIR)/tss.o: $(USER)/tss.c $(USER)/tss.h $(KERNEL)/global.h $(KERNEL)/print.h $(KERNEL)/stdint.h $(KERNEL)/thread.h 
	$(CC) $(CFLAGS) $< -o $@ 
$(BUILD_DIR)/process.o: $(USER)/process.c $(USER)/process.h $(KERNEL)/thread.h $(KERNEL)/memory.h $(KERNEL)/interrupt.h $(KERNEL)/print.h  $(KERNEL)/stdint.h $(KERNEL)/global.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/syscall-init.o: $(USER)/syscall-init.c $(USER)/syscall-init.h $(KERNEL)/print.h  $(KERNEL)/stdint.h $(KERNEL)/global.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/syscall.o: $(USER)/syscall.c $(USER)/syscall.h $(KERNEL)/stdint.h $(KERNEL)/global.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/mystdio.o: $(SLIB)/mystdio.c $(SLIB)/mystdio.h $(KERNEL)/stdint.h $(KERNEL)/global.h $(KERNEL)/string.h $(USER)/syscall.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/kstdio.o: $(KERNEL)/kstdio.c $(KERNEL)/kstdio.h $(KERNEL)/console.h $(SLIB)/mystdio.h $(KERNEL)/print.h $(KERNEL)/stdint.h $(KERNEL)/global.h 
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/ide.o: $(DEVICE)/ide.c $(DEVICE)/ide.h $(KERNEL)/stdint.h $(KERNEL)/global.h $(KERNEL)/bitmap.h $(KERNEL)/list.h $(KERNEL)/sync.h $(KERNEL)/console.h $(KERNEL)/kstdio.h $(KERNEL)/debug.h 
	$(CC) $(CFLAGS) $< -o $@ 


$(BUILD_DIR)/dir.o: $(FS)/dir.c $(FS)/dir.h $(DEVICE)/ide.h $(KERNEL)/global.h $(KERNEL)/memory.h $(KERNEL)/string.h $(KERNEL)/bitmap.h $(KERNEL)/debug.h $(FS)/superblock.h $(FS)/inode.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/file.o: $(FS)/file.c $(FS)/file.h $(DEVICE)/ide.h $(KERNEL)/global.h $(KERNEL)/kstdio.h $(KERNEL)/debug.h $(FS)/superblock.h $(FS)/inode.h $(FS)/dir.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/inode.o: $(FS)/inode.c $(FS)/inode.h $(DEVICE)/ide.h $(KERNEL)/global.h $(KERNEL)/memory.h $(KERNEL)/list.h $(KERNEL)/kstdio.h $(KERNEL)/debug.h $(FS)/superblock.h $(KERNEL)/string.h 
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/fs.o: $(FS)/fs.c $(FS)/fs.h $(DEVICE)/ide.h $(KERNEL)/global.h $(KERNEL)/memory.h $(KERNEL)/console.h $(KERNEL)/kstdio.h $(KERNEL)/debug.h $(FS)/superblock.h $(FS)/inode.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/fork.o: $(USER)/fork.c $(USER)/fork.h $(KERNEL)/stdint.h $(KERNEL)/global.h $(KERNEL)/memory.h $(KERNEL)/thread.h $(KERNEL)/string.h 
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/shell.o: $(SLIB)/shell.c $(SLIB)/shell.h 
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/buildin_cmd.o: $(SLIB)/buildin_cmd.c $(SLIB)/buildin_cmd.h $(FS)/fs.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/exec.o: $(USER)/exec.c $(USER)/exec.h 
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/wait_exit.o: $(USER)/wait_exit.c $(USER)/wait_exit.h 
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/pipe.o: $(SLIB)/pipe.c $(SLIB)/pipe.h
	$(CC) $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/kernel.o: $(KERNEL)/kernel.s
	$(AS) $(ASFLAGS) $< -o $@ 
$(BUILD_DIR)/print.o: $(KERNEL)/print.s
	$(AS) $(ASFLAGS) $< -o $@
$(BUILD_DIR)/switch.o: $(KERNEL)/switch.s
	$(AS) $(ASFLAGS) $< -o $@


bin/kernel.bin:$(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY : clean build
clean:
	cd $(BUILD_DIR)/ && rm -f ./* && rm -rf bin/kernel/bin

build:bin/kernel.bin
