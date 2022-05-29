

#BIN = "prog_no_arg"
#CFLAGS = "-Wall -c -fno-builtin -W -Wstrict-prototypes -Wmissing-prototypes -Wsystem-headers"
#LIB = "../../lib/"
#OBJS = "../../obj/string.o ../../obj/syscall.o ../../obj/mystdio.o ../../obj/debug.o"
#DD_IN = $BIN
#DD_OUT = "/bochs/hd60M.img"

#SEN_CNT = $(ls -l prog_no_arg|awk '{printf("%d", ($5 + 511)/512)}')
nasm -f elf start.s -o ../../obj/start.o

cd ../../obj

#ar rcs ../lib/command/simple.a   bitmap.o  file.o interrupt.o list.o  shell.o   init.o thread.o  buildin_cmd.o  fork.o ioqueue.o string.o timer.o console.o fs.o memory.o switch.o tss.o debug.o  ide.o kernel.o mystdio.o  sync.o dir.o  keyboard.o print.o syscall-init.o  exec.o inode.o  kstdio.o     process.o  syscall.o

ar rcs ../lib/command/simple.a string.o syscall.o mystdio.o debug.o start.o

cd ../lib/command

gcc -m32 -c -fno-builtin  -I ../../lib/kernel/ -I ../../lib/ -o cat.o cat.c

ld -m elf_i386 -Ttext 0xc0011500 cat.o simple.a -o cat

#dd if=./prog_no_arg of=/bochs/hd60M.img bs=512 count=20 seek=300 conv=notrunc conv=sync



